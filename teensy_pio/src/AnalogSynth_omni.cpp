#include "AnalogSynth_omni.h"

// GUItool: begin automatically generated code
AudioInputAnalog         adc1(A5); 
AudioAnalyzeNoteFrequency notefreq;       
AudioConnection          patchCord2(adc1, notefreq);
// GUItool: end automatically generated code

AnalogSynth_omni::AnalogSynth_omni() {
    //create midi to freq table 
    AudioMemory(6);
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
    pinMode(DAC_DIN, OUTPUT);
    pinMode(DAC_FSYNC, OUTPUT);
    digitalWrite(DAC_FSYNC, 1); //start Fsync high, bit 15
    pinMode(DAC_SCLK, OUTPUT);
    pinMode(DAC_RSTB, OUTPUT);
    digitalWrite(DAC_RSTB, 1);
    pinMode(DAC_BPB, OUTPUT);
    digitalWrite(DAC_BPB, 0);
    pinMode(DAC_MUTEB, OUTPUT);
    digitalWrite(DAC_MUTEB, 1); //disable mute
    pinMode(DAC_OSR1, OUTPUT);
    digitalWrite(DAC_OSR1, 0); //osr = 16. adjustable
    pinMode(DAC_OSR2, OUTPUT);
    digitalWrite(DAC_OSR2, 0);

    //configure MUX
    pinMode(MUX_INHIB, OUTPUT);
    // digitalWrite(MUX_INHIB, HIGH);
    pinMode(MUX_A0, OUTPUT);
    pinMode(MUX_B0, OUTPUT);
    pinMode(MUX_C0, OUTPUT);
    pinMode(MUX_A1, OUTPUT);
    pinMode(MUX_B1, OUTPUT);

    notefreq.begin(0.25); //certainty
    // this->tuneOscillators();
}

/* 
Generate MIDI note to DAC voltage table.
Separate offsets for each synth voice
compensates for analog imperfections.
*/
void AnalogSynth_omni::tuneOscillators() {
    updateMode = Upd_Modes::Disabled; //disable regular CV updating
    for (size_t v = 0; v < NUM_VOICES; v++)         //v = current voice
    {

        byte CvNum = (++v * (VCO_CV_0+1)) - 1; //convert to pitch channel, range 0-31 
        for (uint16_t i_CV = -DAC_FULLSCALE; i_CV < DAC_FULLSCALE; i_CV += 1)
        {
            myCvTable[v*8 + VCO_CV_0] = i_CV;
            this->setMuxChannel(CvNum);
            for (size_t i = 0; i < 32; i++)
            {
                this->writeSpecificCV(CvNum);
                delayMicroseconds(BitCLK_MICROS);
            }
            if (notefreq.available())
            {
                float vco_fundamental = notefreq.read();
                for (size_t n = 0; n < 126; ++n)        //n = note
                {
                    if ((vco_fundamental >= midi_toHz[n]) && (vco_fundamental < midi_toHz[n+1]))
                    {
                        // int diff = vco_fundamental - midi_toHz[n];
                        //[voice][note]
                        midiPitchTable[v][n] = i_CV;
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
        this->writeSpecificCV(32);
        break;
    default:
        break;
    }
}

/*
points MUXs to  to a specific AS3394 CV Channel
*/
void AnalogSynth_omni::setMuxChannel(byte CvNum) {
    // digitalWrite(MUX_INHIB, HIGH); //Disable all MUX channels
    digitalWrite(MUX_A0, (CvNum & 0x01));
    digitalWrite(MUX_B0, ((CvNum >> 1) & 0x01));
    digitalWrite(MUX_C0, ((CvNum >> 2) & 0x01));
    digitalWrite(MUX_A1, ((CvNum >> 3) & 0x01));
    digitalWrite(MUX_B1, ((CvNum >> 4) & 0x01));
    // digitalWrite(MUX_INHIB, LOW); //Enable MUX chan
}
 
void AnalogSynth_omni::writeCV_All_Loop() {
    // c_f_sync++;
    if (counter_dacBit == 15 && next_sClk == LOW) {
        digitalWrite(DAC_FSYNC, LOW);
        loopCvNum++;
        if (loopCvNum == 32) {loopCvNum = 0;}
    }
    if (counter_dacBit == 15 && next_sClk == LOW) {
        // int muxChannel = loopCvNum-1;
        // if (muxChannel == -1) {muxChannel = 31;}
        setMuxChannel(loopCvNum);
    }

    if (counter_dacBit == 1 && next_sClk == HIGH)  {
        digitalWrite(DAC_FSYNC, HIGH);
    } 
    
    digitalWrite(DAC_SCLK, next_sClk); //bit 15 LOW
    next_sClk ^=1;

    if (next_sClk) //falling edge
    {
        short dac_word = myCvTable[loopCvNum];
        bool dac_bit = ((dac_word >> counter_dacBit) & 0x01 );
        digitalWrite(DAC_DIN, dac_bit);

        if (counter_dacBit != 0) {counter_dacBit--;}
        else {counter_dacBit = 15;}
  
    }

     
}

//Prioritized CV update functions //

//Write enveloped CVs (Voltage Controlled Amp)
void AnalogSynth_omni::writeCV_Envelopes() {

}

void AnalogSynth_omni::writeSpecificCV(byte CvNum) {
    // next_sClk ^=1;
    // digitalWrite(DAC_SCLK, next_sClk);
    // c_f_sync++;
    // // uint16_t dac_word = myCvTable[loopCvNum];
    // if (next_sClk)
    // {
    //     bool dac_bit = ((myCvTable[CvNum] << (c_f_sync << 1)) & 1<<15 ) >> 15;
    //     digitalWrite(DAC_DIN, dac_bit);
    //     // Serial.println(myCvTable[loopCvNum]);
    // }
    
    // if (c_f_sync > 31) {
    //     digitalWrite(DAC_FSYNC, HIGH);       
    //     digitalWrite(DAC_FSYNC, LOW);
    //     c_f_sync = 0;
    // }
}
//Trigger VCA ADSR modulation for input MIDI message
void noteOn(midi_message msg, std::vector<midi_message> *notes) {

 }

 void noteOff(midi_message msg, std::vector<midi_message> *notes) {

 }


         // if (myCvTable[loopCvNum] != 0)
        // {
        //     Serial.print(loopCvNum);
        //     Serial.print("\t");
        //     Serial.print(myCvTable[loopCvNum]);
        //     Serial.print("\t");
        //     Serial.print(dac_bit);
        //     Serial.print("\t");
        //     Serial.print(counter_dacBit);
        //     Serial.print("\n\n");
        // }    