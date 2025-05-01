#include "SoundPlayer.h"

SoundPlayer::SoundPlayer(uint8_t rxPin, uint8_t txPin)
    : serial(rxPin, txPin) {}

void SoundPlayer::begin()
{
    serial.begin(9600);
    delay(100); // Allow DFPlayer to boot

    if (!player.begin(serial))
    {
        Serial.println(F("DFPlayer init failed!"));
        Serial.println(F("1. Check wiring"));
        Serial.println(F("2. Confirm SD card inserted"));
        while (true)
            ; // Halt system
    }

    player.volume(currentVolume); // todo when usiing eeprom: loadVolume(); // Load from EEPROM
}

void SoundPlayer::play(uint8_t fileNumber)
{
    player.playMp3Folder(fileNumber);
}

void SoundPlayer::stop()
{
    player.stop();
}

void SoundPlayer::volumeUp()
{
    if (currentVolume < 30)
    {
        currentVolume++;
        Serial.print("volume: ");
        Serial.println(currentVolume);
        player.volume(currentVolume);
    }
}

void SoundPlayer::volumeDown()
{
    if (currentVolume > 0)
    {
        currentVolume--;
        Serial.print("volume: ");
        Serial.println(currentVolume);
        player.volume(currentVolume);
    }
}

void SoundPlayer::setVolume(uint8_t vol)
{
    currentVolume = constrain(vol, 0, 30);
    player.volume(currentVolume);
}

uint8_t SoundPlayer::getVolume()
{
    return currentVolume;
}
