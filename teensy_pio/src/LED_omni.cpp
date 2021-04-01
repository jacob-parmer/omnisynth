#include "LED_omni.h"
#include "usbMIDI_omni.h"

DMAMEM int display_memory[NUM_LEDS*6];
int drawing_memory[NUM_LEDS*6];
const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(NUM_LEDS, display_memory, drawing_memory, config);
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
