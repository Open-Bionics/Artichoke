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
*	Globals
*
*/
#include "Globals.h"

#if defined(USE_I2C_ADC)
#include <I2C_ADC.h>			// This library can be downloaded from https://github.com/Open-Bionics
#endif

// initialise Finger class to array
Finger finger[NUM_FINGERS];

// DEMO MODE
struct demoType demo;
// SERIAL CONTROL
struct serialCmdType serialCmd;
// ADVANCED SETTINGS
struct advancedSettingsType advancedSettings;
// USER SETTINGS
struct userSettingsType userSettings;
// TEXT STRINGS
struct textStringType textString;
// MUSCLE SENSE
struct muscleType muscle;
// FINGER & HAND INIT STATES
int currentGrip = FIST_GRIP;				// current grip pattern for muscle sense change
int currentDir = OPEN;	

