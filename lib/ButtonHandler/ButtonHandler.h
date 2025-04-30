#pragma once
#include <Arduino.h>

class ButtonHandler
{
public:
    ButtonHandler(uint8_t pin, bool activeState = LOW, unsigned long longPress = 2000, unsigned long debounce = 50);

    void reset();
    void update();

    bool isShortPress(); // Call once after update()
    bool isLongPress();  // Call once after update()

private:
    uint8_t pin;
    bool activeState;
    unsigned long longPressDuration;
    unsigned long debounceDelay;

    bool isPressing = false;
    bool longPressDetected = false;
    bool shortPressDetected = false;

    unsigned long pressStartTime = 0;
    unsigned long lastDebounceTime = 0;
    bool lastReading = HIGH; // Default unpressed for pull-up config
};
