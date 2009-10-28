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
	setNumInputs (1);		// mono in
	setNumOutputs (1);		// mono out
	setUniqueID ('Comp');	// identify
	canProcessReplacing ();	// supports replacing output
	//canDoubleReplacing ();	// supports double precision processing
	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
	sr = getSampleRate();

	setInput(0.5);
	setLimiter(0);
	setAttack(0.0);
	setRelease(0.0);
	setRatio(0);
	setTresh(1);
	setOutput(0.5);
	oldOutput = 0.5;
	setAutoMakeUp(0);
	att_counter = 0;
	rel_counter = 0;
	komprimuj = false;
}

//-------------------------------------------------------------------------------------------------------
Compressor::~Compressor ()
{
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
		case kLimiter	:	setLimiter(value); break;
		case kAttack	:	setAttack(value); break;
		case kRelease	:	setRelease(value); break;
		case kRatio		:	setRatio(value); break;
		case kTresh		:	setTresh(value); break;
		case kAutoMakeUp :	setAutoMakeUp(value); break;
		case kOutput		:	setOutput(value); break;
	}
}

//-----------------------------------------------------------------------------------------
float Compressor::getParameter (VstInt32 index)
{
	float vystup = 0;
	switch (index) {
		case kInput		:	vystup = iInput; break;
		case kLimiter	:	vystup = iLimiter; break;
		case kAttack	:	vystup = iAttack; break;
		case kRelease	:	vystup = iRelease; break;
		case kRatio		:	vystup = iRatio; break;
		case kTresh		:	vystup = iTresh; break;
		case kAutoMakeUp :	vystup = iAutoMakeUp; break;
		case kOutput		:	vystup = iOutput; break;
	}
	return vystup;
}

//-----------------------------------------------------------------------------------------
void Compressor::getParameterName (VstInt32 index, char* label)
{
	switch (index) {
		case kInput		:	vst_strncpy (label, "Input", kVstMaxParamStrLen); break;
		case kLimiter	:	vst_strncpy (label, "Comp/Lim", kVstMaxParamStrLen); break; 
		case kAttack	:	vst_strncpy (label, "Attack", kVstMaxParamStrLen); break;
		case kRelease	:	vst_strncpy (label, "Release", kVstMaxParamStrLen); break;
		case kRatio		:	vst_strncpy (label, "Ratio", kVstMaxParamStrLen); break;
		case kTresh		:	vst_strncpy (label, "Treshold", kVstMaxParamStrLen); break;
		case kAutoMakeUp :  vst_strncpy (label, "MakeUp", kVstMaxParamStrLen); break;
		case kOutput		: vst_strncpy (label, "Output", kVstMaxParamStrLen);
								
	}	
}

//-----------------------------------------------------------------------------------------
void Compressor::getParameterDisplay (VstInt32 index, char* text)
{
	switch (index) {
		case kInput		:	dB2string(oInput, text, kVstMaxParamStrLen); break;
		case kLimiter	:	{
								if (oLimiter) { 
									vst_strncpy (text, "Limit", kVstMaxParamStrLen);
								}
								else {
									vst_strncpy (text, "Compress", kVstMaxParamStrLen);
								}
								break;
							}
		case kAttack	:	float2string(oAttack, text, kVstMaxParamStrLen); break;
		case kRelease	:	float2string(oRelease, text, kVstMaxParamStrLen); break;
		case kRatio		:	float2string(oRatio, text, kVstMaxParamStrLen); break;
		case kTresh		:	dB2string(oTresh, text, kVstMaxParamStrLen); break;
		case kAutoMakeUp :	{
								if (oAutoMakeUp) { 
									vst_strncpy (text, "YES", kVstMaxParamStrLen);
								}
								else {
									vst_strncpy (text, "NO", kVstMaxParamStrLen);
								}
								break;
							}
		case kOutput		:	dB2string(oOutput, text, kVstMaxParamStrLen); break;
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
		case kOutput		:	vst_strncpy (label, "dB", kVstMaxParamStrLen); break;
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
	iInput = a;
	oInput = exp((iInput-0.5)*3);
	uInput = oInput;
}
//-----------------------------------------------------------------------------------------
void Compressor::setLimiter(float a) {
	iLimiter = a;
	if (a<0.5) {
		uLimiter = false;
		oLimiter = false;
	}
	else {
		uLimiter = true;
		oLimiter = true;
	}
}
//-----------------------------------------------------------------------------------------
void Compressor::setAttack(float a) {
	iAttack = a;
	oAttack = (pow(a,5)*1000)+0.1;
	uAttack = (long)(sr * oAttack);
}
//-----------------------------------------------------------------------------------------
void Compressor::setRelease(float a){
	iRelease = a;
	oRelease = (pow(a,5)*1000)+0.1;
	uRelease = (long)(sr * oRelease);
}
//-----------------------------------------------------------------------------------------
void Compressor::setRatio(float a){
	iRatio = a;
	oRatio = 1+(a*9);
	uRatio = 1/oRatio;
}
//-----------------------------------------------------------------------------------------
void Compressor::setTresh(float a){
	iTresh = a;
	oTresh = a;
	uTresh = a;
}
//-----------------------------------------------------------------------------------------
void Compressor::setAutoMakeUp(float a){
	iAutoMakeUp = a;
	if (a<0.5) {
		uAutoMakeUp = false;
		oAutoMakeUp = false;
	}
	else {
		uAutoMakeUp = true;
		oAutoMakeUp = true;
	}
}
//-----------------------------------------------------------------------------------------
void Compressor::setOutput(float a){
	iOutput = a;
	oOutput = exp((iOutput-0.5)*3);
	uOutput = oOutput;
}
//-----------------------------------------------------------------------------------------
void Compressor::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    float* in  =  inputs[0];
    float* out = outputs[0];

	while (--sampleFrames >= 0){
		if (att_counter > 0) {
			att_counter--;
		}
		if (rel_counter > 0) {
			rel_counter--;
		}
		float x = uInput * (*in++);
		if (uLimiter) {
			(*out++) = funLimiter(x);
		}
		else {
			(*out++) = funCompressor(x);
		}
	}
}

//-----------------------------------------------------------------------------------------
float Compressor::funLimiter(float in){
	float o; //vystupna hodnota pred AutoMakeUp-om
	if (abs(in)<uTresh) {
		o = in;
	}
	else if (in>0){
		o = uTresh;
	}
	else {
		o = -uTresh;
	}

	if (uAutoMakeUp) return (o * (1/uTresh));
	else return (o * uOutput);
}
//-----------------------------------------------------------------------------------------
float Compressor::funCompressor(float in){
	float o;

	if (rel_counter < 0) {
		komprimuj = false;
	}

	if (komprimuj) {
		o = uRatio * in;
		if (abs(in)>uTresh) {
			att_counter = uAttack;
		}
	}

	if (abs(in)<uTresh) {
		o = in;
	}
	else if (in>0){
		att_counter = uAttack;
		komprimuj = true;
		o = uTresh;
	}
	else {
		att_counter = uAttack;
		komprimuj = true;
		o = -uTresh;
	}
	return (in * uOutput);
}