#include <Wiichuck.h>
#include <Wire.h>

Wiichuck wii;

//#define MYSERIAL Serial
#define MYSERIAL SerialUSB

void setup() {
  MYSERIAL.begin(9600);
  wii.init();
  
  wii.calibrate();  // calibration
}

void loop() {
  if (wii.poll()) {
    MYSERIAL.print("joy:");
    MYSERIAL.print(wii.joyX());
    MYSERIAL.print(", ");
    MYSERIAL.print(wii.joyY());
    MYSERIAL.print("  \t");
    
    MYSERIAL.print("accel:");
    MYSERIAL.print(wii.accelX());
    MYSERIAL.print(", ");
    MYSERIAL.print(wii.accelY());
    MYSERIAL.print(", ");
    MYSERIAL.print(wii.accelZ());
    MYSERIAL.print("  \t");
    
    MYSERIAL.print("button:");
    MYSERIAL.print(wii.buttonC());
    MYSERIAL.print(", ");
    MYSERIAL.print(wii.buttonZ());
    MYSERIAL.println("");
  }
  
  delay(100);
}
