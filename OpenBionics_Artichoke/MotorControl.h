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
*	MotorControl.h
*
*/

#ifndef _MOTORCONTROL_H_
#define _MOTORCONTROL_H_


void fingerControl(int fingerNumber, int stopPos, int fingerDirection, int fingerSpeed);

void stopMotor(int fingerNumber);
void stopMotors(void);				// stop all motors;


#endif /*_MOTORCONTROL_H_*/
