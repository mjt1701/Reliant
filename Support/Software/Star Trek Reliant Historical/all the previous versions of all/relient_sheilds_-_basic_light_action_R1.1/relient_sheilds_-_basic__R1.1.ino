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
  
*/

// Include Adafruit NeoPixel Library
#include <Adafruit_NeoPixel.h>

// Define Arduino connection pin for LED data 
const int ledPin = 3;

// Define the number of LEDs
const int ledNum = 8;

//Define nubmer of LEDs in a group (each light is individual
const int ledInGroup = 1;

//Define maximum briteness
const int maxWBright = 32;

int figureWBrightness = maxWBright; 
//set led color
//undo commenting below to play with colors,  set for white for now
// const  ledCplor = ledStrip.Color(0, 0, 0, figureWBrightness

const int offDelay = 300; 
const int firstDelay = 3000; 
const int afterDelay = 4000;


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

Serial.println("cleared them all");


}

// ********************************************* LOOP starts
void loop() {

// Turn all lights on
    ledStrip.fill(ledStrip.Color(0, 0, figureWBrightness, 0), 0, ledNum);
    ledStrip.show(); // Show at once
Serial.println("turn all on");
// Delay until first light goes out
delay(firstDelay);

//Turn off one by one

for (int i=0; i<ledNum; i++) {
  // Turn off one led
  ledStrip.setPixelColor ( i, 0);
  ledStrip.show();

  // Delay until next led goes out
  delay(offDelay);
    
}

delay(afterDelay);
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
