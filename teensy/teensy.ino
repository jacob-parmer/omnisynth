#include "src/AnalogSynth_omni.h"
#include "src/LED_omni.h"
#include <Encoder.h>

using namespace TeensyTimerTool;
#define ENCODER_DO_NOT_USE_INTERRUPTS

OmniMIDI thisMidiDevice = OmniMIDI();
AnalogSynth_omni myAnalogS = AnalogSynth_omni();
OmniLED thisLEDStrip = OmniLED();

Encoder Knob0(10, 11);
Encoder Knob1(20, 21);
Encoder Knob2(22, 23);
Encoder Knob3(0,1);

uint16_t notesCount = 0;
int noteFreqSeq = 0;
short vca = 0;

void sequencer() {
    myAnalogS.myCvTable[VCO_CV_0] = noteFreqSeq;
    //Serial.println(myAnalogS.myCvTable[VCO_CV_0]);
    noteFreqSeq += 0xFF;
    if (noteFreqSeq > 0x7FFF) {
        noteFreqSeq = -DAC_FULLSCALE;
    }
    // myAnalogS.myCvTable[VCO_CV_0] = 0;
    // myAnalogS.myCvTable[MIX_BALANCE_CV_0] = -vca;
    // myAnalogS.myCvTable[VCA_CV_0] = vca;
    // // myAnalogS.myCvTable[VCA_CV_0] = 0;
    // if (vca == 0)
    // {
    //   vca = DAC_FULLSCALE;
    // } else {
    //   vca = 0;
    // }
}
void updateAnalog() {
    myAnalogS.update();
}

PeriodicTimer t_updateCV;
PeriodicTimer t_sequencer;

void setup() {
    
    delay(3000); // safety check while powering up

    // Allows for serial monitoring
    Serial.begin(9600);

    // Setup LEDs

    thisLEDStrip.setup();
    Serial.println("Starting execution...");
    
    // Begin analog synth setup

    /*
    myAnalogS.setup();
    t_updateCV.begin(updateAnalog, 1000_kHz);
    t_sequencer.begin(sequencer, 500_Hz);
    myAnalogS.myCvTable[VCO_CV_0] = DAC_4VOLTS/3; 
    // myAnalogS.myCvTable[MOD_AMT_0] = DAC_FULLSCALE/2;
    myAnalogS.myCvTable[WAVE_SEL_0] = -DAC_FULLSCALE/5;
    // myAnalogS.myCvTable[PWM_CV_0] = DAC_FULLSCALE/3;
    myAnalogS.myCvTable[MIX_BALANCE_CV_0] = -DAC_FULLSCALE/2;
    myAnalogS.myCvTable[FILT_RES_CV_0] = DAC_FULLSCALE/5;
    myAnalogS.myCvTable[VCF_CV_0] = -DAC_FULLSCALE/2;
    myAnalogS.myCvTable[VCA_CV_0] = DAC_4VOLTS;

    // myAnalogS.myCvTable[8 + VCO_CV_0] = DAC_FULLSCALE/2;
    // myAnalogS.myCvTable[8 +WAVE_SEL_0] = -1000;
    // myAnalogS.myCvTable[8 +MIX_BALANCE_CV_0] = -DAC_FULLSCALE/2;
    // myAnalogS.myCvTable[8 +VCF_CV_0] = -0xFF;
    // myAnalogS.myCvTable[8 +VCA_CV_0] = DAC_FULLSCALE-1000;
    //myAnalogS.midiPitchTable[0][69];
    */    

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
    int noteOnOrOff = thisMidiDevice.read();  
    thisLEDStrip.setLEDs(thisMidiDevice.getNotesOn(), noteOnOrOff);
    thisLEDStrip.show();
}
