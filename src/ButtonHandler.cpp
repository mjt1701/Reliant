#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(uint8_t pin, bool activeState, unsigned long longPress, unsigned long debounce)
    : pin(pin), activeState(activeState), longPressDuration(longPress), debounceDelay(debounce) {
    pinMode(pin, INPUT);
}

void ButtonHandler::reset() {
    shortPressDetected = false;
    longPressDetected = false;
}

void ButtonHandler::update() {
    bool reading = digitalRead(pin);

    if (reading != lastReading) {
        lastDebounceTime = millis(); // reset debounce timer
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        bool pressed = (reading == activeState);

        if (pressed && !isPressing) {
            // Button just pressed
            isPressing = true;
            pressStartTime = millis();
            longPressDetected = false;
        } else if (pressed && isPressing) {
            // Button held
            if (!longPressDetected && (millis() - pressStartTime >= longPressDuration)) {
                longPressDetected = true;
                Serial.println("Long press triggered");
            }
        } else if (!pressed && isPressing) {
            // Button released
            if (!longPressDetected) {
                shortPressDetected = true;
            }
            isPressing = false;
  //          longPressDetected = false;
        }
    }

    lastReading = reading;
}

bool ButtonHandler::isShortPress() {
    if (shortPressDetected) {
        shortPressDetected = false;
        return true;
    }
    return false;
}

bool ButtonHandler::isLongPress() {
    if (longPressDetected && !isPressing) {
        longPressDetected = false; // Reset only after release
        return true;
    }
    return false;
}
