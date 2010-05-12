#include "BPitchShifter.h"
#include <cmath>
#include "float.h"
#include <fstream>
#include "libraries/fft/fft.h"


using namespace std;

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new BPitchShifter (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
BPitchShifter::BPitchShifter (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, kNumParams)	// 1 program, 1 parameter only
{
	
	setNumInputs (2);		// stereo in
	setNumOutputs (2);		// stereo out
	setUniqueID ('Ptch');	// identify
	canProcessReplacing ();	// supports replacing output

	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
	
	setShift((float)(( (1) -0.5) / 1.5));

	pi = (float)3.141592653589793238462643383279502;
	sr = getSampleRate();
	counter = 0;
	counter2 = 0;

	uFftSize		=	1024;
	uFftSize2		=	uFftSize / 2;
	uOverSamp		=	4;
	FftSizeMax		=	uFftSize;
	FftSizeMax2		=	uFftSize;
	uFftSize_os14	=	(long) ((float)uFftSize/uOverSamp);		//o kolko samplov sa posunie iteracia fft
	uFftSize_os34	=	(long) (uFftSize - uFftSize_os14);		//kolko samplov sa prekryva v dvoch za sebou nasledujucich iteraciach fft, vzhladom na uOverSamp.
	
	phase_expect = 2*pi/uOverSamp;

	/*
	//log file
	ofstream outf("C:/pitch_shift_log.txt");
	outf << "FftSize:" << uFftSize << endl;
	outf << "oversamp:" << uOverSamp << endl;
	outf.close();
	*/
	

	signal_in = new float[FftSizeMax];
	signal_in2 = new float[FftSizeMax];
	signal_fft = new float[FftSizeMax*2];
	signal_out = new float[FftSizeMax];
	signal_out2 = new float[FftSizeMax];
	phase_old = new float[FftSizeMax2+1];
	phase_sum = new float[FftSizeMax2+1];
	magn_in = new float[FftSizeMax];
	freq_in = new float[FftSizeMax];
	magn_out = new float[FftSizeMax];
	freq_out = new float[FftSizeMax];
	memset(signal_in, 0, FftSizeMax * sizeof(float));
	memset(signal_in2, 0, FftSizeMax * sizeof(float));
	memset(signal_out, 0, FftSizeMax * sizeof(float));
	memset(signal_out2, 0, FftSizeMax * sizeof(float));
	memset(signal_fft, 0, FftSizeMax * 2 * sizeof(float));
	memset(phase_old, 0, (FftSizeMax/2+1) * sizeof(float));
	memset(phase_sum, 0, (FftSizeMax/2+1) * sizeof(float));
	memset(magn_in, 0, FftSizeMax * sizeof(float));
	memset(freq_in, 0, FftSizeMax * sizeof(float));
}

//-------------------------------------------------------------------------------------------------------
BPitchShifter::~BPitchShifter ()
{
	delete [] signal_in;
	delete [] signal_in2;
	delete [] signal_out;
	delete [] signal_out2;
	delete [] signal_fft;
	delete [] phase_old;
	delete [] magn_in;
	delete [] freq_in;
	delete [] magn_out;
	delete [] freq_out;
}

//-------------------------------------------------------------------------------------------------------
void BPitchShifter::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void BPitchShifter::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void BPitchShifter::setParameter (VstInt32 index, float value)
{
	switch (index) {
		case kShift		:	setShift(value);		break;
	}
}

//-----------------------------------------------------------------------------------------
float BPitchShifter::getParameter (VstInt32 index)
{
	float vystup = 0;
	switch (index) {
		case kShift		:	vystup = iShift; break;
		
	}
	return vystup;
}

//-----------------------------------------------------------------------------------------
void BPitchShifter::getParameterName (VstInt32 index, char* label)
{
	switch (index) {
		case kShift : vst_strncpy (label, "Shift", kVstMaxParamStrLen); break;
	}
	
}

//-----------------------------------------------------------------------------------------
void BPitchShifter::getParameterDisplay (VstInt32 index, char* text)
{
	switch (index) {
		case kShift : float2string(oShift, text, kVstMaxParamStrLen); break;
	}
}

//-----------------------------------------------------------------------------------------
void BPitchShifter::getParameterLabel (VstInt32 index, char* label)
{
	switch (index) {
		case kShift : vst_strncpy (label, "", kVstMaxParamStrLen); break;
	}
}

//------------------------------------------------------------------------
bool BPitchShifter::getEffectName (char* name)
{
	vst_strncpy (name, "BPitchShifter", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool BPitchShifter::getProductString (char* text)
{
	vst_strncpy (text, "BPitchShifter", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool BPitchShifter::getVendorString (char* text)
{
	vst_strncpy (text, "bFooz", kVstMaxVendorStrLen);
	return true;
}
//------------------------------------------------------------------------
void BPitchShifter::setShift(float a) {
	iShift = a;
	uShift = (float)((a*1.5)+0.5);
	oShift = uShift;
}
//-----------------------------------------------------------------------------------------
VstInt32 BPitchShifter::getVendorVersion ()
{ 
	return 1000; 
}
//-----------------------------------------------------------------------------------------
void BPitchShifter::array_shift(float* pole, long length) { //posunie pole do lava o length/uOverSamp
	int i = 0;
	long size_os = (long)((float)length/uOverSamp);
	for (i=0; i<(length - size_os); i++) {
		pole[i] = pole[i+size_os];
	}
	memset(pole + i, 0, size_os * sizeof(float));
}
//-----------------------------------------------------------------------------------------
void BPitchShifter::pitch_shift(float *magn_in, float *freq_in, float *magn_out, float *freq_out){ //samotny posun
	memset(magn_out, 0, FftSizeMax*sizeof(float));
	memset(freq_out, 0, FftSizeMax*sizeof(float));
	int index = 0;
	for (int i = 0; i <= uFftSize2; i++) { 
		index = (int)(i*uShift);
		if (index <= uFftSize2) { 
			magn_out[index] += magn_in[i]; 
			freq_out[index] = freq_in[i] * uShift; 
		} 
	}
}
//-----------------------------------------------------------------------------------------
void BPitchShifter::analyse(float *signal_fft, float *magn_in, float *freq_in) { //spocita magnitudu a skutocnu frekvenciu pre kazdy bin
	float real, imag; 
	float magn, phase, tmp;
	long qpd;

	for (int i=0; i<=uFftSize2; i++) {
		real = signal_fft[2*i];
		imag = signal_fft[2*i + 1];
		
		magn = 2*sqrt(real*real + imag*imag);
		phase = atan2(imag,real);
		
		/* rozdiel fazy */
		tmp = phase - phase_old[i];
		phase_old[i] = phase;

		/* odcitat ocakavany fazovy rozdiel */
		tmp -= (float)i*phase_expect;
		
		/* namapovat na +/- Pi interval */
		qpd = (long)(tmp/pi);
		if (qpd >= 0) qpd += qpd&1;
		else qpd -= qpd&1;
		tmp -= pi*qpd;
		
		/* odchylka od bin frekvencie z +/- Pi intervalu */
		tmp = uOverSamp*tmp/(2*pi);

		tmp = i*(sr/uFftSize) + tmp*(sr/uFftSize); //k-ta skutocna freqencia
		
		//magnituda a skutocna frekvencia
		magn_in[i] = magn;
		freq_in[i] = tmp;
	}
}
//-----------------------------------------------------------------------------------------
void BPitchShifter::synthese(float *magn_out, float *freq_out, float *signal_fft) {//synteza - spätna analyza
		
		float magn, tmp, phase;
			
		for (int i=0; i <= uFftSize2; i++) {
			/* magnituda a skutocna frekvencia */
			magn = magn_out[i];
			tmp = freq_out[i];
			
			/* odcitat strednu frekvenciu bin-u */
			tmp -= i*(sr/uFftSize);

			/* odchyllka bin-u */
			tmp /= sr/uFftSize;

			/* zapocitat uOverSampling */
			tmp = 2*pi*tmp/uOverSamp;

			tmp += i*phase_expect;

			/* vypocitat fazu bin-u */
			phase_sum[i] += tmp;
			phase = phase_sum[i];

			//spocitat realnu a imaginarnu zlozku
			signal_fft[2*i] = magn*cos(phase);
			signal_fft[2*i+1] = magn*sin(phase);
		} 

		//vynulovat zaporne frekvencie
		for (int i = uFftSize+2; i < 2*uFftSize; i++) signal_fft[i] = 0;
}
// -----------------------------------------------------------------------------------------------------------------
void BPitchShifter::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    float* in1  =  inputs[0];
	float* in2	=  inputs[1];
    float* out1 = outputs[0];
	float* out2 = outputs[1];
		
	while (--sampleFrames >= 0) {
		
		float x = *in1++;
		float x2 = *in2++;
	
		if (counter2<uFftSize_os34) {	//nacitame prve sample
			signal_in[counter2] = x;
			signal_in2[counter2] = x2;
			counter2++;
		}
		else {
			if (counter2+counter<uFftSize) {
				signal_in[counter2+counter] = x;
				signal_in2[counter2+counter] = x2;
			}
			else {
				
				//left channel
				for (int i=0; i<uFftSize; i++) {
					float window = (float)(-0.5*cos(2*pi*i/uFftSize)+0.5);
					signal_fft[2*i] = signal_in[i] * window;
					signal_fft[2*i+1] = 0;
				}
				Fft::Fft(signal_fft, uFftSize, -1);
				analyse(signal_fft, magn_in, freq_in);
				pitch_shift(magn_in, freq_in, magn_out, freq_out);
				synthese(magn_out, freq_out, signal_fft);				
				//synthese(magn_in, freq_in, signal_fft);
				Fft::Fft(signal_fft, uFftSize, 1);				
				array_shift(signal_out, uFftSize);
				for (int i=0; i < uFftSize; i++) {
					float window = (float)(-0.5*cos(2*pi*i/(float)uFftSize)+0.5);
					signal_out[i] += 2*window*signal_fft[2*i]/((uFftSize2)*uOverSamp);
				}
				array_shift(signal_in, uFftSize);
				signal_in[counter2] = x;
				
				

				//right channel
				for (int i=0; i<uFftSize; i++) {
					float window = (float)(-0.5*cos(2*pi*i/uFftSize)+0.5);
					signal_fft[2*i] = signal_in2[i] * window;
					signal_fft[2*i+1] = 0;
				}
				Fft::Fft(signal_fft, uFftSize, -1);
				analyse(signal_fft, magn_in, freq_in);
				pitch_shift(magn_in, freq_in, magn_out, freq_out);
				synthese(magn_out, freq_out, signal_fft);
				Fft::Fft(signal_fft, uFftSize, 1);				
				array_shift(signal_out2, uFftSize);
				for (int i=0; i < uFftSize; i++) {
					float window = (float)(-0.5*cos(2*pi*i/(float)uFftSize)+0.5);
					signal_out2[i] += 2*window*signal_fft[2*i]/((uFftSize2)*uOverSamp);
				}
				array_shift(signal_in2, uFftSize);
				signal_in2[counter2] = x2;
				

				counter = 0;				
			}

			(*out1++) = signal_out[counter];
			(*out2++) = signal_out2[counter];
			counter++;
		}

	}
}

//-----------------------------------------------------------------------------------------
