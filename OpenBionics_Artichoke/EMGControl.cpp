/*	Open Bionics - Artichoke
*	Author - Olly McBride
*	Date - August 2016
*
*	This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
*	To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
*
*	Website - http://www.openbionics.com/
*	GitHub - https://github.com/Open-Bionics
*	Email - ollymcbride@openbionics.com
*
*	EMGControl.cpp
*
*/

#include "Globals.h"

#include "EMGControl.h"

#include "CircleBuff.h"
#include "GripControl.h"
#include "TimerManagement.h"
#include "Utils.h"

#if defined(USE_I2C_ADC)
#include <I2C_ADC.h>			// This library can be downloaded from https://github.com/Open-Bionics
#endif


static bool _EMGinitialised = false;
bool printADCvals;


EMGchannel_t EMG[NUM_EMG_CHANNELS];


void initEMG(void)
{
	for (int c = 0; c < NUM_EMG_CHANNELS; c++)
	{
		for (int i = 0; i < NOISE_BUFFER_SIZE; i++)
		{
			EMG[c].noiseFloor.write(BUFFER_DEFAULT_VAL);
		}
	}

	_EMGinitialised = true;
}

void runEMG(void)
{
	if (!_EMGinitialised)
		initEMG();

	// read the raw sample data and remove the noise floor
	getSample();

	// detect PEAK or HOLD on each EMG channel
	analyseSignal();

	// print ADC values if M3 is enabled
	if (printADCvals)
	{
		for (int c = 0; c < NUM_EMG_CHANNELS; c++)
		{
			MYSERIAL_PRINT_PGM("M");
			MYSERIAL_PRINT(c);
			MYSERIAL_PRINT_PGM(": ");
			MYSERIAL_PRINT(EMG[c].signal);
			MYSERIAL_PRINT_PGM("\tR");
			MYSERIAL_PRINT(c);
			MYSERIAL_PRINT_PGM(": ");
			MYSERIAL_PRINT(EMG[c].sample);
			MYSERIAL_PRINT_PGM("\tN");
			MYSERIAL_PRINT(c);
			MYSERIAL_PRINT_PGM(": ");
			MYSERIAL_PRINT(EMG[c].noiseFloor.readMean());
			MYSERIAL_PRINT_PGM("\tP");
			MYSERIAL_PRINT(c);
			MYSERIAL_PRINT_PGM(": ");
			MYSERIAL_PRINT(EMG[c].PEAK);
			MYSERIAL_PRINT_PGM("\tH");
			MYSERIAL_PRINT(c);
			MYSERIAL_PRINT_PGM(": ");
			MYSERIAL_PRINT(EMG[c].HOLD);

			MYSERIAL_PRINT_PGM("\t");
		}
		MYSERIAL_PRINT_PGM("\n");
	}
	

	controlEMG();
}

void getSample(void)
{

	for (uint8_t c = 0; c < NUM_EMG_CHANNELS; c++)
	{
    // read raw EMG sample
#ifdef USE_I2C_ADC
		EMG[c].sample = ADC2.read(c);
#else
    noInterrupts();
		EMG[c].sample = analogRead(EMG[c].pin);		
    interrupts();		
#endif

    // add to noise floor if muscle is NOT active
		calcNoiseFloor(c, EMG[c].sample);					

    // store the previous signal for signal analysis
		EMG[c].prevSignal = EMG[c].signal;						

    // signal is the size of the active signal (signal > noise floor), and is always >= 0
		EMG[c].signal = EMG[c].sample - EMG[c].noiseFloor.readMean();	
		if (EMG[c].signal < 0)
			EMG[c].signal = 0;

    // check signal activation
		if (EMG[c].signal > 0)
			EMG[c].active = true;
		else
			EMG[c].active = false;
	}

}

void calcNoiseFloor(int muscleNum, int  muscleVal)			// only generate noise floor when muscle is inactive 
{
	// if muscle value is less than the noise floor + 1/4 of the sensitivity offet 
	// (this reduces upwards creep of the nose floor when the muscle is held tensed)
	if (muscleVal < (EMG[muscleNum].noiseFloor.readMean() + (userSettings.sensitivityOffset/4)))
	{
		EMG[muscleNum].noiseFloor.write(muscleVal);		// add to noise floor buffer
	}
}

void analyseSignal(void)
{
	for (int c = 0; c < NUM_EMG_CHANNELS; c++)
	{

		if (!EMG[c].active)		// if the muscle is not active, end analysis
		{
			return;
		}


		if (EMG[c].HOLD_timer.started())					// if HOLD timer is running
		{
			if (EMG[c].HOLD_timer.timeEllapsed(userSettings.holdTime) && !EMG[c].HOLD)	// if PEAK has been HELD and HOLD flag is not set
			{
				EMG[c].HOLD = true;					// set HOLD flag		
				EMG[c].HOLD_timer.stop();			// stop HOLD timer

			}
		}

		if (detect_peakStart(EMG[c].signal, EMG[c].prevSignal))						// if at PEAK_START
		{
			EMG[c].HOLD_timer.start();			// start HOLD timer
#if (NUM_EMG_CHANNELS == 2)
			EMG[c].PEAK = true;					// set single PEAK flag	
#endif

		}
		else if (detect_peakEnd(EMG[c].signal, EMG[c].prevSignal))					// if at PEAK_END
		{
			EMG[c].HOLD_timer.stop();			// stop HOLD timer
#if (NUM_EMG_CHANNELS == 1)
			// if the muscle is not held, then trigger a PEAK
			if(!EMG[c].HOLD)
				EMG[c].PEAK = true;					// set single PEAK flag	
#endif
		}
	}

	
}


