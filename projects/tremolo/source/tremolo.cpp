#include "tremolo.h"
#include <cmath>

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new Tremolo (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
Tremolo::Tremolo (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, kNumParams)	// 1 program, 1 parameter only
{
	setNumInputs (2);		// stereo in
	setNumOutputs (2);		// stereo out
	setUniqueID ('Trem');	// identify
	canProcessReplacing ();	// supports replacing output
	//canDoubleReplacing ();	// supports double precision processing

	counter = 1;	
	fDepth = 0.5;
	fFreq = 0.1;
	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
}

//-------------------------------------------------------------------------------------------------------
Tremolo::~Tremolo ()
{
	// nothing to do here
}

//-------------------------------------------------------------------------------------------------------
void Tremolo::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Tremolo::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Tremolo::setParameter (VstInt32 index, float value)
{
	switch (index) {
		case kDepth : fDepth = value; break;
		case kFreq : fFreq = value; break;
	}
}

//-----------------------------------------------------------------------------------------
float Tremolo::getParameter (VstInt32 index)
{
	float vystup = 0;
	switch (index) {
		case kDepth : vystup = fDepth; break;
		case kFreq : vystup = fFreq; break;
	}
	return vystup;
}

//-----------------------------------------------------------------------------------------
void Tremolo::getParameterName (VstInt32 index, char* label)
{
	switch (index) {
		case kDepth : vst_strncpy (label, "Depth", kVstMaxParamStrLen); break;
		case kFreq : vst_strncpy (label, "Freq", kVstMaxParamStrLen); break;
	}
	
}

//-----------------------------------------------------------------------------------------
void Tremolo::getParameterDisplay (VstInt32 index, char* text)
{
	switch (index) {
		case kDepth : float2string(fDepth * 100, text, kVstMaxParamStrLen); break;
		case kFreq : float2string(fFreq * 10, text, kVstMaxParamStrLen); break;
	}
}

//-----------------------------------------------------------------------------------------
void Tremolo::getParameterLabel (VstInt32 index, char* label)
{
	switch (index) {
		case kDepth : vst_strncpy (label, "%", kVstMaxParamStrLen); break;
		case kFreq : vst_strncpy (label, "Hz", kVstMaxParamStrLen); break;
	}
}

//------------------------------------------------------------------------
bool Tremolo::getEffectName (char* name)
{
	vst_strncpy (name, "Tremolo", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool Tremolo::getProductString (char* text)
{
	vst_strncpy (text, "Tremolo", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool Tremolo::getVendorString (char* text)
{
	vst_strncpy (text, "Steinberg Media Technologies", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 Tremolo::getVendorVersion ()
{ 
	return 700; 
}

//-----------------------------------------------------------------------------------------
void Tremolo::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	float sampleRate = updateSampleRate();
	float modifier;
	float pocet_samplov; //pocet samplov v jednej periode tremola

    while (--sampleFrames >= 0)
     {
		pocet_samplov = sampleRate; // / (fFreq*10);
         if (counter < pocet_samplov) //kolko samplov sa spracuje v jednej vlne tremola
         {
             counter++; //kolko samplov sme zatial spracovali
         }
         else
         {
             counter = 0;
         }

     // calculate the multiplier
     modifier = (1.0 - fDepth) + fDepth * (sin((2 * 3.14 * (fFreq * 10)) * (counter/sampleRate))) * (sin((2 * 3.14 * (fFreq * 10)) * (counter/sampleRate)));
		 
     (*out1++) += (*in1++) * modifier; // replacing
     (*out2++) += (*in2++) * modifier;
     }
}

//-----------------------------------------------------------------------------------------
