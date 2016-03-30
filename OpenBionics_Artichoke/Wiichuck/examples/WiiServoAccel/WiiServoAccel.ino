#include <Wiichuck.h>
#include <Wire.h>
#include <Servo.h> 

Wiichuck wii;
Servo horzServo;  
Servo vertServo;
const int horzServoPin = 3;
const int vertServoPin = 5;
const int STEP = 20;    // in degrees

int horzPos = 0, vertPos = 0;

void setup() {
  Serial.begin(9600);
  horzServo.attach(horzServoPin);
  vertServo.attach(vertServoPin);
  
  horzServo.write(0); 
  vertServo.write(0);   
  
  wii.init();  
  wii.calibrate();  // calibration
}

void loop() {
  if (wii.poll()) {
    int accelX = wii.accelX();
    int accelY = wii.accelY();
    
    int hPos = map(accelX, 300, 700, 0, 180);
    int vPos = map(accelY, 300, 700, 0, 180); 
    
    if (abs(hPos - horzPos) >= STEP) {    
      horzPos = hPos;
      horzServo.write(horzPos);
    }
    
    if (abs(vPos - vertPos) >= STEP) {
      vertPos = vPos;
      vertServo.write(vertPos);
    }  
      
    Serial.print("accle:");
    Serial.print(accelX);
    Serial.print(", ");
    Serial.print(accelY);
    Serial.println("");    
  }
  
  delay(15); 
}
