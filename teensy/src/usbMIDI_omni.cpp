#include "usbMIDI_omni.h"

/* ------------------- PUBLIC FUNCTIONS ------------------- */
OmniMIDI::OmniMIDI() {
    return;
}

int OmniMIDI::read() {

    int out = 0;

    if (Serial.available() != 0) {

        elapsedMillis timeout = 0;
        while ((Serial.available() % MIDI_PACKET_SIZE) != 0) {
            if (timeout > 2500) {
                timeout = 0;
                while (Serial.available()) { // Flushes serial data
                    Serial.print("Flushing data: ");
                    byte something = Serial.read();
                    Serial.println(something);
                }
            }
        }; // waits for full MIDI packet to arrive

        byte header = Serial.read();
        byte data1 = Serial.read();
        byte data2 = Serial.read();

        last_received_message[EVENT_INDEX] = header;
        last_received_message[KEY_INDEX] = data1;
        last_received_message[VELOCITY_INDEX] = data2;

        if (header == NOTE_ON) {
            out = this->omniNoteOn(last_received_message);
        } 
        else if (header == NOTE_OFF) {
            out = this->omniNoteOff(last_received_message);
        }

        Serial.print("Header: ");
        Serial.println(header);        
        Serial.print("Data1: ");
        Serial.println(data1);   
        Serial.print("Data2: ");
        Serial.println(data2);    
    }

    return out;
}

int OmniMIDI::omniNoteOn(midi_message msg) {
    Serial.println("Note on detected.");

    // Checks if the note is already on - aka handles duplicates
    for (std::vector<midi_message>::iterator it = notes_on.begin(); it != notes_on.end(); ++it) {
        midi_message note = *it;
        if (note[KEY_INDEX] == msg[KEY_INDEX]) {
            Serial.println("Duplicate note detected. Not added to notes_on.");
            return 0;
        }
    }

    Serial.println(msg[KEY_INDEX]);
    this->notes_on.push_back(msg);

    return 1;
}

int OmniMIDI::omniNoteOff(midi_message msg) {
    Serial.println("Note off detected.");
    int out = 0;

    for (std::vector<midi_message>::iterator it = notes_on.begin(); it != notes_on.end(); ++it) {
        midi_message note = *it;
        if (note[KEY_INDEX] == msg[KEY_INDEX]) {
            notes_on.erase(it);
            out = 1;
            break;
        }
    }

    return out;
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