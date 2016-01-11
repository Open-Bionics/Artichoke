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
*	CustomGrip
*
*/

/*void gripMovement(int grip, int dir)	// if no speed entered
{
	int pos = MIN_FINGER_POS;
	if(dir == OPEN)
		pos = MAX_FINGER_POS;
	
	gripMovement(grip,dir,pos,MAX_FINGER_SPEED);
}

void gripMovement(int grip, int dir, int pos, int speed)		// used when moving hand incrementally (muscle position control)
{
	static bool prevDir = 0;
	int i;
	int gripMaxPos, gripMinPos;
	currentGrip = grip;
	
	if(speed == BLANK)
		speed = MAX_FINGER_SPEED;
		
	if(dir == BLANK)
		dir = !prevDir;
		
	prevDir = dir;
		
	if(pos == BLANK)
		pos = (!dir) * 100;
	
	for(i=0;i<NUM_FINGERS;i++)
	{
    // map target position 0-100 to FULLY_CLOSED-FULLY_OPEN
	
		gripMinPos = gripPositions[CLOSE][grip][i];
		gripMaxPos = gripPositions[OPEN][grip][i];
		finger[i].writePos(map(pos,0,100,gripMinPos,gripMaxPos));
   
		// if fingers don't move in grip, use speed of 255, else use muscle controlled speed
		if(gripMinPos != gripMaxPos) 
		{
		  finger[i].writeSpeed(speed);
		}
		else 
		{
		  finger[i].writeSpeed(MAX_FINGER_SPEED);
	  }
	}
}
*/