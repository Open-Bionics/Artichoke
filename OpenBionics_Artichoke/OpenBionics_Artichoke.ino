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
*	OpenBionics_Artichoke.ino
*
*/

#include <FingerLib.h>			// This library can be downloaded from https://github.com/Open-Bionics
#include <Wire.h>  
#include <EEPROM.h>

#include "Globals.h"

#include "GripControl.h"
#include "CircleBuff.h"
#include "Demo.h"
#include "EMGControl.h"
#include "MotorControl.h"
#include "PinManagement.h"
#include "SerialControl.h"
#include "TimerManagement.h"
#include "Utils.h"

#ifdef USE_I2C_ADC
#include <I2C_ADC.h>			// This library can be downloaded from https://github.com/Open-Bionics
#endif


#ifdef HANDLE_EN
#include "Wiichuck.h"			// Nunchuck library, written by jnw.walker@gmail.com	
#include "HANDle.h"
#endif



/***************************************************************************************************
*
*	Open Bionics - Artichoke Release Notes
*
*	Version	|	Date		|	Notes
*	V1.0.0	|	08/01/16	|	Initial release for Ada hand and Almond boards using Atmega 2560
*	V1.0.1	|	03/02/16	|	Modified formatting and cleaned up
*	V1.1.0	|	31/03/16	|	Added research and HANDle mode. Fixed motorEn and muscle graph
*	V1.1.1	|	17/05/16	|	Increased PWM timer freq to prevent hum and implemented customDelay() instead of delay()		
*	V1.2.0	|	22/08/16	|	Re-written EMG control (now allows both 1 & 2 channel control)
*
*
*	Artichoke Description
*
*		- Simple hand control software designed to run on the Open Bionics Almond hand controller
*		- Uses FingerLib.h for low level finger control, which allows fingers to be treated as servos
*		- Can be controlled via the following methods:
*			- Serial control (baud 38400)
*			- Muscle control (EMG)
*			- HANDle control (Nunchuck)
*		- Uses either inbuilt ADC or external I2C ADC for muscle sensing and hand control
*		- Enter '?' to view available serial commands
*
****************************************************************************************************/


void setup()
{
	MYSERIAL.begin(38400);		// start serial

#if defined(USE_I2C_ADC) || defined(HANDLE_EN)
	Wire.begin();				// if using I2C for I2C_ADC or for HANDle control, initialise I2C
#endif
	timerSetup();				// start timer interrupts
	setDefaults();				// initialise serialCmd.buffs, finger positions and muscle control, read EEPROM presets
	IOconfig();					// config finger pins, initialise port expander

	startUpMessages();			// print welcome message, current hand configuration/settings
} 

void loop()
{
	if (advancedSettings.muscleCtrlFlag > 0)	// muscle/EMG control
		runEMG();

	if (demoFlag)								// demo mode
		demoMode();

	// if researchFlag == 1, and no other command is recognised, use CSV string as target motor positions
	if (advancedSettings.researchFlag == 1)		// if 'A10'
		researchMode_CSV_TX();

#ifdef HANDLE_EN			
	if (advancedSettings.HANDle_en)				// HANDle (Nunchuck) control
		HANDleMain();	
#endif
}
