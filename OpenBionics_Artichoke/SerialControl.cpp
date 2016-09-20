/*	Open Bionics - Artichoke
*	Author - Olly McBride
*	Date - December 2015
*
*	This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
*	To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
*	
*	Website - http://www.openbionics.com/
*	GitHub - https://github.com/Open-Bionics
*	Email - ollymcbride@openbionics.com
*
*	SerialControl.cpp
*
*/


#include <Arduino.h>			// for type definitions
#include <FingerLib.h>			// for MYSERIAL
#include "Globals.h"

#include "Demo.h"
#include "EMGControl.h"
#include "GripControl.h"
#include "MotorControl.h"
#include "PinManagement.h"
#include "SerialControl.h"
#include "TimerManagement.h"
#include "Utils.h"

#ifdef HANDLE_EN
#include "HANDle.h"
#endif

struct serialCmdType serialCmd;

void serialEvent()
{
	getCmd();
	if(serialCmd.cmdsToGet)
	{
		serialCmd.cmdsToGet--;

		//Serial3.println(serialCmd.cmdBuff);

		processCommand();
		manageSerialSettings();         // toggle demo & muscle mode on/off
	}
}

bool codeSeen(char code)
{
	serialCmd.strPtr = strchr(serialCmd.cmdBuff, code);
	return (serialCmd.strPtr != NULL);  //Return True if a character was found
}

float codeVal()
{
  return (strtod(&serialCmd.cmdBuff[serialCmd.strPtr - serialCmd.cmdBuff + 1], NULL));
}

// return numerical value after the char code, if no char code, return (-1)
float parsenumber(char code) 
{
	if(codeSeen(code))
		return codeVal();
	else
		return BLANK;
}

void getCmd(void)
{
	char serialChar;
	int buffCount = 0;

	if (MYSERIAL.available())
	{
		while (1)
		{
			serialChar = MYSERIAL.read();
			serialCmd.cmdBuff[buffCount++] = serialChar;
			delayMicroseconds(255);

			// if new line (/n or /l) or carriage return, end of string
			if (((uint8_t)serialChar == (uint8_t)13) || ((uint8_t)serialChar == (uint8_t)10))
			{
				serialCmd.cmdBuff[buffCount] = 0;  //terminate string
				serialCmd.cmdsToGet++;
				buffCount = 0;          //clear buffer

				break;
			}
		}
	}
}

void processCommand(void)    // interpret and store received serial commands
{
	// return numbers after char in serialCmd.buff
	serialCmd.advancedFlag = parsenumber('A');      // A advanced control
	serialCmd.demoFlag = parsenumber('D');          // D demo mode
	serialCmd.fingerNum = parsenumber('F');         // F finger number
	serialCmd.gripNum = parsenumber('G');           // G predefined grips
	serialCmd.handFlag = parsenumber('H');          // H left or right hand
	serialCmd.muscleCtrlFlag = parsenumber('M');    // M muscle control
	serialCmd.stopPos = parsenumber('P');           // P stop position
	serialCmd.speed = parsenumber('S');             // S speed
	serialCmd.holdTime = parsenumber('T');          // T amount of time for muscle to be tensed for a grip change
	serialCmd.sensitivityAdjust = parsenumber('U'); // U update sensitivity offset
	serialCmd.instructionsFlag = parsenumber('?');  // ? sends serial instructions

	// direction control       
	if(parsenumber('O') == 0)                       // O OPEN
		serialCmd.direction = OPEN;
	else if(parsenumber('C') == 0)                  // C CLOSE
		serialCmd.direction = CLOSE;
	else
	{                           // if a grip is entered without a direction
		if(serialCmd.gripNum != BLANK)
		{
			currentDir = !currentDir;
			serialCmd.direction = currentDir;     // toggle grip direction
		}
	}

	// constrain the numerical values received over serial
	constrainReceivedValues();
}

