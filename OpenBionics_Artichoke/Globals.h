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

// Architecture specific include
#ifndef ARDUINO_AVR_MEGA2560
#error "This software only supports boards with an AVR or SAMD processor."
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
#define MAX_BUF			64					// maximum size of Serial incoming serialCmd.buff
// MUSCLE SENSE
#define NUM_SAMPLES				100			// size of muscle readings serialCmd.buff   // default 500
#define NUM_PEAKS				8			// size of previous peaks serialCmd.buff
#define DEFAULT_PEAKS			900			// initial value to be loaded into default peaks buffer
#define NUM_DIR_SAMPLES			200			// size of previous arm movements buffer
#define MUSCLE_POS_INCREMENT	4			// distance to move when using muscle position control  // default 2
#define NOISE_OFFSET			300			// don't include any value greater than noise floor + NOISE_OFFSET

// FINGERLIB FINGER CLASS
extern Finger finger[NUM_FINGERS];

// MUSCLE SENSE
struct muscleType
{
 	int pins[2];				// muscle sense ADC pin names
 	int printADCvals = 0;       // flag to print muscle ADC readings
 	int noiseFloorFlag = 0;     // flag to indicate if noise floor has been calculated 
};
extern struct muscleType muscle;

// SERIAL CONTROL
struct serialCmdType
{
	char cmdBuff[MAX_BUF];	// buffer and variable to store incoming Serial string
	char* strPtr;           // pointer to char used for codeSeen() and codeVal()
	int cmdsToGet = 0;      // number of commands left in buffer (promotes expansion to ring buffer in future)
	// serial flags
	int fingerNum = BLANK;
	int gripNum = BLANK;
	int direction = BLANK;
	int stopPos = BLANK;
	int speed = BLANK;
	int advancedFlag = BLANK;
	int instructionsFlag = BLANK;
	int muscleCtrlFlag = BLANK;
	int handFlag = BLANK;
	int demoFlag = BLANK;
	int sensitivityAdjust = BLANK;
	int resetFlag = BLANK;
	int noiseFloor = BLANK;
	int holdTime = BLANK;  
};
extern struct serialCmdType serialCmd;

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

// DEMO MODE
struct demoType
{
  int flag = 0;
  int gripDuration = 800;    // amount of time each grip runs in demo mode (milliseconds)
};
extern struct demoType demo;

// TEXT STRINGS
struct textStringType
{
	const char* open_close[3] = {"Open","Close","None"};				// hand/finger directions
	const char* off_on[2] = {"OFF","ON"};						// states
	const char* right_left[2] = {"Right","Left"};				// hand types
	const char* disabled_enabled[2] = {"Disabled","Enabled"};   // motor states
	const char* grips[NUM_GRIPS+1] = {"Fist","Palm","Thumbs Up","Point","Pinch","Tripod","Finger Roll"};  // grip names
};
extern struct textStringType textString;

// FINGER & HAND LATCH_STATE_LOCS
extern int currentGrip;				// current grip pattern for muscle sense change
extern int currentDir;

// SOFTWARE VERSION NUMBER
#define VERSION_N  1.0

#endif

