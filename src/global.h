/*
 NN   N  EEEEE  IIIII  L
 N N  N  E        I    L
 N  N N  EEEE     I    L
 N  N N  E        I    L
 N   NN  EEEEE  IIIII  LLLLL

  Relent shields
  Lights set of WS2812 RGBW LEDs to re-enact the relient shields going down

  My Matt Troha

*/

const uint8_t rxPin = 10;		  // Define Arduino connection pin fo software serial rx
const uint8_t txPin = 11;		  // Define Arduino connection pin fo software serial tx
const uint8_t buttonPin = 6;	  // Define Arduino connection pin for button
const bool buttonActivated = LOW; // Indicate that button pushed is HIGH
const uint8_t volume = 15;		  // Sound volume for dfPlayer From 0 to 30  //todo change back to 20

// Shield LEDs
const uint8_t shieldDataPin = 3;	 // Define Arduino connection pin for shields LED data
const uint8_t shieldLEDnum = 8;		 // Define the total number of shields LEDs
const uint8_t shieldMaxBright = 32;	 // Define maximum shield LED brightness  //todo set final volume
const uint8_t shieldRedValue = 0;	 // Define the shield LED color RGBW
const uint8_t shieldGreenValue = 0;	 // Define the shield LED color RGBW
const uint8_t shieldBlueValue = 255; // Define the shield LED color RGBW
const uint8_t shieldWhiteValue = 0;	 // Define the shield LED color RGBW

// Ship LEDs
const uint8_t shipDataPin = 4;		// Define Arduino connection pin for Ship LED data
const uint8_t shipLEDnum = 8;		// Define the total number of Ship LEDs
const uint8_t shipMaxBright = 32;	// Define maximum Ship LED brightness  // ? combine shield and ship ??
const uint8_t shipRedValue = 0;		// Define the Ship LED color RGBW
const uint8_t shipGreenValue = 0;	// Define the Ship LED color RGBW
const uint8_t shipBlueValue = 0;	// Define the Ship LED color RGBW
const uint8_t shipWhiteValue = 255; // Define the Ship LED color RGBW

// mp3 file ID on card
const int mp3File1 = 1; // mp3 file number of intro
const int mp3File2 = 2; // mp3 file number of shields going up
const int mp3File3 = 3; // mp3 file number of shields going down
const int mp3File4 = 4; // mp3 file number of full scene

// Audio timing data
const unsigned long ledOffDelay = 800;	  // Time until next LED goes on or off
const unsigned long mp3WaitTime = 483000; // Amount of time to wait from start of mp3-2 and shields go down
const int afterDelay = 4000;
const unsigned long runOutMP3s2Time = 17000; // Time from end of shields goind to end of mp3-2
const unsigned long resetDelay = 5000;		 // Time to delay after mp3-2 ends and retart loop

// todo not used const unsigned long mp3File1Delay = xxx;   // todo find/insert correct timing here
const unsigned long mp3File2Delay = 7600;	 // Trigger delay after File start for next action
const unsigned long mp3File3_1Delay = 11600; // Trigger delay after File start for next action
const unsigned long mp3File3_2Delay = 27000; // Trigger delay after File start for next action
const unsigned long mp3File4_1Delay = 3000;	 // Trigger delay after File start for next action
const unsigned long mp3File4_2Delay = 4000;	 // Trigger delay after File start for next action

unsigned long mp3s1Time;	   // Time that mp3 file 1 starts  // ? needed  replaced by imm above
unsigned long mp3s2Time;	   // Time that mp3 file 2 starts   // ? needed replaced by imm above
uint8_t ledNextTurnOffNum = 0; // LED number to turn off in shielddown sequence
unsigned long buttonPressTime; // Time when button pressed
unsigned long ledOffTime;	   // Time when previous LED turned off
unsigned long mp3s2OutTime;	   // ? needed
unsigned long runOutTime;	   // ? needed

//  long button press
long buttonTimer = 0;
long longPressTime = 2000;
bool buttonStatus;
bool buttonActive = false;  // detect the first loop after the button has been pressed or released
bool longPressActive = false;  //allow the code to stop the long press from activating more than once, and will stop the short press function being activated when we release the button after a long press

enum shieldStates
{
	initial,		  // 0  play mp3-1 and turn on ship LEDs
	buttonPressFile1, // 1  wait for button press to start File 1
	playFile1,		  // 2  play mp3-2
	buttonPressFile2, // 3  wait for button press to start File 2
	playFile2,		  // 4  play mp3-2
	cueShieldsUp,	  // 5  wait for shields up cue
	shieldsUp,		  // 6  shields go up
	buttonPressFile3, // 7  wait for button press to start File 3
	playFile3,		  // 8  play mp3-3
	cueShieldsDown,	  // 9  wait for shields down cue
	shieldsDown,	  // 10 shields going down
	cuePhaserFire,	  // 11 wait for phaser fire
	firePhasers,	  // 12 fire phasers (LED lightning)
	buttonPressFile4, // 13 wait for button press to start File 4
	fullScene,		  // 14 do full scene with no additional button pushes
	waitToStartAgain  // 15 reset and start over  // ? what to do here
};

enum shieldStates shldState = initial; // = initial;
