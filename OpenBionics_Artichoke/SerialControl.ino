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
*	SerialControl
*
*/

void serialEvent()
{
	getCmd();
	if(serialCmd.cmdsToGet)
	{
		serialCmd.cmdsToGet--;
		processCommand();
		manageSerialSettings();         // toggle demo & muscle mode on/off
	}
}

bool codeSeen(char code)
{
	serialCmd.strPtr = strchr(serialCmd.cmdBuff, code);
	return (serialCmd.strPtr != NULL);  //Return True if a character was found
}

float codeVal()
{
  return (strtod(&serialCmd.cmdBuff[serialCmd.strPtr - serialCmd.cmdBuff + 1], NULL));
}

float parsenumber(char code) 
{
	if(codeSeen(code))
		return codeVal();
	else
		return BLANK;
}

void getCmd(void)
{
	char serialChar;
	int buffCount = 0;

	// while there's data in the Serial buffer
	while(MYSERIAL.available() > 0)
	{
		delay(1);							// allow time for buffer to free up
		serialChar = MYSERIAL.read();
		serialCmd.cmdBuff[buffCount++] = serialChar;
	}
	
	// once the serial buffer is clear, set flag to indicate a received command to process and print the received command
	if(buffCount > 0)
	{
		serialCmd.cmdBuff[buffCount] = 0;	//terminate string
		MYSERIAL.print("\n");
		MYSERIAL.println(serialCmd.cmdBuff);
		serialCmd.cmdsToGet++;
		buffCount = 0;					//clear buffer
	}
}

void processCommand(void)    // interpret and store received serial commands
{
	// return numbers after char in serialCmd.buff
	serialCmd.advancedFlag = parsenumber('A');      // A advanced control
	serialCmd.demoFlag = parsenumber('D');          // D demo mode
	serialCmd.fingerNum = parsenumber('F');         // F finger number
	serialCmd.gripNum = parsenumber('G');           // G predefined grips
	serialCmd.handFlag = parsenumber('H');          // H left or right hand
	serialCmd.muscleCtrlFlag = parsenumber('M');    // M muscle control
	serialCmd.noiseFloor = parsenumber('N');        // N calculate noise floor
	serialCmd.stopPos = parsenumber('P');           // P stop position
	serialCmd.speed = parsenumber('S');             // S speed
	serialCmd.holdTime = parsenumber('T');          // T amount of time for muscle to be tensed for a grip change
	serialCmd.sensitivityAdjust = parsenumber('U'); // U update sensitivity offset
	serialCmd.instructionsFlag = parsenumber('?');  // ? sends serial instructions

	// direction control       
	if(parsenumber('O') == 0)                       // O OPEN
		serialCmd.direction = OPEN;
	else if(parsenumber('C') == 0)                  // C CLOSE
		serialCmd.direction = CLOSE;
	else
	{                           // if a grip is entered without a direction
		if(serialCmd.gripNum != BLANK)
		{
			currentDir = !currentDir;
			serialCmd.direction = currentDir;     // toggle grip direction
		}
	}

	constrainReceivedValues();
}

