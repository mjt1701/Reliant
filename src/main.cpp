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
ButtonHandler button(buttonPin, buttonActivated);
SoundPlayer sound(rxPin, txPin);

void setup()
{
	pinMode(buttonPin, INPUT);	   // Push button pin
	digitalWrite(buttonPin, HIGH); // turn on internal PullUP resistor

	sound.begin(volume);

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

if (button.isLongPress()) { 
shldState=SET_VOLUME;
Serial.println("LONG PRESS ACTION");
}
Serial.println(shldState);
	switch (shldState)
	{

	case initial: //  anything that needs done at turn on //todo remove initial state
		shldState = buttonPressFile1;
		break;

	case buttonPressFile1: //  wait for buttonpress to start
	{
	//	if (buttonStatus == buttonActivated) // Look for button press  ----first time
	if(button.isShortPress())
	{
			shldState = playFile1;
		}
	}
	break;

	case playFile1: //  turn on file 1, turn on ship lights
	{
		shipLED.fillColor(shipRedValue, shipGreenValue, shipBlueValue, shipWhiteValue);
		sound.play(mp3File1);
		shldState = buttonPressFile2;
	}
	break;

	case buttonPressFile2: //  wait for buttonpress  // * more description
	{
	//	if (buttonStatus == buttonActivated) // Look for button press  ----first time
	if(button.isShortPress())
	{
			shldState = playFile2;
		}
	}
	break;

	case playFile2: //  button hit to launch scene; play file 2  //todo why the second time thing
	{
	//	if (buttonStatus == buttonActivated) // Look for button press  ----second time
	//	if(button.isShortPress())
		{
			buttonPressTime = millis();
			sound.play(mp3File2);
			shldState = cueShieldsUp;
		}
	}
	break;

	case cueShieldsUp: // wait for time to start shields up
	{
		// TODO  does button press need to wait for mp3-1 to complete?

		if (millis() - buttonPressTime > mp3File2Delay)
		{
			shldState = shieldsUp;
		}
	}
	break;

	case shieldsUp: // turn shield leds on
	{
		if (ledNextTurnOffNum == shieldLEDnum) // is this the last led to go on
		{
			ledNextTurnOffNum = 0; // Reset for next time
			shldState = buttonPressFile3;
		}
		else
		{
			if ((millis() - ledOffTime > ledOffDelay) || (ledNextTurnOffNum == 0)) // Is it time to turn next light off?
			{

				shieldLED.setPixelColor(ledNextTurnOffNum, shieldRedValue, shieldGreenValue, shieldBlueValue, shieldWhiteValue);

				ledNextTurnOffNum++;
				ledOffTime = millis();
			}
		}
	}
	break;

	case buttonPressFile3:
	{
	
	//	if (buttonStatus == buttonActivated) // Look for button press  ----third time
	if(button.isShortPress())
		{
			shldState = playFile3;
		}
	}
	break;

	case playFile3: //   add comment
	{
		buttonPressTime = millis();
		sound.play(mp3File3);
		shldState = cueShieldsDown;
	}
	break;

	case cueShieldsDown: //  * add comment
	{
		if (millis() - buttonPressTime > mp3File3_1Delay) // is it time tostart shields down
		{
			ledOffTime = millis();
			shldState = shieldsDown;
		}
	}
	break;

	case shieldsDown: // shields going down
	{
		if (ledNextTurnOffNum == shieldLEDnum) // is this the last led to go out
		{
			ledNextTurnOffNum = 0; // Reset for next time
			shldState = cuePhaserFire;
		}
		else
		{
			if ((millis() - ledOffTime > ledOffDelay) || (ledNextTurnOffNum == 0)) // Is it time to turn next light off?
			{
				shieldLED.setPixelColor(ledNextTurnOffNum, 0, 0, 0, 0);
				ledNextTurnOffNum++;
				ledOffTime = millis();
			}
		}
	}
	break;

	case cuePhaserFire: // wait for buttonpress
	{
		if (millis() - buttonPressTime > mp3File3_2Delay) // is it time to start lightning?
		{
			shldState = firePhasers;
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
	}
	break;

	case buttonPressFile4: // wait for buttonpress  // * more description
	{
		if (buttonStatus == buttonActivated) // Look for button press  ----first time
		{
			//	shldState = fullScene;
		}
	}
	break;

	case fullScene: // do the entire scene without pause for 
	{
		break;
	}

	case SET_VOLUME: // set volume 
	{
		sound.stop();
		shieldLED.clear();
		shipLED.clear();
		shipLED.flash();
		shipLED.flash();
		shipLED.flash();
		shipLED.flash();
		delay(5000);

		shldState= initial;
		button.reset(); // Prevents unwanted short press being used in next state

		break;
	}
	
	}
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

/*
void longButtonHold()
{
	if (buttonStatus == LOW) // Button pressed  //was HIGH in example (prob due to PULL UP)
	{
		if (buttonActive == false) // false means the program is detecting the button being pressed for the first time
		{
			buttonActive = true;
			buttonTimer = millis();
		}
		if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) //  it is now a long button press, execute long button press actions
		{
			longPressActive = true; // long button press activated, executed actions and now wait for button release
			// do long button press actions
			shldState = initial;
			myDFPlayer.stop();
			initializeLEDstrips();
		}
	}
	else
	{ // Button not pressed

		if (buttonActive == true)
		{ //  first time thru after button released

			if (longPressActive == true)
			{
				longPressActive = false;
			}
			else
			{ //  then it was a short click

				// for reliant: continue as normal
				// todo action taken when short press completed // carry on with normal activity
			}

			buttonActive = false;
		}
	}
}
*/