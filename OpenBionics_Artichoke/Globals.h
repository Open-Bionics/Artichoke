/*	Open Bionics - Artichoke-
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
*	Globals.h
*
*/

#ifndef _GLOBALS_h
#define _GLOBALS_h

#include <Arduino.h>	// data types (uint8_t...)
#include <FingerLib.h>	// finger control


// Uncomment any of the following to enable various features
#define NUM_EMG_CHANNELS	2	// select the number of EMG channels to use (1 or 2)
//#define USE_I2C_ADC			// I2C muscle sensors (requires I2C_ADC.h from https://github.com/Open-Bionics/Arduino_Libraries)
//#define HANDLE_EN				// HANDle (Nunchuck) control


// ERROR MANAGEMENT
#ifndef ARDUINO_AVR_MEGA2560
#error "This software only supports boards with an Atmega2560 processor."
#endif

#if (NUM_EMG_CHANNELS < 1) || (NUM_EMG_CHANNELS > 2)
#error "This software only supports a maximum of 2 EMG channels"
#endif

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
#define BLANK		(-1)	

// FINGERS
#define NUM_FINGERS		5
#define FINGER0			0		// thumb
#define FINGER1			1		// index
#define FINGER2			2		// middle
#define FINGER3			3		// ring
#define FINGER4			4		// pinky

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
extern Finger finger[NUM_FINGERS];			// instances of the Finger class, from FingerLib.h

// FINGER & HAND STATES
extern int currentGrip;						// current grip pattern for muscle sense change
extern int currentDir;						// current direction of the hand (Open/Closed)

// ADVANCED SETTINGS
struct advancedSettingsType					// these settings are stored in EEPROM and are used to enter different modes at startup
{
	// one time config flags
	uint8_t initConfigFlag = false;			// used to indicate whether EEPROM has been configured correctly
	uint8_t handFlag = BLANK;				// left or right hand
											// control flags		
	uint8_t instructionsFlag = BLANK;		// display serial instructions on start up
	uint8_t muscleGraphFlag = BLANK;		// display muscle graph on startup
	uint8_t muscleCtrlFlag = BLANK;			// enable muscle control (standard or position) on startup
	uint8_t gripFlag = BLANK;				// enable/disable grip change 
	uint8_t demoFlag = BLANK;				// run demo mode on startup
	uint8_t motorEnable = BLANK;			// enable/disable motors

	uint8_t HANDle_en = BLANK;				// enable HANDle mode from startup
	uint8_t researchFlag = BLANK;			// change hand control inputs
};
extern struct advancedSettingsType advancedSettings;

// USER SETTINGS
struct userSettingsType						// these settings are also stored in EEPROM, and are custom to each user
{
	int sensitivityOffset = BLANK;			// the sensitivity of the EMG controller
	int holdTime = BLANK;					// time the muscle needs to be held tensed to perform a grip change
};
extern struct userSettingsType userSettings;

// TEXT STRINGS
struct textStringType
{
	const char* open_close[3] = {"Open","Close","None"};				// hand/finger directions
	const char* off_on[2] = {"OFF","ON"};								// states
	const char* right_left[2] = {"Right","Left"};						// hand types
	const char* disabled_enabled[2] = {"Disabled","Enabled"};			// motor states
	const char* grips[NUM_GRIPS+1] = {"Fist","Palm","Thumbs Up","Point","Pinch","Tripod","Finger Roll"};  // grip names
	
};
extern struct textStringType textString;



// STORES SERIAL STRINGS IN PROGMEM, SAVES 3KB RAM
#define  FORCE_INLINE __attribute__((always_inline)) inline

FORCE_INLINE void serialprintPGM(const char *str)
{
	char ch = pgm_read_byte(str);
	while (ch)
	{
		MYSERIAL.write(ch);
		ch = pgm_read_byte(++str);
	}
}

#define MYSERIAL_PRINT(x) MYSERIAL.print(x);
#define MYSERIAL_PRINT_F(x,y) MYSERIAL.print(x,y);
#define MYSERIAL_PRINTLN(x) do {MYSERIAL.print(x);MYSERIAL.write('\n');} while(0)

#define MYSERIAL_PRINT_PGM(x) serialprintPGM(PSTR(x));
#define MYSERIAL_PRINTLN_PGM(x) do{serialprintPGM(PSTR(x));MYSERIAL.write('\n');} while(0)


// SOFTWARE VERSION NUMBER
#define VERSION_N  1.2			// 11/08/16

#endif /*_GLOBALS_h*/