// constrain the numerical values received over serial
void constrainReceivedValues(void)
{
	if(serialCmd.fingerNum != BLANK)
		serialCmd.fingerNum = constrain(serialCmd.fingerNum, 0,NUM_FINGERS-1);

	if(serialCmd.stopPos != BLANK)
		serialCmd.stopPos = constrain(serialCmd.stopPos, 0,100);

	if(serialCmd.direction != BLANK)
		serialCmd.direction = constrain(serialCmd.direction, 0,1);

	if(serialCmd.gripNum != BLANK)
		serialCmd.gripNum = constrain(serialCmd.gripNum, 0,NUM_GRIPS-1);

	if(serialCmd.speed != BLANK)
		serialCmd.speed = constrain(serialCmd.speed, 0,MAX_FINGER_SPEED);

	if(serialCmd.stopPos != BLANK)
		serialCmd.stopPos = constrain(serialCmd.stopPos, 0,100);

	if(serialCmd.advancedFlag != BLANK)
		serialCmd.advancedFlag = constrain(serialCmd.advancedFlag, 0,15);

	if(serialCmd.muscleCtrlFlag != BLANK)
		serialCmd.muscleCtrlFlag = constrain(serialCmd.muscleCtrlFlag, 0,5);

	if(serialCmd.handFlag != BLANK)
		serialCmd.handFlag = constrain(serialCmd.handFlag, 0,2);

	if(serialCmd.demoFlag != BLANK)
		serialCmd.demoFlag = constrain(serialCmd.demoFlag, 0,1);

	if(serialCmd.instructionsFlag != BLANK)
		serialCmd.instructionsFlag = constrain(serialCmd.instructionsFlag, 0,1);

	if(serialCmd.sensitivityAdjust != BLANK)
		serialCmd.sensitivityAdjust = constrain(serialCmd.sensitivityAdjust, 0,1024);

	if(serialCmd.resetFlag != BLANK)
		serialCmd.resetFlag = constrain(serialCmd.resetFlag, 0,1);
}

