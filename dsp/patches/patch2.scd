/*
desc: Sounds like an outer-space boiling noise machine.
date: 1/13/21
*/

/*
Synth function.
*/
SynthDef.new("tone2", {
	arg freq, amp=0.6, gate=0, bend=0;
	var freq_mod, raw_sig, filter_sig, sig, env,
	lpf_cutoff=20000, hpf_cutoff=20, amp_mod,
	attack_time=0.01, decay_time=0.3, sustain=0.5, release_time=1.0,
	bias=0.0;

	/*
	signal creation section.
	*/
	freq_mod = LFNoise0.kr(freq).exprange(200,1000);
	amp_mod = LFNoise1.kr(freq).exprange(0.01,amp);
	raw_sig = SinOsc.ar(freq_mod);

	/*
	Envelope section.
	*/
	env = EnvGen.kr(Env.adsr(
		attack_time, decay_time,
		sustain, release_time,
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
