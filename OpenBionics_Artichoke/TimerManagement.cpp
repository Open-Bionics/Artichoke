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
*	TimerManagement.h
*
*/

#include <Arduino.h>			// for type definitions
#include <FingerLib.h>			// for MYSERIAL	
#include "TimerManagement.h"


static uint32_t _secondCount = 0;

void timerSetup(void)      
{
  _attachFuncToTimer(milliSecInterrupt);		// attach function to 1ms FingerLib timer interrupt
}

void milliSecInterrupt(void)
{
	static uint32_t sCounter = 0;
	
	// increment counter every millisecond
	sCounter++;
	
	// if 1000ms has passed
	if(sCounter >= 1000)
	{
		sCounter = 0;
		_secondCount ++;
	}
	
	#ifdef ARDUINO_AVR_MEGA2560
	// 	static bool toggle = false;
	//
	//	// PA3 = DIO 5
	// 	if(toggle)
	// 	PORTA |= (1 << 3);
	// 	else
	// 	PORTA &= ~(1<<3);
	//
	// 	toggle = !toggle;
	#endif
	
	
	
// 	// latches a pin for a set duration
// 	if(latchIndex>=0)
// 		latchCounter();
// 
// 	// vibrates a haptic motor for a duration/pattern
// 	#ifdef USE_HAPTIC
// 	if(portExpander.runVibFlag!= -1)
// 		runVibration();
// 	#endif
}

double customSeconds(void)
{
	return _secondCount;
}



/*uint8_t ellapsedTime(uint16_t inTime, uint8_t index)
{
  static uint8_t start = 0;
  static uint16_t ellapsedCount = 0;
  static uint16_t ellapsedTarget = 0;

  static int startTime[255];

  if(!start)
  {
    start = 1;
    startTime[index] = customMillis();
  }

  if((customMillis() - startTime[index]) >= inTime)
  {
    start = 0;
    return 1;
  }
  else
  {
    return 0;
  }
}

uint8_t ellapsedTime(uint16_t inTime)
{
  return (ellapsedTime(inTime,0));
} */