void constrainReceivedValues(void)
{
	if(serialCmd.fingerNum != BLANK)
		serialCmd.fingerNum = constrain(serialCmd.fingerNum, 0,NUM_FINGERS-1);

	if(serialCmd.stopPos != BLANK)
		serialCmd.stopPos = constrain(serialCmd.stopPos, 0,100);

	if(serialCmd.direction != BLANK)
		serialCmd.direction = constrain(serialCmd.direction, 0,1);

	if(serialCmd.gripNum != BLANK)
		serialCmd.gripNum = constrain(serialCmd.gripNum, 0,NUM_GRIPS-1);

	if(serialCmd.speed != BLANK)
		serialCmd.speed = constrain(serialCmd.speed, 0,MAX_FINGER_SPEED);

	if(serialCmd.stopPos != BLANK)
		serialCmd.stopPos = constrain(serialCmd.stopPos, 0,100);

	if(serialCmd.advancedFlag != BLANK)
		serialCmd.advancedFlag = constrain(serialCmd.advancedFlag, 0,5);

	if(serialCmd.muscleCtrlFlag != BLANK)
		serialCmd.muscleCtrlFlag = constrain(serialCmd.muscleCtrlFlag, 0,5);

	if(serialCmd.handFlag != BLANK)
		serialCmd.handFlag = constrain(serialCmd.handFlag, 0,2);

	if(serialCmd.demoFlag != BLANK)
		serialCmd.demoFlag = constrain(serialCmd.demoFlag, 0,1);

	if(serialCmd.instructionsFlag != BLANK)
		serialCmd.instructionsFlag = constrain(serialCmd.instructionsFlag, 0,1);

	if(serialCmd.sensitivityAdjust != BLANK)
		serialCmd.sensitivityAdjust = constrain(serialCmd.sensitivityAdjust, 0,1024);

	if(serialCmd.resetFlag != BLANK)
		serialCmd.resetFlag = constrain(serialCmd.resetFlag, 0,1);

	if(serialCmd.noiseFloor != BLANK)
		serialCmd.noiseFloor = constrain(serialCmd.noiseFloor, 0,1024);
}

