#ifndef __Pitch_shifter__
#define __Pitch_shifter__

#include "libraries/source/audioeffectx.h"
#include <iostream>
#include <fstream>

using namespace std;

enum { 
	kShift,
	kNumParams
};

class Pitch_shifter : public AudioEffectX
{
public:
	Pitch_shifter (audioMasterCallback audioMaster);
	~Pitch_shifter ();

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
	void setShift(float a);
	void array_shift(float x[]);
	void pitch_shift(float *magn_in, float *freq_in, float *magn_out, float *freq_out);
	void analyse(float *signal_fft, float *magn_in, float *freq_in);
	void synthese(float *magn_in, float *freq_in, float *signal_fft);
	void Fft(float *fftBuffer, long fftFrameSize, long sign);

	float sr;
	float iShift, oShift, uShift;
	char programName[kVstMaxProgNameLen + 1];
	int counter, counter2;
	long fft_size2;
	
	float pi;
	float *signal_in, *signal_out, *signal_fft;
	float *phase_old, *magn_in, *magn_out, *freq_in, *freq_out, *phase_sum;
	double phase_expect; //ocakavany fazovy rozdiel
	double oversamp; //oversampling factor
};

#endif