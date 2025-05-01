/*
 NN   N  EEEEE  IIIII  L
 N N  N  E        I    L
 N  N N  EEEE     I    L
 N  N N  E        I    L
 N   NN  EEEEE  IIIII  LLLLL

  Relent shields
  Lights set of WS2812 RGBW LEDs to re-enact the relient shields going down

  this is the version for Neil (Matt version not created yet)
  My Matt Troha

moved code to  initializeLEDstrips()

moved code to PlatfomrIO

refactor to add OOP for led strips and sound

*/

#include "Arduino.h"
#include "SoftwareSerial.h"		 //SOftware Serial Bus to support DFPlayer
#include "DFRobotDFPlayerMini.h" //DFRobot DFPlayer Library
#include <Adafruit_NeoPixel.h>	 //Adafruit NeoPixel Library

#include "global.h"
#include "LEDStrip.h"
#include "SoundPlayer.h"
#include "ButtonHandler.h"

LEDStrip shieldLED(shieldDataPin, shieldLEDnum, SHIELD_LED_TYPE);
LEDStrip shipLED(shipDataPin, shipLEDnum, SHIP_LED_TYPE); // or NEO_RGBW + NEO_KHZ800 later if needed
ButtonHandler button(buttonPin);  
SoundPlayer sound(rxPin, txPin);

void setup()
{
	sound.begin();

	// todo remove or comment out Serial Monitor outputs
	Serial.begin(9600); // Setup Serial Monitor

	Serial.println("Starting Program");

	//	initializeLEDstrips();
	shieldLED.begin(shieldMaxBright);
	shipLED.begin(shipMaxBright);

	// Startup indicator: turn both strips on for 2 seconds
	shieldLED.fillColor(shieldRedValue, shieldGreenValue, shieldBlueValue, shieldWhiteValue);
	shipLED.fillColor(shipRedValue, shipGreenValue, shipBlueValue, shipWhiteValue);
	delay(2000);

	shieldLED.clear();
	shipLED.clear();
}

