#include "pitch_shifter.h"
#include <cmath>


using namespace std;

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new Pitch_shifter (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
Pitch_shifter::Pitch_shifter (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, kNumParams)	// 1 program, 1 parameter only
{
	
	setNumInputs (1);		// stereo in
	setNumOutputs (1);		// stereo out
	setUniqueID ('Ptch');	// identify
	canProcessReplacing ();	// supports replacing output
	//canDoubleReplacing ();	// supports double precision processing

	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
	
	setShift(( (1) -0.5) / 1.5);

	pi = 3.141592653589793238462643383279502;
	sr = getSampleRate();
	counter = 0;
	counter2 = 0;
	fft_size = 2048;
	fft_size2 = 2*fft_size;
	oversamp = 4; //>0
	phase_expect = 2*pi/oversamp;

	signal_in = new float[fft_size];
	signal_fft = new float[fft_size2];
	signal_out = new float[fft_size2];
	phase_old = new float[fft_size/2+1];
	phase_sum = new float[fft_size/2+1];
	magn_in = new float[fft_size];
	freq_in = new float[fft_size];
	magn_out = new float[fft_size];
	freq_out = new float[fft_size];
	memset(signal_in, 0, fft_size * sizeof(float));
	memset(signal_out, 0, fft_size2 * sizeof(float));
	memset(signal_fft, 0, fft_size2 * sizeof(float));
	memset(phase_old, 0, (fft_size/2+1) * sizeof(float));
	memset(phase_sum, 0, (fft_size/2+1) * sizeof(float));
	memset(magn_in, 0, fft_size * sizeof(float));
	memset(freq_in, 0, fft_size * sizeof(float));
}

//-------------------------------------------------------------------------------------------------------
Pitch_shifter::~Pitch_shifter ()
{
	delete signal_in;
	delete signal_out;
	delete signal_fft;
	delete phase_old;
	delete magn_in;
	delete freq_in;
	delete magn_out;
	delete freq_out;
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
		case kShift : setShift(value); break;
	}
}

//-----------------------------------------------------------------------------------------
float Pitch_shifter::getParameter (VstInt32 index)
{
	float vystup = 0;
	switch (index) {
		case kShift : vystup = iShift; break;
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
	return 700; 
}
//-----------------------------------------------------------------------------------------
void Pitch_shifter::array_shift(float pole[]) { //posunie pole do lava o fft_size/oversamp
	for (int i=0; i<(int)(fft_size-(fft_size/oversamp)); i++) {
		pole[i] = pole[i+(int)(fft_size/oversamp)];
	}
}
//-----------------------------------------------------------------------------------------
void Pitch_shifter::pitch_shift(float *magn_in, float *freq_in, float *magn_out, float *freq_out){ //samotny posun
	memset(magn_out, 0, fft_size*sizeof(double));
	memset(freq_out, 0, fft_size*sizeof(double));
	int index;
	for (int i = 0; i <(int)(fft_size/2); i++) { 
		index = (int)(i*uShift);
		if (index <(int)(fft_size/2)) { 
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

	for (int i=0; i<(int)(fft_size/2); i++) {
		real = signal_fft[2*i];
		imag = signal_fft[2*i + 1];
		
		magn = 2*sqrt(real*real + imag*imag);
		phase = atan2(imag,real);
		
		tmp = phase - phase_old[i];
		phase_old[i] = phase;
		tmp -= i*phase_expect;
		qpd = tmp/pi;
		if (qpd >= 0) qpd += qpd&1;
		else qpd -= qpd&1;
		tmp -= pi*qpd;
		tmp = oversamp*tmp/(2*pi);
		tmp = i*(sr/fft_size) + tmp*(sr/fft_size); //k-ta skutocna freqencia
		
		//magnituda a skutocna frekvencia
		magn_in[i] = magn;
		freq_in[i] = tmp;
	}
}
//-----------------------------------------------------------------------------------------
void Pitch_shifter::synthese(float *magn_out, float *freq_out, float *signal_fft) {//synteza - spätna analyza
		
		float magn, tmp, phase;
			
		for (int i=0; i <(int)(fft_size/2); i++) {
			magn = magn_out[i];
			tmp = freq_out[i];

			tmp -= i*(sr/fft_size);
			tmp /= sr/fft_size;
			tmp = 2*pi*tmp/oversamp;
			tmp += i*phase_expect;
			phase_sum[i] += tmp;
			phase = phase_sum[i];

			//spocitat realnu a imaginarnu zlozku
			signal_fft[2*i] = magn*cos(phase);
			signal_fft[2*i+1] = magn*sin(phase);
		} 

		//vynulovat zaporne frekvencie
		for (int i = fft_size+2; i < 2*fft_size; i++) signal_fft[i] = 0;
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
   // float* in2  =  inputs[1];
    float* out1 = outputs[0];
   // float* out2 = outputs[1];
		
	while (--sampleFrames >= 0) {
		
		float x = *in1++;

		if (counter2<fft_size-fft_size/oversamp) {
			signal_in[counter2++] = x;
		}
		else {
			if (counter2+counter<fft_size) {
				signal_in[counter2+counter] = x;
			}
			else {
				for (int i=0; i<fft_size; i++) {
					//double window = -0.5*cos(2*pi*i/fft_size)+0.5;
					signal_fft[2*i] = signal_in[i];//* window;
					signal_fft[2*i+1] = 0;
				}
				Fft(signal_fft, fft_size, -1);
				
				analyse(signal_fft, magn_in, freq_in);
				
				pitch_shift(magn_in, freq_in, magn_out, freq_out);
				
				synthese(magn_out, freq_out, signal_fft);
				
				Fft(signal_fft, fft_size, 1);
				
				array_shift(signal_out);
				for (int i=0; i < fft_size; i++) {
					//double window = -0.5*cos(2*pi*i/fft_size)+0.5;
					//signal_out[i] += 2*window*signal_fft[2*i]/((fft_size/2)*oversamp);
					signal_out[i] = signal_fft[2*i]/fft_size;
				}

				array_shift(signal_in);
				counter = 1;
				signal_in[counter2+counter] = x;
			}

			(*out1++) = signal_out[counter++];
		}

	}
}

//-----------------------------------------------------------------------------------------
