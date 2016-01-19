
#define PRINT_DEMO_GRIP

void demoMode(void)
{ 
  MYSERIAL.println("Demo starting");
  
  printGrip(FIST_GRIP,OPEN);
  gripMovement(FIST_GRIP,BLANK,OPEN,BLANK);
  delay(demo.gripDuration);
  
  printGrip(FIST_GRIP,CLOSE);
  gripMovement(FIST_GRIP,BLANK,CLOSE,BLANK);
  delay(demo.gripDuration);
  
  printGrip(FIST_GRIP,OPEN);
  gripMovement(FIST_GRIP,BLANK,OPEN,BLANK);
  delay(demo.gripDuration);
  
  printGrip(FINGER_ROLL,CLOSE);
  fingerRoll(300,CLOSE);
  
  printGrip(FINGER_ROLL,OPEN);
  fingerRoll(300,OPEN);
  
  printGrip(FINGER_ROLL,CLOSE);
  fingerRoll(300,CLOSE);
  delay(demo.gripDuration/2);
  
  printGrip(FINGER_ROLL,OPEN);
  fingerRoll(300,OPEN);
  
  printGrip(FINGER_ROLL,CLOSE);
  fingerRoll(300,CLOSE);
  delay(demo.gripDuration/4);
  
  printGrip(FIST_GRIP,OPEN);
  gripMovement(FIST_GRIP,BLANK,OPEN,BLANK);
  delay(demo.gripDuration);

  for(int i=0;i<NUM_GRIPS;i++)
  {
	printGrip(i,CLOSE);
	gripMovement(i,BLANK,CLOSE,BLANK);
    delay(demo.gripDuration);
	
	printGrip(i,OPEN);
    gripMovement(i,BLANK,OPEN,BLANK);
    delay(demo.gripDuration);
  }
  
  printGrip(TRIPOD_GRIP,CLOSE);
  gripMovement(TRIPOD_GRIP,BLANK,CLOSE,BLANK);
  delay(demo.gripDuration);
  
  printGrip(TRIPOD_GRIP,OPEN);
  gripMovement(TRIPOD_GRIP,BLANK,OPEN,BLANK);
  delay(demo.gripDuration);
  
  printGrip(TRIPOD_GRIP,CLOSE);
  gripMovement(TRIPOD_GRIP,BLANK,CLOSE,BLANK);
  delay(demo.gripDuration);
  
  printGrip(FIST_GRIP,OPEN);
  gripMovement(FIST_GRIP,BLANK,OPEN,BLANK);
  delay(demo.gripDuration);
  
  printGrip(FINGER_ROLL,CLOSE);
  fingerRoll(430,CLOSE);
  delay(demo.gripDuration/2);
  
  printGrip(FIST_GRIP,OPEN);
  gripMovement(FIST_GRIP,BLANK,OPEN,BLANK);
  
  if(!advancedSettings.demoFlag)
	demo.flag = 0;

  MYSERIAL.println("Demo complete");
  
  // if demo mode is enabled from start up
  if(advancedSettings.demoFlag)
  {
	  MYSERIAL.println("\nStart up demo mode ON");
	  MYSERIAL.println("Enter A0 to disable this mode");
	  MYSERIAL.println("The hand is only responsive to serial commands \nat the end of each demo cycle");
  }
}

void fingerRoll(int del,int dir)
{
  if(dir==CLOSE)
  {
    for(int i=NUM_FINGERS-1;i>=0;i--)
    {
      fingerControl(i,100,0,MAX_FINGER_SPEED);
      delay(del);
    }
  }
  else
  {
    for(int i=0;i<NUM_FINGERS;i++)
    {
      fingerControl(i,0,0,MAX_FINGER_SPEED);
      delay(700);
    }
  }
}

void printGrip(int grip, int dir)
{
  #ifdef PRINT_DEMO_GRIP
    MYSERIAL.print(textString.grips[grip]);
    MYSERIAL.print(" ");
    MYSERIAL.println(textString.open_close[dir]);
  #endif
}
