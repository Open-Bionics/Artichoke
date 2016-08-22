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

#ifndef _TIMER_MANAGEMENT_H_
#define _TIMER_MANAGEMENT_H_

void timerSetup(void);
void milliSecInterrupt(void);
double customSeconds(void);
bool customDelay(double delVal);



// NON_BLOCKING DELAY CLASS
class NB_DELAY
{
public:
	NB_DELAY();
	void start(long delVal);
	bool finished(void);
	bool started(void);
	long now(void);
	void stop(void);

	bool timeEllapsed(long delVal);
private:
	long _startTime;
	long _interval;
	bool _started = false;
};

// NON_BLOCKING TIMER CLASS
class NB_TIMER
{
public:
	NB_TIMER();
	void start(void);
	bool started(void);
	long now(void);
	long stop(void);

	bool timeEllapsed(long interval);
private:
	long _startTime;
	bool _started = false;

};

#endif /*_TIMER_MANAGEMENT_H_*/