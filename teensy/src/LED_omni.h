// Author: Jacob Parmer
// Date: Mar 20, 2021
#ifndef LED_OMNI_H
#define LED_OMNI_H


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <string>
#include <vector>
#include <stdlib.h>
#include "usbMIDI_omni.h"

#define PIN            12

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      20

#define SEL_BASIC 0
#define SEL_VOLUME_METER 1
#define SEL_SNOWFLAKES 2
#define NUM_PATTERNS 3

const int NUM_LEDS = 20;
const int DATA_PIN = 2;

const String supported_patterns[NUM_PATTERNS] = {"BASIC", "VOLUME_METER", "SNOWFLAKES"};

/* ------------------- CLASS DEFINITION ------------------- */
class OmniLED {

    public:
        OmniLED();
        void setup();
        void setLEDs(std::vector<midi_message> notes_on, int noteOnOrOff);
        void show();

    private:
        int selected_pattern;
        void setPattern(String pattern);
        void BASIC(std::vector<midi_message> notes_on);
        void VOLUME_METER(std::vector<midi_message> notes_on);
        void SNOWFLAKES(std::vector<midi_message> notes_on, int noteOnOrOff);
        
        Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
};

#endif