void manageSerialSettings(void)
{
	if(serialCmd.fingerNum != BLANK)
	{
		MYSERIAL_PRINT_PGM("\n");
		MYSERIAL_PRINT_PGM("Finger ");
		MYSERIAL_PRINTLN(serialCmd.fingerNum);
		
		MYSERIAL_PRINT_PGM("Direction ");
		if(serialCmd.direction != BLANK)
			MYSERIAL_PRINTLN(textString.open_close[serialCmd.direction]);
		else
		{	
			int fingerDir = !finger[serialCmd.fingerNum].readDir();
			MYSERIAL_PRINTLN(textString.open_close[fingerDir]);
		}
		
		MYSERIAL_PRINT_PGM("Stop Position ");
		if(serialCmd.stopPos != BLANK)
			MYSERIAL_PRINTLN(serialCmd.stopPos);
		else
		MYSERIAL_PRINTLN_PGM("None");
		
		MYSERIAL_PRINT_PGM("Speed ");
		if(serialCmd.speed != BLANK)
			MYSERIAL_PRINTLN(serialCmd.speed);
		else
			MYSERIAL_PRINTLN(finger[serialCmd.fingerNum].readTargetSpeed());

		fingerControl(serialCmd.fingerNum, serialCmd.stopPos, serialCmd.direction, serialCmd.speed);
	}

	
	else if(serialCmd.gripNum != BLANK)
	{
		MYSERIAL_PRINT_PGM("Grip ");
		MYSERIAL_PRINTLN(textString.grips[serialCmd.gripNum]);
		
		MYSERIAL_PRINT_PGM("Direction ");
		if(serialCmd.direction != BLANK)
			MYSERIAL_PRINTLN(textString.open_close[serialCmd.direction]);
		else
			MYSERIAL_PRINTLN_PGM("None");
		
		MYSERIAL_PRINT_PGM("Stop Position ");
		if(serialCmd.stopPos != BLANK)
			MYSERIAL_PRINTLN(serialCmd.stopPos);
		else
			MYSERIAL_PRINTLN((100 * serialCmd.direction));
			
		MYSERIAL_PRINT_PGM("Speed ");
		if(serialCmd.speed != BLANK)
			MYSERIAL_PRINTLN(serialCmd.speed);
		else
		{
			int fingerToRead = (serialCmd.gripNum==THUMBSUP_GRIP)?FINGER0:FINGER1;
			MYSERIAL_PRINTLN(finger[fingerToRead].readTargetSpeed());
		}
			
		MYSERIAL_PRINT_PGM("\n");

		gripMovement(serialCmd.gripNum, serialCmd.stopPos, serialCmd.direction, serialCmd.speed);
	}
  
	else if(serialCmd.instructionsFlag != BLANK)
	{
		printInstructions();
	}
  
	else if(serialCmd.advancedFlag != BLANK)
	{
		switch(serialCmd.advancedFlag)
		{
			case 0: // demo mode
				advancedSettings.demoFlag = !advancedSettings.demoFlag;   // toggle flag
				demoFlag = advancedSettings.demoFlag;      
				MYSERIAL_PRINT_PGM("Demo mode toggled ");
				MYSERIAL_PRINTLN(textString.off_on[advancedSettings.demoFlag]); // print ON/OFF
				break;
			case 1: // serial instructions
				advancedSettings.instructionsFlag = !advancedSettings.instructionsFlag; // toggle flag
				MYSERIAL_PRINT_PGM("Initial serial instructions toggled ");
				MYSERIAL_PRINTLN(textString.off_on[advancedSettings.instructionsFlag]); // print ON/OFF
				break;
			case 2: // muscle graph
				advancedSettings.muscleGraphFlag = !advancedSettings.muscleGraphFlag; // toggle flag
				MYSERIAL_PRINT_PGM("Muscle graph mode toggled ");
				MYSERIAL_PRINTLN(textString.off_on[advancedSettings.muscleGraphFlag]);  // print ON/OFF
				break;
			case 3: // motor enable/disable
				advancedSettings.motorEnable = !advancedSettings.motorEnable;   // toggle flag
				MYSERIAL_PRINT_PGM("Motors ");
				
				MYSERIAL_PRINTLN(textString.disabled_enabled[advancedSettings.motorEnable]);  // print Disabled/Enabled
				for(int i=0;i<NUM_FINGERS;i++)
				{
					if(advancedSettings.motorEnable)
						finger[i].enableMotor();
					else
						finger[i].disableMotor();
				}
				break;
			case 5: // motors emergency stop
				stopMotors();
				MYSERIAL_PRINTLN_PGM("Stop Motors");
				break;
			case 10:
				advancedSettings.researchFlag = !advancedSettings.researchFlag;
				MYSERIAL_PRINT_PGM("Research mode 1 toggled ");
				MYSERIAL_PRINTLN(textString.off_on[advancedSettings.researchFlag]);
				break;
#ifdef HANDLE_EN
			case 11:
				advancedSettings.HANDle_en = !advancedSettings.HANDle_en;
				MYSERIAL_PRINT_PGM("HANDle mode ");
				MYSERIAL_PRINTLN(textString.disabled_enabled[advancedSettings.HANDle_en]);
				EEPROM_writeStruct(ADVANCED_CTRL_LOC, advancedSettings);
				break;
			case 12:
				toggleHANDleSerial();
				break;
#endif
			default:
				break;
		}
		EEPROM_writeStruct(ADVANCED_CTRL_LOC,advancedSettings);   // save settings to EEPROM
	}
  
	else if(serialCmd.muscleCtrlFlag != BLANK)
	{
		switch(serialCmd.muscleCtrlFlag)
		{
			case -1:            // default
				break;
			case 0:             // muscle control off
				advancedSettings.muscleCtrlFlag = 0;
				MYSERIAL_PRINTLN_PGM("Muscle control OFF ");
				digitalWrite(LED_KNUCKLE,LOW);  // turn off muscle control LED in knuckle
				break;
			case 1:             // standard muscle control
				advancedSettings.muscleCtrlFlag = 1;
				MYSERIAL_PRINTLN_PGM("Standard muscle control ON");
				digitalWrite(LED_KNUCKLE,HIGH);   // turn on muscle control LED in knuckle
				break;
			case 2:             // position muscle control
				advancedSettings.muscleCtrlFlag = 2;
				MYSERIAL_PRINTLN_PGM("Muscle position control ON");
				digitalWrite(LED_KNUCKLE,HIGH);   // turn on muscle control LED in knuckle
				break;
			case 3:             // print muscle readings
				if(advancedSettings.muscleCtrlFlag != 0)
				{
					printADCvals = !printADCvals;
					MYSERIAL_PRINT_PGM("Display muscle readings ");
					MYSERIAL_PRINTLN(textString.off_on[printADCvals]);     // print ON/OFF
				}
				break;
			default:
				break;
		}
      
		EEPROM_writeStruct(ADVANCED_CTRL_LOC,advancedSettings);   // store muscle mode in EEPROM
	}
 
	else if(serialCmd.handFlag != BLANK)  // if 'H#'
	{
		if(serialCmd.handFlag != 0)
		{
			advancedSettings.handFlag = serialCmd.handFlag;
			IOconfig();             // reconfigure finger pins
			EEPROM_writeStruct(ADVANCED_CTRL_LOC,advancedSettings);    // store hand in EEPROM
		}
		MYSERIAL_PRINT_PGM("Hand is ");
		MYSERIAL_PRINTLN(textString.right_left[advancedSettings.handFlag-1]); // print which hand is entered
	}
 
	else if(serialCmd.demoFlag == 0) // if 'D'
	{
		demoFlag = !demoFlag;
		MYSERIAL_PRINT_PGM("Demo Mode ");
		MYSERIAL_PRINTLN(textString.off_on[demoFlag]);
	}
  
	else if(serialCmd.sensitivityAdjust != BLANK)
	{
		if(serialCmd.sensitivityAdjust != 0)
		{
			userSettings.sensitivityOffset = serialCmd.sensitivityAdjust;
			EEPROM_writeStruct(USER_SETTINGS_LOC,userSettings);
		}
		MYSERIAL_PRINT_PGM("Muscle sensor sensitivity ");
		MYSERIAL_PRINTLN(userSettings.sensitivityOffset);
	}

	else if(serialCmd.holdTime != BLANK)
	{
		if(serialCmd.holdTime != 0)
		{
			userSettings.holdTime = serialCmd.holdTime;
			EEPROM_writeStruct(USER_SETTINGS_LOC,userSettings);
		}
		MYSERIAL_PRINT_PGM("Grip change hold duration ");
		MYSERIAL_PRINTLN(userSettings.holdTime);
	}
	
	// if research mode == 1, and no other command is recognised, use CSV string as target motor positions 
	else if (advancedSettings.researchFlag == 1)		// if 'A10'
	{
		researchMode_CSV_RX(serialCmd.cmdBuff);
	}

	clearAll();									// clear all serial commands (clear serialCmd.buffs)
}

