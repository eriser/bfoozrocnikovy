#include "doubledelay.h"
#include <stdio.h>
#include <string.h>

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new DoubleDelay (audioMaster);
}


//-----------------------------------------------------------------------------
DoubleDelay::DoubleDelay (audioMasterCallback audioMaster)
	: AudioEffectX (audioMaster, 1, kNumParams)
{
	setNumInputs (2);
	setNumOutputs (2);
	setUniqueID ('DDly');
	canProcessReplacing ();
	vst_strncpy (programName, "Default", kVstMaxProgNameLen);

	sr = getSampleRate();
	cursor = 0;
	delay = 0.5;
	bufferL = new float[44100];
	bufferR = new float[44100];
	
	fDelay = 0;
	fWet = 0.5;
	fDry = 0.5;

	setDelay(0.5);

	resume ();		// flush buffer
}

//------------------------------------------------------------------------
DoubleDelay::~DoubleDelay ()
{
	delete [] bufferL;
	delete [] bufferR;
}


//------------------------------------------------------------------------
void DoubleDelay::setDelay (float fdelay)
{
	fDelay = fdelay;
	cursor = 0;
	delay = (long)(fdelay * (float)(sr - 1));
}

//------------------------------------------------------------------------
void DoubleDelay::setProgramName (char *name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void DoubleDelay::getProgramName (char *name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void DoubleDelay::resume ()
{

	memset (bufferL, 0, sr * sizeof (float));
	memset (bufferR, 0, sr * sizeof (float));
	//AudioEffectX::resume ();
	//AudioEffectX::resume ();
}

//------------------------------------------------------------------------
void DoubleDelay::setParameter (VstInt32 index, float value)
{
	switch (index)
	{
		case kDelay :    setDelay (value);					break;
		case kWet :		 fWet = value;						break;
		case kDry :      fDry = value;						break;
	}
}

//------------------------------------------------------------------------
float DoubleDelay::getParameter (VstInt32 index)
{
	float v = 0;

	switch (index)
	{
		case kDelay :    v = fDelay;	break;
		case kWet	:	 v = fWet;		break;
		case kDry :      v = fDry;		break;
	}
	return v;
}

//------------------------------------------------------------------------
void DoubleDelay::getParameterName (VstInt32 index, char *label)
{
	switch (index)
	{
		case kDelay :    strcpy (label, "Delay");		break;
		case kWet :		 strcpy (label, "Wet");	break;
		case kDry :      strcpy (label, "Dry");		break;
	}
}

//------------------------------------------------------------------------
void DoubleDelay::getParameterDisplay (VstInt32 index, char *text)
{
	switch (index)
	{
		case kDelay :    int2string (delay, text, kVstMaxParamStrLen);			break;
		case kWet :		 dB2string (fWet, text, kVstMaxParamStrLen);			break;
		case kDry :      dB2string (fDry, text, kVstMaxParamStrLen);			break;
	}
}

//------------------------------------------------------------------------
void DoubleDelay::getParameterLabel (VstInt32 index, char *label)
{
	switch (index)
	{
		case kDelay :    strcpy (label, "samples");	break;
		case kWet :		strcpy (label, "dB");	break;
		case kDry :      strcpy (label, "dB");		break;
	}
}

//------------------------------------------------------------------------
bool DoubleDelay::getEffectName (char* name)
{
	strcpy (name, "DoubleDelay");
	return true;
}

//------------------------------------------------------------------------
bool DoubleDelay::getProductString (char* text)
{
	strcpy (text, "DoubleDelay");
	return true;
}

//------------------------------------------------------------------------
bool DoubleDelay::getVendorString (char* text)
{
	strcpy (text, "bFooz");
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 DoubleDelay::getVendorVersion ()
{ 
	return 100; 
}

//---------------------------------------------------------------------------
void DoubleDelay::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];

	while (--sampleFrames >= 0)
	{
		float x1 = *in1++;
		float x2 = *in2++;
		float y1 = bufferL[cursor];
		float y2 = bufferR[cursor];

		bufferL[cursor] = x1;
		bufferR[cursor++] = x2;
		
		if (cursor >= delay)
			cursor = 0;
		
		(*out1++) = fDry*x1 + fWet*y1;
		(*out2++) = fDry*x2 + fWet*y2;
	}
}
