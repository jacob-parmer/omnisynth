/*This header file will contain definitions for the 
attack sustain delay release (ADSR)*/
#ifndef omnisynth_adsr_h
#define omnisynth_adsr_h
#include "effect_envelope.h"
#include "Arduino.h"
#include "AudioStream.h"
#include "utility/dspinst.h"

#define SAMPLES_PER_MSEC (AUDIO_SAMPLE_RATE_EXACT/1000.0)
/*Declare the ADSR effects class and instantiate default values for testing*/
class ADSR_effects_envelope {
    public:
        state = 0;
        /*Declare default values*/
		delay(0.0f);  
		attack(10.5f);
		hold(2.5f);
		decay(35.0f);
		sustain(0.5f);
		release(300.0f);
		releaseNoteOn(5.0f);

    void noteOn();
    void noteOff();
    void delay(float milliseconds) {
		delay_count = milliseconds2count(milliseconds);
	}
	void attack(float milliseconds) {
		attack_count = milliseconds2count(milliseconds);
		if (attack_count == 0) attack_count = 1;
	}
	void hold(float milliseconds) {
		hold_count = milliseconds2count(milliseconds);
	}
	void decay(float milliseconds) {
		decay_count = milliseconds2count(milliseconds);
		if (decay_count == 0) decay_count = 1;
	}
	void sustain(float level) {
		if (level < 0.0) level = 0;
		else if (level > 1.0) level = 1.0;
		sustain_mult = level * 1073741824.0;
	}
	void release(float milliseconds) {
		release_count = milliseconds2count(milliseconds);
		if (release_count == 0) release_count = 1;
	}
    void releaseNoteOn(float milliseconds) {
		release_forced_count = milliseconds2count(milliseconds);
		if (release_count == 0) release_count = 1;
	}
    bool isActive();
	bool isSustain();
};
#define attack_time 750
#define delay_time 200
#define sustain_time 750
#define release_time 800

/*Declare the ADSR array that will contain the actual data*/
typedef float CV_ARRAY_4VOICE[NUM_CVs]; //Stores DAC values to write