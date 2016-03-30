Wiichuck
========

Wii Nunchuck Library for Arduino

Written by jnw.walker@gmail.com
Originally hosted on http://code.google.com/p/wiichuck/

Installation
============
1. Extract to the hardware/libraries directory in your Arduino install directory.
2. Compile by either reloading the Arduino IDE or selecting the board again.

Usage
=====
Create a new instance of this class with the default constructor:

  Wiichuck chuck;

Initialize the controller by specifying what pins you've connected VCC and GND to. Default with be analog pins 2 and 3. Specify 0 if you've connected the controller to VCC and GND instead of output pins.

  chuck.init();
  chuck.init(12, 13);
  chuck.init(0, 0);

Requests data from the nunchuck

  chuck.poll();

Sets the zero values to the last values read from the controller.

  chuck.calibrate();

Get the values of the joystick/accelerator/buttons

  chuck.joyX();
  chuck.joyY();

  chuck.accelX();
  chuck.accelY();
  chuck.accelZ();

  chuck.buttonZ();
  chuck.buttonC();   
