#include "BCompressor.h"
#include <cmath>
#include <fstream>
#include <iostream>

//using namespace std;


AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new BCompressor (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
BCompressor::BCompressor (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, kNumParams)	// 1 program, 1 parameter only
{
	setNumInputs (1);		// mono in
	setNumOutputs (1);		// mono out
	setUniqueID ('Comp');	// identify
	canProcessReplacing ();	// supports replacing output
	//canDoubleReplacing ();	// supports double precision processing
	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
	sr = (long)getSampleRate();

	setInput(0.5);
	setLimiter(0);
	setAttack((float)0.456); //pow((20-0.1)/1000, 1/5));
	setRelease((float)0.630); //pow((100-0.1)/1000, 1/5));
	setRatio(0);
	setTresh(1);
	setOutput(0.5);
	oldOutput = 0.5;
	setAutoMakeUp(0);
	att_counter = 0;
	rel_counter = 0;
	att_counter_old = 0;
	komprimuj = false;
}

//-------------------------------------------------------------------------------------------------------
BCompressor::~BCompressor ()
{
}

//-------------------------------------------------------------------------------------------------------
void BCompressor::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void BCompressor::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void BCompressor::setParameter (VstInt32 index, float value)
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
float BCompressor::getParameter (VstInt32 index)
{
	float vystup = 0;
	switch (index) {
		case kInput		:	vystup = iInput; break;
		case kLimiter	:	vystup = floor((float)(iLimiter+0.5)); break;
		case kAttack	:	vystup = iAttack; break;
		case kRelease	:	vystup = iRelease; break;
		case kRatio		:	vystup = iRatio; break;
		case kTresh		:	vystup = iTresh; break;
		case kAutoMakeUp :	vystup = floor((float)(iAutoMakeUp+0.5)); break;
		case kOutput		:	vystup = iOutput; break;
	}
	return vystup;
}

//-----------------------------------------------------------------------------------------
void BCompressor::getParameterName (VstInt32 index, char* label)
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
void BCompressor::getParameterDisplay (VstInt32 index, char* text)
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
									vst_strncpy (text, "ON", kVstMaxParamStrLen);
								}
								else {
									vst_strncpy (text, "OFF", kVstMaxParamStrLen);
								}
								break;
							}
		case kOutput		:	dB2string(oOutput, text, kVstMaxParamStrLen); break;
	}
}

//-----------------------------------------------------------------------------------------
void BCompressor::getParameterLabel (VstInt32 index, char* label)
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
bool BCompressor::getEffectName (char* name)
{
	vst_strncpy (name, "BCompressor", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool BCompressor::getProductString (char* text)
{
	vst_strncpy (text, "BCompressor", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool BCompressor::getVendorString (char* text)
{
	vst_strncpy (text, "bFooz", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 BCompressor::getVendorVersion ()
{ 
	return 100;
}

//-----------------------------------------------------------------------------------------
void BCompressor::setInput(float a) {
	iInput = a;
	oInput = exp((float)((iInput-0.5)*3));
	uInput = oInput;
}
//-----------------------------------------------------------------------------------------
void BCompressor::setLimiter(float a) {
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
void BCompressor::setAttack(float a) {
	iAttack = a;
	oAttack = (float)((pow(a,5)*1000)+0.1);
	uAttack = (long)(sr * oAttack / 1000); //atack v samploch
}
//-----------------------------------------------------------------------------------------
void BCompressor::setRelease(float a){
	iRelease = a;
	oRelease = (float)((pow(a,5)*1000)+0.1);
	uRelease = (long)(sr * oRelease / 1000); //release v samploch
}
//-----------------------------------------------------------------------------------------
void BCompressor::setRatio(float a){
	iRatio = a;
	oRatio = 1+(a*9);
	uRatio = 1/oRatio;
}
//-----------------------------------------------------------------------------------------
void BCompressor::setTresh(float a){
	iTresh = a;
	oTresh = a;
	uTresh = a;
}
//-----------------------------------------------------------------------------------------
void BCompressor::setAutoMakeUp(float a){
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
void BCompressor::setOutput(float a){
	iOutput = a;
	oOutput = (float)(exp((iOutput-0.5)*3));
	uOutput = oOutput;
}
//-----------------------------------------------------------------------------------------
void BCompressor::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	
	/*ofstream log;
	log.open("log.txt");
	log << "COMPRESSOR LOG";
	log << "\nattack = " << uAttack;
	log << "\nrelease = " << uRelease;
	log << "\ntreshold = " << uTresh;
	log << "\n";*/

	float* in  =  inputs[0];
    float* out = outputs[0];
	while (--sampleFrames >= 0){
		float o;
		if (rel_counter > 0) {
			rel_counter--;
		}
		if (att_counter > 0) {
			att_counter--;
		}
		float x = uInput * (*in++);
		if (uLimiter) {
			o = funLimiter(x);
		}
		else {
			o = funBCompressor(x);
		}
		if (uAutoMakeUp)	(*out++) = (o * (1/uTresh));
		else				(*out++) = (o * uOutput);
	}

}

//-----------------------------------------------------------------------------------------
float BCompressor::funLimiter(float in){
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

	return o;
}
//-----------------------------------------------------------------------------------------
float BCompressor::funBCompressor(float in){
	float o = 0;
	
	if (rel_counter>0) {
		komprimuj = true;
	}
	else {
		komprimuj = false;
	}
	
	if (abs(in)>uTresh) { //prekrocenie tresholdu
		if (rel_counter == 0 && att_counter == 0) {
			att_counter = uAttack;
		}
		else if (att_counter==0) {
			rel_counter = uRelease;
		}
	}

	if (att_counter == 1) {
		rel_counter = uRelease + 1;
	}

	if (komprimuj && abs(in)>uTresh) {
		o = uTresh + (abs(in) - uTresh)*uRatio;
		if (in<0) { o = -o; }
	}
	else {
		o = in;
	}

	return (o);
}