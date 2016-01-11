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
*	Animation
*
*/

#include "AnimationPositions.h"

/* 
 * Instead of using a lookup table, calculate the position of the finger by mapping the target input pos (inPos) to 
 * the values within the animation array. This also allows for the finger to remain at a set position for a period 
 * of time and then continue the grip pattern. 
 * 
 * The function looks through the array of positions for the specific grip and maps inPos to the grip position 
 * values within the array for each finger.
 * 
 * How it works:
 * - A target position (0-100) is input to the function along with the desired grip number
 * - If the target position is between two count values, the output target position of the finger is mapped 
 * between the grip position values within the array that correspond to the count values
 * - If a BLANK (-1) is encountered, the function looks at the next value within the array that isn't 
 * a BLANK (-1)
 * 
 */

// if some values are left blank, use defaults
void gripMovement(int gripNum, int inPos, int dir, int speed)
{
	int i;
	
	if(inPos == BLANK)
	{
		if(dir == OPEN)
			inPos = 0;
		if(dir == CLOSE)
			inPos = 100;
	}
	
	if(dir != BLANK)
		currentDir = dir;
		
	if(speed != BLANK)
	{
		for(i=0;i<NUM_FINGERS;i++)
		{
			finger[i].writeSpeed(speed);
		}
	}
	
	gripMovement(gripNum,inPos);
}

void gripMovement(int gripNum, int inPos)
{
  int fingerNum;
  int stepNum;
  int posA, posB;
  int countA, countB;
  int stepModA=0, stepModB=0;
  int currentCountVal, nextCountVal;

  inPos = constrain(inPos,0,100);

  for(stepNum=0;stepNum<NUM_ANIM_STEPS;stepNum++)
  {
    currentCountVal = animation[gripNum][stepNum][ANIM_COUNT_LOC];
    nextCountVal = animation[gripNum][stepNum+1][ANIM_COUNT_LOC];
	
    if(IS_BETWEEN(inPos,currentCountVal,nextCountVal))
    {
      for(fingerNum=0;fingerNum<NUM_FINGERS;fingerNum++)
      {
        do
        {
          posA = animation[gripNum][(stepNum-stepModA)][fingerNum+1];
          if(posA == -1) 
			stepModA ++;
        } while(posA == -1);
		
        do
        {
          posB = animation[gripNum][(stepNum+1+stepModB)][fingerNum+1];
          if(posB == -1) 
		  stepModB ++;
        } while(posB == -1);
		
        countA = animation[gripNum][(stepNum-stepModA)][ANIM_COUNT_LOC];
        countB = animation[gripNum][(stepNum+1+stepModB)][ANIM_COUNT_LOC];

        finger[fingerNum].writePos(map(inPos,countA,countB,posA,posB));  

        stepModA=0; 
        stepModB=0;
      }
      break;
    }
  }
}
