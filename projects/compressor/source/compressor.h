#ifndef __Compressor__
#define __Compressor__

#include "libraries/source/audioeffectx.h"

enum {
	kInput,
	kAttack,
	kRelease,
	kRatio,
	kTresh,
	kGain,
	kNumParams
};

class Compressor : public AudioEffectX
{
public:
	Compressor (audioMasterCallback audioMaster);
	~Compressor ();

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

	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion ();

protected:

	void setInput(float input);
	void setAttack(float attack);
	void setRelease(float release);
	void setRatio(float ratio);
	void setTresh(float tresh);
	void setGain(float gain);


	long sr, counter, max_counter;
	float fInput, fAttack, fRelease, fRatio, fTresh, fGain; //vstupna hodnota z vst ovladaca ([0,1])
	float pInput, pAttack, pRelease, pRatio, pTresh, pGain; //vystupna hodnota
	long sAttack, sRelease; //hodnota prepocitana na sample
	char programName[kVstMaxProgNameLen + 1];
};

#endif