void setDefaults(void)
{
	// read EEPROM settings
	EEPROM_readStruct(USER_SETTINGS_LOC,userSettings);
	EEPROM_readStruct(ADVANCED_CTRL_LOC,advancedSettings);

	if(advancedSettings.initConfigFlag != true) 
		initialEEPROMconfig();		// if running for the first time, write default values to EEPROM
			
	currentGrip = FIST_GRIP;
	demoFlag = advancedSettings.demoFlag;
	
	clearAll();       // set all serial/command serialCmd.buff variables to -1

}

void clearAll(void)    //clear all input variables for the next string
{
	int i;
	for(i=0;i<SERIAL_BUF_SIZE;i++)
	{
		serialCmd.cmdBuff[i] = 0;
	}

	serialCmd.fingerNum = BLANK;
	serialCmd.direction = BLANK;
	serialCmd.stopPos = BLANK;
	serialCmd.speed = BLANK;
	serialCmd.gripNum = BLANK;
	serialCmd.advancedFlag = BLANK;
	serialCmd.muscleCtrlFlag = BLANK;
	serialCmd.instructionsFlag = BLANK;
	serialCmd.handFlag = BLANK;
	serialCmd.demoFlag = BLANK;
	serialCmd.sensitivityAdjust = BLANK;
	serialCmd.resetFlag = BLANK;
	serialCmd.holdTime = BLANK;
}

void initialEEPROMconfig(void)			// write default values to EEPROM
{
	MYSERIAL_PRINT_PGM("Initialising EEPROM values...");
	
	advancedSettings.handFlag = RIGHT;
	advancedSettings.instructionsFlag = ON;
	advancedSettings.muscleCtrlFlag = OFF;
	advancedSettings.muscleGraphFlag = OFF;
	advancedSettings.gripFlag = ON;
	advancedSettings.demoFlag = ON;
	advancedSettings.motorEnable = EN;
	advancedSettings.HANDle_en = OFF;
	advancedSettings.initConfigFlag = true;  // set flag to signal config complete
  
	userSettings.sensitivityOffset = 500;
	userSettings.holdTime = 400;
    
	EEPROM_writeStruct(ADVANCED_CTRL_LOC,advancedSettings);
	EEPROM_writeStruct(USER_SETTINGS_LOC,userSettings);
	
	MYSERIAL_PRINTLN_PGM("Complete");
}


void researchMode_CSV_RX(char *CSVString)			// receive CSV string and write target positions to motors
{
	int posVals[NUM_FINGERS];

	convertFromCSV(CSVString, posVals);		// convert CSV string to CSV individual position values

	for (int i = 0; i<NUM_FINGERS; i++)
	{
		finger[i].writePos(posVals[i]);
	}
}