void manageSerialSettings(void)
{
	if(serialCmd.fingerNum != BLANK)
	{
		MYSERIAL.print("\n");
		MYSERIAL.print("Finger ");
		MYSERIAL.println(serialCmd.fingerNum);
		
		MYSERIAL.print("Direction ");
		if(serialCmd.direction != BLANK)
			MYSERIAL.println(textString.open_close[serialCmd.direction]);
		else
		{	
			int fingerDir = !finger[serialCmd.fingerNum].readDir();
			MYSERIAL.println(textString.open_close[fingerDir]);
		}
		
		MYSERIAL.print("Stop Position ");
		if(serialCmd.stopPos != BLANK)
			MYSERIAL.println(serialCmd.stopPos);
		else
		MYSERIAL.println("None");
		
		MYSERIAL.print("Speed ");
		if(serialCmd.speed != BLANK)
			MYSERIAL.println(serialCmd.speed);
		else
			MYSERIAL.println(finger[serialCmd.fingerNum].readTargSpeed());
	}
	else if(serialCmd.gripNum != BLANK)
	{
		MYSERIAL.print("Grip ");
		MYSERIAL.println(textString.grips[serialCmd.gripNum]);
		
		MYSERIAL.print("Direction ");
		if(serialCmd.direction != BLANK)
			MYSERIAL.println(textString.open_close[serialCmd.direction]);
		else
			MYSERIAL.println("None");
		
		MYSERIAL.print("Stop Position ");
		if(serialCmd.stopPos != BLANK)
			MYSERIAL.println(serialCmd.stopPos);
		else
			MYSERIAL.println((100 * serialCmd.direction));
			
		MYSERIAL.print("Speed ");
		if(serialCmd.speed != BLANK)
			MYSERIAL.println(serialCmd.speed);
		else
		{
			int fingerToRead = (serialCmd.gripNum==THUMBSUP_GRIP)?FINGER0:FINGER1;
			MYSERIAL.println(finger[fingerToRead].readTargSpeed());
		}
			
		MYSERIAL.print("\n");
	}
  
	else if(serialCmd.instructionsFlag != BLANK)
	{
		printInstructions();
	}
  
	else if(serialCmd.advancedFlag != BLANK)
	{
		switch(serialCmd.advancedFlag)
		{
			case 0: // demo mode
				advancedSettings.demoFlag = !advancedSettings.demoFlag;   // toggle flag
				demo.flag = advancedSettings.demoFlag;      
				MYSERIAL.print("Demo mode toggled ");
				MYSERIAL.println(textString.off_on[advancedSettings.demoFlag]); // print ON/OFF
				break;
			case 1: // serial instructions
				advancedSettings.instructionsFlag = !advancedSettings.instructionsFlag; // toggle flag
				MYSERIAL.print("Initial serial instructions toggled ");
				MYSERIAL.println(textString.off_on[advancedSettings.instructionsFlag]); // print ON/OFF
				break;
			case 2: // muscle graph
				advancedSettings.muscleGraphFlag = !advancedSettings.muscleGraphFlag; // toggle flag
				MYSERIAL.print("Muscle graph mode toggled ");
				MYSERIAL.println(textString.off_on[advancedSettings.muscleGraphFlag]);  // print ON/OFF
				break;
			case 3: // motor enable/disable
				advancedSettings.motorEnable = !advancedSettings.motorEnable;   // toggle flag
				MYSERIAL.print("Motors ");
				
				MYSERIAL.println(textString.disabled_enabled[advancedSettings.motorEnable]);  // print Disabled/Enabled
				for(int i=0;i<NUM_FINGERS;i++)
				{
					if(advancedSettings.motorEnable)
						finger[i].enableMotor();
					else
						finger[i].disableMotor();
				}
				break;
			case 5: // motors emergency stop
				stopMotors();
				MYSERIAL.println("Stop Motors");
				break;
			default:
				break;
		}
		EEPROM_writeStruct(ADVANCED_CTRL_LOC,advancedSettings);   // save settings to EEPROM
	}
  
	else if(serialCmd.muscleCtrlFlag != BLANK)
	{
		switch(serialCmd.muscleCtrlFlag)
		{
			case -1:            // default
				break;
			case 0:             // muscle control off
				advancedSettings.muscleCtrlFlag = 0;
				MYSERIAL.println("Muscle control OFF ");
				digitalWrite(LED_KNUCKLE,LOW);  // turn off muscle control LED in knuckle
				break;
			case 1:             // standard muscle control
				advancedSettings.muscleCtrlFlag = 1;
				MYSERIAL.println("Standard muscle control ON");
				digitalWrite(LED_KNUCKLE,HIGH);   // turn on muscle control LED in knuckle
				break;
			case 2:             // position muscle control
				advancedSettings.muscleCtrlFlag = 2;
				MYSERIAL.println("Muscle position control ON");
				digitalWrite(LED_KNUCKLE,HIGH);   // turn on muscle control LED in knuckle
				break;
			case 3:             // print muscle readings
				if(advancedSettings.muscleCtrlFlag != 0)
				{
					muscle.printADCvals = !muscle.printADCvals;
					MYSERIAL.print("Display muscle readings ");
					MYSERIAL.println(textString.off_on[muscle.printADCvals]);     // print ON/OFF
				}
				break;
			case 4:             // enable/disable grip change
				if(advancedSettings.muscleCtrlFlag != 0)
				{
					advancedSettings.gripFlag = !advancedSettings.gripFlag;
					MYSERIAL.print("Grip Change Mode ");
					MYSERIAL.println(textString.disabled_enabled[advancedSettings.gripFlag]);     // print Disabled/Enabled
				}
				break;
			default:
				break;
		}
		if(serialCmd.muscleCtrlFlag != 4)
		{
			MYSERIAL.print("Grip Change Mode ");
			MYSERIAL.println(textString.off_on[advancedSettings.gripFlag]);
		}
      
		EEPROM_writeStruct(ADVANCED_CTRL_LOC,advancedSettings);   // store muscle mode in EEPROM
	}
 
	else if(serialCmd.handFlag != BLANK)  // if 'H#'
	{
		if(serialCmd.handFlag != 0)
		{
			advancedSettings.handFlag = serialCmd.handFlag;
			IOconfig();             // reconfigure finger pins
			EEPROM_writeStruct(ADVANCED_CTRL_LOC,advancedSettings);    // store hand in EEPROM
		}
		MYSERIAL.print("Hand is ");
		MYSERIAL.println(textString.right_left[advancedSettings.handFlag-1]); // print which hand is entered
	}
 
	else if(serialCmd.demoFlag == 0) // if 'D'
	{
		demo.flag = 1;
		MYSERIAL.println("Demo Mode");
		MYSERIAL.println(" The hand is only responsive to serial commands\n at the end of each demo cycle");
	}
  
	else if(serialCmd.sensitivityAdjust != BLANK)
	{
		if(serialCmd.sensitivityAdjust != 0)
		{
			userSettings.sensitivityOffset = serialCmd.sensitivityAdjust;
			EEPROM_writeStruct(USER_SETTINGS_LOC,userSettings);
		}
		MYSERIAL.print("Muscle sensor sensitivity ");
		MYSERIAL.println(userSettings.sensitivityOffset);
	}
  
	else if(serialCmd.noiseFloor != BLANK)
	{
		runNoiseFloorCalc();
	}

	else if(serialCmd.holdTime != BLANK)
	{
		if(serialCmd.holdTime != 0)
		{
			userSettings.holdTime = serialCmd.holdTime;
			EEPROM_writeStruct(USER_SETTINGS_LOC,userSettings);
		}
		MYSERIAL.print("Grip change hold duration ");
		MYSERIAL.println(userSettings.holdTime);
	}
}

