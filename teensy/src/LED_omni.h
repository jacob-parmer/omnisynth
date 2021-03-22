// Author: Jacob Parmer
// Date: Mar 20, 2021

#include <OctoWS2811.h>

#define RED    0xFF0000
#define GREEN  0x84C781
#define BLUE   0x0000FF
#define YELLOW 0xFFFF00
#define PINK   0xFF1088
#define ORANGE 0xE05800
#define WHITE  0xFFFFFF
#define OFF    0x000000

const int NUM_LEDS = 20;
const int DATA_PIN = 2;

DMAMEM int display_memory[NUM_LEDS*6];
int drawing_memory[NUM_LEDS*6];
const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(NUM_LEDS, display_memory, drawing_memory, config);

class OmniLED {

    public:
        OmniLED();
        void setup();
        void getNextLEDState(std::vector<midi_message> notes_on);
        void setCurrentLEDState();
        void show();

    private:

};

OmniLED::OmniLED() {
    return;
}

void OmniLED::setup() {
    leds.begin();
    leds.show();
    return;
}

void OmniLED::getNextLEDState(std::vector<midi_message> notes_on) {

    if (notes_on.empty()) {

         for (int i=0; i < leds.numPixels(); i++) {
            leds.setPixel(i, 0, 0, 0);
        }

    } else {

        int red = int(avgNoteVelocity(notes_on) * 2);
        Serial.print("Red value: ");
        Serial.println(red);
        for (int i=0; i < leds.numPixels(); i++) {
            leds.setPixel(i, red, 0, 0);
        }
    }
    
    return;
}

void OmniLED::setCurrentLEDState() {
    return;
}

void OmniLED::show() {
    leds.show();
    return;
}

