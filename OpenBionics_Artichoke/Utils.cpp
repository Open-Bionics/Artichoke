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
*	Utils.h
*
*/

#include <Arduino.h>			// for type definitions
#include <FingerLib.h>			// for MYSERIAL
#include "Globals.h"

#include "Utils.h"

// PIN TOGGLING
uint32_t togglePinNum;
volatile uint32_t *togglePinMode;
volatile uint32_t *togglePinOut;
static bool _togglePinState = LOW;

void pinToggleInit(int pinNum)
{
	togglePinNum = digitalPinToBitMask(pinNum);
	togglePinMode = (uint32_t *) portModeRegister(digitalPinToPort(pinNum));
	togglePinOut = (uint32_t *) portOutputRegister(digitalPinToPort(pinNum));
	
	// set pin 13 port as output
	*togglePinMode |= togglePinNum;
}

void togglePin(void)
{
	if(_togglePinState)
		*togglePinOut |= togglePinNum;
	else
		*togglePinOut &= ~togglePinNum;

	_togglePinState = !_togglePinState;
}

void togglePin(uint8_t INPUT_STATE)
{
	if(_togglePinState)
		*togglePinOut |= togglePinNum;
	else
		*togglePinOut &= ~togglePinNum;

	_togglePinState = !_togglePinState;
}


/*
// ARRAY MANAGEMENT
int arrayMean(int *inArray, int size)    //calculate mean of array
{
  int l;
  long total = 0;
  int result;

  for(l=0; l<size; l++)
  {
    total += inArray[l];
  }
  result = total / size;

  return result;
}

int rollingMean(int *inArray, int size, int val)		//calculate rolling mean of array
{
	int l;
	long total = 0;
	int result;

	for(l=0; l<size-1; l++)
	{
		inArray[l] = inArray[l+1];
		total += inArray[l];
	}
	inArray[size-1] = val;
	total += val;
	result = total / size;

	return result;
}

int rollingMode(int *inArray, int size, int val)    //calculate rolling mode of array
{
	int l;
	int max = 0;
  
	const int maxNumSize = 1024;  

	int modeArray[maxNumSize] = {0};    // initialise mode array

	for(l=0; l<size-1; l++)
	{
		inArray[l] = inArray[l+1];

		modeArray[inArray[l]] ++; // increment array pos corresponding to mode val
	}
	inArray[size-1] = val;
	modeArray[val] ++;          // increment array pos corresponding to mode val
  
	for(l=0;l<maxNumSize;l++)
	{
	if(modeArray[l]>modeArray[max])
	{
		max = l;
	}
	}
	return max;
}


void rollingBuff(int *inArray, int size, int val)   //calculate rolling mean of array
{
  int l;
  for(l=0; l<size-1; l++)
  {
    inArray[l] = inArray[l+1];

  }
  inArray[size-1] = val;
}*/

void printArray(int* inArray, int arraySize)	// print array over serial
{
	int w;
	for(w=0;w<arraySize;w++)
	{
		MYSERIAL_PRINT(w);
		MYSERIAL_PRINT_PGM(" ");
		MYSERIAL_PRINTLN(inArray[w]);
		//(inArray[w]==2)?MYSERIAL_PRINTLN_PGM("-"):MYSERIAL_PRINTLN_PGM(textString.open_close[inArray[w]]);
	}
	MYSERIAL_PRINTLN_PGM("****end of array****");
}

void printArray(char* inArray, int arraySize)	// print array over serial
{
	int w;
	for(w=0;w<arraySize;w++)
	{
		MYSERIAL_PRINT(w);
		MYSERIAL_PRINT_PGM(" ");
		MYSERIAL_PRINTLN(inArray[w]);
		//(inArray[w]==2)?MYSERIAL_PRINTLN_PGM("-"):MYSERIAL_PRINTLN_PGM(textString.open_close[inArray[w]]);
	}
	MYSERIAL_PRINTLN_PGM("****end of array****");
}

//void overwriteArray(int* inArray, int arraySize, int val)		// overwrite array with value
//{
//	int w;
//	for(w=0;w<arraySize;w++)
//	{
//		inArray[w]=val;
//	}
//}


bool isEven(int n)
{
	if (n % 2 == 0)
		return 1;
	else
		return 0;
}

// VARIABLE MAPPING
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


// CSV

// converts a CSV line from string to individual values within int array, returns number of variables
int convertFromCSV(char *inString, int *valArray)
{
	const char* CSV_seperator = ",";
	char *tmpVal, *tmpLoc;
	int tokenIndex = 0;
	tmpVal = strtok_r(inString, CSV_seperator, &tmpLoc);     // get pointer of first token
	while (tmpVal)
	{
		valArray[tokenIndex++] = atoi(tmpVal);

		tmpVal = strtok_r(NULL, CSV_seperator, &tmpLoc);      // continue searching through string
	}
	return tokenIndex;
}
