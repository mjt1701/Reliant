#include "LEDStrip.h"

LEDStrip::LEDStrip(uint8_t dataPin, uint16_t numPixels, neoPixelType type)
	: strip(numPixels, dataPin, type), numPixels(numPixels) {}

void LEDStrip::begin(uint8_t brightness)
{
	strip.begin();
	strip.setBrightness(brightness);
	strip.show(); // Ensure LEDs start off
}

void LEDStrip::fillColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
	uint32_t color = strip.Color(r, g, b, w);
	strip.fill(color, 0, numPixels);
	strip.show();
}

void LEDStrip::clear()
{
	strip.clear();
	strip.show();
}

void LEDStrip::show()
{
	strip.show();
}

void LEDStrip::flash(uint8_t minFlashes, uint8_t maxFlashes)
{
	int flashCount = random(minFlashes, maxFlashes);
	int r = random(40, 80);
	int g = random(10, 25);
	int b = random(0, 10);
	int w = 50; // todo unused variable

	for (int i = 0; i < flashCount; ++i)
	{
		uint32_t color = strip.Color(
			r + random(0, 50),
			g + random(0, 50),
			b + random(0, 50),
			random(10, 30));

		strip.fill(color, 0, 2);
		strip.show();
		delay(random(5, 75));

		strip.fill(color, 4, 2);
		strip.show();
		delay(random(5, 75));

		strip.fill(color, 6, 2);
		strip.show();
		delay(random(5, 75));

		strip.fill(color, 2, 2);
		strip.show();
		delay(random(5, 75));

		strip.clear();
		strip.show();
		delay(random(1, 50));
	}
}

void LEDStrip::setPixelColor(uint16_t pixelIndex, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
	if (pixelIndex < numPixels)
	{
		uint32_t color = strip.Color(r, g, b, w);
		strip.setPixelColor(pixelIndex, color);
		strip.show();
	}
}

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
