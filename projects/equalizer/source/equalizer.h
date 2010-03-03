#ifndef __Equalizer__
#define __Equalizer__

#include "libraries/source/audioeffectx.h"
#include <iostream>
#include <fstream>

using namespace std;

enum { 
	kBass,
	kMid,
	kTreb,
	kNumParams
};

class Equalizer : public AudioEffectX
{
public:
	Equalizer (audioMasterCallback audioMaster);
	~Equalizer ();

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
	long fft_size;
	void setBass(float a);
	void setMid(float a);
	void setTreb(float a);
	void Fft(float *fftBuffer, long fftFrameSize, long sign);
	void equalize(float *signal);

	float sr, pi, freqPerBin;
	char programName[kVstMaxProgNameLen + 1];
	int counter;
	
	float *signal_in, *signal_out, *signal_fft;

	float iBass, iMid, iTreb, uBass, uMid, uTreb, oBass, oTreb, oMid;
	float freqBass, freqMid, freqTreb;
};

#endif