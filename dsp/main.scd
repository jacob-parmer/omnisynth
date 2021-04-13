/*
desc: Main / top level for the Omnisynth.
author: Omar Barazanji
date: 1/16/21
*/

// boot server
s.boot;

Server.default.options.device = "ASIO : ASIO4ALL v2";

// variables for synthDef's.
~bend = 8193;
~notes = Array.newClear(128);

// variables for OSC parameter control.
~notes_on = Array.newClear(32);
~notes_on_count = 0;
~notes_on_ndx = Array.newClear(128);

// variables for OSC UDP ack system
~curr_msg_note_on = [];
~curr_msg_note_off = [];
~ack_table_note_on = Array.fill(128, {arg i; i=1});
~ack_table_note_off = Array.fill(128, {arg i; i=1});

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

// Connection to OSC via localhost.
n = NetAddr.new("127.0.0.1", 7771);


// begin main
(

/* OSCFunc called when Python sends "/omni" a message.
 * This function will control the OmniSynth created in Python.
 */
o = OSCFunc( {

	// arguments to OSCFunc.
	| msg, time, addr, recvPort |
	var temp;
	// post all messages to terminal window.
	[msg, time, addr, recvPort].postln;

	case

		// Ack system for noteOn messages (not as important for now).
	    // {msg[1] == "/noteOn".asSymbol} {

		// 	if (msg[2] == ~curr_msg_note_on[1].asInteger) {
		// 		if (msg[3] == ~curr_msg_note_on[2].asInteger) {
		// 			~ack_table_note_on[msg[2].asInteger] = 1;
		// 		}
		// 	};
		// }

		// Ack system for noteOff messages (important to ensure no stuck notes).
	    {msg[1] == "/noteOff".asSymbol} {
			if (msg[2] == ~curr_msg_note_off[1].asInteger) {
				if (msg[3] == ~curr_msg_note_off[2].asInteger) {
					~ack_table_note_off[msg[2].asInteger] = 1;
				}
			};
		}

		// Synth patch selector.
		{msg[1] == "synthSel".asSymbol} {
			~synth = msg[2].asString;
			~notes = Array.newClear(128);
	        ~synth.postln;
		}

		// Parameter adjustment.
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

		// compiles all SynthDef's.
		{msg[1] == "compile".asSymbol} {
			msg[2].asString.load;
		}

		// base case - set synth to default patch.
		{true} {~synth = "tone1"};

}, '/omni');


/*
Note on function.
*/
MIDIdef.noteOn(\noteOnDef, {
	arg vel, nn, chan, src;
	var temp;
	[vel, nn].postln;
	// temp = Array.fill(3, {arg i; i=~curr_msg_note_on[i]}); //previous
	// ~curr_msg_note_on = ["/noteOn",nn,vel];
	// if (~ack_table_note_on[nn] == 1,
	// {
	// 	~ack_table_note_on[nn] = 0;
	n.sendMsg("/noteOn",nn,vel);
	// } , 
	// {
	// 	"possible packet drop".postln;
	// 	// send previous and current
	// 	n.sendMsg(temp[0],temp[1],temp[2]);
	// 	n.sendMsg("/noteOn",nn,vel);
	// });
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
	var temp;
	[vel, nn].postln;

	// store previous noteOff messsage
	temp = Array.fill(3, {arg i; i=~curr_msg_note_off[i]});
	~curr_msg_note_off = ["/noteOff",nn,vel];
	if (~ack_table_note_off[nn] == 1,
	{
		~ack_table_note_off[nn]=0;
		n.sendMsg("/noteOff",nn,vel);	
	} , {
		"possible packet drop".postln;
		// send previous and current
		n.sendMsg(temp[0],temp[1],temp[2]);
		n.sendMsg("/noteOff",nn,vel);
	});
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
