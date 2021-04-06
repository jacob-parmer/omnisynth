#include "AnalogSynth_omni.h"

// testing right-justified DAC
// GUItool: begin automatically generated code
AudioSynthWaveformDc     dcZero;        // zero constant for left audio chan
AudioSynthWaveformDc     CVdc;            // CV for right audio chan; Range -1 : 1
AudioInputAnalog         adc1(A5); 
AudioAnalyzeNoteFrequency notefreq;
AudioOutputI2S2          i2s2_1;         
AudioConnection          patchCord1(dcZero, 0, i2s2_1, 0);
AudioConnection          patchCord2(adc1, notefreq);
AudioConnection          patchCord3(CVdc, 0, i2s2_1, 1);
// GUItool: end automatically generated code

AnalogSynth_omni::AnalogSynth_omni() {
    //create midi to freq table   
    int a = 440; // a is 440 hz...
    for (int x = 0; x < 127; ++x)
    {
    midi_toHz[x] = (a / 32) * (2 ^ ((x - 9) / 12));
    }
    CV_ARRAY_4VOICE myCvTable {0};
    updateMode = Upd_Modes::Loop;  
    return;
}

void AnalogSynth_omni::setup() {
    //configure DAC
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
    //configure MUX
    pinMode(MUX_INHIB, OUTPUT);
    digitalWrite(MUX_INHIB, HIGH);
    pinMode(MUX_A0, OUTPUT);
    pinMode(MUX_B0, OUTPUT);
    pinMode(MUX_C0, OUTPUT);
    pinMode(MUX_A1, OUTPUT);
    pinMode(MUX_B1, OUTPUT);
    notefreq.begin(0.25); //certainty
    //set all CV channels to 0
    for (size_t cv = 0; cv < 31; ++cv)
    {
        this->writeCV(cv);
        delayMicroseconds(250);
    }
    
    this->tuneOscillators();
}

/* 
Generate MIDI note to DAC voltage table.
Separate offsets for each synth voice
compensates for analog imperfections.
*/
void AnalogSynth_omni::tuneOscillators() {
    updateMode = Upd_Modes::Disabled; //disable regular CV updating
    for (size_t v = 0; v < NUM_VOICES; v++) //v = current voice
    {

        byte CVnum = (++v * (VCO_CV_0+1)) - 1; //convert to pitch channel, range 0-31 
        for (float d_CV = -1; d_CV < 1; d_CV+=(2/DAC_16BIT))
        {
            this->writeCV(CVnum, d_CV);
            delayMicroseconds(50);
            if (notefreq.available())
            {
                float vco_fundamental = notefreq.read();
                for (size_t n = 0; n < 126; ++n) //n = note
                {
                    if (vco_fundamental >= midi_toHz[n] && vco_fundamental < midi_toHz[n+1])
                    {
                        int diff = vco_fundamental - midi_toHz[n];
                        midiPitchTable[v][n] = d_CV;
                    }
                    
                }
                
                
            }
            
        }
        delayMicroseconds(300);        
    }
    updateMode = Upd_Modes::Loop;   
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

void AnalogSynth_omni::writeCV(byte CVnum, float d_CV) {
    digitalWrite(MUX_INHIB, HIGH); //Disable all MUX channels
    digitalWrite(MUX_A0, CVnum & 0x01);
    digitalWrite(MUX_B0, (CVnum >> 1) & 0x01);
    digitalWrite(MUX_C0, (CVnum >> 2) & 0x01);
    digitalWrite(MUX_A1, (CVnum >> 3) & 0x01);
    digitalWrite(MUX_B1, (CVnum >> 4) & 0x01);
    CVdc.amplitude(d_CV);
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