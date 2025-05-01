#pragma once

#include <Adafruit_NeoPixel.h>

class LEDStrip
{
public:
    LEDStrip(uint8_t dataPin, uint16_t numPixels, neoPixelType type);

    void begin(uint8_t brightness);
    void fillColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
    void clear();
    void show();
    void flash(uint8_t minFlashes = 9, uint8_t maxFlashes = 15);
    void setPixelColor(uint16_t pixelIndex, uint8_t r, uint8_t g, uint8_t b, uint8_t w);

private:
    Adafruit_NeoPixel strip;
    uint16_t numPixels;
};
