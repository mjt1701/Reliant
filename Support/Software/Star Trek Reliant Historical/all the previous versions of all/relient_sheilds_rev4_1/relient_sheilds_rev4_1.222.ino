/*
 NN   N  EEEEE  IIIII  L
 N N  N  E        I    L
 N  N N  EEEE     I    L
 N  N N  E        I    L
 N   NN  EEEEE  IIIII  LLLLL

  Relent Sheilds
  Lights set of WS2812 RGBW LEDs to re-enact the relient sheilds going down
  My Matt Troha

  Revision History
  Rev 1.0
  set up software to perform basic light action of relient sheilds going down
  all lights on, then one by one they turn off

  Rrev 1.1
  updated to run on sumulator wokwi.com
  minor updates

  Renamed to include both light and sound
  Rev1.0

  Rev2.0 changed to Case statement for main flow

  Rev3.0 replaced mode state with enum paradigm
  Rev3.1 split case 4 into two separate cases; including making some of it functions
  Rev3.2 clean up variables etc from 3.x, add dfPlayer intialize delay

  Version 2
   uses switch to start sequence

 Rev 4.1 add  ship lights; (still need to determine color etc)  this adds new sheildLEDstrip instance and renames LED variables  etc
	  - also need to refactors functions to reference variable for the LED objects


*/

#include "Arduino.h"
#include "SoftwareSerial.h"		 //SOftware Serial Bus to support DFPlayer
#include "DFRobotDFPlayerMini.h" //DFRobot DFPlayer Library
#include <Adafruit_NeoPixel.h>	 //Adafruit NeoPixel Library

const int rxPin = 10;			  // Define Arduino connection pin fo software serial rx
const int txPin = 11;			  // Define Arduino connection pin fo software serial tx
const int buttonPin = 6;		  // Define Arduino connection pin for button
const bool buttonActivated = LOW; // Indicate that button pushed is HIGH
const int volume = 10;			  // Sound volume for dfPlayer

const int sheildDataPin = 3;		 // Define Arduino connection pin for Sheilds LED data
const int sheildLEDnum = 8;			 // Define the total number of Sheilds LEDs
const int sheildLEDinGroup = 1;		 // Define nubmer of Sheild LEDs in a group ---not used???
const int sheildMaxBright = 32;		 // Define maximum Sheild LED brightness
const uint8_t SheildRedValue = 0;	 // Define the Sheild LED color RGB
const uint8_t sheildGreenValue = 0;	 // Define the Sheild LED color RGB
const uint8_t sheildBlueValue = 255; // Define the Sheild LED color RGB

const int shipDataPin = 4;			// Define Arduino connection pin for Ship LED data
const int shipLEDnum = 8;			// Define the total number of Ship LEDs
const int shipLEDinGroup = 1;		// Define nubmer of Ship LEDs in a group  ---not used???
const int shipMaxBright = 32;		// Define maximum Ship LED brightness
const uint8_t ShipRedValue = 255;	// Define the Ship LED color RGB
const uint8_t shipGreenValue = 255; // Define the Ship LED color RGB
const uint8_t shipBlueValue = 255;	// Define the Ship LED color RGB

const uint8_t Reliant1mp3Num = 1; // mp3 file number of intro
const uint8_t Reliant2mp3Num = 2; // mp3 file number of sheilds going down
const uint8_t Reliant3mp3Num = 3; // mp3 file number of
const uint8_t Reliant4mp3Num = 4; // mp3 file number of
const uint8_t Reliant5mp3Num = 5; // mp3 file number of
const uint8_t Reliant6mp3Num = 6; // mp3 file number of

const unsigned long ledOffDelay = 1000;	  // Time until next LED goes out
const unsigned long mp3WaitTime = 483000; // Amount of time to wait from start of mp3-2 and sheilds go down
const int afterDelay = 4000;
const unsigned long runOutMP3s2Time = 17000; // Time from end of sheilds goind to end of mp3-2
const unsigned long resetDelay = 5000;		 // Time to delay after mp3-2 ends and retart loop

unsigned long mp3s1Time;	   // Time that mp3 file 1 starts
unsigned long mp3s2Time;	   // Time that mp3 file 2 starts
int ledNextTurnOffNum = 0;	   // number of LED to turn off in sheilddown sequence
unsigned long buttonPressTime; // Time when button pressed
unsigned long ledOffTime;	   // Time when next LED is turned off
unsigned long mp3s2OutTime;	   // Time when mp3-2 ended
unsigned long runOutTime;	   // Time when mp3-2 ended

