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

#define EVENT_INDEX 0
#define KEY_INDEX 1
#define VELOCITY_INDEX 2

#define MIDI_PACKET_SIZE 3

typedef std::array<byte, MIDI_PACKET_SIZE> midi_message;

/* ------------------- CLASS DEFINITION ------------------- */
class OmniMIDI {

    public:
        OmniMIDI();
        void read();
        void omniNoteOn(midi_message msg);
        void omniNoteOff(midi_message msg);
        std::vector<midi_message> getNotesOn();

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

        last_received_message[EVENT_INDEX] = header;
        last_received_message[KEY_INDEX] = data1;
        last_received_message[VELOCITY_INDEX] = data2;

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
    Serial.println("Note on detected.");
    this->notes_on.push_back(msg);
    Serial.println(notes_on.size());
}

void OmniMIDI::omniNoteOff(midi_message msg) {
    Serial.println("Note off detected.");
    for (std::vector<midi_message>::iterator it = notes_on.begin(); it != notes_on.end(); ++it) {
        midi_message note = *it;
        if (note[KEY_INDEX] == msg[KEY_INDEX]) {
            notes_on.erase(it);
            break;
        }
    }
}

std::vector<midi_message> OmniMIDI::getNotesOn() { return this->notes_on; }


/* ------------------- HELPER FUNCTIONS ------------------- */

float avgNoteVelocity(std::vector<midi_message> notes_on) {

    int velocity_sum = 0;
    for (std::vector<midi_message>::iterator it = notes_on.begin(); it != notes_on.end(); ++it) {
        midi_message note = *it;
        velocity_sum += note[VELOCITY_INDEX];
    }

    Serial.print("Velocity sum: ");
    Serial.println(velocity_sum);
    float avg_velocity = velocity_sum / notes_on.size();

     Serial.print("Average velocity: ");
    Serial.println(avg_velocity);   
    return avg_velocity;

}
