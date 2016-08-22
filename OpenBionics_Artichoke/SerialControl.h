/*	Open Bionics - Artichoke
*	Author - Olly McBride
*	Date - December 2015
*
*	This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
*	To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
*
*	Website - http://www.openbionics.com/
*	GitHub - https://github.com/Open-Bionics
*	Email - ollymcbride@openbionics.com
*
*	SerialControl.h
*
*/

#ifndef _SERIALCONTROL_H_
#define _SERIALCONTROL_H_

//#define SERIAL_DEBUG

// SERIAL BFF_SIZE
#define SERIAL_BUF_SIZE	32		// maximum size of Serial incoming serialCmd.buff

// EEPROM LOCATIONS
#define ADVANCED_CTRL_LOC	  20	// starting pos for advanced control flags
#define USER_SETTINGS_LOC	  30	// starting pos for stored user settings

void serialEvent(void);
bool codeSeen(char code);
float codeVal();
float parsenumber(char code) ;
void getCmd(void);

void processCommand(void);			// interpret and store received serial commands
void constrainReceivedValues(void);

void manageSerialSettings(void);

void setDefaults(void);
void clearAll(void);				//clear all input variables for the next string
void initialEEPROMconfig(void);		// write default values to EEPROM
void startUpMessages(void);
void printInstructions(void);

void researchMode_CSV_RX(char *CSVString);			// receive CSV string and write target positions to motors
void researchMode_CSV_TX(void);			// receive CSV string and write target positions to motors


// SERIAL CONTROL
struct serialCmdType
{
	char cmdBuff[SERIAL_BUF_SIZE];	// buffer and variable to store incoming Serial string
	char* strPtr;           // pointer to char used for codeSeen() and codeVal()
	int cmdsToGet = 0;      // number of commands left in buffer (promotes expansion to ring buffer in future)

	int fingerNum = BLANK;
	int gripNum = BLANK;
	int direction = BLANK;
	int stopPos = BLANK;
	int speed = BLANK;
	int advancedFlag = BLANK;
	int instructionsFlag = BLANK;
	int muscleCtrlFlag = BLANK;
	int handFlag = BLANK;
	int demoFlag = BLANK;
	int sensitivityAdjust = BLANK;
	int resetFlag = BLANK;
	int holdTime = BLANK;
	int battFlag = BLANK;
	int storeData = BLANK;
	#ifdef FX_MODE
	int FXcharacter = BLANK;
	#endif
	int HANDleFlag = BLANK;
	
};
 
extern struct serialCmdType serialCmd;


#endif /*_SERIALCONTROL_H_*/
