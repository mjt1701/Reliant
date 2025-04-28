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
  
*/

#include "Arduino.h"
#include "SoftwareSerial.h"       //SOftware Serial Bus to support DFPlayer
#include "DFRobotDFPlayerMini.h"  //DFRobot DFPlayer Library
#include <Adafruit_NeoPixel.h>    //Adafruit NeoPixel Library

const int buttonPin = 6;    // Define Arduino connection pin for button
const int ledPin = 3;       // Define Arduino connection pin for LED data
const int ledNum = 8;       // Define the total number of LEDs
const int ledInGroup = 1;   // Define nubmer of LEDs in a group (each light is individual
const int maxWBright = 32;  // Define maximum LED brightness
//const ??? ledColor = ????   // LED color
const int offDelay = 300;   // Time until next LED goes out
const int firstDelay = 3000;
const int afterDelay = 4000;
const int delayUntilLightsOff = 4000;  // Amount of time to wait until lights start turning off

const bool buttonActivated = LOW;  //Indicate that button is pushed
const uint8_t Reliant1mp3Num = 1;  //mp3 file number of intro
const uint8_t Reliant2mp3Num = 2;  //mp3 file number of sheilds going down

int figureWBrightness = maxWBright;

//set led color


SoftwareSerial mySoftwareSerial(10, 11);  // RX, TX
DFRobotDFPlayerMini myDFPlayer;


// void printDetail(uint8_t type, int value);

Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(ledNum, ledPin, NEO_GRBW + NEO_KHZ800);

void setup() {
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);  //turn on internal resistor

  Serial.begin(9600);  // Setup Serial Monitor

  // Print to Serial Monitor
  Serial.println("Starting Program");

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    // change this to flash the lights
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true)
      ;
  }

  ledStrip.begin();  // Initialize the LEDs
  clearLEDs();       // Clear all LEDs
  ledStrip.show();   // Show the result of clearing the LEDs

  Serial.println("cleared them all");

  myDFPlayer.volume(20);                     //Set volume value. From 0 to 30
  myDFPlayer.playMp3Folder(Reliant1mp3Num);  //Play the first mp3
}

// ********************************************* LOOP starts
void loop() {

  // Turn all lights on
  ledStrip.fill(ledStrip.Color(0, 0, figureWBrightness, 0), 0, ledNum);
  ledStrip.show();  // Show at once
  Serial.println("turn all on");
  // Delay until first light goes out
  delay(firstDelay);      // Delay until first light goes out

  //Turn off one by one
  for (int i = 0; i < ledNum; i++) {
    // Turn off one led
    ledStrip.setPixelColor(i, 0);
    ledStrip.show();
    delay(offDelay);    // Delay until next led goes out
  }

  delay(afterDelay);
  //Finished
  Serial.println("Loop Ended!!!");
  Serial.println(".................................");


  //Play intro
  // if not already playing  ++++++++++++++++++



 // if (digitalRead(buttonPin) == buttonActivated) {
 //   myDFPlayer.playMp3Folder(Reliant2mp3Num);
    // wait x seconds and start lights
 //   delay(3000);
 //   Serial.println("at this point the lights would start");
    // }


    if (digitalRead(buttonPin) == buttonActivated) {
      Serial.println("the button was pushed");
      myDFPlayer.playMp3Folder(Reliant2mp3Num);

      // wait x seconds and start lights
      delay(delayUntilLightsOff);
      Serial.println("at this point the lights would start going off");

    } else if (myDFPlayer.readState() == 0)  //put the if here
      myDFPlayer.playMp3Folder(Reliant2mp3Num);
    {
      // if intro (mp3=1) has stopped then restart it
    }
  // }
}
// Function to set all LEDs off  *****change??????
void clearLEDs() {
  // Cycle through all LEDs
  for (int i = 0; i < ledNum; i++) {
    // Set color to zero which is off
    ledStrip.setPixelColor(i, 0);
  }
}


/*
  void printDetail(uint8_t type, int value) {
    switch (type) {
      case TimeOut:
        Serial.println(F("Time Out!"));
        break;
      case WrongStack:
        Serial.println(F("Stack Wrong!"));
        break;
      case DFPlayerCardInserted:
        Serial.println(F("Card Inserted!"));
        break;
      case DFPlayerCardRemoved:
        Serial.println(F("Card Removed!"));
        break;
      case DFPlayerCardOnline:
        Serial.println(F("Card Online!"));
        break;
      case DFPlayerPlayFinished:
        Serial.print(F("Number:"));
        Serial.print(value);
        Serial.println(F(" Play Finished!"));
        break;
      case DFPlayerError:
        Serial.print(F("DFPlayerError:"));
        switch (value) {
          case Busy:
            Serial.println(F("Card not found"));
            break;
          case Sleeping:
            Serial.println(F("Sleeping"));
            break;
          case SerialWrongStack:
            Serial.println(F("Get Wrong Stack"));
            break;
          case CheckSumNotMatch:
            Serial.println(F("Check Sum Not Match"));
            break;
          case FileIndexOut:
            Serial.println(F("File Index Out of Bound"));
            break;
          case FileMismatch:
            Serial.println(F("Cannot Find File"));
            break;
          case Advertise:
            Serial.println(F("In Advertise"));
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
  }
*/
