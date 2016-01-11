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
*	MuscleSense
*
*/

// BUFFER SIZES
#define NUM_MUSC_SAMP	150
#define NUM_NOISE_SAMP	NUM_MUSC_SAMP
#define NUM_DIR_SAMP	200

// number of milliseconds before each grip change
#define GRIP_CHANGE_INTERVAL	700

// BUFFERS
static int readingsBuff[2][NUM_MUSC_SAMP];				// raw signal readings
static int noiseBuff[2][NUM_NOISE_SAMP];				// noise floor readings
static int armDirBuff[NUM_DIR_SAMP];					// calculated arm directions

// MUSCLE VALUES
static int noiseFloor[2];
static int thresh[2];

void muscleControl(void)
{
	bool activation[2] = {false,false};						// muscle activation flags
	int signal[2];											// muscle signal
	
	static bool initFlag = false;
	
	if(!initFlag)
	{
		initFlag = true;
		runNoiseFloorCalc();
	}
	
	// read muscle signals and store in a buffer
	signal[0] = readMuscle(0);
	signal[1] = readMuscle(1);
	
	// update noise floor & thresh if no signal activation
	checkActivation(signal,activation);
	
	// determine arm direction
	determineArmDirection(signal);
	
	// print ADC values if M3 is enabled
	if(muscle.printADCvals)
	printADCValues(signal, activation, armDirBuff[NUM_DIR_SAMP-1]);
	
	// if grip change enabled (M4 to toggle)
	if(advancedSettings.gripFlag)
	checkGripChange();
	
	// 	if standard muscle control enabled
	if(advancedSettings.muscleCtrlFlag == 1)
	standardMuscleControl(armDirBuff[NUM_DIR_SAMP-1]);
	
	// 	if positional muscle control enabled
	if(advancedSettings.muscleCtrlFlag == 2)
	positionalMuscleControl(signal);
}

void runNoiseFloorCalc(void)
{
	int signal[2];
	MYSERIAL.println("Calculating noise threshold, please stay relaxed.");
	for(int u=0;u<NUM_MUSC_SAMP;u++)
	{
		for(int m=0;m<2;m++)
		{
			signal[m] = readMuscle(m);
			noiseBuff[m][u] = signal[m];
		}
	}
	noiseFloor[0] = calcNoiseFloor(0, signal[0]);
	noiseFloor[1] = calcNoiseFloor(1, signal[1]);
	thresh[0] = calcThresh(0,noiseFloor[0]);
	thresh[1] = calcThresh(1,noiseFloor[1]);
	MYSERIAL.println("Complete");
}


int readMuscle(int muscleNum)		// reads a muscle sensor, stores in rolling serialCmd.buff, stores average in muscle.noiseFloor			(muscle.readings[muscleNum][NUM_SAMPLES-1] is current reading)
{
	int muscleValue;
	
	muscleValue = readMuscleSensor(muscleNum);							// read muscle value
	rollingBuff(readingsBuff[muscleNum],NUM_MUSC_SAMP,muscleValue);		// store value in rolling FIFO buffer
	return readingsBuff[muscleNum][NUM_MUSC_SAMP-1];					// return muscle value
}

int readMuscleSensor(int muscleNum)
{
	int readVal;
	
	// read muscle value over I2C
	#ifdef USE_I2C_ADC
	readVal = ADC2.read(muscleNum);
	#else
	// read muscle value from pin
	noInterrupts();
	readVal = analogRead(muscle.pins[muscleNum]);
	interrupts();
	#endif
	
	return readVal;
}

void checkActivation(int* signal, bool* activation)
{
	activation[0] = false;
	activation[1] = false;
	
	// generate activation flag
	if(signal[0] >= thresh[0])
	activation[0] = true;
	
	if(signal[1] >= thresh[1])
	activation[1] = true;
	
	// if not activated, calculate noise and recalculate thresh
	if(!activation[0])
	{
		noiseFloor[0] = calcNoiseFloor(0, signal[0]);
		thresh[0] = calcThresh(0,noiseFloor[0]);
	}
	
	if(!activation[1])
	{
		noiseFloor[1] = calcNoiseFloor(1, signal[1]);
		thresh[1] = calcThresh(1, noiseFloor[1]);

	}
}

int calcNoiseFloor(int muscleNum, int reading)
{
	return rollingMean(noiseBuff[muscleNum],NUM_NOISE_SAMP,reading);			// store reading in noise buffer and return the mean value
}

int calcThresh(int muscleNum, int noise)		// return threshold of (muscle.peakAverage-muscle.noiseFloor)/2 + muscle.noiseFloor
{
	return noise + userSettings.sensitivityOffset;
}

void determineArmDirection(int* signal)
{
	int over[2];
	
	// calculate the muscle impulse as any value over the threshold
	over[0] = ((signal[0] - thresh[0]) < 0)? 0 : (signal[0] - thresh[0]);
	over[1] = ((signal[1] - thresh[1]) < 0)? 0 : (signal[1] - thresh[1]);
	
	// add current arm direction to the end of the buffer
	if(over[0] > over[1])
	rollingBuff(armDirBuff,NUM_DIR_SAMP,OPEN);		// if OPEN
	else if(over[1] > over[0])
	rollingBuff(armDirBuff,NUM_DIR_SAMP,CLOSE);		// if CLOSE
	else
	rollingBuff(armDirBuff,NUM_DIR_SAMP,2);			// else NONE
}

