#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

class SoundPlayer {
public:
    SoundPlayer(uint8_t rxPin, uint8_t txPin);

    void begin(uint8_t volume);
    void play(uint8_t fileNumber);
    void stop();                       //!  todo for use with long buttonpress 

private:
    SoftwareSerial serial;
    DFRobotDFPlayerMini player;
};

#endif
