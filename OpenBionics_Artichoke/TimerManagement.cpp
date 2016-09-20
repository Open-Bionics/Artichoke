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
#include "TimerManagement.h"	// for customDelay()


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
}

// The timer frequencies have been changed, which 'breaks' delay() and millis()
double customSeconds(void)
{
	return _secondCount;
}


bool customDelay(double delVal)
{
	double _prevTime = 0;

	_prevTime = customMillis();

	while ((customMillis() - _prevTime) <= delVal)
	{
		delay(1);
	}

	return true;
}



////////////////////// NON_BLOCKING DELAY CLASS //////////////////////////

NB_DELAY::NB_DELAY()
{

}

void NB_DELAY::start(long delVal)
{
	_startTime = customMillis();
	_interval = delVal;
	_started = true;
}

bool NB_DELAY::finished(void)
{
	if ((customMillis() - _startTime) >= _interval)
	{
		_started = false;
		return 1;
	}
	else
		return 0;
}

bool NB_DELAY::started(void)
{
	return _started;
}

long NB_DELAY::now(void)
{
	if (!_started)
		return (-1);
	else
		return (customMillis() - _startTime);
}

void NB_DELAY::stop(void)
{
	_started = false;
}

bool NB_DELAY::timeEllapsed(long delVal)
{
	if (!started())			// if timer is not started (first run of timer)
	{
		start(delVal);		// start timer
		return false;		// duration has not ellapsed, so return false
	}
	if (finished())			// if timer is finished
	{
		start(delVal);		// restart timer
		return true;		// duration has ellapsed, so restart timer and return true
	}
	else
	{
		return false;
	}
}

////////////////////// NON_BLOCKING TIMER CLASS //////////////////////////
NB_TIMER::NB_TIMER()
{

}

void NB_TIMER::start(void)
{
	_startTime = customMillis();
	_started = true;
}

bool NB_TIMER::started(void)
{
	return _started;
}

long NB_TIMER::now(void)
{
	if (!_started)
		return 0;
	else
		return (customMillis() - _startTime);
}

long NB_TIMER::stop(void)
{
	long timeEllapsed;

	timeEllapsed = now();
	_started = false;

	return timeEllapsed;
}

bool NB_TIMER::timeEllapsed(long interval)
{
	if (now() >= interval)
		return true;
	else
		return false;
}
