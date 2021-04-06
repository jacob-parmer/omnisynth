// Author: Jacob Parmer
// Date: Mar 8, 2021
#ifndef OMNIMIDI_H
#define OMNIMIDI_H

#include <Arduino.h>
#include <stdio.h>
#include <vector>
#include <array>
#define NOTE_OFF 0x64
#define NOTE_ON 0x61
#define AFTERTOUCH 0xA1
#define CONTIN_CONTROL 0xB1
#define PATCH_CHANGE 0xC1
#define CHANNEL_PRESSURE 0xD1
#define PITCH_BEND 0xE1

#define EVENT_INDEX 0
#define KEY_INDEX 1
#define VELOCITY_INDEX 2

#define MIDI_PACKET_SIZE 3

//Standard MIDI message size.
typedef std::array<byte, MIDI_PACKET_SIZE> midi_message;

/* ------------------- CLASS DEFINITION ------------------- */
class OmniMIDI {

    public:
        OmniMIDI();
        void read();
        void omniNoteOn(midi_message);
        void omniNoteOff(midi_message);
        std::vector<midi_message> getNotesOn();

    private:
        midi_message last_received_message;
        std::vector<midi_message> notes_on;

};

float avgNoteVelocity(std::vector<midi_message>);
#endif