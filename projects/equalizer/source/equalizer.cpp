#include "Equalizer.h"
#include <cmath>


using namespace std;

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new Equalizer (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
Equalizer::Equalizer (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, kNumParams)	// 1 program, 1 parameter only
{
	
	setNumInputs (1);		// stereo in
	setNumOutputs (1);		// stereo out
	setUniqueID ('Ptch');	// identify
	canProcessReplacing ();	// supports replacing output
	//canDoubleReplacing ();	// supports double precision processing

	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
	
	setBass(0.5);
	setMid(0.5);
	setTreb(0.5);

	freqBass = 1000;
	freqMid = 1000;
	freqTreb = 10000;

	pi = 3.141592653589793238462643383279502;
	sr = getSampleRate();
	counter = 0;
	fft_size = 2048;
	freqPerBin = sr / fft_size;

	signal_in = new float[fft_size];
	signal_fft = new float[fft_size*2];
	signal_out = new float[fft_size*2];
	memset(signal_in, 0, fft_size * sizeof(float));
	memset(signal_out, 0, fft_size* 2 * sizeof(float));
	memset(signal_fft, 0, fft_size*2 * sizeof(float));
}

//-------------------------------------------------------------------------------------------------------
Equalizer::~Equalizer ()
{
	/*delete[] signal_in;
	delete[] signal_out;
	delete[] signal_fft;*/
}

//-------------------------------------------------------------------------------------------------------
void Equalizer::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Equalizer::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Equalizer::setParameter (VstInt32 index, float value)
{
	switch (index) {
		case kBass	:	setBass(value); break;
		case kMid	:	setMid(value); break;
		case kTreb	:	setTreb(value); break;
	}
}

//-----------------------------------------------------------------------------------------
float Equalizer::getParameter (VstInt32 index)
{
	float vystup = 0;
	switch (index) {
		case kBass	:	vystup = iBass; break;
		case kMid	:	vystup = iMid; break;
		case kTreb	:	vystup = iTreb; break;
	}
	return vystup;
}

//-----------------------------------------------------------------------------------------
void Equalizer::getParameterName (VstInt32 index, char* label)
{
	switch (index) {
		case kBass	:	vst_strncpy (label, "Bass", kVstMaxParamStrLen); break;
		case kMid	:	vst_strncpy (label, "Middle", kVstMaxParamStrLen); break;
		case kTreb	:	vst_strncpy (label, "Treble", kVstMaxParamStrLen); break;
	}
	
}

//-----------------------------------------------------------------------------------------
void Equalizer::getParameterDisplay (VstInt32 index, char* text)
{
	switch (index) {
		case kBass	:	float2string(oBass, text, kVstMaxParamStrLen); break;
		case kMid	:	dB2string(oMid, text, kVstMaxParamStrLen); break;
		case kTreb	:	dB2string(oTreb, text, kVstMaxParamStrLen); break;
	}
}

//-----------------------------------------------------------------------------------------
void Equalizer::getParameterLabel (VstInt32 index, char* label)
{
	switch (index) {
		case kBass	:	vst_strncpy (label, "dB", kVstMaxParamStrLen); break;
		case kMid	:	vst_strncpy (label, "dB", kVstMaxParamStrLen); break;
		case kTreb	:	vst_strncpy (label, "dB", kVstMaxParamStrLen); break;
	}
}

//------------------------------------------------------------------------
bool Equalizer::getEffectName (char* name)
{
	vst_strncpy (name, "Equalizer", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool Equalizer::getProductString (char* text)
{
	vst_strncpy (text, "Equalizer", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool Equalizer::getVendorString (char* text)
{
	vst_strncpy (text, "", kVstMaxVendorStrLen);
	return true;
}
//------------------------------------------------------------------------
void Equalizer::setBass(float a) {
	iBass = a;
	uBass = exp((iBass-0.5)*2.764);
	oBass = uBass;
	uBass = uBass * uBass;
}
//------------------------------------------------------------------------
void Equalizer::setMid(float a) {
	iMid = a;
	uMid = exp((iMid-0.5)*2.764);
	oMid = uMid;
}
//------------------------------------------------------------------------
void Equalizer::setTreb(float a) {
	iTreb = a;
	uTreb = exp((iTreb-0.5)*2.764);
	oTreb = uTreb;
}
//-----------------------------------------------------------------------------------------
VstInt32 Equalizer::getVendorVersion ()
{ 
	return 700; 
}
//-----------------------------------------------------------------------------------------
void Equalizer::equalize(float *signal) {
	float mul1 = 1, mul2 = 1, mul3 = 1;

	for (int i=0; i<fft_size; i++) {
		/*if (i*freqPerBin < freqBass) {
			mul += uBass * (freqPerBin*i / freqBass);
		}
		else {
			mul *= uBass * (freqBass / (freqPerBin*i));
		}
		if (i*freqPerBin < freqMid) {
			mul *= uMid * (freqPerBin*i / freqMid);
		}
		else {
			mul *= uMid * (freqMid / (freqPerBin*i));
		}
		if (i*freqPerBin < freqTreb) {
			mul *= uTreb * (freqPerBin*i / freqTreb);
		}
		else {
			mul *= uTreb * (freqTreb / (freqPerBin*i));
		}*/

		double mul_pom;
		if (i*freqPerBin < freqBass) {
			mul_pom = (freqPerBin*i / freqBass) * 1.5;
		}
		else {
			mul_pom = freqBass / (freqPerBin*i) * 1.5;
		}
		
		if (uBass==1) {
			mul1 = 1;
		}
		else if (uBass>=1) {
			mul1 = 1 + ((uBass-1) * mul_pom);
		}
		else {
			mul1 = 1 - (uBass * mul_pom);
		}

		/*if (i*freqPerBin < freqMid) {
			mul2 = uMid * (freqPerBin*i / freqMid);
		}
		else {
			mul2 = uMid * (freqMid / (freqPerBin*i));
		}

		if (i*freqPerBin < freqTreb) {
			mul3 = uTreb * (freqPerBin*i / freqTreb);
		}
		else {
			mul3 = uTreb * (freqTreb / (freqPerBin*i));
		}*/
		
		//float pom = (abs(mul2 - mul3)/2) + min(mul1, mul3); //stred medzi bass a treble
		//signal[2*i] *= mul1 * mul2 * mul3;
		//signal[2*i + 1] *= mul1 * mul2 * mul3;
		signal[2*i] *= mul1;
		signal[2*i + 1] *= mul1;
	}
}
// -----------------------------------------------------------------------------------------------------------------
void Equalizer::Fft(float *fftBuffer, long fftFrameSize, long sign)
/* 
	FFT routine, (C)1996 S.M.Bernsee. Sign = -1 is FFT, 1 is iFFT (inverse)
	Fills fftBuffer[0...2*fftFrameSize-1] with the Fourier transform of the
	time domain data in fftBuffer[0...2*fftFrameSize-1]. The FFT array takes
	and returns the cosine and sine parts in an interleaved manner, ie.
	fftBuffer[0] = cosPart[0], fftBuffer[1] = sinPart[0], asf. fftFrameSize
	must be a power of 2. It expects a complex input signal (see footnote 2),
	ie. when working with 'common' audio signals our input signal has to be
	passed as {in[0],0.,in[1],0.,in[2],0.,...} asf. In that case, the transform
	of the frequencies of interest is in fftBuffer[0...fftFrameSize].
*/
{
	float wr, wi, arg, *p1, *p2, temp;
	float tr, ti, ur, ui, *p1r, *p1i, *p2r, *p2i;
	long i, bitm, j, le, le2, k;

	for (i = 2; i < 2*fftFrameSize-2; i += 2) {
		for (bitm = 2, j = 0; bitm < 2*fftFrameSize; bitm <<= 1) {
			if (i & bitm) j++;
			j <<= 1;
		}
		if (i < j) {
			p1 = fftBuffer+i; p2 = fftBuffer+j;
			temp = *p1; *(p1++) = *p2;
			*(p2++) = temp; temp = *p1;
			*p1 = *p2; *p2 = temp;
		}
	}
	for (k = 0, le = 2; k < (long)(log((double)fftFrameSize)/log(2.)+.5); k++) {
		le <<= 1;
		le2 = le>>1;
		ur = 1.0;
		ui = 0.0;
		arg = pi / (le2>>1);
		wr = cos(arg);
		wi = sign*sin(arg);
		for (j = 0; j < le2; j += 2) {
			p1r = fftBuffer+j; p1i = p1r+1;
			p2r = p1r+le2; p2i = p2r+1;
			for (i = j; i < 2*fftFrameSize; i += le) {
				tr = *p2r * ur - *p2i * ui;
				ti = *p2r * ui + *p2i * ur;
				*p2r = *p1r - tr; *p2i = *p1i - ti;
				*p1r += tr; *p1i += ti;
				p1r += le; p1i += le;
				p2r += le; p2i += le;
			}
			tr = ur*wr - ui*wi;
			ui = ur*wi + ui*wr;
			ur = tr;
		}
	}
}
//-----------------------------------------------------------------------------------------
void Equalizer::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    float* in1  =  inputs[0];
   // float* in2  =  inputs[1];
    float* out1 = outputs[0];
   // float* out2 = outputs[1];
		
	while (--sampleFrames >= 0) {
		
		float x = *in1++;

		if (counter<fft_size) {
			signal_in[counter] = x;
		}
		else {
			
			for (int i=0; i<fft_size; i++) {
				signal_fft[2*i] = signal_in[i];
				signal_fft[2*i+1] = 0;
			}
			Fft(signal_fft, fft_size, -1);
			
			equalize(signal_fft);

			Fft(signal_fft, fft_size, 1);
			
			for (int i=0; i < fft_size; i++) {
				signal_out[i] = signal_fft[2*i]/fft_size;
			}

			counter = 0;
			signal_in[counter] = x;	
		}
		(*out1++) = signal_out[counter++];
	}
}

//-----------------------------------------------------------------------------------------
