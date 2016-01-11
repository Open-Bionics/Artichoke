/*	Open Bionics - Artichoke
*	Author - Olly McBride
*	Date - December 2015
*
*	This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
*	To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
*	
*	Website - http://www.openbionics.com/
*	GitHub - https://github.com/Open-Bionics
*
*	OpenBionics_Artichoke
*
*/
#include <FingerLib.h>			// This library can be downloaded from https://github.com/Open-Bionics
#include <Wire.h>  
#include <EEPROM.h>
#include "Globals.h"
#include "Utils.h"


/***************************************************************************************************
*
*	Open Bionics - Artichoke Release Notes
*
*	Version	|	Date		|	Notes
*	V1.0	|	08/01/16	|	Initial release for Ada hand and Almond boards using Atmega 2560
*			|				|
*			|				|
*
*
*	Artichoke Description
*
*		- Simple hand control software designed to run on the Open Bionics Almond hand controller
*		- Uses FingerLib.h for low level finger control, which allows fingers to be treated as servos
*		- Uses either inbuilt ADC or external I2C ADC for muscle sensing and hand control
*		- Allows hand control via Serial (baud 38400), enter '?' to view available Serial commands
*
****************************************************************************************************/


void setup()
{
	MYSERIAL.begin(38400);		// start serial
	
	#ifdef USE_I2C_ADC
		Wire.begin();			// if using I2C ADC, initialise I2C
	#endif
	timerSetup();				// start timer interrupts
	setDefaults();				// initialise serialCmd.buffs, finger positions and muscle control, read EEPROM presets
	IOconfig();					// config finger pins, initialise port expander
	
	startUpMessages();			// print welcome message, current hand configuration/settings
}

void loop()
{
	if (advancedSettings.muscleGraphFlag)
		muscleGraph();
	if (advancedSettings.muscleCtrlFlag > 0)	// muscle control
		muscleControl();						
	if (serialCmd.fingerNum != BLANK)			// finger control
		fingerControl(serialCmd.fingerNum, serialCmd.stopPos, serialCmd.direction, serialCmd.speed);				
	if (serialCmd.gripNum != BLANK)				// grip control
		gripMovement(serialCmd.gripNum, serialCmd.stopPos, serialCmd.direction, serialCmd.speed);
	if (demo.flag)								// demo mode
		demoMode();

	clearAll();									// clear all serial command serialCmd.buffs
}

