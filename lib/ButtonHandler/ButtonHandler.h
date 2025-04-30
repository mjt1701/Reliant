#pragma once
#include <Arduino.h>

class ButtonHandler
{
public:
    ButtonHandler(uint8_t pin, bool activeState = LOW, unsigned long longPress = 2000, unsigned long debounce = 50);

    void reset();
    void update();

    bool isShortPress();
  //  bool isLongPress();       // true after long press and release
 //   bool isNewLongPress();    // true the moment long press starts
    bool wasLongPressStart(); // Returns true once when long press first detected
    bool isHeldLong();        // True as long as the button is held beyond long press duration
    bool isPhysicallyReleased() const;
    

private:
    uint8_t pin;
    bool activeState;
    unsigned long longPressDuration;
    unsigned long debounceDelay;

    bool shortPressDetected = false;
    bool longPressDetected = false;
    bool longPressJustStarted = false;
    bool isPressing = false;
    

    unsigned long pressStartTime = 0;
    unsigned long lastDebounceTime = 0;
    bool lastReading = HIGH; // Default unpressed for pull-up config
};
