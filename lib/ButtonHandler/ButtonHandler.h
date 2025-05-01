#pragma once
#include <Arduino.h>

class ButtonHandler
{
public:
  ButtonHandler(uint8_t pin);
  void update();
   void reset(); 
   bool wasShortPressed() ;
  bool wasLongPressStart();
  bool wasLongPressed();

private:
  uint8_t pin;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
bool currentStableState = HIGH;
bool lastStableState = HIGH;
unsigned long pressStartTime = 0;
bool shortPressReported = false;
bool longPressStarted = false;      // for start
bool longPressReleased = false;     // for end
unsigned long  longPressDuration = 1500;

};