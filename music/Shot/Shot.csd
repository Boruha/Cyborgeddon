<CsoundSynthesizer>
<CsOptions>
-o Shot.wav
</CsOptions>
<CsInstruments>

sr = 22000
ksmps = 2
nchnls = 1
0dbfs = 1

instr 1
	iDur = p3 / 2
	iAmp = 0dbfs / 2
	
	kExponAmpBrownNoise expon iAmp/2, iDur/2, 0.001
	aBrownNoise fractalnoise kExponAmpBrownNoise*iAmp, 2
	aPinkNoise  fractalnoise kExponAmpBrownNoise*iAmp/2, 1
	aWhiteNoise fractalnoise kExponAmpBrownNoise*iAmp/16, 0
	
	kExponFrecLFO  expon iAmp, iDur, 0.1
	kExponAmpLFO  expon iAmp/8, iDur, 0.1
	aLFO1 lfo kExponAmpLFO*iAmp, kExponFrecLFO*110*2
	aLFO2 lfo kExponAmpLFO*iAmp, kExponFrecLFO*110*1.75
	aLFO3 lfo kExponAmpLFO*iAmp, kExponFrecLFO*110*1.5
	aLFO4 lfo kExponAmpLFO*iAmp, kExponFrecLFO*110*1.25
	aLFO5 lfo kExponAmpLFO*iAmp, kExponFrecLFO*110*1
	
	aNoise = aPinkNoise + aBrownNoise + aWhiteNoise
	aLFO = aLFO1 + aLFO2 + aLFO3 + aLFO4 + aLFO5
	
	out aNoise + aLFO
endin	

</CsInstruments>
<CsScore>
t 0 60

i 1 0 1
e
</CsScore>
</CsoundSynthesizer>
<bsbPanel>
 <label>Widgets</label>
 <objectName/>
 <x>100</x>
 <y>100</y>
 <width>320</width>
 <height>240</height>
 <visible>true</visible>
 <uuid/>
 <bgcolor mode="nobackground">
  <r>255</r>
  <g>255</g>
  <b>255</b>
 </bgcolor>
</bsbPanel>
<bsbPresets>
</bsbPresets>