void setDefaults(void)
{
	// read EEPROM settings
	EEPROM_readStruct(USER_SETTINGS_LOC,userSettings);
	EEPROM_readStruct(ADVANCED_CTRL_LOC,advancedSettings);

	if(advancedSettings.initConfigFlag != true) 
		initialEEPROMconfig();		// if running for the first time, write default values to EEPROM
			
	currentGrip = FIST_GRIP;
	demo.flag = advancedSettings.demoFlag;
	
	clearAll();       // set all serial/command serialCmd.buff variables to -1

}

void clearAll(void)    //clear all input variables for the next string
{
	int i;
	for(i=0;i<MAX_BUF;i++)
	{
		serialCmd.cmdBuff[i] = 0;
	}

	serialCmd.fingerNum = BLANK;
	serialCmd.direction = BLANK;
	serialCmd.stopPos = BLANK;
	serialCmd.speed = BLANK;
	serialCmd.gripNum = BLANK;
	serialCmd.advancedFlag = BLANK;
	serialCmd.muscleCtrlFlag = BLANK;
	serialCmd.instructionsFlag = BLANK;
	serialCmd.handFlag = BLANK;
	serialCmd.demoFlag = BLANK;
	serialCmd.sensitivityAdjust = BLANK;
	serialCmd.resetFlag = BLANK;
	serialCmd.noiseFloor = BLANK;
	serialCmd.holdTime = BLANK;
}

void initialEEPROMconfig(void)			// write default values to EEPROM
{
	MYSERIAL.print("Initialising EEPROM values...");
	
	advancedSettings.handFlag = RIGHT;
	advancedSettings.instructionsFlag = ON;
	advancedSettings.muscleCtrlFlag = OFF;
	advancedSettings.muscleGraphFlag = OFF;
	advancedSettings.gripFlag = ON;
	advancedSettings.demoFlag = ON;
	advancedSettings.motorEnable = EN;
	advancedSettings.initConfigFlag = true;  // set flag to signal config complete
  
	userSettings.sensitivityOffset = 200;
	userSettings.holdTime = 100;
    
	EEPROM_writeStruct(ADVANCED_CTRL_LOC,advancedSettings);
	EEPROM_writeStruct(USER_SETTINGS_LOC,userSettings);
	
	MYSERIAL.println("Complete");
}

void startUpMessages(void)
{
	MYSERIAL.print("Open Bionics - Artichoke V");
  MYSERIAL.println((float)VERSION_N);

	MYSERIAL.print(textString.right_left[advancedSettings.handFlag-1]);
	MYSERIAL.println(" Hand");

	#if defined(USE_I2C_ADC)
	MYSERIAL.println("I2C Muscle Sensors");
	#endif
	#if defined(USE_I2C_IO)
	MYSERIAL.println("I2C Port Expander");
	#endif

	if(advancedSettings.muscleCtrlFlag > 0)
	{
		// if muscle mode is on, print on
		switch(advancedSettings.muscleCtrlFlag)
		{
			case 1:             // standard muscle control
				MYSERIAL.println("Standard Muscle Control ON");
				digitalWrite(LED_KNUCKLE,HIGH); // turn on muscle control LED in knuckle
				break;
			case 2:             // position muscle control
				MYSERIAL.println("Muscle Position Control ON");
				digitalWrite(LED_KNUCKLE,HIGH);   // turn on muscle control LED in knuckle
				break;
			default:
				break;
		}
		MYSERIAL.print("Grip Change Mode ");
		MYSERIAL.println(textString.off_on[advancedSettings.gripFlag]);
	}
	
	if(advancedSettings.instructionsFlag) 
		printInstructions();			// if instructions not turned off, print initial instructions

	// if demo mode is enabled from start up
	if(advancedSettings.demoFlag)
	{
		MYSERIAL.println("Start up demo mode ON");
		MYSERIAL.println("Enter A0 to disable this mode");
		MYSERIAL.println("The hand is only responsive to serial commands \nat the end of each demo cycle");
	}
}

