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
*	Utils
*
*/

#define RESOLUTION 1024    // resolution of ADC 

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
  int modeArray[RESOLUTION] = {0};    // initialise mode array

  for(l=0; l<size-1; l++)
  {
    inArray[l] = inArray[l+1];

    modeArray[inArray[l]] ++; // increment array pos corresponding to mode val
  }
  inArray[size-1] = val;
  modeArray[val] ++;          // increment array pos corresponding to mode val
  
  for(l=0;l<RESOLUTION;l++)
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
}

void printArray(int* inArray, int arraySize)	// print array over serial
{
	int w;
	for(w=0;w<arraySize;w++)
	{
		MYSERIAL.print(w);
		MYSERIAL.print(" ");
		MYSERIAL.println(inArray[w]);
	}
	MYSERIAL.println("****end of array****");
}

void printArray(char* inArray, int arraySize)	// print array over serial
{
	int w;
	for(w=0;w<arraySize;w++)
	{
		MYSERIAL.print(w);
		MYSERIAL.print(" ");
		MYSERIAL.println(inArray[w]);
	}
	MYSERIAL.println("****end of array****");
}

void overwriteArray(int* inArray, int arraySize, int val)		// overwrite array with value
{
	int w;
	for(w=0;w<arraySize;w++)
	{
		inArray[w]=val;
	}
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

