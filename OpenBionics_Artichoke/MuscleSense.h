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
*	MuscleSense.h
*
*/

#ifndef _MUSCLESENSE_H_
#define _MUSCLESENSE_H_

// BUFFER SIZES
#define NUM_MUSC_SAMP	150
#define NUM_NOISE_SAMP	NUM_MUSC_SAMP
#define NUM_DIR_SAMP	200

// number of milliseconds before each grip change
#define GRIP_CHANGE_INTERVAL	700

void muscleControl(void);
void runNoiseFloorCalc(void);

int readMuscle(int muscleNum);		// reads a muscle sensor, stores in rolling serialCmd.buff, stores average in muscle.noiseFloor			(muscle.readings[muscleNum][NUM_SAMPLES-1] is current reading)
int readMuscleSensor(int muscleNum);

void checkActivation(int* signal, bool* activation);
int calcNoiseFloor(int muscleNum, int reading);
int calcThresh(int muscleNum, int noise);		// return threshold of (muscle.peakAverage-muscle.noiseFloor)/2 + muscle.noiseFloor

void determineArmDirection(int* signal);
void checkGripChange(void);
int gripChange(void);						// search through prevArmDir array, if grip change detected return 1, else return 0
int handIsCurrentlyOpen(void);
int intervalEllapsed(long interval);

void standardMuscleControl(int muscleDir);
void positionalMuscleControl(int *signal);

void printADCValues(int* signal, bool* activation, int dir);
void muscleGraph(void);

extern int musclePins[2];			// muscle sense ADC pin names
extern int printADCvals;			// flag to print muscle ADC readings
extern int noiseFloorFlag;			// flag to indicate if noise floor has been calculated

#endif /*_MUSCLESENSE_H_*/