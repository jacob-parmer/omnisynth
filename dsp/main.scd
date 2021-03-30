/*
desc: Main / top level for the Omnisynth.
author: Omar Barazanji
date: 1/16/21
*/

// Config and global variables
s.boot;

Server.default.options.device = "ASIO : ASIO4ALL v2";

~bend = 8193;
~notes = Array.newClear(128);
~notes_on = Array.newClear(32);
~notes_on_count = 0;
~notes_on_ndx = Array.newClear(128);

// Default patch params
~synth = "tone1";
~lpf_cutoff = 20000;
~hpf_cutoff = 20;
~attack_time = 0.01;
~decay_time = 0.3;
~sustain = 0.5;
~release_time = 1.0;
~volume = 127;

// Set freq to standard Hz
nil.set(\freq, 880);

// Grab any MIDI device plugged in.
MIDIClient.init;
MIDIIn.connectAll;

// Connection to OSC
n = NetAddr.new("127.0.0.1", 7771);


// begin main
(

/* OSCFunc called when Python sends "/omni" a message.
 * This function will control the OmniSynth created in Python.
 */
o = OSCFunc( {

	// arguments to OSCFunc
	| msg, time, addr, recvPort |
	var temp;
	// post all messages to terminal window
	[msg, time, addr, recvPort].postln;

	case

		{msg[1] == "synthSel".asSymbol} {
			~synth = msg[2].asString;
			~notes = Array.newClear(128);
	        ~synth.postln;
		}

		{msg[1] == "filterSel".asSymbol} {
			if (msg[2] == "lpf".asSymbol) {
				~lpf_cutoff = msg[3].asFloat;
			};
			if (msg[2] == "hpf".asSymbol) {
				~hpf_cutoff = msg[3].asFloat;
			};
			if (msg[2] == "vol".asSymbol) {
				~volume = msg[3].asFloat;
			};
			if (msg[2] == "attack".asSymbol) {
				~attack_time = msg[3].asFloat;
			};
			if (msg[2] == "decay".asSymbol) {
				~decay_time = msg[3].asFloat;
			};
			if (msg[2] == "sustain".asSymbol) {
				~sustain = msg[3].asFloat;
			};
			if (msg[2] == "release".asSymbol) {
				~release = msg[3].asFloat;
			};
			if (~notes_on_count > 0) {
				for (0,~notes_on_count, {
					arg i;
					temp = ~notes_on[i];
					~notes[temp].set(\lpf_cutoff, ~lpf_cutoff);
					~notes[temp].set(\hpf_cutoff, ~hpf_cutoff);
					~notes[temp].set(\attack_time, ~attack_time);
					~notes[temp].set(\decay_time, ~decay_time);
					~notes[temp].set(\sustain, ~sustain);
					~notes[temp].set(\release_time, ~release_time);				});
			};
		}

		{msg[1] == "compile".asSymbol} {
			msg[2].asString.load;
		}

		{true} {~synth = "tone1"};

}, '/omni');


/*
Note on function.
*/
MIDIdef.noteOn(\noteOnDef, {
	arg vel, nn, chan, src;
	[vel, nn].postln;
	n.sendMsg("/noteOn", nn, vel);
	~notes_on.add(nn);
	~notes_on_ndx[nn] = ~notes_on_count;
	~notes_on_count = ~notes_on_count + 1;
	~notes[nn] = Synth.new(
		~synth,
		[
			\freq, nn.midicps,
			\amp, vel.linexp(1,127,0.01,0.03),
			\vol, ~volume.linexp(1,127,0.01, 2),
			\gate, 1,
			\bend, ~bend.linlin(0,16383,-2,2),
			\lpf_cutoff, ~lpf_cutoff,
			\hpf_cutoff, ~hpf_cutoff,
			\attack_time, ~attack_time,
			\decay_time, ~decay_time,
			\sustain, ~sustain,
			\release_time, ~release_time
		]
	);
});

/*
Note off function.
*/
MIDIdef.noteOff(\noteOffDef, {
	arg vel, nn;
	[vel, nn].postln;
	n.sendMsg("/noteOff", nn, vel);
	~notes_on_count = ~notes_on_count - 1;
	~notes_on.removeAt(~notes_on_ndx[nn]);
	~notes_on_ndx[nn] = nil;
	~notes[nn].set(\gate, 0);
	~notes[nn] = nil;
});

/*
Pitch wheel function.
*/
MIDIdef.bend(\bendDef, {
	arg val, chan;
	[val, chan].postln;
	~bend = val;
	~notes.do{arg synth; synth.set(\bend, val.linlin(0,16383, -2, 2))};
}, chan:0);

MIDIdef.cc(\controlDef, {
	arg val, src, chan;
	[val, src, chan].postln;
	n.sendMsg("/control",val, src, chan);
});

) // end main
