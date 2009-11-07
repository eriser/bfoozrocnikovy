#ifndef __Compressor__
#define __Compressor__

#include "libraries/source/audioeffectx.h"

enum {
	kInput,
	kLimiter,
	kAttack,
	kRelease,
	kRatio,
	kTresh,
	kAutoMakeUp,
	kOutput,
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
	void setLimiter(float limiter);
	void setAttack(float attack);
	void setRelease(float release);
	void setRatio(float ratio);
	void setTresh(float tresh);
	void setAutoMakeUp(float automakeup);
	void setOutput(float Output);

	float funLimiter(float in);
	float funCompressor(float in);


	long sr;
	float iInput, iLimiter, iAttack, iRelease, iRatio, iTresh, iAutoMakeUp, iOutput; //(input) vstupna hodnota z vst ovladaca ([0,1])
	float oInput, oAttack, oRelease, oRatio, oTresh, oOutput; //(output) vystupna hodnota, ktora je zobrazovana na ovladaci
	float uInput, uRatio, uTresh, uOutput; //(used) hodnota, ktora sa pouziva
	long uAttack, uRelease; //hodnota prepocitana na sample
	long att_counter, att_counter_old, rel_counter; //odpocitavanie pre attack a release, v samploch
	float oldOutput;
	bool uAutoMakeUp, oAutoMakeUp, uLimiter, oLimiter, komprimuj;
	char programName[kVstMaxProgNameLen + 1];
};

#endif