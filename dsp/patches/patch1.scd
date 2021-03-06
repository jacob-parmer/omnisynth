/*
desc: Default patch.
author: Omar Barazanji
date: 11/11/20
*/


/*
Synth function.
*/
SynthDef.new("tone1", {
	arg freq=440, lpf_cutoff=20000, hpf_cutoff=20,
		amp=0.6, gate=0, bend=0, out=0,
		attack_time=0.01, decay_time=0.3, sustain=0.5, release_time=1.0,
		bias=0.0;


	var sig, sig1, sig2, raw_sig, filter_sig, env;

	/*
	signal creation section.
	*/
	sig1 = LFTri.ar(freq * bend.midiratio);
	sig2 = SinOsc.ar((2*freq) * bend.midiratio);
    raw_sig = sig1 + sig2;

	/*
	Envelope section.
	*/
	env = EnvGen.kr(Env.adsr(
		attack_time,decay_time,sustain,
		release_time, -4, bias), gate, doneAction:2);

	/*
	Filter section.
	*/
	filter_sig = LPF.ar(raw_sig, lpf_cutoff);
	filter_sig = HPF.ar(filter_sig, hpf_cutoff);

	/*
	Combining all components.
	*/
	sig = filter_sig * env * amp !2;

	Out.ar(out, sig);
}).store;
