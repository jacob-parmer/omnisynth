// Author: Jacob Parmer
// Date: Mar 20, 2021
#ifndef LED_OMNI_H
#define LED_OMNI_H
#include <OctoWS2811.h>
#include <stdio.h>
#include "usbMIDI_omni.h"

#define RED    0xFF0000
#define GREEN  0x84C781
#define BLUE   0x0000FF
#define YELLOW 0xFFFF00
#define PINK   0xFF1088
#define ORANGE 0xE05800
#define WHITE  0xFFFFFF
#define OFF    0x000000

#define NUM_LEDS 20
#define DATA_PIN 2

class OmniLED {

    public:
        OmniLED();
        void setup();
        void getNextLEDState(std::vector<midi_message> notes_on);
        void setCurrentLEDState();
        void show();

    private:

};


#endif