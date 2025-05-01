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

	// if (button.wasLongPressStart() && shldState != SET_VOLUME) { 
	// 	button.reset();  // ✅ Clear any stale button states right before entering SET_VOLUME
	// 	shldState = SET_VOLUME;
	// 	Serial.println("LONG PRESS ACTION");
	// }
	// Serial.println(shldState);
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
		}

	}
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

	case playFile2: //  button hit to launch scene; play file 2  //todo why the second time thing
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

		shldState = buttonPressFile4;
		Serial.println(shldState);
	}
	break;

	case buttonPressFile4: // wait for buttonpress  // * more description
	// {
	// 	if (buttonStatus == buttonActivated) // Look for button press  ----first time
	// 	{
	// 		//	shldState = fullScene;
	// 	}
	// }
	break;

	case fullScene: // do the entire scene without pause for
	{
		break;
	}


	case SET_VOLUME: {
		Serial.println("lets set the volume...");
		shldState = initial;
		button.reset();
		// static bool initialized = false;
		// static unsigned long lastInteraction = 0;
		// static unsigned long volumeCooldown = 300;
		// static unsigned long lastVolumeChange = 0;
		// static bool waitingForRelease = false; 
	
		// unsigned long now = millis();  // Only get millis() once per loop
	
		// if (!initialized) {
		// 	Serial.println("Entered SET_VOLUME mode");
		// 	initialized = true;
		// 	waitingForRelease = true;
		// 	lastInteraction = now;
		// 	button.reset();  // Clear any leftover flags
		// 	break;           // Exit this loop iteration cleanly
		// }

		// if (waitingForRelease) {
		// 	if (button.isPhysicallyReleased()) {
		// 		waitingForRelease = false;  // Button was released — resume input
		// 	} else {
		// 		break;  // Still holding — wait
		// 	}
		// }
		
	
		// // Volume Down – hold
		// if (button.isHeldLong() && (now - lastVolumeChange > volumeCooldown)) {
		// 	sound.volumeDown();
		// 	lastVolumeChange = now;
		// 	lastInteraction = now;
		// 	Serial.println("Volume DOWN");
		// 	shipLED.flash();
		// 	shipLED.flash();
		// }
	
		// // Volume Up – short press
		// if (button.isShortPress() && (now - lastVolumeChange > volumeCooldown)) {
		// 	sound.volumeUp();
		// 	lastVolumeChange = now;
		// 	lastInteraction = now;
		// 	Serial.println("Volume UP");
		// 	shipLED.flash();
		// }
	
		// // Exit volume mode after inactivity
		// if (now - lastInteraction > 5000) {
		// 	Serial.println("Exiting SET_VOLUME mode");
		// 	shldState = initial;
		// 	initialized = false;
		// 	button.reset();
		// }
	
		break;
	}
	
	

	}
		//	}
	} // ==============  END void(loop)

	/*
	void reliantLEDflash()
	{
		// lightning variables
		// use rgbw neopixel adjust the following values to tweak lightning base color
		int r = random(40, 80);
		int g = random(10, 25);
		int b = random(0, 10);
		// return 32 bit color
		uint32_t color = shipLEDstrip.Color(r, g, b, 50);
		// number of flashes
		int flashCount = random(9, 15); // 5-15
		// flash white brightness range - 0-255
		int flashBrightnessMin = 10;
		int flashBrightnessMax = 30; // can go to 255 //todo see what works for reliant
		int flashBrightness = random(flashBrightnessMin, flashBrightnessMax);
		// flash duration range - ms
		int flashDurationMin = 5;
		int flashDurationMax = 75;
		// flash off range - ms
		int flashOffsetMin = 0;
		int flashOffsetMax = 75;
		// time to next flash range - ms
		int nextFlashDelayMin = 1;
		int nextFlashDelayMax = 50;

		for (int flash = 0; flash <= flashCount; flash += 1)
		{
			// add variety to color
			int colorV = random(0, 50);

			// flash segments of neopixel strip
			color = shipLEDstrip.Color(r + colorV, g + colorV, b + colorV, flashBrightness);
			shipLEDstrip.fill(color, 0, 2); // 0,4
			shipLEDstrip.show();
			// delay(2000);
			delay(random(flashOffsetMin, flashOffsetMax));
			colorV = random(0, 50);
			color = shipLEDstrip.Color(r + colorV, g + colorV, b + colorV, flashBrightness);
			shipLEDstrip.fill(color, 4, 2); // 8,4
			shipLEDstrip.show();
			//  delay(2000);
			delay(random(flashOffsetMin, flashOffsetMax));
			colorV = random(0, 50);
			color = shipLEDstrip.Color(r + colorV, g + colorV, b + colorV, flashBrightness);
			shipLEDstrip.fill(color, 6, 2); // 4,4
			shipLEDstrip.show();
			//  delay(2000);
			delay(random(flashOffsetMin, flashOffsetMax));
			colorV = random(0, 50);
			color = shipLEDstrip.Color(r + colorV, g + colorV, b + colorV, flashBrightness);
			shipLEDstrip.fill(color, 2, 2); // 9,14
			shipLEDstrip.show();
			//  delay(2000);
			delay(random(flashDurationMin, flashDurationMax));
			shipLEDstrip.clear();
			shipLEDstrip.show();
			// delay(2000);
			delay(random(nextFlashDelayMin, nextFlashDelayMax));
		}
	}
	*/

