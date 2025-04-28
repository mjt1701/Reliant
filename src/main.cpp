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

*/

#include "Reliant_Project.h"
#include "global.h"

// #include "ledStrip_functions.h"

SoftwareSerial mySoftwareSerial(rxPin, txPin);
DFRobotDFPlayerMini myDFPlayer;

Adafruit_NeoPixel shieldLEDstrip = Adafruit_NeoPixel(shieldLEDnum, shieldDataPin, NEO_GRBW + NEO_KHZ800); // shields LED object
Adafruit_NeoPixel shipLEDstrip = Adafruit_NeoPixel(shipLEDnum, shipDataPin, NEO_GRBW + NEO_KHZ800);		  // Ship LED object

void setup()
{
	pinMode(buttonPin, INPUT);	   // Push button pin
	digitalWrite(buttonPin, HIGH); // turn on internal PullUP resistor

	mySoftwareSerial.begin(9600);
	delay(100); // dfPlayer needs a little time to initialize

	// todo remove or comment out Serial Monitor outputs
	Serial.begin(115200); // Setup Serial Monitor

	Serial.println("Starting Program");
	Serial.print(" !myDFPlayer.begin(mySoftwareSerial): ");
	Serial.println(!myDFPlayer.begin(mySoftwareSerial));

	if (!myDFPlayer.begin(mySoftwareSerial))
	{ // Use softwareSerial to communicate with dfPlayer
		Serial.println(F("Unable to begin:"));
		Serial.println(F("1.Please recheck the connection!"));
		Serial.println(F("2.Please insert the SD card!"));
		while (true)
			;
	}

	myDFPlayer.volume(volume); // Set volume

	// Initialize the LEDs,  turn each strip on and off
	initializeLEDstrips();
}

