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
*	Timer
*
*/

static uint32_t _secondCount = 0;

void timerSetup(void)      
{
  _attachFuncToTimer(milliSecInterrupt);
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

double customSeconds(void)
{
	return _secondCount;
}