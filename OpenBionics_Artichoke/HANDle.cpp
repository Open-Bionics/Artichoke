/*	Open Bionics - Artichoke
*	Author - Olly McBride
*	Date - January 2016
*
*	This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
*	To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
*
*	Website - http://www.openbionics.com/
*	GitHub - https://github.com/Open-Bionics
*	Email - ollymcbride@openbionics.com
*
*	HANDle.cpp
*
*/

#include <Arduino.h>			// for type definitions
#include <FingerLib.h>			// for MYSERIAL
#include "Globals.h"

#if defined(HANDLE_EN)
#include "Wiichuck/Wiichuck.h"				// Nunchuck library, written by jnw.walker@gmail.com
#include "HANDle.h"
#include "Animation.h"
#include "TimerManagement.h"

#define MID_TOLERANCE 0.04					// position tolerance from center

Wiichuck wii;
bool HANDleSerialFlag = false;

void HANDleMain(void)
{
	static bool initialised = false;		// initialised flag
	static float HANDlePos = 0;				// starting pos for the HANDle
	
	float yPos, increment;
	
	// configuration
	if(!initialised)
	{
		Wire.begin();
		wii.init();

		MYSERIAL.println("Calibrating mid position of joystick");
		wii.poll();
		customDelay(100);
		wii.poll();
		wii.calibrate();  // calibration (needs a poll before hand to work)
		MYSERIAL.println("Complete");
		initialised = true;
	}
	
	wii.poll();
	
	if(wii.buttonC()&&wii.buttonZ())				// if both buttons are pressed at the same time, calibrate mid position of the joystick
	{
		MYSERIAL.println("Calibrating mid position of joystick");
		wii.poll();
		customDelay(100);
		wii.poll();
		wii.calibrate();  // calibration (needs a poll before hand to work)
	}
	
	if(wii.buttonC())							// if one button press, change grip on a button press 
	{
		currentGrip ++;
		if(currentGrip >= NUM_GRIPS)
			currentGrip = 0;

		MYSERIAL.print("Grip number ");
		MYSERIAL.print(currentGrip);
		MYSERIAL.print("  ");
		MYSERIAL.println(textString.grips[currentGrip]);
		
		customDelay(200);
	}
	if(wii.buttonZ())
	{
		currentGrip --;
		if(currentGrip < 0)
			currentGrip = NUM_GRIPS-1;
	
			MYSERIAL.print("Grip number ");
			MYSERIAL.print(currentGrip);
			MYSERIAL.print("  ");
			MYSERIAL.println(textString.grips[currentGrip]);
		
		customDelay(200);
	}	
	
	yPos = wii.joyY();
	
	if(HANDleSerialFlag)
	{
		MYSERIAL.print("Grip ");
		MYSERIAL.print(currentGrip);
		MYSERIAL.print("  \tjoyPos ");
		MYSERIAL.print(yPos);
	}
	
	yPos = constrain(yPos,-90,90);
	yPos = (float) yPos/(180/PI);			// deg to rad

	increment = 2*sin(yPos);
	
	if(abs(yPos) <= MID_TOLERANCE)
	{
		increment = 0;
		yPos = 0;
	}
	
	HANDlePos += increment;
	HANDlePos = constrain(HANDlePos,0,100);
		
	gripMovement(currentGrip,HANDlePos);
	
	
	
	if(HANDleSerialFlag)
	{
		MYSERIAL.print("  \tHANDle pos ");
		MYSERIAL.print(yPos);
		MYSERIAL.print("  \tHand pos ");
		MYSERIAL.println((int) HANDlePos);
	}
	else
	{
		customDelay(10);		// delay to counteract time spent printing to serial (required as Nunchuck IC is slow)
	}

}


void toggleHANDleSerial(void)
{
	HANDleSerialFlag = !HANDleSerialFlag;
	MYSERIAL.print("HANDle Serial ");
	MYSERIAL.println(textString.disabled_enabled[HANDleSerialFlag]);
}

#endif /*HANDLE_EN*/