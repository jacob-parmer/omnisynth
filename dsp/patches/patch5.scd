/*
desc: FM patch.
author: Omar Barazanji
date: 11/11/20
*/


/*
Synth function.
*/
SynthDef.new("tone5", {

	arg freq=440, lpf_cutoff=20000, hpf_cutoff=20,
		amp=0.6, gate=0, bend=0, out=0,
        mod_freq=1000, mod_amp=500, vol=1,
		attack_time=0.01, decay_time=0.3, sustain=0.5, release_time=1.0,
		bias=0.0;

	var sig, mod_sig, raw_sig, filter_sig, env, mod;

	/*
	signal creation section.
	*/
	mod_sig = SinOsc.ar(mod_freq * bend.midiratio, mul:mod_amp);
	raw_sig = SinOsc.ar((freq + mod_sig) * bend.midiratio);

	/*
	Envelope section.
	*/
	env = EnvGen.kr(Env.adsr(attack_time,decay_time,sustain,
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