void printInstructions(void)
{
	MYSERIAL.print("\n");
	MYSERIAL.println("         Open Bionics Serial Commands - Artichoke ");
	MYSERIAL.println("___________________________________________________________");
	MYSERIAL.print("\n");
	MYSERIAL.println("Grip Control (G#)");
	MYSERIAL.println("Command     Description");
	MYSERIAL.println("G0          Fist");
	MYSERIAL.println("G1          Palm");
	MYSERIAL.println("G2          Thumbs Up");
	MYSERIAL.println("G3          Point");
	MYSERIAL.println("G4          Pinch");
	MYSERIAL.println("G5          Tripod");
	MYSERIAL.print("\n");
	MYSERIAL.println("Finger Control (F#, P##, S##)");
	MYSERIAL.println("Command     Description");
	MYSERIAL.println("F#          Finger, where # is the finger number (F0 - F4)");
	MYSERIAL.println("F# P50      Finger # to position 50 (P0 - P100)");
	MYSERIAL.println("F# S255     Finger # at speed 255 (S0-S255)");
	MYSERIAL.print("\n");
	MYSERIAL.println("Direction (O, C)");
	MYSERIAL.println("Command     Description");
	MYSERIAL.println("## O        Open, where ## is a Grip or Finger command");
	MYSERIAL.println("## C        Close, where ## is a Grip or Finger command");
	MYSERIAL.print("\n");
	MYSERIAL.println("Muscle Control (M#, U#, T#, N)");
	MYSERIAL.println("Command     Description");
	MYSERIAL.println("M0          Muscle control OFF");
	MYSERIAL.println("M1          Standard muscle control ON");
	MYSERIAL.println("M2          Muscle position control ON");
	MYSERIAL.println("M3          Toggle whether to display muscle readings");
	MYSERIAL.println("U###        Change muscle sensor sensitivity");
	MYSERIAL.println("T###        Change 'muscle hold' grip change duration");
	MYSERIAL.println("N           Calculate noise floor");
	MYSERIAL.print("\n");
	MYSERIAL.println("Advanced Settings (A#, ?)");
	MYSERIAL.println("Command     Description");
	MYSERIAL.println("A0          Toggle 'Demo Mode' ON/OFF");
	MYSERIAL.println("A1          Toggle 'Serial Commands Instructions' ON/OFF");
	MYSERIAL.println("A2          Toggle 'Muscle Graph mode' ON/OFF");
	MYSERIAL.println("A3          Enable/Disable motors");
	MYSERIAL.println("A5          Emergency stop motors");
	MYSERIAL.println("?           Display serial commands list");
	MYSERIAL.print("\n\n\n");
	MYSERIAL.println("Examples");
	MYSERIAL.println("G0 O        Fist Grip Open");
	MYSERIAL.println("G2 C        Thumbs Up Close");
	MYSERIAL.println("F0 O        Thumb Open");
	MYSERIAL.println("F0          Toggle thumb direction (open/close)");
	MYSERIAL.println("F4 P50      Pinky to position 50");
	MYSERIAL.println("F1 P50 S80  Index finger to position 50 at speed 80");
	MYSERIAL.println("F0 O S200   Thumb Open at speed 200");
	MYSERIAL.print("\n\n");
}
