#include "SoundPlayer.h"

SoundPlayer::SoundPlayer(uint8_t rxPin, uint8_t txPin)
    : serial(rxPin, txPin) {}

void SoundPlayer::begin(uint8_t volume) {
    serial.begin(9600);
    delay(100); // Allow DFPlayer to boot

    if (!player.begin(serial)) {
        Serial.println(F("DFPlayer init failed!"));
        Serial.println(F("1. Check wiring"));
        Serial.println(F("2. Confirm SD card inserted"));
        while (true); // Halt system
    }

    player.volume(volume);
}

void SoundPlayer::play(uint8_t fileNumber) {
    player.playMp3Folder(fileNumber);
}

void SoundPlayer::stop() {
    player.stop();
}
