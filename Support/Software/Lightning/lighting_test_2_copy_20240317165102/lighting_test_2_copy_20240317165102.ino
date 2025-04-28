/*
this is a test to develop the lightning effect for the reliant being hit by phaser fire
*/

#include "SoftwareSerial.h"
#include <Adafruit_NeoPixel.h>

// set number of pixels in strip
int numPix = 8; // 16
// set pin to control neopixels
int neoPin = 3;
// set initial brightness 0-255
int brightness = 30; // 255

// create led object
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(numPix, neoPin, NEO_GRBW + NEO_KHZ800);

void setup()
{
	// initialize neopixels
	ledStrip.begin();
	ledStrip.setBrightness(brightness);
	ledStrip.show();

	// initialize serial port for output
		Serial.begin(9600);
}

void loop()
{
	// lightning variables
	// use rgbw neopixel adjust the following values to tweak lightning base color
	int r = random(40, 80);
	int g = random(10, 25);
	int b = random(0, 10);
	// return 32 bit color
	uint32_t color = ledStrip.Color(r, g, b, 50);
	// number of flashes
	int flashCount = random(9, 15); //5-15
	// flash white brightness range - 0-255
	int flashBrightnessMin = 10;
	int flashBrightnessMax = 30;  //can go to 255 //todo see what works for reliant
	// flash duration range - ms
	int flashDurationMin = 5;
	int flashDurationMax = 75;
	// flash off range - ms
	int flashOffsetMin = 0;
	int flashOffsetMax = 75;
	// time to next flash range - ms
	int nextFlashDelayMin = 1;
	int nextFlashDelayMax = 50;
	// map white value to volume - louder is brighter
	int flashBrightness = random(flashBrightnessMin, flashBrightnessMax);

	// debug serial print
	Serial.println("FLASH");
	Serial.print("Brightness: ");
	Serial.println(flashBrightness);
  	Serial.print("flashCount: ");
	Serial.println(flashCount);
	Serial.print("-");

	for (int flash = 0; flash <= flashCount; flash += 1)
	{
		// add variety to color
		int colorV = random(0, 50);

		// flash segments of neopixel strip
		color = ledStrip.Color(r + colorV, g + colorV, b + colorV, flashBrightness);
		ledStrip.fill(color, 0, 2 );  // 0,4
		ledStrip.show();
   // delay(2000);
		delay(random(flashOffsetMin, flashOffsetMax));
    colorV = random(0, 50);
    color = ledStrip.Color(r + colorV, g + colorV, b + colorV, flashBrightness);
		ledStrip.fill(color, 4, 2);  //8,4
		ledStrip.show();
  //  delay(2000);
		delay(random(flashOffsetMin, flashOffsetMax));
    colorV = random(0, 50);
    color = ledStrip.Color(r + colorV, g + colorV, b + colorV, flashBrightness);
		ledStrip.fill(color, 6, 2);  //4,4
		ledStrip.show();
  //  delay(2000);
		delay(random(flashOffsetMin, flashOffsetMax));
    colorV = random(0, 50);
    color = ledStrip.Color(r + colorV, g + colorV, b + colorV, flashBrightness);
		ledStrip.fill(color, 2, 2);  //9,14
		ledStrip.show();
  //  delay(2000);
		delay(random(flashDurationMin, flashDurationMax));
		ledStrip.clear();
		ledStrip.show();
   // delay(2000);
		delay(random(nextFlashDelayMin, nextFlashDelayMax));
	}
	// delay before start over
	delay(3000);

}