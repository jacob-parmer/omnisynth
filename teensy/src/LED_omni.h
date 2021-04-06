// Author: Jacob Parmer
// Date: Mar 20, 2021

#include <OctoWS2811.h>
#include <string>

#define RED    0xFF0000
#define GREEN  0x84C781
#define BLUE   0x0000FF
#define YELLOW 0xFFFF00
#define PINK   0xFF1088
#define ORANGE 0xE05800
#define WHITE  0xFFFFFF
#define OFF    0x000000

#define SEL_BASIC 0
#define SEL_VOLUME_METER 1
#define NUM_PATTERNS 2

const int NUM_LEDS = 20;
const int DATA_PIN = 2;

DMAMEM int display_memory[NUM_LEDS*6];
int drawing_memory[NUM_LEDS*6];
const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(NUM_LEDS, display_memory, drawing_memory, config);

String supported_patterns[NUM_PATTERNS] = {"BASIC", "VOLUME_METER"};

/* ------------------- CLASS DEFINITION ------------------- */
class OmniLED {

    public:
        OmniLED();
        void setup();
        void setLEDs(std::vector<midi_message> notes_on);
        void show();

    private:
        int selected_pattern;
        void setPattern(String pattern);
        void BASIC(std::vector<midi_message> notes_on);
        void VOLUME_METER(std::vector<midi_message> notes_on);
};

/* ------------------- PUBLIC FUNCTIONS ------------------- */
OmniLED::OmniLED() {
    return;
}

void OmniLED::setup() {
    leds.begin();
    leds.show();
    this->setPattern("BASIC");
    return;
}

void OmniLED::setLEDs(std::vector<midi_message> notes_on) {

    switch(this->selected_pattern) {
         case SEL_BASIC:
            this->BASIC(notes_on);

        case SEL_VOLUME_METER:
            this->VOLUME_METER(notes_on);       
    }  
    return;
}

void OmniLED::show() {
    leds.show();
    return;
}

/* ------------------- PRIVATE FUNCTIONS ------------------- */
void OmniLED::setPattern(String pattern) {
    bool found = false;
    for (unsigned int i=0; i<sizeof(supported_patterns); i++) {
        if (pattern == supported_patterns[i]) {
            this->selected_pattern = i;
            found = true;
        }
    }

    if (found == false) {
        Serial.println("Selected pattern value not found in supported_patterns. ");
    }
}

void OmniLED::BASIC(std::vector<midi_message> notes_on) {
    if (notes_on.empty()) {

         for (int i=0; i < leds.numPixels(); i++) {
            leds.setPixel(i, 0, 0, 0);
        }

    } else {

        int red = int(avgNoteVelocity(notes_on) * 2);
        for (int i=0; i < leds.numPixels(); i++) {

            Serial.print("Setting LED ");
            Serial.print(i);
            Serial.print(" to red value ");
            Serial.println(red);
            
            leds.setPixel(i, red, 0, 0);
        }
    }
}

void OmniLED::VOLUME_METER(std::vector<midi_message> notes_on) {
    // NOT YET IMPLEMENTED
    return;
}

