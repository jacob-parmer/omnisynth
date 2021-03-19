#include <FastLED.h>
#include "src/config.h"
#include "src/usbMIDI_omni.h"

OmniMIDI thisMidiDevice = OmniMIDI();
Config conf = Config();
const int NUM_LEDS = 20;
const int DATA_PIN = 2;
CRGB leds[NUM_LEDS];
  
void setup() {
	delay(3000); // safety check while powering up
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS);

  Serial.println("Starting Execution...");
}

void loop() {
  // put your main code here, to run repeatedly.
    //int microsec = 2000000 / leds.numPixels();
    thisMidiDevice.read();
}
