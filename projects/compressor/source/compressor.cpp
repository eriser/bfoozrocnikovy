#include "compressor.h"
#include <cmath>

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new Compressor (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
Compressor::Compressor (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, kNumParams)	// 1 program, 1 parameter only
{
	setNumInputs (2);		// stereo in
	setNumOutputs (2);		// stereo out
	setUniqueID ('Comp');	// identify
	canProcessReplacing ();	// supports replacing output
	//canDoubleReplacing ();	// supports double precision processing
	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name

	setInput(0.6);
	setAttack(0.0);
	setRelease(0.0);
	setRatio(0);
	setTresh(1);
	setGain(0.5);
}

//-------------------------------------------------------------------------------------------------------
Compressor::~Compressor ()
{
	// nothing to do here
}

//-------------------------------------------------------------------------------------------------------
void Compressor::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Compressor::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Compressor::setParameter (VstInt32 index, float value)
{
	switch (index) {
		case kInput		:	setInput(value); break;
		case kAttack	:	setAttack(value); break;
		case kRelease	:	setRelease(value); break;
		case kRatio		:	setRatio(value); break;
		case kTresh		:	setTresh(value); break;
		case kGain		:	setGain(value); break;
	}
}

//-----------------------------------------------------------------------------------------
float Compressor::getParameter (VstInt32 index)
{
	float vystup = 0;
	switch (index) {
		case kInput		:	vystup = fInput; break;
		case kAttack	:	vystup = fAttack; break;
		case kRelease	:	vystup = fRelease; break;
		case kRatio		:	vystup = fRatio; break;
		case kTresh		:	vystup = fTresh; break;
		case kGain		:	vystup = fGain; break;
	}
	return vystup;
}

//-----------------------------------------------------------------------------------------
void Compressor::getParameterName (VstInt32 index, char* label)
{
	switch (index) {
		case kInput		:	vst_strncpy (label, "Input", kVstMaxParamStrLen); break;
		case kAttack	:	vst_strncpy (label, "Attack", kVstMaxParamStrLen); break;
		case kRelease	:	vst_strncpy (label, "Release", kVstMaxParamStrLen); break;
		case kRatio		:	vst_strncpy (label, "Ratio", kVstMaxParamStrLen); break;
		case kTresh		:	vst_strncpy (label, "Treshold", kVstMaxParamStrLen); break;
		case kGain		:	vst_strncpy (label, "Gain", kVstMaxParamStrLen); break;
	}	
}

//-----------------------------------------------------------------------------------------
void Compressor::getParameterDisplay (VstInt32 index, char* text)
{
	switch (index) {
		case kInput		:	dB2string(pInput, text, kVstMaxParamStrLen); break;
		case kAttack	:	float2string(pAttack, text, kVstMaxParamStrLen); break;
		case kRelease	:	float2string(pRelease, text, kVstMaxParamStrLen); break;
		case kRatio		:	float2string(pRatio, text, kVstMaxParamStrLen); break;
		case kTresh		:	dB2string(pTresh, text, kVstMaxParamStrLen); break;
		case kGain		:	dB2string(pGain, text, kVstMaxParamStrLen); break;
	}
}

//-----------------------------------------------------------------------------------------
void Compressor::getParameterLabel (VstInt32 index, char* label)
{
	switch (index) {
		case kInput		:	vst_strncpy (label, "dB", kVstMaxParamStrLen); break;
		case kAttack	:	vst_strncpy (label, "ms", kVstMaxParamStrLen); break;
		case kRelease	:	vst_strncpy (label, "ms", kVstMaxParamStrLen); break;
		case kRatio		:	vst_strncpy (label, "", kVstMaxParamStrLen); break;
		case kTresh		:	vst_strncpy (label, "dB", kVstMaxParamStrLen); break;
		case kGain		:	vst_strncpy (label, "dB", kVstMaxParamStrLen); break;
	}
}

//------------------------------------------------------------------------
bool Compressor::getEffectName (char* name)
{
	vst_strncpy (name, "Compressor", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool Compressor::getProductString (char* text)
{
	vst_strncpy (text, "Compressor", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool Compressor::getVendorString (char* text)
{
	vst_strncpy (text, "bFooz", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 Compressor::getVendorVersion ()
{ 
	return 100; 
}

//-----------------------------------------------------------------------------------------
void Compressor::setInput(float a) {
	fInput = a;
	pInput = a+0.4;
}
//-----------------------------------------------------------------------------------------
void Compressor::setAttack(float a) {
	fAttack = a;
	pAttack = pow(a,5)*1000;
}
//-----------------------------------------------------------------------------------------
void Compressor::setRelease(float a){
	fRelease = a;
	pRelease = pow(a,5)*1000;
}
//-----------------------------------------------------------------------------------------
void Compressor::setRatio(float a){
	fRatio = a;
	pRatio = 1+(a*9);
}
//-----------------------------------------------------------------------------------------
void Compressor::setTresh(float a){
	fTresh = a;
	pTresh = a;
}
//-----------------------------------------------------------------------------------------
void Compressor::setGain(float a){
	fGain = a;
	pGain = a+0.5;
}
//-----------------------------------------------------------------------------------------
void Compressor::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

    while (--sampleFrames >= 0)
     {
		 
     }
}

//-----------------------------------------------------------------------------------------
