# Open Bionics - Artichoke

Open Bionics Robotics is the Open Source branch of the Open Bionics company, in 
Included in this repositry;

- Artichoke V1.2 - The firmware release for Open Bionics robotic hand kits, requires [FingerLib.h] ( https://github.com/Open-Bionics/FingerLib )


This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/


## Artichoke Release Notes

	Version	|	Date		|	Notes
	V1.0.0	|	08/01/16	|	Initial release for Ada hand and Almond boards using Atmega 2560
	V1.0.1	|	03/02/16	|	Modified formatting and cleaned up
	V1.1.0	|	31/03/16	|	Added research and HANDle mode. Fixed motorEn and muscle graph
	V1.1.1	|	17/05/16	|	Increased PWM timer freq to prevent hum and implemented customDelay() instead of delay()		
	V1.2.0	|	22/08/16	|	Re-written EMG control (now allows both 1 & 2 channel control)


	Artichoke Description

		- Simple hand control software designed to run on the Open Bionics Almond hand controller
		- Uses FingerLib.h for low level finger control, which allows fingers to be treated as servos
		- Can be controlled via the following methods:
			- Serial control (baud 38400)
			- Muscle control (EMG)
			- HANDle control (Nunchuck)
		- Uses either inbuilt ADC or external I2C ADC for muscle sensing and hand control
		- Enter '?' to view available serial commands