void checkGripChange(void)
{
	if(gripChange() && handIsCurrentlyOpen())   // if double tap and in the open position
	{
		if(intervalEllapsed(GRIP_CHANGE_INTERVAL))				// if interval has passed, change grip   (prevents grips cycling too quickly)
		{
			MYSERIAL.println("Grip Change");
			(currentGrip < (NUM_GRIPS-1)) ? currentGrip++ : currentGrip = 0;		// change grip

			armDirBuff[NUM_DIR_SAMP-1] = 2;							// insert stopper in array to signal a hold is found
			currentDir = CLOSE;
			
			MYSERIAL.print("Grip ");
			MYSERIAL.println(textString.grips[currentGrip]);
		}
	}
}

int gripChange(void)						// search through prevArmDir array, if grip change detected return 1, else return 0
{
	int arrayPos;							// position within array
	int moreThan = userSettings.holdTime;	// number of directions in each sections to count as a double tap
	int dirVal = OPEN;						// direction sequence to search for/match with9
	int dirCount = 0;						// number of directions in each section
	
	for(arrayPos=NUM_DIR_SAMP-1;arrayPos>=0;arrayPos--)          // count backwards through array
	{
		if(armDirBuff[arrayPos] == dirVal)			// if element is desired dir, increment dir counter
		dirCount++;
		else										// if not next desired dir or next desired dir but number of current dir < morethan, break
		break;
	}
	
	// if grip change detected, return 1, else return 0
	if(dirCount >= moreThan)
		return 1;
	else
		return 0;
}

int handIsCurrentlyOpen(void)
{
	int fingerToRead = FINGER1;   // always read the position of finger 1 (index finger for all grips apart from thumbs up)
	int deltaTarget = 100;
	
	// if in thumbs up, check finger position
	if(currentGrip == THUMBSUP_GRIP)
		fingerToRead = FINGER0;

	if((finger[fingerToRead].readPos() - MIN_FINGER_POS) < deltaTarget)
		return 1;
	else
		return 0;
}

int intervalEllapsed(long interval)
{
	static long prevTime = 0;
	if((customMillis() - prevTime) >= interval)
	{
		prevTime = customMillis();
		return 1;
	}
	else
		return 0;
}
	
void standardMuscleControl(int muscleDir)
{
	if((muscleDir == OPEN) && (currentDir != OPEN))	// if OPEN
	{
		MYSERIAL.println("Arm OPEN");
		gripMovement(currentGrip,BLANK,OPEN,BLANK);
	}
	else if((muscleDir == CLOSE) && (currentDir != CLOSE))	// if CLOSE
	{
		MYSERIAL.println("Arm CLOSE");
		gripMovement(currentGrip,BLANK,CLOSE,BLANK);
	}
}

void positionalMuscleControl(int *signal)
{
	static float position;
	float increment, delta;
	
	int dir = armDirBuff[NUM_DIR_SAMP-1];

	// use Sine to map muscle signal to position to allow for increased sensitivity at slow speeds
	if(dir != 2)			// if direction is Open or Closed
	{
		delta = (float) (signal[dir] - thresh[dir]);			// magnitude of signal above the thresh
		delta = map(delta,0,(1023-thresh[dir]),0,90);			// remapped to 0 - 90
		
		delta = (float) delta/(180/PI);							// deg to rad
		increment = sin(delta);									// normalise to 0 - 1
		
		if(dir == OPEN)											// set dir
			increment = -increment;
		
		position += increment;
		position = constrain(position,0,100);					// set limits
	}

	gripMovement(currentGrip,(uint16_t)position,BLANK,MAX_FINGER_SPEED);
}

void printADCValues(int* signal, bool* activation, int dir)
{
	MYSERIAL.print("M0 = ");
	MYSERIAL.print(signal[0]);
	MYSERIAL.print("  T0 = ");
	MYSERIAL.print(thresh[0]);
	MYSERIAL.print("  N0 = ");
	MYSERIAL.print(noiseFloor[0]);
	MYSERIAL.print("  A0 = ");
	MYSERIAL.print(activation[0]);
	MYSERIAL.print("      M1 = ");
	MYSERIAL.print(signal[1]);
	MYSERIAL.print("  T1 = ");
	MYSERIAL.print(thresh[1]);
	MYSERIAL.print("  N1 = ");
	MYSERIAL.print(noiseFloor[1]);
	MYSERIAL.print("  A1 = ");
	MYSERIAL.print(activation[1]);
	MYSERIAL.print("   DIR ");
	MYSERIAL.println(textString.open_close[dir]);
}

void muscleGraph(void)
{
	if(!advancedSettings.muscleCtrlFlag)		// if muscle mode is disabled
	{
		readMuscle(0);
		readMuscle(1);
	}
	
	MYSERIAL.print(readingsBuff[0][NUM_MUSC_SAMP-1]);
	MYSERIAL.print(" ");
	MYSERIAL.print(thresh[0]);
	MYSERIAL.print(" ");
	MYSERIAL.print(readingsBuff[1][NUM_MUSC_SAMP-1]);
	MYSERIAL.print(" ");
	MYSERIAL.print(thresh[1]);
	MYSERIAL.print(" ");
	MYSERIAL.print('\r');
}