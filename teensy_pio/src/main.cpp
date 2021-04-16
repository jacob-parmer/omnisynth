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
int noteFreqSeq = 0;
void sequencer() {
  myAnalogS.myCvTable[VCO_CV_0] = noteFreqSeq;
  noteFreqSeq += 0xF;
  if (noteFreqSeq > 0x7FFF) {
    noteFreqSeq = 0;
  }
}
void updateAnalog() {
  myAnalogS.update();
}

PeriodicTimer t_updateCV;
PeriodicTimer t_sequencer;

void setup() {
  Serial.begin(9600);
  myAnalogS.setup();
  t_updateCV.begin(updateAnalog, 1_kHz);
  // t_sequencer.begin(sequencer, 1_Hz);
  myAnalogS.myCvTable[VCO_CV_0] = -DAC_FULLSCALE;
  // myAnalogS.myCvTable[WAVE_SEL_0] = -1000;
  myAnalogS.myCvTable[MIX_BALANCE_CV_0] = -DAC_FULLSCALE/2;
  // myAnalogS.myCvTable[VCF_CV_0] = -0xFF;
  myAnalogS.myCvTable[VCA_CV_0] = DAC_FULLSCALE;

  // myAnalogS.myCvTable[8 + VCO_CV_0] = DAC_FULLSCALE/2;
  // myAnalogS.myCvTable[8 +WAVE_SEL_0] = -1000;
  // myAnalogS.myCvTable[8 +MIX_BALANCE_CV_0] = -DAC_FULLSCALE/2;
  // myAnalogS.myCvTable[8 +VCF_CV_0] = -0xFF;
  // myAnalogS.myCvTable[8 +VCA_CV_0] = DAC_FULLSCALE-1000;
  //myAnalogS.midiPitchTable[0][69];
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

