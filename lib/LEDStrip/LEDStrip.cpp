#include "LEDStrip.h"

LEDStrip::LEDStrip(uint8_t dataPin, uint16_t numPixels, neoPixelType type)
    : strip(numPixels, dataPin, type), numPixels(numPixels) {}

void LEDStrip::begin(uint8_t brightness) {
    strip.begin();
    strip.setBrightness(brightness);
    strip.show(); // Ensure LEDs start off
}

void LEDStrip::fillColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    uint32_t color = strip.Color(r, g, b, w);
    strip.fill(color, 0, numPixels);
    strip.show();
}

void LEDStrip::clear() {
    strip.clear();
    strip.show();
}

void LEDStrip::show() {
    strip.show();
}

void LEDStrip::flash(uint8_t minFlashes, uint8_t maxFlashes) {
    int flashCount = random(minFlashes, maxFlashes);
    int r = random(40, 80);
    int g = random(10, 25);
    int b = random(0, 10);
    int w = 50;

    for (int i = 0; i < flashCount; ++i) {
        uint32_t color = strip.Color(
            r + random(0, 50),
            g + random(0, 50),
            b + random(0, 50),
            random(10, 30)
        );

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

void LEDStrip::setPixelColor(uint16_t pixelIndex, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    if (pixelIndex < numPixels) {
        uint32_t color = strip.Color(r, g, b, w);
        strip.setPixelColor(pixelIndex, color);
        strip.show();
    }
}
