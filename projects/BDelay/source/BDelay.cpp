#include "BDelay.h"
#include <stdio.h>
#include <string.h>

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new BDelay (audioMaster);
}


//-----------------------------------------------------------------------------
BDelay::BDelay (audioMasterCallback audioMaster)
	: AudioEffectX (audioMaster, 1, kNumParams)
{
	setNumInputs (2);
	setNumOutputs (2);
	setUniqueID ('Dly');
	canProcessReplacing ();
	vst_strncpy (programName, "Default", kVstMaxProgNameLen);

	sr = (long)getSampleRate();
	cursor = 0;

	bufferL = new float[44100];
	bufferR = new float[44100];
	
	fDelay = 0;
	fWet = 0.5;
	fDry = 0.5;

	setDelay(0.5);

	resume ();		// flush buffer
}

//------------------------------------------------------------------------
BDelay::~BDelay ()
{
	delete [] bufferL;
	delete [] bufferR;
}


//------------------------------------------------------------------------
void BDelay::setDelay (float fdelay)
{
	fDelay = fdelay;
	cursor = 0;
	delay = (long)(fdelay * (float)(sr - 1));
}

//------------------------------------------------------------------------
void BDelay::setProgramName (char *name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void BDelay::getProgramName (char *name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void BDelay::resume ()
{

	memset (bufferL, 0, sr * sizeof (float));
	memset (bufferR, 0, sr * sizeof (float));
	//AudioEffectX::resume ();
	//AudioEffectX::resume ();
}

//------------------------------------------------------------------------
void BDelay::setParameter (VstInt32 index, float value)
{
	switch (index)
	{
		case kDelay :    setDelay (value);					break;
		case kWet :		 fWet = value;						break;
		case kDry :      fDry = value;						break;
	}
}

//------------------------------------------------------------------------
float BDelay::getParameter (VstInt32 index)
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
void BDelay::getParameterName (VstInt32 index, char *label)
{
	switch (index)
	{
		case kDelay :    strcpy (label, "Delay");		break;
		case kWet :		 strcpy (label, "Wet");	break;
		case kDry :      strcpy (label, "Dry");		break;
	}
}

//------------------------------------------------------------------------
void BDelay::getParameterDisplay (VstInt32 index, char *text)
{
	switch (index)
	{
		case kDelay :    int2string ((int)((delay*1000.)/sr), text, kVstMaxParamStrLen);			break;		
		case kWet :		 dB2string (fWet, text, kVstMaxParamStrLen);			break;
		case kDry :      dB2string (fDry, text, kVstMaxParamStrLen);			break;
	}
}

//------------------------------------------------------------------------
void BDelay::getParameterLabel (VstInt32 index, char *label)
{
	switch (index)
	{
		case kDelay :    strcpy (label, "ms");	break;
		case kWet :		strcpy (label, "dB");	break;
		case kDry :      strcpy (label, "dB");		break;
	}
}

//------------------------------------------------------------------------
bool BDelay::getEffectName (char* name)
{
	strcpy (name, "BDelay");
	return true;
}

//------------------------------------------------------------------------
bool BDelay::getProductString (char* text)
{
	strcpy (text, "BDelay");
	return true;
}

//------------------------------------------------------------------------
bool BDelay::getVendorString (char* text)
{
	strcpy (text, "bFooz");
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 BDelay::getVendorVersion ()
{ 
	return 100; 
}

//---------------------------------------------------------------------------
void BDelay::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];

	long c_pom = 0;

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
		
		c_pom++;

		(*out1++) = fDry*x1 + fWet*y1;
		(*out2++) = fDry*x2 + fWet*y2;
	}
}
