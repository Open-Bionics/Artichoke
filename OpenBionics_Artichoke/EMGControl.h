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
*	EMGControl.h
*
*/

#ifndef _EMG_CONTROL_H_
#define _EMG_CONTROL_H_


#include "CircleBuff.h"
#include "TimerManagement.h"

// Uncomment one of the following to change the behaviour of Proportional muscle mode when using a single EMG channel
//#define TENSE_CLOSE_RELAX_OPEN			// tense to close the hand, relax to open
#define TENSE_OPEN_RELAX_CLOSE		// tense to open the hand, relax to close



#define NOISE_BUFFER_SIZE	128
#define BUFFER_DEFAULT_VAL	925


void runEMG(void);
void getSample(void);
void analyseSignal(void);
void controlEMG(void);


void initEMG(void);
void calcNoiseFloor(int muscleNum, int  muscleVal);			// only generate noise floor when muscle is inactive 
void standardEMGControl(void);
void proportionalEMGControl(void);
double calcPosChange(uint16_t sample);


bool detect_peakStart(int currVal, int prevVal);
bool detect_peakEnd(int currVal, int prevVal);



extern bool printADCvals;


extern int musclePins[NUM_EMG_CHANNELS];				// EMG pins



typedef struct
{
	int pin;
	
	CIRCLE_BUFFER noiseFloor = CIRCLE_BUFFER(NOISE_BUFFER_SIZE);

	NB_TIMER HOLD_timer;

	bool active;
	int sample;
	int signal;
	int prevSignal;

	bool PEAK;
	bool HOLD;

} EMGchannel_t;

extern EMGchannel_t EMG[NUM_EMG_CHANNELS];


#endif /*_EMG_CONTROL_H_*/
