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
*	Demo.h
*
*/

#ifndef _DEMO_H_
#define _DEMO_H_

#define PRINT_DEMO_GRIP

void demoMode(void);
void fingerRoll(int del, int dir);
void printGrip(int grip, int dir);
bool checkSerial(void);

extern int demoFlag;


#endif /*_DEMO_H_*/
