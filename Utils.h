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
*	Utils.h
*
*/
#include <Arduino.h>  // for type definitions


template <class T> int EEPROM_writeStruct(int ee, const T& value)
{
	const byte* p = (const byte*)(const void*)&value;
	unsigned int i;
	for (i = 0; i < sizeof(value); i++)
	{
		EEPROM.write(ee++, *p++);
	}
	return i;
}

template <class T> int EEPROM_readStruct(int ee, T& value)
{
	byte* p = (byte*)(void*)&value;
	unsigned int i;
	for (i = 0; i < sizeof(value); i++)
	{
		*p++ = EEPROM.read(ee++);
	}
	return i;
}

template <class T> int clearStruct(T& value, int newVal)
{
	byte* p = (byte*)(void*)&value;
	unsigned int i;
	for (i = 0; i < sizeof(value); i++)
	{
		*p++ = newVal;
	}
	return i;
}
