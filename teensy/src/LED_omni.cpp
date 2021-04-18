#include "LED_omni.h"

/* ------------------- PUBLIC FUNCTIONS ------------------- */
OmniLED::OmniLED() {
    return;
}

void OmniLED::setup() {
    pixels.begin(); // This initializes the NeoPixel library.
    this->setPattern("SNOWFLAKES");
    
    for (int i=0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(0,0,0));
    }
    return;
}

void OmniLED::setLEDs(std::vector<midi_message> notes_on, int noteOnOrOff) {

    switch(this->selected_pattern) {
         case SEL_BASIC:
            this->BASIC(notes_on);

        case SEL_VOLUME_METER:
            this->VOLUME_METER(notes_on);    

        case SEL_SNOWFLAKES:
            this->SNOWFLAKES(notes_on, noteOnOrOff);   
    }  
    return;
}

void OmniLED::show() {
    pixels.show();
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
    return;
}

void OmniLED::BASIC(std::vector<midi_message> notes_on) {
    
    if (notes_on.empty()) {

         for (int i=0; i < NUMPIXELS; i++) {
             pixels.setPixelColor(i, pixels.Color(0,0,0));
        }

    } else {

        int red = int(avgNoteVelocity(notes_on) * 2);
        for (int i=0; i < NUMPIXELS; i++) {
            pixels.setPixelColor(i, pixels.Color(0,red,0));
        }
    }
    return;
}

void OmniLED::SNOWFLAKES(std::vector<midi_message> notes_on, int noteOnOrOff) {

    if (noteOnOrOff) {
        for (std::vector<midi_message>::iterator it = notes_on.begin(); it != notes_on.end(); ++it) {
            midi_message note = *it;

            int r = (rand() % ( 255 + 1 ));
            int g = (rand() % ( 255 + 1 ));
            int b = ((r + g) % ( 255 + 1 ));

            int j = note[KEY_INDEX] % NUMPIXELS;
            int k = (j + (5 % NUMPIXELS) % NUMPIXELS);
            int l = (j - (5 % NUMPIXELS) % NUMPIXELS);
            int m = (j + (13 % NUMPIXELS) % NUMPIXELS);
            int n = (j - (13 % NUMPIXELS) % NUMPIXELS);
            pixels.setPixelColor(j, pixels.Color(r, g, b));
            pixels.setPixelColor(k, pixels.Color(r, g, b));
            pixels.setPixelColor(l, pixels.Color(r, g, b));
            pixels.setPixelColor(m, pixels.Color(r, g, b));
            pixels.setPixelColor(n, pixels.Color(r, g, b));
        }
    }
}

void OmniLED::VOLUME_METER(std::vector<midi_message> notes_on) {
    // NOT YET IMPLEMENTED
    return;
}


