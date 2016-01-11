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
*	MotorControl
*
*/


void fingerControl(int fingerNumber, int stopPos, int fingerDirection, int fingerSpeed)
{
	if(fingerSpeed != BLANK)											// if speed is entered, set motor speed
		finger[fingerNumber].writeSpeed(fingerSpeed);
	if(stopPos != BLANK)												// if target pos is entered, set new motor pos
		finger[fingerNumber].writePos(map(stopPos,0,100,0,1023));
	else if(fingerDirection != BLANK)									// else if a direction is entered, set new motor direction
		finger[fingerNumber].writeDir(fingerDirection);
	else if((fingerDirection == BLANK) && (stopPos == BLANK))			// else if neither targ pos or direction are entered, toggle finger direction
		finger[fingerNumber].open_close();
}

void stopMotor(int fingerNumber)
{
	finger[fingerNumber].stopMotor();
}

void stopMotors(void)				// stop all motors
{
	stopMotor(0);
	stopMotor(1);
	stopMotor(2);
	stopMotor(3);
	stopMotor(4);
}
