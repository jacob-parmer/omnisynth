#include "src/config.h"
#include "src/usbMIDI_omni.h"
#include "src/LED_omni.h"

OmniMIDI thisMidiDevice = OmniMIDI();
OmniLED thisLEDStrip = OmniLED();
Config conf = Config();

void setup() {
	delay(3000); // safety check while powering up
  Serial.begin(9600);
  thisLEDStrip.setup();
  Serial.println("Starting Execution...");
}

void loop() {
    thisMidiDevice.read();
    thisLEDStrip.setLEDs(thisMidiDevice.getNotesOn());
    thisLEDStrip.show();
}
