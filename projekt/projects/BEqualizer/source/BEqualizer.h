#ifndef __BEqualizer__
#define __BEqualizer__

#include "libraries/vst/audioeffectx.h"
#include <iostream>
#include <fstream>

using namespace std;

enum { 
	kBass,
	kMid,
	kTreb,
	kGain,
	kNumParams
};

///\brief Implementovany efekt equalizer ako VST efekt.
///
///Dokumentacia k metodam je uvedena v dokumentacii k VST SDK.

class BEqualizer : public AudioEffectX
{
public:
	BEqualizer (audioMasterCallback audioMaster);
	~BEqualizer ();

	// Processing
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
	//virtual void processDoubleReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);

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

private:
	//methods
	void setBass(float a);
	void setMid(float a);
	void setTreb(float a);
	void setGain(float a);
	void Fft(float *fftBuffer, long fftFrameSize, long sign);
	void equalize(float *signal);


	//properties
	long fftSize;
	long fftSize2;

	float sr, pi, freqPerBin;
	char programName[kVstMaxProgNameLen + 1];
	int counter;

	float *signal_in, *signal_in2, *signal_out, *signal_out2, *signal_fft;

	float iBass, iMid, iTreb, uBass, uMid, uTreb, oBass, oTreb, oMid;
	float iGain, oGain, uGain;
	float freqBass, freqMid, freqTreb;
};

#endif