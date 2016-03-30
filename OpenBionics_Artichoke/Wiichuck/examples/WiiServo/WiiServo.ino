#include <Wiichuck.h>
#include <Wire.h>
#include <Servo.h> 

Wiichuck wii;
Servo horzServo;  
Servo vertServo;
const int horzServoPin = 3;
const int vertServoPin = 5;

void setup() {
  Serial.begin(9600);
  horzServo.attach(horzServoPin);
  vertServo.attach(vertServoPin);
  
  wii.init();  
  wii.calibrate();  // calibration
}

void loop() {
  if (wii.poll()) {
    int joyX = wii.joyX();
    int joyY = wii.joyY();
    
    if (joyX > 200) {
      // turn servo right
        horzServo.write(180);
    } else if (joyX < 40) {
      // turn servo left
        horzServo.write(0);
    }  
    
    if (joyY > 200) {
      // turn servo up
      vertServo.write(180);
    } else if (joyY < 40) {
      // turn servo down
      vertServo.write(0);
    }
    delay(15); 
  }
}
