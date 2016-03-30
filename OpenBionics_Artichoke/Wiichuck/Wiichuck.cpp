#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "Wiichuck.h"
#include <Wire.h>

void Wiichuck::init(void) {
	// Set the output pins to VCC and GND
// 	if(powerPin > 0) {
// 	  pinMode(powerPin, OUTPUT);
// 	  digitalWrite(powerPin, HIGH);
// 	}
// 	
// 	if(groundPin > 0) {
// 	  pinMode(groundPin, OUTPUT);
// 	  digitalWrite(groundPin, LOW);
// 	}
	
	delay(100);
	
	Wire.begin();
	Wire.beginTransmission(Address);
#if (ARDUINO >= 100)    
	Wire.write(0x40);
	Wire.write((uint8_t)0x00);
#else
	Wire.send(0x40);
	Wire.send((uint8_t)0x00);
#endif    
	Wire.endTransmission();
	
	// Set default calibration
	calib.joyX = calib.joyY = 128;
	calib.accelX = calib.accelY = calib.accelZ = 125; // accel and lsb together == 500.
	calib.lsbX = calib.lsbY = calib.lsbZ = 0;
}

uint8_t Wiichuck::poll() {
	Wire.requestFrom(Address, 6);// request data from nunchuck
      
	int bytes = 0;
	while(Wire.available() && bytes < 6) {
		// receive uint8_t as an integer
#if (ARDUINO >= 100)            
		data.buffer[bytes++] = decode(Wire.read());
#else
		data.buffer[bytes++] = decode(Wire.receive());
#endif        
	}
    delay(5);
	// send request for next data payload
	Wire.beginTransmission(Address);
#if (ARDUINO >= 100)        
	Wire.write((uint8_t)0x00); 
#else
	Wire.send((uint8_t)0x00); 
#endif
	Wire.endTransmission();
      
	/*delay(100);*/
	return bytes >= 5;
}

void Wiichuck::calibrate() {
	calib.joyX = data.parsed.joyX;
	calib.joyY = data.parsed.joyY;
	calib.accelX = data.parsed.accelX;
	calib.accelY = data.parsed.accelY;
	calib.accelZ = data.parsed.accelZ;
	calib.lsbX = data.parsed.lsbX;
	calib.lsbY = data.parsed.lsbY;
	calib.lsbZ = data.parsed.lsbZ;
}

int Wiichuck::getMidY()
{
	return data.parsed.joyY;
}

