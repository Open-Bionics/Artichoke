/*	Open Bionics - Artichoke-
*	Author - Olly McBride
*	Date - December 2015
*
*	This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
*	To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
*	
*	Website - http://www.openbionics.com/
*	GitHub - https://github.com/Open-Bionics
*
*	Globals.h
*
*/

#ifndef _GLOBALS_h
#define _GLOBALS_h

#include <Arduino.h>	// init data types (uint8_t...)
#include <FingerLib.h>

// Architecture specific include
#ifndef ARDUINO_AVR_MEGA2560
#error "This software only supports boards with an Atmega2560 processor."
#endif

// Uncomment any of the following to enable I2C_ADC muscle control
//#define USE_I2C_ADC             // I2C muscle sensors

// MACROS
#define IS_BETWEEN(x,a,b)     (((x>=a)&&(x<=b))?(1):(0))        // check if value x is between values a and b

// LEDS
#define LED_WRIST	16
#define LED_KNUCKLE	24
#define LED_EDGE	9
// DIRECTIONS
#define RIGHT		1	    // handIs
#define LEFT		2	    // handIs
#define OPEN		0	    // hand
#define CLOSE		1	    // hand
#define NONE		2	    // hand
#define FALL		0	    // muscle signal
#define RISE		1	    // muscle signal
#define OFF			0	    // EEPROM flag
#define ON			1	    // EEPROM flag
#define DISABLE		0	    // ADC channels
#define EN			1	    // ADC channels
#define BLANK		(-1)	// animation array
// FINGERS
#define NUM_FINGERS		5
#define FINGER0			0		// thumb flex
#define FINGER1			1		// index flex
#define FINGER2			2		// middle flex
#define FINGER3			3		// ring flex
#define FINGER4			4		// pinky flex
// GRIPS
#define NUM_GRIPS		6
#define FIST_GRIP		0
#define PALM_GRIP	    1
#define THUMBSUP_GRIP	2
#define POINT_GRIP		3
#define PINCH_GRIP		4
#define TRIPOD_GRIP		5
#define FINGER_ROLL		6					// only used for Demo mode
// EEPROM LOCATIONS
#define ADVANCED_CTRL_LOC	  20			// starting pos for advanced control flags
#define USER_SETTINGS_LOC	  30			// starting pos for stored user settings
// DEFAULTS
#define FULLY_OPEN		MIN_FINGER_POS		// fully open position for the hand, from FingerLib.h
#define FULLY_CLOSED	MAX_FINGER_POS		// fully closed position for the hand, from FingerLib.h

// FINGERLIB FINGER CLASS
extern Finger finger[NUM_FINGERS];
// FINGER & HAND STATES
extern int currentGrip;				// current grip pattern for muscle sense change
extern int currentDir;

// ADVANCED SETTINGS
struct advancedSettingsType
{
	// one time config flags
	uint8_t initConfigFlag = false;
	uint8_t handFlag = BLANK;
	// control flags
	uint8_t instructionsFlag = BLANK;
	uint8_t muscleGraphFlag = BLANK;
	uint8_t muscleCtrlFlag = BLANK;
	uint8_t gripFlag = BLANK;
	uint8_t demoFlag = BLANK;
	uint8_t motorEnable = BLANK;
};
extern struct advancedSettingsType advancedSettings;

// USER SETTINGS
struct userSettingsType
{
	int sensitivityOffset = BLANK;
	int holdTime = BLANK;             // time the muscle needs to be held tensed to perform a grip change
};
extern struct userSettingsType userSettings;

// TEXT STRINGS
struct textStringType
{
	const char* open_close[3] = {"Open","Close","None"};				// hand/finger directions
	const char* off_on[2] = {"OFF","ON"};								// states
	const char* right_left[2] = {"Right","Left"};						// hand types
	const char* disabled_enabled[2] = {"Disabled","Enabled"};			// motor states
	const char* board[4] = {"Adult","Four Motor","Child","Almond"};     // board names
	const char* grips[NUM_GRIPS+1] = {"Fist","Palm","Thumbs Up","Point","Pinch","Tripod","Finger Roll"};  // grip names
	
};
extern struct textStringType textString;


// SOFTWARE VERSION NUMBER
#define VERSION_N  1.0
#define OB_BOARD   3

#endif /*_GLOBALS_h*/
