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
*	PinManagement
*
*/

void IOconfig(void)   // assign pins for each finger, store pins within list and set as INPUT/OUTPUT
{
	pinAssignment();

	#ifdef USE_I2C_ADC
		// if using I2C over the headphone port, pull the SCL and SDA high using A6 & A7, as they are connected via 10k pullups
		pinMode(A6,OUTPUT);
		pinMode(A7,OUTPUT);
		digitalWrite(A6,HIGH);
		digitalWrite(A7,HIGH);
		
		ADC2.begin();            // initialise I2C_ADC for muscle sensing
	#else
		pinMode(muscle.pins[0],INPUT);		// if using standard ADC, config pins
		pinMode(muscle.pins[1],INPUT);
	#endif
}

void pinAssignment(void)
{
	if(advancedSettings.handFlag == RIGHT)
	{
		finger[0].attach(5,2,A0);					// attach the direction pins and the analog sense pins for each motor
		finger[1].attach(3,6,A1);					// attach the direction pins and the analog sense pins for each motor
		finger[2].attach(7,8,A2);					// attach the direction pins and the analog sense pins for each motor
		finger[3].attach(10,9,A3);					// attach the direction pins and the analog sense pins for each motor
		finger[4].attach(11,12,A4);					// attach the direction pins and the analog sense pins for each motor
	}
	else if(advancedSettings.handFlag == LEFT)
	{
		finger[0].attach(5,2,A0);					// attach the direction pins and the analog sense pins for each motor
		finger[1].attach(11,12,A4);					// attach the direction pins and the analog sense pins for each motor
		finger[2].attach(10,9,A3);					// attach the direction pins and the analog sense pins for each motor
		finger[3].attach(7,8,A2);					// attach the direction pins and the analog sense pins for each motor
		finger[4].attach(3,6,A1);					// attach the direction pins and the analog sense pins for each motor
	}
	
	muscle.pins[0] = A6;
	muscle.pins[1] = A7;
}
