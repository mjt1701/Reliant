#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

class SoundPlayer
{
public:
    SoundPlayer(uint8_t rxPin, uint8_t txPin);

    void begin();
    void play(uint8_t fileNumber);
    void volumeUp();
    void volumeDown();
    void setVolume(uint8_t volume);
    uint8_t getVolume();

private:
    SoftwareSerial serial;
    DFRobotDFPlayerMini player;
    uint8_t currentVolume = 15; 
};

#endif
