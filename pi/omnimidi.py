'''
Sends note on / off data to the Teensy for LED control.

author: Omar Barazanji
date: 3/21/2021

Python: 3.7.x
'''

# used to package the serial data in midi format
import midi
import os
from midi import MidiConnector 
from midi import Message, NoteOff, NoteOn


class OmniMidi:

    def __init__(self):

        self.conn = 0
        if os.path.exists('/dev/ttyACM0'):
            # connection to the Teensy serial port
            self.conn = MidiConnector('/dev/ttyACM0')
    
    # sends note events to Teensy.
    def send_note(self, evnt):
        if self.conn == 0: return 0
        note = evnt[0]
        nn = self.note_event[1]
        vel = self.note_event[2]
        if note=="/noteOn":
            note_on = NoteOn(nn, vel)
            msg = Message(note_on,2)
        elif note=="/noteOff":
            note_off = NoteOff(nn, vel)
            msg = Message(note_off,2)
        self.conn.write(msg)

if __name__ == "__main__":
    pass
