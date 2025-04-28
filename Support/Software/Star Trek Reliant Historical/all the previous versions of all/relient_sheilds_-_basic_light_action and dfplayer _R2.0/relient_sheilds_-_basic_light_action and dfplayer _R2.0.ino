/*
  Relent Sheilds 
  Lights set of WS2812 RGBW LEDs to re-enact the relient sheilds going down
  My Matt Troha

  Revision History
  Rev 1.0 
  set up software to perform basic light action of relient sheilds going down
  all lights on, then one by one they turn off

  Rrev 1.1
  updated to run on sumulator wokwi.com
  minor updates

  Renamed to include both light and sound
  Rev1.0

  Rev2.0 changed to Case statement for main flow


  
*/

#include "Arduino.h"
#include "SoftwareSerial.h"       //SOftware Serial Bus to support DFPlayer
#include "DFRobotDFPlayerMini.h"  //DFRobot DFPlayer Library
#include <Adafruit_NeoPixel.h>    //Adafruit NeoPixel Library

const int rxPin = 10;                    // Define Arduino connection pin fo software serial rx
const int txPin = 11;                    // Define Arduino connection pin fo software serial tx
const int buttonPin = 6;                 // Define Arduino connection pin for button
const int ledPin = 3;                    // Define Arduino connection pin for LED data
const int ledNum = 8;                    // Define the total number of LEDs
const int ledInGroup = 1;                // Define nubmer of LEDs in a group (each light is individual
const int maxBright = 32;                // Define maximum LED brightness
const uint8_t r = 0;                     // Define the LED color RGB
const uint8_t g = 0;                     // Define the LED color RGB
const uint8_t b = 255;                   // Define the LED color RGB
const int offDelay = 300;                // Time until next LED goes out
const unsigned long mp3WaitTime = 3000;  // Amount of time to wait from start of mp3-2 and sheilds go down
const int afterDelay = 4000;
const unsigned long delayUntilLightsOff = 4000;  // Amount of time to wait until lights start turning off
const int volume = 20;                           // Sound volume for dfPlayer
const bool buttonActivated = LOW;                // Indicate that button is pushed
const uint8_t Reliant1mp3Num = 1;                // mp3 file number of intro
const uint8_t Reliant2mp3Num = 2;                // mp3 file number of sheilds going down

int mode = 0;  // mode state - 1. MP3-1 running
               //              2. MP3-2 running only
               //              3. MP3-2 running and sheilds activated

unsigned long mp3s1Time;    // Time that mp3 file 1 starts
unsigned long mp3s2Time;    // Time that mp3 file 2 starts
int ledNextTurnOffNum = 0;  // number of LED to turn off in sheilddown sequence
unsigned long buttonPressTime;

SoftwareSerial mySoftwareSerial(rxPin, txPin);  // RX, TX
DFRobotDFPlayerMini myDFPlayer;

Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(ledNum, ledPin, NEO_GRBW + NEO_KHZ800);
//**************are the lights that NEIL has RGB or RGBW???????????????

void setup() {
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);  //turn on internal resistor  HOW IS IT WIRED?????

  mySoftwareSerial.begin(9600);
  Serial.begin(115200);  // Setup Serial Monitor

  // Print to Serial Monitor
  Serial.println("Starting Program");

  Serial.print("rx pin: ");
  Serial.println(rxPin);
  Serial.print("tx pin: ");
  Serial.println(txPin);

  Serial.print(" !myDFPlayer.begin(mySoftwareSerial): ");
  Serial.println(!myDFPlayer.begin(mySoftwareSerial));



  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with dfPlayer
    // change this to flash the lights
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true)
      ;
  }

  // Initialize the LEDs
  ledStrip.begin();
  clearLEDs();
  ledStrip.show();
  ledStrip.setBrightness(maxBright);
  ledStrip.fill(ledStrip.Color(r, g, b, 0), 0, ledNum);
  ledStrip.show();

  // Initialize the dfPlayer
  myDFPlayer.volume(volume);
  myDFPlayer.playMp3Folder(Reliant1mp3Num);  // Start playing the first mp3
  mp3s1Time = millis();
  mode = 1;
}

// ********************************************* LOOP starts
void loop() {

  switch (mode) {
    case 1:  // mp3-1 playing
      // Look for button press
      if (digitalRead(buttonPin) == buttonActivated) {
        myDFPlayer.playMp3Folder(Reliant2mp3Num);
        buttonPressTime = millis();
        mode = 2;
        break;
      }

    case 2:  // mp3-2 playing only ie need to wait for time to start sheilds down
      if (millis() - buttonPressTime > mp3WaitTime) {
        mode = 3;
      }
      break;

    case 3:  // mp3-2 playing and shwilds going down
      if (ledNextTurnOffNum = 0) {
        ledStrip.setPixelColor(ledNextTurnOffNum, 0);  // Turn off first LED
        ledStrip.show();
        ledNextTurnOffNum++;
        break;
      }

      if (ledNextTurnOffNum = ledNum) {  // last light is out (or change to nothing and handle after mp3 is done)
        ledNextTurnOffNum = 0;
      } else {
        ledStrip.setPixelColor(ledNextTurnOffNum, 0);  // Turn off next LED
        ledStrip.show();
        ledNextTurnOffNum++;
      }
      break;

    default:;
  }
}



// Function to set all LEDs off  *****change??????
void clearLEDs() {
  // Cycle through all LEDs
  for (int i = 0; i < ledNum; i++) {
    // Set color to zero which is off
    ledStrip.setPixelColor(i, 0);
  }
}