void researchMode_CSV_TX(void)			// receive CSV string and write target positions to motors
{
	for (int i = 0; i<NUM_FINGERS; i++)
	{
		MYSERIAL_PRINT(finger[i].readPos());			// send CSV variable
		if (i < NUM_FINGERS - 1)
			MYSERIAL_PRINT_PGM(",");						// send comma between variables, don't put send one at the end
	}
	MYSERIAL_PRINT_PGM("\n");
}

void startUpMessages(void)
{
	MYSERIAL_PRINT_PGM("Open Bionics - Artichoke V");
	MYSERIAL_PRINTLN((float)VERSION_N);
  
	MYSERIAL_PRINTLN_PGM("Almond board");

	MYSERIAL_PRINT(textString.right_left[advancedSettings.handFlag-1]);
	MYSERIAL_PRINTLN_PGM(" Hand");

	#if defined(USE_I2C_ADC)
	MYSERIAL_PRINTLN_PGM("I2C Muscle Sensors");
	#endif
	#if defined(USE_I2C_IO)
	MYSERIAL_PRINTLN_PGM("I2C Port Expander");
	#endif

	if(advancedSettings.muscleCtrlFlag > 0)
	{
		// if muscle mode is on, print on
		switch(advancedSettings.muscleCtrlFlag)
		{
			case 1:             // standard muscle control
				MYSERIAL_PRINTLN_PGM("Standard Muscle Control ON");
				digitalWrite(LED_KNUCKLE,HIGH); // turn on muscle control LED in knuckle
				break;
			case 2:             // position muscle control
				MYSERIAL_PRINTLN_PGM("Muscle Position Control ON");
				digitalWrite(LED_KNUCKLE,HIGH);   // turn on muscle control LED in knuckle
				break;
			default:
				break;
		}
		MYSERIAL_PRINT_PGM("Grip Change Mode ");
		MYSERIAL_PRINTLN(textString.off_on[advancedSettings.gripFlag]);
	}
	
	if(advancedSettings.instructionsFlag) 
		printInstructions();			// if instructions not turned off, print initial instructions

	// if demo mode is enabled from start up
	if(advancedSettings.demoFlag)
	{
		MYSERIAL_PRINTLN_PGM("Start up demo mode ON");
		MYSERIAL_PRINTLN_PGM("Enter A0 to disable this mode");
		MYSERIAL_PRINTLN_PGM("The hand is only responsive to serial commands \nat the end of each demo cycle");
	}
}

