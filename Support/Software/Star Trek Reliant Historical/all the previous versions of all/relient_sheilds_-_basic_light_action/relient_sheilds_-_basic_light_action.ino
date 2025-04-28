/*
  Relent Sheilds 
  Lights set of WS2812 RGBW LEDs to re-enact the relient sheilds going down
  My Matt Troha

  Revision History
  Rev 1.0 
  set up software to perform basic light action of relient sheilds going down
  all lights on, then one by one they turn off
  
  
*/

// Include Adafruit NeoPixel Library
#include <Adafruit_NeoPixel.h>

// Define Arduino connection pin for LED data 
const int ledPin = 2;

// Define the number of LEDs
const int ledNum = 56;

//Define nubmer of LEDs in a group (each light is individual
const int ledInGroup = 1;

//Define maximum briteness
const int maxWBright = 122;

int figureWBrightness = maxWBright; 
//set led color
//undo commenting below to play with colors,  set for white for now
// const  ledCplor = ledStrip.Color(0, 0, 0, figureWBrightness

const int offDelay = 100; 
const int firstDelay = 1000; 


// Create instance of NeoPixel class
// Parameter 1 = number of pixels in leds
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(ledNum, ledPin, NEO_GRBW + NEO_KHZ800); 

void setup() {
  // Setup Serial Monitor
  Serial.begin(9600);

  // Print to Serial Monitor
  Serial.println("Starting Program");

  // Initialize the LEDs
  ledStrip.begin(); 
  // Clear them all
  clearLEDs();
  // Show the result of clearing the LEDs
  ledStrip.show();

}

// ********************************************* LOOP starts
void loop() {

// Turn all lights on
    ledStrip.fill(ledStrip.Color(0, 0, 0, figureWBrightness), 1, ledNum);
    ledStrip.show(); // Show at once

// Delay until first light goes out
delay(firstDelay);

//Turn off one by one

for (int i=1; i<ledNum; i++) {
  // Turn off one led
  ledStrip.setPixelColor ( i, 0);
  ledStrip.show();

  // Delay until next led goes out
  delay(offDelay);
    
}



/*
  
  // Loop through each figure circuit
  for(int i = 0; i < numFigs; i ++){
 
    // Read the photoresistor
  //  int photoVal = readMux(i); 


// SIMULATION CODE FOR ABOVE (READING THE PHOTORESISTOR)
 int photoVal = photoTestData[i];
 
 Serial.print("Enter test value: ");
 Serial.print("for cell number :");
 Serial.println(i);
 
   while (Serial.available() == 0) {
  }
    photoVal = Serial.parseInt();
 

    Serial.print("Channel ");
    Serial.print(i);
    Serial.print("     photovalue: ");
    Serial.print(photoVal);
    //Serial.println(" ");
figureWBrightness = 0;
    //If the channel photoresistor read value is above threshold, then light the LED
    if ( photoVal > thresOn[i]) {
      // Set cooresponding brightness based on Figure brightness
      figureWBrightness = maxWBright;   
    }
 
    Serial.print(" figureWBrightness: ");
    Serial.print(figureWBrightness);
    Serial.println(" ");
    //Turn on the light for this channel
    //First Side of lights
    ledStrip.fill(ledStrip.Color(0, 0, 0, figureWBrightness), i*ledInGroup, ledInGroup);
    Serial.print("side 1: ");
    Serial.print("  White : ");
    Serial.print(figureWBrightness);
    Serial.print("  First: ");
    Serial.print(i*ledInGroup);
    Serial.print("  Count: ");
    Serial.print(ledInGroup);
    Serial.println(" ");

    //Second Side of lights   
    ledStrip.fill(ledStrip.Color(0, 0, 0, figureWBrightness), i * ledInGroup + ledNum / 2, ledInGroup);
    ledStrip.show(); // Show both sides at once
    Serial.print("side 2:  ");
    Serial.print("  White : ");
    Serial.print(figureWBrightness);
    Serial.print("  First: ");
    Serial.print(i * ledInGroup + ledNum / 2);
    Serial.print("  Count: ");
    Serial.print(ledInGroup);
    Serial.println(" ");
    
    //delay(10);  //delay for photoresistor reads
  }
  
  Serial.println(" ");
  delay(5000);

*/  

  //Finished
  Serial.println("Loop Ended!!!"); 
  Serial.println(".................................");
}

// Function to set all LEDs off  *****change??????
void clearLEDs()
{
  // Cycle through all LEDs
  for (int i=0; i<ledNum; i++)  
  {
    // Set color to zero which is off
    ledStrip.setPixelColor(i, 0);
  }
}
