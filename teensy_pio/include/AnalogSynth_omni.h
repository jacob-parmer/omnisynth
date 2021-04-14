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


#define DAC_DIN 2
#define DAC_FSYNC 3
#define DAC_SCLK 4
#define DAC_RSTB 5 //ACTIVE LOW RESET
#define DAC_BPB 6 //ACTIVE LOW BYPASS FILTER
#define DAC_MUTEB 7 //ACTIVE LOW MUTE
#define DAC_OSR1 8 //OVERSAMPLE RATE
#define DAC_OSR2 9 //OVERSAMPLE RATE
#define DAC_16BIT 65536
#define BitCLK_MICROS 150
/*
CV PARAMETERS DEFINITIONS
points to the voice zero parameter channels.
Add 8 to increment param address to next voice.
*/
#define ENVELOPE_TIMESTEP_ms 5
#define VCO_CV_0 0
#define MOD_AMT_0 1
#define WAVE_SEL_0 2
#define PWM_CV_0 3
#define MIX_BALANCE_CV_0 4
#define FILT_RES_CV_0 5
#define VCF_CV_0 6
#define VCA_CV_0 7
#define NUM_VOICES 4
#define NUM_CVs 32

#include "usbMIDI_omni.h"
#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <array>
#include <TeensyTimerTool.h>

typedef short CV_ARRAY_4VOICE[NUM_CVs]; //Stores DAC values to write. two's complement.

class AnalogSynth_omni {

    public:
        AnalogSynth_omni();
        void setup();
        void tuneOscillators();
        void update();
        void noteOn(midi_message, std::vector<midi_message> *);
        void noteOff(midi_message, std::vector<midi_message> *);
        void updateEnvelopes();
        CV_ARRAY_4VOICE myCvTable;
        //MIDI note to pitch (Hz)
        float midi_toHz[127];
        
        //MIDI note to DAC voltage (freq compensation)
        float midiPitchTable[NUM_VOICES][127];
    private:
        enum class Upd_Modes {Loop, Envelopes, Pitches, Disabled};
        void setMuxChannel(byte);
        void writeCV_All_Loop();
        void writeCV_Envelopes();
        void writeSpecificCV(byte);


        bool sClk = 1;
        byte c_f_sync = 0;
        byte c_dacBitCounter = 16;
        byte loopCvNum = 0;
        Upd_Modes updateMode;  

};
#endif