// ********************************************* LOOP starts
void loop()
{
	Serial.print(" Shield State: ");
	Serial.println(shldState);

	buttonStatus = digitalRead(buttonPin);

	//		Serial.print("    buttonStatus: ");
	//		Serial.print(buttonStatus);
	//		Serial.print("    buttonActivated: ");
	//		Serial.println(buttonActivated);

	//		delay(1500);

	longButtonHold(); // test for long button press/hold/release

	switch (shldState)
	{

	case initial: // 0 anything that needs done at turn on
		shldState = buttonPressFile1;
		break;

	case buttonPressFile1: // 1 wait for buttonpress to start
	{
		if (buttonStatus == buttonActivated) // Look for button press  ----first time
		{
			shldState = playFile1;
		}
	}
	break;

	case playFile1: // 2 turn on file 1, turn on ship lights
	{
		//  turn ship leds on, play file 1
		delay(1000);

		shipLEDstrip.fill(shipLEDstrip.Color(shipRedValue, shipGreenValue, shipBlueValue, shipWhiteValue), 0, shipLEDnum);
		shipLEDstrip.show();
		//	Serial.println("    lights on ");
		// delay(1000);
		//	Serial.println("    pause 1 sec ");

		myDFPlayer.playMp3Folder(mp3File1);
		shldState = buttonPressFile2;
		//	Serial.println("    mp3File1  on ");
		//		delay(4000);
	}
	break;

	case buttonPressFile2: // 3 wait for buttonpress  // * more description
	{
		if (buttonStatus == buttonActivated) // Look for button press  ----first time
		{
			shldState = playFile2;
		}
	}
	break;

	case playFile2: // 4 button hit to launch scene; play file 2
	{
		if (buttonStatus == buttonActivated) // Look for button press  ----second time
		{
			buttonPressTime = millis();
			myDFPlayer.playMp3Folder(mp3File2);
			shldState = cueShieldsUp;
		}
	}
	break;

	case cueShieldsUp: // wait for time to start shields up
	{
		// todo does button press need to wait for mp3-1 to complete?
		/*		Serial.println(" millis() | buttonpresstime | millis() - buttonPressTime: ");
				Serial.print(millis());
				Serial.print("  ");
				Serial.print(buttonPressTime);
				Serial.print("  ");
				Serial.println(millis() - buttonPressTime);
		*/
		if (millis() - buttonPressTime > mp3File2Delay)
		{

			shldState = shieldsUp;
		}
	}
	break;

	case shieldsUp: // turn shield leds on
	{
		/*
		Serial.print(" buttonPressTime: ");
		Serial.println(buttonPressTime);
		Serial.print(" millis(): ");
		Serial.println(millis());
		Serial.print(" millis() - buttonPressTime: ");
		Serial.println(millis() - buttonPressTime);
*/

		if (ledNextTurnOffNum == shieldLEDnum) // is this the last led to go on
		{
			ledNextTurnOffNum = 0; // Reset for next time
			//		ledOffTime = millis(); // ? may not need this
			shldState = buttonPressFile3;
		}
		else
		{
			if ((millis() - ledOffTime > ledOffDelay) || (ledNextTurnOffNum == 0)) // Is it time to turn next light off?
			{
				shieldLEDstrip.setPixelColor(ledNextTurnOffNum, shieldLEDstrip.Color(shieldRedValue, shieldGreenValue, shieldBlueValue, shieldWhiteValue)); // Turn off next LED
				shieldLEDstrip.show();
				ledNextTurnOffNum++;
				ledOffTime = millis();
			}
		}
	}
	break;

	case buttonPressFile3:
	{
		if (buttonStatus == buttonActivated) // Look for button press  ----third time
		{
			shldState = playFile3;
		}
	}
	break;

	case playFile3: //   add comment
	{
		buttonPressTime = millis();
		myDFPlayer.playMp3Folder(mp3File3);
		shldState = cueShieldsDown;

		//	Serial.print(" buttonPressTime: ");
		//	Serial.println(buttonPressTime);
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
		/*
		Serial.print("ledNextTurnOffNum: ");
		Serial.println(ledNextTurnOffNum);

		Serial.print(" buttonPressTime: ");
		Serial.println(buttonPressTime);
		Serial.print(" millis(): ");
		Serial.println(millis());
		Serial.print(" millis() - buttonPressTime: ");
		Serial.println(millis() - buttonPressTime);
		*/

		if (ledNextTurnOffNum == shieldLEDnum) // is this the last led to go out
		{
			ledNextTurnOffNum = 0; // Reset for next time
			//		ledOffTime = millis(); // ? may not need this
			shldState = cuePhaserFire;
		}
		else
		{
			if ((millis() - ledOffTime > ledOffDelay) || (ledNextTurnOffNum == 0)) // Is it time to turn next light off?
			{
				shieldLEDstrip.setPixelColor(ledNextTurnOffNum, 0); // Turn off next LED
				shieldLEDstrip.show();
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
		reliantLEDflash();
		reliantLEDflash();
		reliantLEDflash();
		reliantLEDflash();
		// reliantLEDflash();
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

	case fullScene: // do the entire scene
	{
		// todo grab all  the stuff and  put it here; will need additional timeing infor for mp3-4

		if (!buttonStatus == buttonActivated) // Look for button press  ----first time
		{
			// shldState = playFile1;
			break;
		}

		if (millis() - buttonPressTime > mp3File4_1Delay) // is it time tostart shields down
		{
			ledOffTime = millis();
			shldState = shieldsDown;
		}

		break;

		//

		if (ledNextTurnOffNum == shieldLEDnum) // is this the last led to go on
		{
			ledNextTurnOffNum = 0; // Reset for next time
			//		ledOffTime = millis(); // ? may not need this
			shldState = buttonPressFile3;
		}
		else
		{
			if ((millis() - ledOffTime > ledOffDelay) || (ledNextTurnOffNum == 0)) // Is it time to turn next light off?
			{
				shieldLEDstrip.setPixelColor(ledNextTurnOffNum, shieldLEDstrip.Color(shieldRedValue, shieldGreenValue, shieldBlueValue, shieldWhiteValue)); // Turn off next LED
				shieldLEDstrip.show();
				ledNextTurnOffNum++;
				ledOffTime = millis();
			}
		}
		break;

		//
		if (millis() - buttonPressTime > mp3File3_1Delay) // is it time tostart shields down
		{
			ledOffTime = millis();
			shldState = shieldsDown;
		}
		break;

		//

		if (ledNextTurnOffNum == shieldLEDnum) // is this the last led to go out
		{
			ledNextTurnOffNum = 0; // Reset for next time
			//		ledOffTime = millis(); // ? may not need this
			shldState = buttonPressFile4;
		}
		else
		{
			if ((millis() - ledOffTime > ledOffDelay) || (ledNextTurnOffNum == 0)) // Is it time to turn next light off?
			{
				shieldLEDstrip.setPixelColor(ledNextTurnOffNum, 0); // Turn off next LED
				shieldLEDstrip.show();
				ledNextTurnOffNum++;
				ledOffTime = millis();
			}
		}
		break;

		//

		// todo

		/*  commented out the sections to start over as Neil does not have that
			case fullScene: // shields have gone down, mp3-2 may be playing , then reset to start again

				//      Serial.print("millis: ");
				//      Serial.println(millis());
				//      Serial.print("ledOffTime: ");
				//      Serial.println(ledOffTime);

				//      Serial.print("runOutMP3s2Time: ");
				//      Serial.println(runOutMP3s2Time);

				Serial.print("millis() - ledOffTime: ");
				Serial.println(millis() - ledOffTime);

				if (millis() - ledOffTime > runOutMP3s2Time) // has the runout time completed
				{
					shldState = waitToStartAgain;
					mp3s2OutTime = millis();
				}
				break;
			case waitToStartAgain:
				if (millis() - mp3s2OutTime > resetDelay) // has the time start over completed
				{
					shldState = initial;
					resetLeds();
					resetdfPlayer();
					//     Serial.println("Reset for next go around ");
				}

				// Serial.print("millis() - mp3s2OutTime: ");
				// Serial.println(millis() - mp3s2OutTime);

				break;
				// default:;

				*/

		break;
	}
	}
} // ==============  END void(loop)

void initializeLEDstrips()
{

	shieldLEDstrip.begin();
	shipLEDstrip.begin();

	shieldLEDstrip.setBrightness(shieldMaxBright);
	shieldLEDstrip.fill(shieldLEDstrip.Color(shieldRedValue, shieldGreenValue, shieldBlueValue, shieldWhiteValue), 0, shieldLEDnum);
	shieldLEDstrip.show();
	delay(1000);

	shieldLEDstrip.fill(shieldLEDstrip.Color(0, 0, 0, 0), 0, shieldLEDnum);
	shieldLEDstrip.show();

	shipLEDstrip.setBrightness(shipMaxBright);
	shipLEDstrip.fill(shipLEDstrip.Color(shipRedValue, shipGreenValue, shipBlueValue, shipWhiteValue), 0, shipLEDnum);
	shipLEDstrip.show();
	delay(1000);

	shipLEDstrip.fill(shipLEDstrip.Color(0, 0, 0, 0), 0, shipLEDnum);
	shipLEDstrip.show();
}

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
	/*
		// debug serial print
		Serial.println("FLASH");
		Serial.print("Brightness: ");
		Serial.println(flashBrightness);
		Serial.print("flashCount: ");
		Serial.println(flashCount);
		Serial.print("-");
	*/

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
