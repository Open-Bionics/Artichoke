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
*	PinManagement.h
*
*/

#ifndef _PINMANAGEMENT_H_
#define _PINMANAGEMENT_H_

// LEDS
#define LED_WRIST_PIN	16
#define LED_KNUCKLE_PIN	24
#define LED_EDGE_PIN	9

void IOconfig(void);   // assign pins for each finger, store pins within list and set as INPUT/OUTPUT
void pinAssignment(void);

void setWristLED(bool state);
void setKnuckleLED(bool state);
void setEdgeLED(bool state);

#ifdef USE_I2C_IO
int checkArmBtn(void);
#endif


// PIN LATCHING
// #define LATCH_DURATION     10
// #define LATCH_NUM_GPIO	   10
// #define LATCH_COUNT_LOC		0
// #define LATCH_PIN_LOC		1
// #define LATCH_STATE_LOC		2
// #define DIR1			0
// #define DIR2			1
// #define SENSE
// extern signed int latchIndex;				// index of the pin latch array holding the pin and count value
// extern int latchStack[LATCH_NUM_GPIO][3];			// array to hold the latching pin and the count duration
// void latchPin(uint8_t pin, uint8_t val, int duration);
// void latchCounter(void);
// void reorderStack(int delPos);

#endif /*_PINMANAGEMENT_H_*/