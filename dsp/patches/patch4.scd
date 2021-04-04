/*
desc:
date: 1/16/21
*/

/*
Synth function.
*/
SynthDef.new(\tone4, {
	arg freq=440, amp=0.6, gate=0, bend=0,
	lpf_cutoff=20000, hpf_cutoff=20,
	attack_time=0.01, decay_time=0.3, sustain=0.5,
	release_time=1.0, bias=0.0;

	var raw_sig, filter_sig, sig, env;

	/*
	Signal creation section.
	*/
	raw_sig = Formant.ar(freq * bend.midiratio, 500,200);

	/*
	Envelope section.
	*/
	env = EnvGen.kr(Env.adsr(
		attack_time, decay_time, sustain, release_time,
		-4, bias), gate, doneAction:2);

	/*
	Filter section.
	*/
	filter_sig = LPF.ar(raw_sig, lpf_cutoff);
	filter_sig = HPF.ar(filter_sig, hpf_cutoff);

	/*
	Combining all components.
	*/
	sig = filter_sig * env * amp !2;

	Out.ar(0, sig);
}).store;