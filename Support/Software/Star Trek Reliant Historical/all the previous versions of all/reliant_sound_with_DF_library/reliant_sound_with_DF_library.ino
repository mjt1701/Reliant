///              RELIANT PROJECT
///              Sound design and testing
//////////////////////////////////////////

//
//                  by Matt Troha

//Revision Hisory
// changeed name to reliant_sound.ino // mp3.ino  = baseline code
// changeed name to reliant_sound.ino
// Rev 0.1  11/u26/2022
//   Assumes two sound files on SD Card in MP3 folder
//     - 0001 AtRest
//     - 0002 SheildsDown
//   Initial changes from baseline
//     - Made into two buttons, Intialize, and Sheilds Go Down
// changeed name to reliant_sound.ino
//   Moved from descrete commands to DFPlayer to using DF library


#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySerial(10, 11);  // RX, TX
DFRobotDFPlayerMini myDFPlayer;
// # define Start_Byte 0x7E
// # define Version_Byte 0xFF
// # define Command_Length 0x06
// # define End_Byte 0xEF
// # define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]

#define ACTIVATED LOW
#define delay4CMD 100
#define delayInit 1000  //maybe rather grab the time and do a while loop in void loop

//int buttonInitialize = 2;
int buttonSheildsDown = 6;
boolean isPlaying = false;

uint8_t Reliant1mpgNum = 1;  //mp3 number of start up sound
uint8_t Reliant2mpgNum = 2;  //mp3 number of sheilds going down sound



void setup() {
delay (5000);
  // pinMode(buttonInitialize, INPUT);
  // digitalWrite(buttonInitialize,HIGH);
//  pinMode(buttonSheildsDown, INPUT);
//  digitalWrite(buttonSheildsDown, HIGH);

//  mySerial.begin(9600);
  Serial.begin(115200);  //for Serial Print during testing
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));


 // myDFPlayer.volume(20);  //Set volume value. From 0 to 30

  Serial.println(F("DFPlayer Mini online."));



  myDFPlayer.playMp3Folder(Reliant1mpgNum);
}



void loop() {
  delay(5000);
  //check for button press

  /*
 if (digitalRead(buttonSheildsDown) == ACTIVATED)
  {
    myDFPlayer.playMp3Folder(Reliant2mpgNum);  }
*/
}
