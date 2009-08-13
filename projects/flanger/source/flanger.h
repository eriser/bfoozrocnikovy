#ifndef __Flanger__
#define __Flanger__

#include "libraries/source/audioeffectx.h"

enum { 
	kDelay,
	kSweep,
	kSpeed,
	kMix,
	kNumParams
};

class Flanger : public AudioEffectX
{
public:
	Flanger (audioMasterCallback audioMaster);
	~Flanger ();

	// Processing
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
	//virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);

	// Program
	virtual void setProgramName (char* name);
	virtual void getProgramName (char* name);

	// Parameters
	virtual void setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterName (VstInt32 index, char* text);

	virtual void resume ();

	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion ();

protected:
	void setDelay(float delay);
	void setSweep(float sweep);
	void setSpeed(float speed);
	char programName[kVstMaxProgNameLen + 1];
	double pi;
	float fDelay, fSweep, fSpeed, fMix, delay_sucinitel, sweep_sucinitel;
	float speed, sr;
	float* bufferL;
	float* bufferR;
	long delay, sweep, act_delay, max_delay, counter, kdesom;
};

#endif