// SIGNAL FEATURE DETECTION
bool detect_peakStart(int currVal, int prevVal)
{
	if ((prevVal < userSettings.sensitivityOffset) && (currVal >= userSettings.sensitivityOffset))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool detect_peakEnd(int currVal, int prevVal)
{
	if ((prevVal >= userSettings.sensitivityOffset) && (currVal < userSettings.sensitivityOffset))
	{
		return true;
	}
	else
	{
		return false;
	}
}


void controlEMG(void)
{
	if (EMG[0].HOLD)
	{
		// reset HOLD flag
		EMG[0].HOLD = false;

		// cycle to next grip
		currentGrip++;
		if (currentGrip > NUM_GRIPS)
			currentGrip = 0;

		// change to new grip in the OPEN position
		gripMovement(currentGrip, 0, BLANK, MAX_FINGER_SPEED);


		MYSERIAL_PRINTLN_PGM("Grip Change");
		MYSERIAL_PRINT_PGM("Grip ");
		MYSERIAL_PRINTLN(textString.grips[currentGrip]);
	}


	// 	if standard muscle control enabled
	if (advancedSettings.muscleCtrlFlag == 1)
	{
		standardEMGControl();
	}

	// 	if positional muscle control enabled
	if (advancedSettings.muscleCtrlFlag == 2)
	{
		proportionalEMGControl();
	}
}


void standardEMGControl(void)
{
#if (NUM_EMG_CHANNELS == 1)

	// if channel has triggered a PEAK, and hand is not already OPEN
	if(EMG[0].PEAK)
	{
		EMG[0].PEAK = false;                // reset PEAK trigger
    
		MYSERIAL_PRINT_PGM("Arm ");
		MYSERIAL_PRINTLN(textString.open_close[!currentDir]);

		gripMovement(currentGrip, BLANK, !currentDir, BLANK);
	}

#elif (NUM_EMG_CHANNELS == 2)
	// if OPEN channel has triggered a PEAK, and hand is not already OPEN
	if(EMG[0].PEAK && currentDir != OPEN)
	{
		EMG[0].PEAK = false;                // reset PEAK trigger
		MYSERIAL_PRINTLN_PGM("Arm OPEN");
		gripMovement(currentGrip, BLANK, OPEN, BLANK);
	}
	else if (EMG[1].PEAK && currentDir != CLOSE)
	{
		EMG[1].PEAK = false;                // reset PEAK trigger
		MYSERIAL_PRINTLN_PGM("Arm CLOSE");
		gripMovement(currentGrip, BLANK, CLOSE, BLANK);
	}
  
#endif
}





void proportionalEMGControl(void)
{
#if (NUM_EMG_CHANNELS == 1)

	static double pos = 0;

#if defined(TENSE_CLOSE_RELAX_OPEN)
	if (EMG[0].signal > 0)
	{
		pos -= calcPosChange(EMG[0].signal);
	}
	else
	{
		pos += calcPosChange(900);
	}
#elif defined(TENSE_OPEN_RELAX_CLOSE)
	if (EMG[0].signal > 0)
	{
		pos += calcPosChange(EMG[0].signal);
	}
	else
	{
		pos -= calcPosChange(900);
	}
#endif

	//MYSERIAL_PRINT_PGM("pos: ");
	//MYSERIAL_PRINTLN_PGM(pos);

	pos = constrain(pos, 0, 100);

	gripMovement(currentGrip, (uint16_t)pos, BLANK, MAX_FINGER_SPEED);

#elif (NUM_EMG_CHANNELS == 2)
	static double pos = 0;

	if (EMG[0].signal > EMG[1].signal)
	{
		pos += calcPosChange(EMG[0].signal);
	}
	else if (EMG[0].signal < EMG[1].signal)
	{
		pos -= calcPosChange(EMG[1].signal);
	}

	//MYSERIAL_PRINT_PGM("pos: ");
	//MYSERIAL_PRINTLN_PGM(pos);

	pos = constrain(pos, 0, 100);

	gripMovement(currentGrip, (uint16_t)pos, BLANK, MAX_FINGER_SPEED);

#endif
}


double calcPosChange(uint16_t sample)
{
	bool invert = false;
	double exp;

	double  power = 2;				// expo 
	double sensitivity = 2;			// gradient 

	static double k = pow((1024 / sensitivity), power);		// normalise, then modify expo sensitivity


	if (isEven(power))				// fix issue that -x^2 = x?^2 
		invert = true;

	//MYSERIAL_PRINT_PGM("samp: ");
	//MYSERIAL_PRINT_PGM(sample);
	//MYSERIAL_PRINT_PGM("\tpower: ");
	//MYSERIAL_PRINT_PGM(power);
	//MYSERIAL_PRINT_PGM("\tk: ");
	//MYSERIAL_PRINT_PGM(k);


	// calculate position change as exponent (sensitivity is x^power proportional to _yPos)
	exp = pow(sample, power) / k;

	//MYSERIAL_PRINT_PGM("exp: ");
	//MYSERIAL_PRINTLN_PGM(exp);

	if (invert)						// fix issue that -x^2 = x?^2
		exp = -exp;

	return exp;
}
