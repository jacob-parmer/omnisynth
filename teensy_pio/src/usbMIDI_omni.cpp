#include "usbMIDI_omni.h"

OmniMIDI::OmniMIDI() {
    return;
}

void OmniMIDI::read() {

    if (Serial.available() != 0) {

        while ((Serial.available() % MIDI_PACKET_SIZE) != 0) {
            Serial.print("Waiting on rest of midi packet. Size of serial queue: ");
            Serial.println(Serial.available());
            delay(5);
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