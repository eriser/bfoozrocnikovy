#include "BEqualizer.h"
#include <cmath>
#include "libraries/fft/fft.h"


using namespace std;

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new BEqualizer (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
BEqualizer::BEqualizer (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, kNumParams)	// 1 program, 1 parameter only
{
	
	setNumInputs (2);		// stereo in
	setNumOutputs (2);		// stereo out
	setUniqueID ('Eqlz');	// identify
	canProcessReplacing ();	// supports replacing output
	//canDoubleReplacing ();	// supports float precision processing

	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
	
	setBass(0.5);
	setMid(0.5);
	setTreb(0.5);
	setGain(0.5);

	freqBass = 100;
	freqMid = 1000;
	freqTreb = 10000;
	
	pi = (float)3.141592653589793238462643383279502;
	sr = getSampleRate();
	counter = 0;
	fftSize = 2048;
	fftSize2 = fftSize / 2;
	freqPerBin = sr / fftSize;

	signal_in = new float[fftSize];
	signal_in2 = new float[fftSize];
	signal_fft = new float[fftSize*2];
	signal_out = new float[fftSize];
	signal_out2 = new float[fftSize];
	memset(signal_in, 0, fftSize * sizeof(float));
	memset(signal_in2, 0, fftSize * sizeof(float));
	memset(signal_out, 0, fftSize * sizeof(float));
	memset(signal_out2, 0, fftSize * sizeof(float));
	memset(signal_fft, 0, fftSize * 2 * sizeof(float));
}

//-------------------------------------------------------------------------------------------------------
BEqualizer::~BEqualizer ()
{
	delete [] signal_in;
	delete [] signal_in2;
	delete [] signal_out;
	delete [] signal_out2;
	delete [] signal_fft;
}

//-------------------------------------------------------------------------------------------------------
void BEqualizer::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void BEqualizer::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void BEqualizer::setParameter (VstInt32 index, float value)
{
	switch (index) {
		case kBass	:	setBass(value); break;
		case kMid	:	setMid(value); break;
		case kTreb	:	setTreb(value); break;
		case kGain	:	setGain(value); break;
	}
}

//-----------------------------------------------------------------------------------------
float BEqualizer::getParameter (VstInt32 index)
{
	float vystup = 0;
	switch (index) {
		case kBass	:	vystup = iBass; break;
		case kMid	:	vystup = iMid; break;
		case kTreb	:	vystup = iTreb; break;
		case kGain	:	vystup = iGain;	break;
	}
	return vystup;
}

//-----------------------------------------------------------------------------------------
void BEqualizer::getParameterName (VstInt32 index, char* label)
{
	switch (index) {
		case kBass	:	vst_strncpy (label, "Bass", kVstMaxParamStrLen); break;
		case kMid	:	vst_strncpy (label, "Middle", kVstMaxParamStrLen); break;
		case kTreb	:	vst_strncpy (label, "Treble", kVstMaxParamStrLen); break;
		case kGain	:	vst_strncpy (label, "Output", kVstMaxParamStrLen); break;
	}
	
}

//-----------------------------------------------------------------------------------------
void BEqualizer::getParameterDisplay (VstInt32 index, char* text)
{
	switch (index) {
		case kBass	:	dB2string(oBass, text, kVstMaxParamStrLen); break;
		case kMid	:	dB2string(oMid, text, kVstMaxParamStrLen); break;
		case kTreb	:	dB2string(oTreb, text, kVstMaxParamStrLen); break;
		case kGain	:	dB2string(oGain, text, kVstMaxParamStrLen); break;
	}
}

//-----------------------------------------------------------------------------------------
void BEqualizer::getParameterLabel (VstInt32 index, char* label)
{
	switch (index) {
		case kBass	:	vst_strncpy (label, "dB", kVstMaxParamStrLen); break;
		case kMid	:	vst_strncpy (label, "dB", kVstMaxParamStrLen); break;
		case kTreb	:	vst_strncpy (label, "dB", kVstMaxParamStrLen); break;
		case kGain	:	vst_strncpy (label, "dB", kVstMaxParamStrLen); break;
	}
}

//------------------------------------------------------------------------
bool BEqualizer::getEffectName (char* name)
{
	vst_strncpy (name, "BEqualizer", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool BEqualizer::getProductString (char* text)
{
	vst_strncpy (text, "BEqualizer", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool BEqualizer::getVendorString (char* text)
{
	vst_strncpy (text, "bFooz", kVstMaxVendorStrLen);
	return true;
}
//------------------------------------------------------------------------
void BEqualizer::setBass(float a) {
	iBass = a;
	uBass = (float)exp((iBass-0.5)*2.764);
	oBass = uBass;
	uBass = uBass;
}
//------------------------------------------------------------------------
void BEqualizer::setMid(float a) {
	iMid = a;
	uMid = (float)exp((iMid-0.5)*2.764);
	oMid = uMid;
}
//------------------------------------------------------------------------
void BEqualizer::setTreb(float a) {
	iTreb = a;
	uTreb = (float)exp((iTreb-0.5)*2.764);
	oTreb = uTreb;
}
//-----------------------------------------------------------------------------------------
void BEqualizer::setGain(float a){
	iGain = a;
	uGain = (float)exp((iGain-0.5)*2.764);
	oGain = uGain;
}
//-----------------------------------------------------------------------------------------

VstInt32 BEqualizer::getVendorVersion ()
{ 
	return 1000; 
}
//-----------------------------------------------------------------------------------------
void BEqualizer::equalize(float *signal) {
	float mul = 1;	//hlavny multiplikator
	float actFreq;
	float mul_pom = 1;	//pomocny mmultiplikator
	float dy = 1;		//smernica y
	float dx = 1;		//smernica x

	
	for (int i=0; i<fftSize; i++) {
		actFreq = i*freqPerBin;	//aktualna frekvencia
		
		//nedobra verzia
		/*if (actFreq<300) {	//bass
			if (actFreq<freqBass) {
				mul_pom = ( (1.2*actFreq) / freqBass) - 0.2;
			}
			else {
				mul_pom = ( (-actFreq * 0.5) / freqBass ) + 1.5;
			}
			mul = pow(uBass, mul_pom);
		}
		else if (actFreq<3000) {	//middle
			if (actFreq<freqMid) {
				mul_pom = ( (1.45*actFreq) / freqMid) - 0.45;
			}
			else {
				mul_pom = ( (-actFreq * 0.5) / freqMid ) + 1.5;
			}
			mul = pow(uMid, mul_pom);
		}
		else {	//treble
			if (actFreq<freqTreb) {
				mul_pom = ( (1.45*actFreq) / freqTreb) - 0.45;
			}
			else {
				mul_pom = ( (-actFreq) / freqTreb ) + 2;
			}
			mul = 4; //pow(uTreb, mul_pom);
		}
		*/

		if (actFreq<freqBass) {	//20..100
			mul = pow(uBass, (actFreq/freqBass));
		}
		else if (actFreq<freqMid) {	//100..1k
			dy = uMid - uBass;
			dx = freqMid - freqBass;
			mul = uBass + ( (actFreq-freqBass)*(dy/dx) );
		}	
		else if (actFreq<freqTreb) { //1k..10k
			dy = uTreb - uMid;
			dx = freqTreb - freqMid;
			mul = uMid + ( (actFreq-freqMid)*(dy/dx) );
		}
		else {	//10k..20k
			mul = pow(uTreb, (freqTreb/actFreq) );
		}
		
		signal[2*i] *= mul;
		signal[2*i + 1] *= mul;
		
	}
}
// -----------------------------------------------------------------------------------------------------------------
void BEqualizer::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
		
	while (--sampleFrames >= 0) {
		
		float x = *in1++;
		float x2 = *in2++;

		if (counter<fftSize) {
			signal_in[counter] = x;
			signal_in2[counter] = x2;
		}
		else {
			
			//left
			for (int i=0; i<fftSize; i++) {
				signal_fft[2*i] = signal_in[i];
				signal_fft[(2*i)+1] = 0;
			}
			Fft::Fft(signal_fft, fftSize, -1);
			equalize(signal_fft);
			Fft::Fft(signal_fft, fftSize, 1);
			for (int i=0; i<fftSize; i++) {
				signal_out[i] = signal_fft[2*i] / fftSize;
			}

			//right
			for (int i=0; i<fftSize; i++) {
				signal_fft[2*i] = signal_in2[i];
				signal_fft[(2*i)+1] = 0;
			}
			Fft::Fft(signal_fft, fftSize, -1);
			equalize(signal_fft);
			Fft::Fft(signal_fft, fftSize, 1);
			for (int i=0; i<fftSize; i++) {
				signal_out2[i] = signal_fft[2*i] / fftSize;
			}
				
			counter = 0;
			signal_in[counter] = x;
			signal_in2[counter] = x2;
		}
		(*out1++) = signal_out[counter] * uGain;
		(*out2++) = signal_out2[counter] * uGain;
		counter++;
	}
}

//-----------------------------------------------------------------------------------------
