'''
Sends note on / off data to the Teensy for LED control.

author: Omar Barazanji
date: 3/21/2021

Python: 3.7.x
'''

# used to package the serial data in midi format
import midi
from midi import MidiConnector 
from midi import Message, NoteOff, NoteOn
import time

from osc import OmniCollider

class OmniMidi:

    def __init__(self):

        # connection to the Teensy serial port
        # self.conn = MidiConnector('/dev/ttyACM0')

        # initialize OSC module for UDP communication with Supercollider.
        self.sc_note_on = OmniCollider()
        self.sc_note_off = OmniCollider()
        self.sc_cc = OmniCollider()
        self.note_on_event = []
        self.note_off_event = []

    # grabs note on / off data from SC and sends to Teensy.
    def note_stream(self):
        self.sc_note_on.receive("/noteOn")
        self.sc_note_off.receive("/noteOff")
        self.sc_cc.receive("/control")
        self.note_on_event = self.sc_note_on.midi_evnt
        self.note_off_event = self.sc_note_off.midi_evnt
        # self.send_note()
    
    # sends note events to Teensy.
    def send_note(self):
        note_on_nn = self.note_on_event[1]
        note_on_vel = self.note_on_event[2]
        note_off_nn = self.note_off_event[1]
        note_off_vel = self.note_off_event[2]
        note_on = NoteOn(note_on_nn, note_on_vel)
        note_off = NoteOff(note_off_nn, note_off_vel)
        msg1 = Message(note_on,2)
        msg2 = Message(note_off,2)
        self.conn.write(msg1)
        self.conn.write(msg2)

if __name__ == "__main__":
    Midi = OmniMidi()
    while True:
        Midi.note_stream()