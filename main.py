"""
Main / Top Level for the OmniAura Synthesizer.

author: Omar Barazanji
date: 11/12/20

Python 3.7.x
"""

import numpy as np
import os


# Used for sending / receiving data from supercollider.
from pi.osc import OmniCollider
from pi.omnimidi import OmniMidi

class Omni():

    def __init__(self):

        # initialize OSC module for UDP communication with Supercollider.
        self.sc = OmniCollider(17)
        self.sc.map_dispatcher("/control")
        self.sc.map_dispatcher("/noteOn")
        self.sc.map_dispatcher("/noteOff")

        # Initialize Teensy serial communication
        # self.teensy = OmniMidi()

        # holds control events from UDP stream.
        self.control_evnt = []

        # holds note on / off events from UDP stream.
        self.note_evnt = []

        # used for turning on midi learn.
        self.midi_learn_on = False

        # holds all control knobs and their values.
        #     organization: self.knob_table[knob_addr] = value
        #     where knob_addr = (src, chan) from the MIDI cc knob.
        self.knob_table = dict()

        # holds all knob mappings to SC params.
        #     organization: self.knob_map[knob_addr] = filter_name.
        self.knob_map = dict()

        # holds history of last value sent through the UDP stream to SC.
        #     organization: self.knob_map_hist[filter_name] = value.
        self.knob_map_hist = dict()

        # Table that will be outputted to DAC & Mux
        self.cv_table = [[0 for x in range(8)] for y in range(4)] 

        # LUT for freq control messages, maps 0-127 to 20 - 20000 Hz.
        self.cc_to_freq = np.linspace(20,20000,128).tolist()

        # LUT for adsr control messages, maps 0-127 to .001 - 1 (seconds or amplitude).
        self.cc_to_adsr = np.linspace(0.001, 1, 128).tolist()

    def value_map(self, filt, inp):
        value = inp
        if filt == "lpf" or filt == "hpf":
            value = self.cc_to_freq[inp]
        if filt == "attack" or filt == "decay" or filt == "sustain" or filt == "release":
            value = self.cc_to_adsr[inp]
        return value


    # opens UDP stream for MIDI control messages.
    def open_stream(self, *args):
        self.sc.receive()
        try:
            event = self.sc.midi_evnt[0]
        except IndexError:
            event = ""

        if event == "/control":
            self.control_evnt = self.sc.midi_evnt
            if self.midi_learn_on:
                self.midi_learn(self.control_evnt)
            if len(self.knob_map) != 0:
                for knob_addr in self.knob_map:
                    filter_name = self.knob_map[knob_addr]
                    raw_value = self.knob_table[knob_addr]
                    value = self.value_map(filter_name, raw_value)
                    self.filter_sel(filter_name, value)
        if event == "/noteOn" or event == "noteOff":
            self.note_evnt = self.sc.midi_evnt
            # self.teensy.send_note(self.note_evnt)


    # implement a way to close stream (may be on GUI).
    def close_stream(self):
        pass

    # update every 10ms
    def update_cv(self):
        # DAC & Mux update period is 10/32 ms.
        pass

    # compiles all synthDef's in dsp folder.
    def sc_compile(self):
        command = "/omni"
        control = "compile"
        directory = "dsp/patches/"
        for patch in os.listdir(directory):
            filedir = directory + patch
            path = os.path.abspath(filedir).replace("\\", "/")
            self.sc.transmit(command, control, path)

    # turns on / off synthDef's from SC.
    def synth_sel(self, synth_name):
        command = "/omni"
        control = "synthSel"
        self.sc.transmit(command, control, synth_name)

    # select filter and param value.
    def filter_sel(self, filter_name, value):
        command = "/omni"
        control = "filterSel"
        if filter_name in self.knob_map_hist and self.knob_map_hist[filter_name] != value:
            self.sc.transmit(command, control, filter_name, value)
            self.knob_map_hist[filter_name] = value
        elif filter_name not in self.knob_map_hist: # if first instance
            self.sc.transmit(command, control, filter_name, value)
            self.knob_map_hist[filter_name] = value

    # creates dict for all control knobs on MIDI controller.
    def midi_learn(self, midi_msg):
        if len(midi_msg) == 4:
            val = midi_msg[1]
            src = midi_msg[2]
            chan = midi_msg[3]
            self.knob_table[(src,chan)] = val

    # maps a knob to an SC parameter.
    # params:
    #     knob_addr = (src, chan) 
    #     filter_name = "lpf" (for example)
    def map_knob(self, knob_addr, filter_name):
        self.knob_map[knob_addr] = filter_name

def quick_map(OmniSynth):
    OmniSynth.map_knob((7,0), "attack")
    OmniSynth.map_knob((7,1), "decay")
    OmniSynth.map_knob((7,2), "sustain")
    OmniSynth.map_knob((7,3), "release")
    OmniSynth.map_knob((10,4), "lpf")
    OmniSynth.map_knob((10,5), "hpf")

if __name__ == "__main__":
    OmniSynth = Omni() # initialize Omni class.
    OmniSynth.sc_compile() # compiles all synthDefs.

    # uncomment below to test midi_learn for knobs.
    OmniSynth.midi_learn_on = True # turn on midi learn.
    while (True):
        OmniSynth.open_stream()
        
    #     # After midi_learn is complete, the GUI runs:
    #     # for example:
    #     #     OmniSynth.map_knob((7,1), "lpf")   

    OmniSynth.synth_sel("tone1")
    OmniSynth.filter_sel("lpf", 20000)
    OmniSynth.filter_sel("hpf", 20)