void printInstructions(void)
{
	MYSERIAL_PRINT_PGM("\n");
	MYSERIAL_PRINTLN_PGM("         Open Bionics Serial Commands - Artichoke ");
	MYSERIAL_PRINTLN_PGM("___________________________________________________________");
	MYSERIAL_PRINT_PGM("\n");
	MYSERIAL_PRINTLN_PGM("Grip Control (G#)");
	MYSERIAL_PRINTLN_PGM("Command     Description");
	MYSERIAL_PRINTLN_PGM("G0          Fist");
	MYSERIAL_PRINTLN_PGM("G1          Palm");
	MYSERIAL_PRINTLN_PGM("G2          Thumbs Up");
	MYSERIAL_PRINTLN_PGM("G3          Point");
	MYSERIAL_PRINTLN_PGM("G4          Pinch");
	MYSERIAL_PRINTLN_PGM("G5          Tripod");
	MYSERIAL_PRINT_PGM("\n");
	MYSERIAL_PRINTLN_PGM("Finger Control (F#, P##, S##)");
	MYSERIAL_PRINTLN_PGM("Command     Description");
	MYSERIAL_PRINTLN_PGM("F#          Finger, where # is the finger number (F0 - F4)");
	MYSERIAL_PRINTLN_PGM("F# P50      Finger # to position 50 (P0 - P100)");
	MYSERIAL_PRINTLN_PGM("F# S255     Finger # at speed 255 (S0 - S255)");
	MYSERIAL_PRINT_PGM("\n");
	MYSERIAL_PRINTLN_PGM("Direction (O, C)");
	MYSERIAL_PRINTLN_PGM("Command     Description");
	MYSERIAL_PRINTLN_PGM("## O        Open, where ## is a Grip or Finger command");
	MYSERIAL_PRINTLN_PGM("## C        Close, where ## is a Grip or Finger command");
	MYSERIAL_PRINT_PGM("\n");
	MYSERIAL_PRINTLN_PGM("Muscle Control (M#, U#, T#, N)");
	MYSERIAL_PRINTLN_PGM("Command     Description");
	MYSERIAL_PRINTLN_PGM("M0          Muscle control OFF");
	MYSERIAL_PRINTLN_PGM("M1          Standard muscle control ON");
	MYSERIAL_PRINTLN_PGM("M2          Muscle position control ON");
	MYSERIAL_PRINTLN_PGM("M3          Toggle whether to display muscle readings");
	MYSERIAL_PRINTLN_PGM("U###        Change muscle sensor sensitivity");
	MYSERIAL_PRINTLN_PGM("T###        Change 'muscle hold' grip change duration");
	MYSERIAL_PRINTLN_PGM("N           Calculate noise floor");
	MYSERIAL_PRINT_PGM("\n");
	MYSERIAL_PRINTLN_PGM("Advanced Settings (H#, A#, ?)");
	MYSERIAL_PRINTLN_PGM("Command     Description");
	MYSERIAL_PRINTLN_PGM("H           View hand configuration (LEFT or RIGHT)");
	MYSERIAL_PRINTLN_PGM("H1          Set hand to be RIGHT");
	MYSERIAL_PRINTLN_PGM("H2          Set hand to be LEFT");
	MYSERIAL_PRINTLN_PGM("A0          Toggle 'Demo Mode' ON/OFF");
	MYSERIAL_PRINTLN_PGM("A1          Toggle 'Serial Commands Instructions' ON/OFF");
	MYSERIAL_PRINTLN_PGM("A2          Toggle 'Muscle Graph mode' ON/OFF");
	MYSERIAL_PRINTLN_PGM("A3          Enable/Disable motors");
	MYSERIAL_PRINTLN_PGM("A5          Emergency stop motors");
	MYSERIAL_PRINTLN_PGM("A10         Research Mode 0, fast position control");
	MYSERIAL_PRINTLN_PGM("A11         Research Mode 1, HANDle control (Nunchuck)");
	MYSERIAL_PRINTLN_PGM("A12         Research Mode 1, HANDle  data dump");
	MYSERIAL_PRINTLN_PGM("?           Display serial commands list");
	MYSERIAL_PRINT_PGM("\n\n\n");
	MYSERIAL_PRINTLN_PGM("Examples");
	MYSERIAL_PRINTLN_PGM("G0 O        Fist Grip Open");
	MYSERIAL_PRINTLN_PGM("G2 C        Thumbs Up Close");
	MYSERIAL_PRINTLN_PGM("F0 O        Thumb Open");
	MYSERIAL_PRINTLN_PGM("F0          Toggle thumb direction (open/close)");
	MYSERIAL_PRINTLN_PGM("F4 P50      Pinky to position 50");
	MYSERIAL_PRINTLN_PGM("F1 P50 S80  Index finger to position 50 at speed 80");
	MYSERIAL_PRINTLN_PGM("F0 O S200   Thumb Open at speed 200");
	MYSERIAL_PRINT_PGM("\n\n");




	// MODE ESCAPE TEXT
	if(!advancedSettings.motorEnable)			// enable motors
		MYSERIAL_PRINTLN_PGM("Motors disabled, enter 'A3' to re-enable them");
	if (advancedSettings.muscleGraphFlag)		// print muscle data over serial
		MYSERIAL_PRINTLN_PGM("Muscle graph mode, enter 'A2' to disable this mode");
	if (advancedSettings.muscleCtrlFlag > 0)	// muscle/EMG control
		MYSERIAL_PRINTLN_PGM("EMG/muscle control mode, enter 'M0' to disable this mode");
	if (demoFlag)								// demo mode
		MYSERIAL_PRINTLN_PGM("Demo mode, enter 'A0' to disable this mode");
	// if researchFlag == 1, and no other command is recognised, use CSV string as target motor positions
	if (advancedSettings.researchFlag == 1)		// if 'A10'
		MYSERIAL_PRINTLN_PGM("Research mode 0, enter 'A10' to disable this mode");

#ifdef HANDLE_EN			
	if (advancedSettings.HANDle_en)				// HANDle (Nunchuck) control
		MYSERIAL_PRINTLN_PGM("HANDle mode, enter 'A11' to disable this mode");
#endif
	
	MYSERIAL_PRINT_PGM("\n\n");
}
