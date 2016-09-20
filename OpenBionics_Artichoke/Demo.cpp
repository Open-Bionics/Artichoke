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
*	Demo.cpp
*
*/

#include <Arduino.h>			// for type definitions
#include <FingerLib.h>			// for MYSERIAL
#include "Globals.h"
#include "Demo.h"
#include "GripControl.h"
#include "MotorControl.h"
#include "TimerManagement.h"

int demoFlag = 0;
int gripDuration = 1000;    // amount of time each grip runs in demo mode (milliseconds)
int gripSpeed = 230;

void demoMode(void)
{
	MYSERIAL_PRINTLN_PGM("Demo starting");
	
	printGrip(FIST_GRIP,OPEN);
	gripMovement(FIST_GRIP,BLANK,OPEN, gripSpeed);
 if(checkSerial())
 return;
	customDelay(gripDuration);
	
	printGrip(FIST_GRIP,CLOSE);
	gripMovement(FIST_GRIP,BLANK,CLOSE, gripSpeed);
  if(checkSerial())
  return;
	customDelay(gripDuration);
	
	printGrip(FIST_GRIP,OPEN);
	gripMovement(FIST_GRIP,BLANK,OPEN, gripSpeed);
  if(checkSerial())
  return;
	customDelay(gripDuration);
	
	printGrip(FINGER_ROLL,CLOSE);
	fingerRoll(300,CLOSE);
	if (checkSerial())
		return;
	customDelay(gripDuration / 4);
	
	printGrip(FINGER_ROLL,OPEN);
	fingerRoll(300,OPEN);
	if (checkSerial())
		return;
	customDelay(gripDuration / 2);
	
	printGrip(FINGER_ROLL,CLOSE);
	fingerRoll(300,CLOSE);
  if(checkSerial())
  return;
	customDelay(gripDuration/2);
	
	printGrip(FINGER_ROLL,OPEN);
	fingerRoll(300,OPEN);
	
	printGrip(FINGER_ROLL,CLOSE);
	fingerRoll(300,CLOSE);
  if(checkSerial())
  return;
	customDelay(gripDuration/4);
	
	printGrip(FIST_GRIP,OPEN);
	gripMovement(FIST_GRIP,BLANK,OPEN, gripSpeed);
  if(checkSerial())
  return;
	customDelay(gripDuration);

	for(int i=0;i<NUM_GRIPS;i++)
	{
		printGrip(i,CLOSE);
		gripMovement(i,BLANK,CLOSE, gripSpeed);
    if(checkSerial())
    return;
		customDelay(gripDuration);
		
		printGrip(i,OPEN);
		gripMovement(i,BLANK,OPEN, gripSpeed);
    if(checkSerial())
    return;
		customDelay(gripDuration);
	}
	
	#ifdef TRIPOD_GRIP
	printGrip(TRIPOD_GRIP,CLOSE);
	gripMovement(TRIPOD_GRIP,BLANK,CLOSE, gripSpeed);
  if(checkSerial())
  return;
	customDelay(gripDuration);
	
	printGrip(TRIPOD_GRIP,OPEN);
	gripMovement(TRIPOD_GRIP,BLANK,OPEN, gripSpeed);
  if(checkSerial())
  return;
	customDelay(gripDuration);
	#endif
	
	printGrip(PINCH_GRIP,CLOSE);
	gripMovement(PINCH_GRIP,BLANK,CLOSE, gripSpeed);
  if(checkSerial())
  return;
	customDelay(gripDuration);
	
	printGrip(FIST_GRIP,OPEN);
	gripMovement(FIST_GRIP,BLANK,OPEN, gripSpeed);
  if(checkSerial())
  return;
	customDelay(gripDuration);
	
	printGrip(FINGER_ROLL,CLOSE);
	fingerRoll(430,CLOSE);
	customDelay(gripDuration/2);
	
	printGrip(FIST_GRIP,OPEN);
	gripMovement(FIST_GRIP,BLANK,OPEN, gripSpeed);
  if(checkSerial())
  return;
	
	if(!advancedSettings.demoFlag)
		demoFlag = 0;

	MYSERIAL_PRINTLN_PGM("Demo complete");
	
	// if demo mode is enabled from start up
	if(advancedSettings.demoFlag)
	{
		MYSERIAL_PRINTLN_PGM("\nStart up demo mode ON");
		MYSERIAL_PRINTLN_PGM("Enter A0 to disable this mode");
	}
}

void fingerRoll(int del, int dir)
{
	if(dir==CLOSE)
	{
		for(int i=NUM_FINGERS-1;i>=0;i--)
		{
			fingerControl(i,100,0, gripSpeed);
      if(checkSerial())
		return;
			customDelay(del);
		}
	}
	else
	{
		for(int i=0;i<NUM_FINGERS;i++)
		{
			fingerControl(i,0,0, gripSpeed);
      if(checkSerial())
		return;
			customDelay(del);
		}
	}
}

bool checkSerial(void)
{
  if(MYSERIAL.available())
  {
    demoFlag = 0;
    return 1;
  }
  else
  {
    return 0;
  }
}

void printGrip(int grip, int dir)
{
  #ifdef PRINT_DEMO_GRIP
    MYSERIAL_PRINT(textString.grips[grip]);
    MYSERIAL_PRINT_PGM(" ");
    MYSERIAL_PRINTLN(textString.open_close[dir]);
  #endif
}
