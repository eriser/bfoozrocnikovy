#include "pitch_shifter.h"
#include <cmath>
#include "float.h"
#include <fstream>


using namespace std;

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new Pitch_shifter (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
Pitch_shifter::Pitch_shifter (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, kNumParams)	// 1 program, 1 parameter only
{
	
	setNumInputs (2);		// stereo in
	setNumOutputs (2);		// stereo out
	setUniqueID ('Ptch');	// identify
	canProcessReplacing ();	// supports replacing output
	//canDoubleReplacing ();	// supports double precision processing

	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
	
	setShift(( (1) -0.5) / 1.5);

	pi = 3.141592653589793238462643383279502;
	sr = getSampleRate();
	counter = 0;
	counter2 = 0;

	uFftSize		=	1024;
	uFftSize2		=	uFftSize / 2;
	uOverSamp		=	4;
	FftSizeMax		=	uFftSize;
	FftSizeMax2		=	uFftSize;
	uFftSize_os14	=	(long) ((double)uFftSize/uOverSamp);		//o kolko samplov sa posunie iteracia fft
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
Pitch_shifter::~Pitch_shifter ()
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
void Pitch_shifter::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Pitch_shifter::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Pitch_shifter::setParameter (VstInt32 index, float value)
{
	switch (index) {
		case kShift		:	setShift(value);		break;
	}
}

//-----------------------------------------------------------------------------------------
float Pitch_shifter::getParameter (VstInt32 index)
{
	float vystup = 0;
	switch (index) {
		case kShift		:	vystup = iShift; break;
		
	}
	return vystup;
}

//-----------------------------------------------------------------------------------------
void Pitch_shifter::getParameterName (VstInt32 index, char* label)
{
	switch (index) {
		case kShift : vst_strncpy (label, "Shift", kVstMaxParamStrLen); break;
	}
	
}

//-----------------------------------------------------------------------------------------
void Pitch_shifter::getParameterDisplay (VstInt32 index, char* text)
{
	switch (index) {
		case kShift : float2string(oShift, text, kVstMaxParamStrLen); break;
	}
}

//-----------------------------------------------------------------------------------------
void Pitch_shifter::getParameterLabel (VstInt32 index, char* label)
{
	switch (index) {
		case kShift : vst_strncpy (label, "", kVstMaxParamStrLen); break;
	}
}

//------------------------------------------------------------------------
bool Pitch_shifter::getEffectName (char* name)
{
	vst_strncpy (name, "Pitch_shifter", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool Pitch_shifter::getProductString (char* text)
{
	vst_strncpy (text, "Pitch_shifter", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool Pitch_shifter::getVendorString (char* text)
{
	vst_strncpy (text, "", kVstMaxVendorStrLen);
	return true;
}
//------------------------------------------------------------------------
void Pitch_shifter::setShift(float a) {
	iShift = a;
	uShift = (a*1.5)+0.5;
	oShift = uShift;
}
//-----------------------------------------------------------------------------------------
VstInt32 Pitch_shifter::getVendorVersion ()
{ 
	return 1000; 
}
//-----------------------------------------------------------------------------------------
void Pitch_shifter::array_shift(float* pole, long length) { //posunie pole do lava o length/uOverSamp
	int i = 0;
	long size_os = (long)((double)length/uOverSamp);
	for (i=0; i<(length - size_os); i++) {
		pole[i] = pole[i+size_os];
	}
	memset(pole + i, 0, size_os * sizeof(float));
}
//-----------------------------------------------------------------------------------------
void Pitch_shifter::pitch_shift(float *magn_in, float *freq_in, float *magn_out, float *freq_out){ //samotny posun
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
void Pitch_shifter::analyse(float *signal_fft, float *magn_in, float *freq_in) { //spocita magnitudu a skutocnu frekvenciu pre kazdy bin
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
		tmp -= (double)i*phase_expect;
		
		/* namapovat na +/- Pi interval */
		qpd = tmp/pi;
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
void Pitch_shifter::synthese(float *magn_out, float *freq_out, float *signal_fft) {//synteza - spätna analyza
		
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
void Pitch_shifter::Fft(float *fftBuffer, long fftFrameSize, long sign)
/* 
	FFT routine, (C)1996 S.M.Bernsee. Sign = -1 is FFT, 1 is iFFT (inverse)
	Fills fftBuffer[0...2*fftFrameSize-1] with the Fourier transform of the
	time domain data in fftBuffer[0...2*fftFrameSize-1]. The FFT array takes
	and returns the cosine and sine parts in an interleaved manner, ie.
	fftBuffer[0] = cosPart[0], fftBuffer[1] = sinPart[0], asf. fftFrameSize
	must be a power of 2. It expects a complex input signal (see footnote 2),
	ie. when working with 'common' audio signals our input signal has to be
	passed as {in[0],0.,in[1],0.,in[2],0.,...} asf. In that case, the transform
	of the frequencies of interest is in fftBuffer[0...fftFrameSize].
*/
{
	float wr, wi, arg, *p1, *p2, temp;
	float tr, ti, ur, ui, *p1r, *p1i, *p2r, *p2i;
	long i, bitm, j, le, le2, k;

	for (i = 2; i < 2*fftFrameSize-2; i += 2) {
		for (bitm = 2, j = 0; bitm < 2*fftFrameSize; bitm <<= 1) {
			if (i & bitm) j++;
			j <<= 1;
		}
		if (i < j) {
			p1 = fftBuffer+i; p2 = fftBuffer+j;
			temp = *p1; *(p1++) = *p2;
			*(p2++) = temp; temp = *p1;
			*p1 = *p2; *p2 = temp;
		}
	}
	for (k = 0, le = 2; k < (long)(log((double)fftFrameSize)/log(2.)+.5); k++) {
		le <<= 1;
		le2 = le>>1;
		ur = 1.0;
		ui = 0.0;
		arg = pi / (le2>>1);
		wr = cos(arg);
		wi = sign*sin(arg);
		for (j = 0; j < le2; j += 2) {
			p1r = fftBuffer+j; p1i = p1r+1;
			p2r = p1r+le2; p2i = p2r+1;
			for (i = j; i < 2*fftFrameSize; i += le) {
				tr = *p2r * ur - *p2i * ui;
				ti = *p2r * ui + *p2i * ur;
				*p2r = *p1r - tr; *p2i = *p1i - ti;
				*p1r += tr; *p1i += ti;
				p1r += le; p1i += le;
				p2r += le; p2i += le;
			}
			tr = ur*wr - ui*wi;
			ui = ur*wi + ui*wr;
			ur = tr;
		}
	}
}
//-----------------------------------------------------------------------------------------
void Pitch_shifter::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
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
					double window = -0.5*cos(2*pi*i/uFftSize)+0.5;
					signal_fft[2*i] = signal_in[i] * window;
					signal_fft[2*i+1] = 0;
				}
				Fft(signal_fft, uFftSize, -1);
				analyse(signal_fft, magn_in, freq_in);
				pitch_shift(magn_in, freq_in, magn_out, freq_out);
				synthese(magn_out, freq_out, signal_fft);				
				//synthese(magn_in, freq_in, signal_fft);
				Fft(signal_fft, uFftSize, 1);				
				array_shift(signal_out, uFftSize);
				for (int i=0; i < uFftSize; i++) {
					double window = -0.5*cos(2*pi*i/(double)uFftSize)+0.5;
					signal_out[i] += 2*window*signal_fft[2*i]/((uFftSize2)*uOverSamp);
				}
				array_shift(signal_in, uFftSize);
				signal_in[counter2] = x;
				
				

				//right channel
				for (int i=0; i<uFftSize; i++) {
					double window = -0.5*cos(2*pi*i/uFftSize)+0.5;
					signal_fft[2*i] = signal_in2[i] * window;
					signal_fft[2*i+1] = 0;
				}
				Fft(signal_fft, uFftSize, -1);
				analyse(signal_fft, magn_in, freq_in);
				pitch_shift(magn_in, freq_in, magn_out, freq_out);
				synthese(magn_out, freq_out, signal_fft);
				Fft(signal_fft, uFftSize, 1);				
				array_shift(signal_out2, uFftSize);
				for (int i=0; i < uFftSize; i++) {
					double window = -0.5*cos(2*pi*i/(double)uFftSize)+0.5;
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
