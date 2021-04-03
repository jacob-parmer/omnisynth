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
#define ENVELOPE_TIMESTEP_ms 5
#define VCO_CV_0 0
#define MOD_AMT_0 1
#define WAVE_SEL_0 2
#define PWM_CV_0 3
#define MIX_BALANCE_CV_0 4
#define FILT_RES_CV_0 5
#define VCF_CV_0 6
#define VCA_CV_0 7
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

#define AUDIO_SAMPLE_RATE_EXACT 1600.0f //far less than audio sample rate is required. 
#define CV_UPDATE_PERIOD 10 //ms


typedef double CV_ARRAY_4VOICE[NUM_CVs]; //Stores DAC values to write
using namespace TeensyTimerTool;

class AnalogSynth_omni {

    public:
        AnalogSynth_omni();
        void setup();
        void tuneOscillators();
        void writeCV(byte);
        void noteOn(midi_message, std::vector<midi_message> *);
        void noteOff(midi_message, std::vector<midi_message> *);
        void updateEnvelopes();
    private:
        void writeCV_Linear();
        CV_ARRAY_4VOICE myCvTable;
        byte linearCvNum = 0;        
};

//  ISR(TIMER0_OVF_vect)
// {
//     /* Timer 0 overflow */
// }


// bool AnalogSynth_omni::update_setup(void)
// {
// 	if (update_scheduled) return false;
// 	attachInterruptVector(IRQ_SOFTWARE, software_isr);
// 	NVIC_SET_PRIORITY(IRQ_SOFTWARE, 208); // 255 = lowest priority
// 	NVIC_ENABLE_IRQ(IRQ_SOFTWARE);
// 	update_scheduled = true;
// 	return true;
// }
#endif