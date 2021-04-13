#include "AnalogSynth_omni.h"
#include "LED_omni.h"
#include <Encoder.h>

using namespace TeensyTimerTool;
#define ENCODER_DO_NOT_USE_INTERRUPTS

OmniMIDI thisMidiDevice = OmniMIDI();
AnalogSynth_omni myAnalogS = AnalogSynth_omni();

Encoder Knob0(10, 11);
Encoder Knob1(20, 21);
Encoder Knob2(22, 23);
Encoder Knob3(0,1);
uint16_t notesCount = 0;

void updateAnalog() {
  myAnalogS.update();
}

PeriodicTimer t_updateCV;

void setup() {
  Serial.begin(9600);
  myAnalogS.setup();
  t_updateCV.begin(updateAnalog, 204_kHz);
  myAnalogS.myCvTable[VCO_CV_0] = 0;
  // myAnalogS.myCvTable[WAVE_SEL_0] = -1000;
  myAnalogS.myCvTable[MIX_BALANCE_CV_0] = -0xFF;
  // myAnalogS.myCvTable[VCF_CV_0] = -0xFF;
  myAnalogS.myCvTable[VCA_CV_0] = 32766;



  //myAnalogS.midiPitchTable[0][69];
  // myAnalogS.myCvTable[MIX_BALANCE_CV_0] = 60000;
  // myAnalogS.myCvTable[8+VCA_CV_0] = 1000;
  // myAnalogS.myCvTable[8+VCO_CV_0] = 60000;
  // myAnalogS.myCvTable[8+VCF_CV_0] = 10000;
  // myAnalogS.myCvTable[16+VCA_CV_0] = 1000;
  // myAnalogS.myCvTable[16+VCO_CV_0] = 60000;
  // myAnalogS.myCvTable[16+VCF_CV_0] = 10000;
}

void loop() {
  // put your main code here, to run repeatedly:
  // int Enc0 = Knob0.read();
  // int Enc1 = Knob1.read();
  // thisMidiDevice.read();
  // std::vector<midi_message> notes = thisMidiDevice.getNotesOn();
  // if (notes.size() > notesCount)
  // {
  //   notesCount = notes.size();
    // myAnalogS.noteOn(notes.back(), notes);
  // }
  
}

