// Author: Jacob Parmer
// Date: Mar 8, 2021
#include <vector>
#include <array>

#define NOTE_OFF 0x64
#define NOTE_ON 0x61
#define AFTERTOUCH 0xA1
#define CONTIN_CONTROL 0xB1
#define PATCH_CHANGE 0xC1
#define CHANNEL_PRESSURE 0xD1
#define PITCH_BEND 0xE1

#define MIDI_PACKET_SIZE 3

typedef std::array<byte, MIDI_PACKET_SIZE> midi_message;

class OmniMIDI {

    public:
        OmniMIDI();
        void read();
        void omniNoteOn(midi_message msg);
        void omniNoteOff(midi_message msg);

    private:
        midi_message last_received_message;
        std::vector<midi_message> notes_on;

};

OmniMIDI::OmniMIDI() {
    return;
}

void OmniMIDI::read() {

    if (Serial.available() != 0) {

        while ((Serial.available() % MIDI_PACKET_SIZE) != 0) {
            Serial.print("Waiting on rest of midi packet. Size of serial queue: ");
            Serial.println(Serial.available());
            delay(200);
        }; // waits for full MIDI packet to arrive

        byte header = Serial.read();
        byte data1 = Serial.read();
        byte data2 = Serial.read();

        last_received_message[0] = header;
        last_received_message[1] = data1;
        last_received_message[2] = data2;

        if (header == NOTE_ON) {
            this->omniNoteOn(last_received_message);
        } 
        else if (header == NOTE_OFF) {
            this->omniNoteOff(last_received_message);
        }

        Serial.println(header);
        
    }
}

void OmniMIDI::omniNoteOn(midi_message msg) {
    Serial.print("Note on detected. Header: ");
    this->notes_on.push_back(msg);
}

void OmniMIDI::omniNoteOff(midi_message msg) {
    Serial.println("Note off detected.");
}
