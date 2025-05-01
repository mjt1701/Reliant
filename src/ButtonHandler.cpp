

// ButtonHandler::ButtonHandler(uint8_t pin, bool activeState, unsigned long longPress, unsigned long debounce)
//     : pin(pin), activeState(activeState), longPressDuration(longPress), debounceDelay(debounce) {
//     pinMode(pin, INPUT);
// }

// void ButtonHandler::reset() {
//     shortPressDetected = false;
//     longPressDetected = false;
//     longPressJustStarted = false;
//     Serial.println("ButtonHandler::reset() called");
// }

// void ButtonHandler::update() {
//     bool reading = digitalRead(pin);


//     if (reading != lastReading) {
//         lastDebounceTime = millis(); // reset debounce timer
//     }

//     if ((millis() - lastDebounceTime) > debounceDelay) {
//         bool pressed = (reading == activeState);

//         if (pressed && !isPressing) {
//             // Button just pressed
//             isPressing = true;
//             pressStartTime = millis();
//             longPressDetected = false;
//             longPressJustStarted = false;
//         } else if (pressed && isPressing) {
//             // Button held
//             if (!longPressDetected && (millis() - pressStartTime >= longPressDuration)) {
//                 longPressDetected = true;
//                 longPressJustStarted = true; // <-- Flag moment of long press
//                 Serial.println("Long press triggered");
//             }
//         } else if (!pressed && isPressing) {
//             // Button released
//             if (!longPressDetected) {
//                 shortPressDetected = true;
//             }
//             isPressing = false;
//         }
//     }

//     lastReading = reading;
// }

// bool ButtonHandler::isShortPress() {
//     if (shortPressDetected) {
//         shortPressDetected = false;
//         return true;
//     }
//     return false;
// }



// bool ButtonHandler::wasLongPressStart() {
//     if (longPressDetected) {
//         longPressDetected = false;
//         return true;
//     }
//     return false;
// }

// bool ButtonHandler::isHeldLong() {
//     return isPressing && (millis() - pressStartTime >= longPressDuration);
// }

// bool ButtonHandler::isPhysicallyReleased() const {
//     return lastReading != activeState;
// }

#include <Arduino.h>
#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(uint8_t pin) : pin(pin) {
    pinMode(pin, INPUT_PULLUP);
    currentStableState = digitalRead(pin);
    lastStableState = currentStableState;
}

void ButtonHandler::update() {
    bool reading = digitalRead(pin);
    unsigned long currentTime = millis();

    if (reading != currentStableState) {
        lastDebounceTime = currentTime;
        currentStableState = reading;
    }

    if ((currentTime - lastDebounceTime) > debounceDelay) {
        if (currentStableState != lastStableState) {
            lastStableState = currentStableState;

            if (currentStableState == LOW) {
                // Button just pressed
                Serial.println("Debounced State Changed: PRESSED");
                pressStartTime = currentTime;
                longPressStarted = false;
            } else {
                // Button just released
                Serial.println("Debounced State Changed: RELEASED");
                unsigned long pressDuration = currentTime - pressStartTime;

                if (pressDuration > 1000) {
                    longPressReleased = true;
                    Serial.println("Long press released");
                } else {
                    shortPressReported = true;
                    Serial.println("Short press detected");
                }
            }
        } else if (currentStableState == LOW) {
            if (!longPressStarted && (currentTime - pressStartTime >= longPressDuration)) {
                longPressStarted = true;
                Serial.println("Long press started");
            }
        }
    }
}


bool ButtonHandler::wasShortPressed() {
    if (shortPressReported) {
        shortPressReported = false;
        return true;
    }
    return false;
}

void ButtonHandler::reset() {
    shortPressReported = false;
    longPressReleased = false;
    longPressStarted = false;
}

bool ButtonHandler::wasLongPressStart() {
    if (longPressStarted) {
        longPressStarted = false;
        return true;
    }
    return false;
}

bool ButtonHandler::wasLongPressed() {
    if (longPressReleased) {
        longPressReleased = false;
        return true;
    }
    return false;
}