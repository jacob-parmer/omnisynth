#include "AnalogSynth_omni.h"
#include "LED_omni.h"
#include "usbMIDI_omni.h"
#include <Encoder.h>
#define ENCODER_DO_NOT_USE_INTERRUPTS

OmniMIDI thisMidiDevice = OmniMIDI();
AnalogSynth_omni myAnalogS = AnalogSynth_omni();

Encoder Knob0(10, 11);
Encoder Knob1(20, 21);
Encoder Knob2(22, 23);
Encoder Knob3(0,1);
void setup() {
  myAnalogS.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  int Enc0 = Knob0.read();
  int Enc1 = Knob1.read();
  thisMidiDevice.read();
}