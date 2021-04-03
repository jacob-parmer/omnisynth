#include "AnalogSynth_omni.h"

// testing right-justified DAC
// GUItool: begin automatically generated code
AudioSynthWaveformDc     dcZero;        // zero constant for left audio chan
AudioSynthWaveformDc     CVdc;            // CV for right audio chan; Range -1 : 1
AudioInputAnalog         adc1(A5); 
AudioAnalyzeNoteFrequency notefreq1;
AudioOutputI2S2          i2s2_1;         
AudioConnection          patchCord1(dcZero, 0, i2s2_1, 0);
AudioConnection          patchCord2(adc1, notefreq1);
AudioConnection          patchCord3(CVdc, 0, i2s2_1, 1);
// GUItool: end automatically generated code

AnalogSynth_omni::AnalogSynth_omni() {
    CV_ARRAY_4VOICE myCvTable {0};
    updateMode = Upd_Modes::Loop;  
    return;
}

void AnalogSynth_omni::setup() {
    CVdc.amplitude(0);
    dcZero.amplitude(0);
    pinMode(DAC_RSTB, OUTPUT);
    digitalWrite(DAC_RSTB, 1);
    pinMode(DAC_BPB, OUTPUT);
    digitalWrite(DAC_BPB, 1);
    pinMode(DAC_MUTEB, OUTPUT);
    digitalWrite(DAC_MUTEB, 1); //disable mute
    pinMode(DAC_OSR1, OUTPUT);
    digitalWrite(DAC_OSR1, 1); //osr = 16. adjustable
    pinMode(DAC_OSR2, OUTPUT);
    digitalWrite(DAC_OSR2, 1);
    
    pinMode(MUX_INHIB, OUTPUT);
    digitalWrite(MUX_INHIB, HIGH);
    pinMode(MUX_A0, OUTPUT);
    pinMode(MUX_B0, OUTPUT);
    pinMode(MUX_C0, OUTPUT);
    pinMode(MUX_A1, OUTPUT);
    pinMode(MUX_B1, OUTPUT);
    // elapsedMillis attk1;
}

void AnalogSynth_omni::tuneOscillators() {

}

/* writes control voltages according to voices mode
(polyphonic synth or drumkit modes)
Call periodically in main */
void AnalogSynth_omni::update() {
    switch (updateMode)
    {
    case Upd_Modes::Loop:
        this->writeCV_All_Loop();
        break;
    case Upd_Modes::Envelopes:
        this->writeCV_Envelopes();
        break;
    case Upd_Modes::Pitches:
        this->writeCV_Pitches();
        break;
    default:
        break;
    }
}

/*
Write Control Voltage to a specific AS3394 CV Channel
points MUXs to channel
Writes to DAC using DC voltage object CVdc
*/
void AnalogSynth_omni::writeCV(byte CVnum) {
    digitalWrite(MUX_INHIB, HIGH); //Disable all MUX channels
    digitalWrite(MUX_A0, CVnum & 0x01);
    digitalWrite(MUX_B0, (CVnum >> 1) & 0x01);
    digitalWrite(MUX_C0, (CVnum >> 2) & 0x01);
    digitalWrite(MUX_A1, (CVnum >> 3) & 0x01);
    digitalWrite(MUX_B1, (CVnum >> 4) & 0x01);
    CVdc.amplitude(myCvTable[CVnum]);
    digitalWrite(MUX_INHIB, LOW); //Enable MUX chan
}
 
 void AnalogSynth_omni::writeCV_All_Loop() {
     writeCV(loopCvNum);
     loopCvNum++;
     if (loopCvNum == 32)
     {
         loopCvNum = 0;
     }
     
 }


//Prioritized CV update functions //

//Write enveloped CVs (Voltage Controlled Amp)
void AnalogSynth_omni::writeCV_Envelopes() {

}

bool AnalogSynth_omni::writeCV_Pitches() {

}
//Trigger VCA ADSR modulation for input MIDI message
void noteOn(midi_message msg, std::vector<midi_message> *notes) {

 }

 void noteOff(midi_message msg, std::vector<midi_message> *notes) {

 }