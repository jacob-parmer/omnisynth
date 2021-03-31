/* Contains all functions for control of four AS3394 analog synth voice chips using DAC and MUX.
*/
#ifndef omnisynth_analog_h
#define omnisynth_analog_h

#define MUX_A0 13
#define MUX_B0 14
#define MUX_C0 15
#define MUX_A1 16
#define MUX_B1 17
#define MUX_INHIB 18

#define DAC_RSTB 5 //ACTIVE LOW RESET
#define DAC_BPB 6 //ACTIVE LOW BYPASS FILTER
#define DAC_MUTEB 7 //ACTIVE LOW MUTE
#define DAC_OSR1 8 //OVERSAMPLE RATE
#define DAC_OSR2 9 //OVERSAMPLE RATE
/*
CV PARAMETERS DEFINITIONS
points to the voice zero parameter channels.
Add 8 to increment param address to next voice.
*/
#define VCO_CV_0 0
#define MOD_AMT_0 1
#define WAVE_SEL_0 2
#define PWM_CV_0 3
#define MIX_BALANCE_CV_0 4
#define FILT_RES_CV_0 5
#define VCF_CV_0 6
#define VCA_CV_0 7

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <array>

#define AUDIO_SAMPLE_RATE_EXACT 1600.0f //far less than audio sample rate is required. 
#define CV_UPDATE_PERIOD 10 //ms
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

typedef double CV_ARRAY_4VOICE[32]; //Stores DAC values to write



class AnalogSynth_omni {

    public:
        AnalogSynth_omni();
        void setup();
        void tuneOscillators();
        void writeCV(byte CVnum, const CV_ARRAY_4VOICE *P);
    private: 
};

AnalogSynth_omni::AnalogSynth_omni() {
    return;
}

void AnalogSynth_omni::setup()
{
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
    
}


void AnalogSynth_omni::tuneOscillators()
{

}

/*
Write Control Voltage to a specific AS3394 CV Channel
*/
void AnalogSynth_omni::writeCV(byte CVnum, const CV_ARRAY_4VOICE *P)
{
    digitalWrite(MUX_INHIB, HIGH); //Disable all MUX channels
    digitalWrite(MUX_A0, CVnum & 0x01);
    digitalWrite(MUX_B0, (CVnum >> 1) & 0x01);
    digitalWrite(MUX_C0, (CVnum >> 2) & 0x01);
    digitalWrite(MUX_A1, (CVnum >> 3) & 0x01);
    digitalWrite(MUX_B1, (CVnum >> 4) & 0x01);
    CVdc.amplitude(*P[CVnum]);
    digitalWrite(MUX_INHIB, LOW); //Enable MUX chan
}
#endif