enum sheildStates
{
	onePlaying,		 // 0 mp3-1 playing
	twoStarted,		 // 1 mp3-2 playing only ie need to wait for time to start sheilds down
	goingDown,		 // 2 mp3-2 playing and sheilds going down
	playingOut,		 // 3 last light is out, mp3-2 still playing (or change to nothing and handle after mp3 is done)
	waitToStartAgain // 4 reset and start over
};

enum sheildStates shldState; // = onePlaying;

SoftwareSerial mySoftwareSerial(rxPin, txPin);
DFRobotDFPlayerMini myDFPlayer;

Adafruit_NeoPixel sheildLEDstrip = Adafruit_NeoPixel(sheildLEDnum, sheildDataPin, NEO_GRBW + NEO_KHZ800); // Sheilds LED object
Adafruit_NeoPixel shipLEDstrip = Adafruit_NeoPixel(shipLEDnum, shipDataPin, NEO_GRBW + NEO_KHZ800);		  // Ship LED object

int loopnum = 1;

void setup()
{
	pinMode(buttonPin, INPUT);
	digitalWrite(buttonPin, HIGH); // turn on internal resistor

	mySoftwareSerial.begin(9600);
	delay(100); // dfPlayer needs a little time to initialize
	// todo remove or comment out Serial Monitor outputs
	Serial.begin(115200); // Setup Serial Monitor

	Serial.println("Starting Program");
	Serial.print(" !myDFPlayer.begin(mySoftwareSerial): ");
	Serial.println(!myDFPlayer.begin(mySoftwareSerial));

	if (!myDFPlayer.begin(mySoftwareSerial))
	{ // Use softwareSerial to communicate with dfPlayer
		// change this to flash the lights
		Serial.println(F("Unable to begin:"));
		Serial.println(F("1.Please recheck the connection!"));
		Serial.println(F("2.Please insert the SD card!"));
		while (true)
			;
	}

	// Initialize the LEDs and dfPlayer
	sheildLEDstrip.begin();
	shipLEDstrip.begin();
	//	resetLeds();
	resetdfPlayer();
}

// ********************************************* LOOP starts
void loop()
{

	Serial.print(" Loop  number: ");
	Serial.println(loopnum);
	loopnum = loopnum + 1;

	resetLeds1(sheildLEDstrip);

} // ==============  END void(loop)

// Function to set all LEDs off  *****change??????
void clearLEDs()
{
	// Cycle through all LEDs
	// todo need to generalize. &sheild, LEDnum
	for (int i = 0; i < sheildLEDnum; i++)
	{
		// Set color to zero which is off
		sheildLEDstrip.setPixelColor(i, 0);
	}
}

void resetLeds()
{
	clearLEDs(); // todo need to generalize. &sheild, maxBright, redVal, grVal, blueVal, whiteVal, LEDnum
	sheildLEDstrip.show();
	sheildLEDstrip.setBrightness(sheildMaxBright);
	sheildLEDstrip.fill(sheildLEDstrip.Color(SheildRedValue, sheildGreenValue, sheildBlueValue, 0), 0, sheildLEDnum);
	sheildLEDstrip.show();
}

void resetdfPlayer()
{
	myDFPlayer.volume(volume);
	myDFPlayer.playMp3Folder(Reliant1mp3Num); // Start playing the first mp3
	mp3s1Time = millis();
	shldState = onePlaying;
}

void resetLeds1(Adafruit_NeoPixel &LEDstrip)
{

	Serial.print(" Entered function ");
	Serial.println("");

	// clearLEDs(); // todo need to generalize. &sheild, maxBright, redVal, grVal, blueVal, whiteVal, LEDnum
	// Cycle through all LEDs  ---from clearLEDs----
	// todo need to generalize. &sheild, LEDnum
	for (int i = 0; i < sheildLEDnum; i++)
	{
		// Set color to zero which is off
		LEDstrip.setPixelColor(i, 0);
	}
	LEDstrip.show();

	Serial.print(" Leds should go out ");
	Serial.println("");

	delay(3000);

	LEDstrip.show();
	LEDstrip.setBrightness(sheildMaxBright);
	LEDstrip.fill(sheildLEDstrip.Color(SheildRedValue, sheildGreenValue, sheildBlueValue, 0), 0, sheildLEDnum);
	LEDstrip.show();

		Serial.print(" Leds should go on ");
	Serial.println("");
	delay(3000);

	sheildLEDstrip.show();
	sheildLEDstrip.setBrightness(sheildMaxBright);
	sheildLEDstrip.fill(sheildLEDstrip.Color(SheildRedValue, sheildGreenValue, sheildBlueValue, 0), 0, sheildLEDnum);
	sheildLEDstrip.show();


	Serial.print(" Leds should go out again ");
	Serial.println("");
	delay(3000);
}