// ********************************************* LOOP starts
void loop()
{
	//	buttonStatus = digitalRead(buttonPin);
	button.update();

	switch (shldState)
	{

	case initial: //  anything that needs done at turn on //todo remove initial state
		shldState = buttonPressFile1;
		Serial.println(shldState);

		break;

	case buttonPressFile1: //  wait for buttonpress to start
	{
		//	if (buttonStatus == buttonActivated) // Look for button press  ----first time
		if (button.wasShortPressed())
		{
			shldState = playFile1;
			Serial.println(shldState);
		}

		if (button.wasLongPressed())
		{
			shldState = SET_VOLUME;
			Serial.println(shldState);
			button.reset();
		}
	}
//	button.reset();
	break;

	case playFile1: //  turn on file 1, turn on ship lights
	{
		shipLED.fillColor(shipRedValue, shipGreenValue, shipBlueValue, shipWhiteValue);
		sound.play(mp3File1);
		shldState = buttonPressFile2;
		Serial.println(shldState);
	}
	break;

	case buttonPressFile2: //  wait for buttonpress  // * more description
	{
		//	if (buttonStatus == buttonActivated) // Look for button press  ----first time
		if (button.wasShortPressed())
		{
			shldState = playFile2;
			Serial.println(shldState);
		}
	}
	break;

	case playFile2: //  button hit to launch scene; play file 2 
	{
		{
			buttonPressTime = millis();
			sound.play(mp3File2);
			shldState = cueShieldsUp;
			Serial.println(shldState);
		}
	}
	break;

	case cueShieldsUp: // wait for time to start shields up
	{
		// TODO  does button press need to wait for mp3-1 to complete?

		if (millis() - buttonPressTime > mp3File2Delay)
		{
			shldState = shieldsUp;
			Serial.println(shldState);
		}
	}
	break;

	case shieldsUp: // turn shield leds on
	{
		if (ledNextTurnOffNum == shieldLEDnum) // is this the last led to go on
		{
			ledNextTurnOffNum = 0; // Reset for next time
			shldState = buttonPressFile3;
			Serial.println(shldState);
		}
		else if ((millis() - ledOffTime > ledOffDelay) || (ledNextTurnOffNum == 0)) // Is it time to turn next light off?
			{
				shieldLED.setPixelColor(ledNextTurnOffNum, shieldRedValue, shieldGreenValue, shieldBlueValue, shieldWhiteValue);
				ledNextTurnOffNum++;
				ledOffTime = millis();
			}
			}
	break;

	case buttonPressFile3:
	{

		//	if (buttonStatus == buttonActivated) // Look for button press  ----third time
		if (button.wasShortPressed())
		{
			shldState = playFile3;
			Serial.println(shldState);
		}
	}
	break;

	case playFile3: //   add comment
	{
		buttonPressTime = millis();
		sound.play(mp3File3);
		shldState = cueShieldsDown;
		Serial.println(shldState);
	}
	break;

	case cueShieldsDown: //  * add comment
	{
		if (millis() - buttonPressTime > mp3File3_1Delay) // is it time tostart shields down
		{
			ledOffTime = millis();
			shldState = shieldsDown;
			Serial.println(shldState);
		}
	}
	break;

	case shieldsDown: // shields going down
	{
		if (ledNextTurnOffNum == shieldLEDnum) // is this the last led to go out
		{
			ledNextTurnOffNum = 0; // Reset for next time
			shldState = cuePhaserFire;
			Serial.println(shldState);
		}
		else if ((millis() - ledOffTime > ledOffDelay) || (ledNextTurnOffNum == 0)) // Is it time to turn next light off?
			{
				shieldLED.setPixelColor(ledNextTurnOffNum, 0, 0, 0, 0);
				ledNextTurnOffNum++;
				ledOffTime = millis();
			}
	}
	break;

	case cuePhaserFire: // wait for buttonpress
	{
		if (millis() - buttonPressTime > mp3File3_2Delay) // is it time to start lightning?
		{
			shldState = firePhasers;
			Serial.println(shldState);
		}
	}
	break;

	case firePhasers: // Reliant ship lights flicker from being phasered
	{
		// fire once at 27 sec
		shipLED.flash();
		shipLED.flash();
		shipLED.flash();
		shipLED.flash();

		shldState = initial;
		Serial.println(shldState);
	}
	break;


	case fullScene: // do the entire scene without pause for
	{
		break;
	}

case SET_VOLUME: {
	//	Serial.println("lets set the volume...");


		static bool initialized = false;
		static unsigned long lastInteraction = 0;
		static unsigned long lastVolumeChange = 0;
		const unsigned long volumeCooldown = 750;
		const unsigned long exitTimeout = 5000;
	
		unsigned long now = millis();
	
		if (!initialized) {
			Serial.println("Entered SET_VOLUME mode");
			initialized = true;
			lastInteraction = now;
			lastVolumeChange = now;
		}
	
		// Volume Up – short press
		if (button.wasShortPressed() && (now - lastVolumeChange > volumeCooldown)) {
			sound.volumeUp();
			lastVolumeChange = now;
			lastInteraction = now;
			Serial.println("Volume UP");
		}
	
		// Volume Down – long press hold
		if (button.wasLongPressStart() && (now - lastVolumeChange > volumeCooldown)) {
			sound.volumeDown();
			lastVolumeChange = now;
			lastInteraction = now;
			Serial.println("Volume DOWN");
		}
	
		// Exit after inactivity
		if (now - lastInteraction > exitTimeout) {
			Serial.println("Exiting SET_VOLUME mode");
	
			// Do all cleanup here
			shldState = initial;
			initialized = false;
			button.reset(); // This clears stale long/short flags
		}
	
		break;
	}
	
	
	

	}
		//	}
	} // ==============  END void(loop)


