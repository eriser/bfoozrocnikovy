#include "Flanger.h"
#include "math.h"

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new Flanger (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
Flanger::Flanger (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, kNumParams)	// 1 program, 1 parameter only
{
	setNumInputs (1);		// mono in
	setNumOutputs (2);		// stereo out
	setUniqueID ('Flng');	// identify
	canProcessReplacing ();	// supports replacing output
	//canDoubleReplacing ();	// supports double precision processing
	
	sr = getSampleRate();
	setSpeed(0.5);
	setSweep(0.5);
	setDelay(0.5);
	fMix = 0.5;
	delay_sucinitel = 0.01; //delay max 10 ms
	sweep_sucinitel = 0.01;
	pi = 3.141592653589793238462643383;
	vst_strncpy(programName, "Defalut", kVstMaxProgNameLen);
	counter = 0;
	kdesom=0;

	max_delay =(long)sr;
	buffer = new float[max_delay];
	resume();	
}

//-------------------------------------------------------------------------------------------------------
Flanger::~Flanger ()
{
	delete[] buffer;
}

//-------------------------------------------------------------------------------------------------------
void Flanger::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Flanger::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Flanger::setParameter (VstInt32 index, float value)
{
	switch (index) {
		case kDelay	: setDelay(value); break;	
		case kSweep : setSweep(value); break;
		case kSpeed : setSpeed(value); break;
		case kMix : fMix = value; break;
	}
}

//-----------------------------------------------------------------------------------------
float Flanger::getParameter (VstInt32 index)
{
	float vystup = 0;
	switch (index) {
		case kDelay	: vystup = fDelay; break;	
		case kSweep : vystup = fSweep; break;
		case kSpeed : vystup = fSpeed; break;
		case kMix : vystup = fMix; break;
	}
	return vystup;
}

//-----------------------------------------------------------------------------------------
void Flanger::getParameterName (VstInt32 index, char* label)
{
	switch (index) {
		case kDelay : vst_strncpy (label, "Delay", kVstMaxParamStrLen); break;
		case kSweep : vst_strncpy (label, "Sweep", kVstMaxParamStrLen); break;
		case kSpeed : vst_strncpy (label, "Speed", kVstMaxParamStrLen); break;
		case kMix : vst_strncpy (label, "Mix", kVstMaxParamStrLen); break;
	}
	
}

//-----------------------------------------------------------------------------------------
void Flanger::getParameterDisplay (VstInt32 index, char* text)
{
	switch (index) {
		case kDelay : float2string(fDelay, text, kVstMaxParamStrLen); break;
		case kSweep : float2string(fSweep, text, kVstMaxParamStrLen); break;
		case kSpeed : float2string(fSpeed, text, kVstMaxParamStrLen); break;
		case kMix : float2string(fMix * 100, text, kVstMaxParamStrLen); break;
	}
}

//-----------------------------------------------------------------------------------------
void Flanger::getParameterLabel (VstInt32 index, char* label)
{
	switch (index) {
		case kDelay : vst_strncpy (label, "", kVstMaxParamStrLen); break;
		case kSweep : vst_strncpy (label, "", kVstMaxParamStrLen); break;
		case kSpeed : vst_strncpy (label, "", kVstMaxParamStrLen); break;
		case kMix : vst_strncpy (label, "%", kVstMaxParamStrLen); break;
	}
}

//------------------------------------------------------------------------
bool Flanger::getEffectName (char* name)
{
	vst_strncpy (name, "Flanger", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool Flanger::getProductString (char* text)
{
	/*char * pom = new char;
	int2string(act_delay, pom, kVstMaxParamStrLen);
	vst_strncpy (text, pom, kVstMaxEffectNameLen);*/
	vst_strncpy (text, "Flanger", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool Flanger::getVendorString (char* text)
{
	vst_strncpy (text, "bFooz", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 Flanger::getVendorVersion ()
{ 
	return 100; 
}
//-----------------------------------------------------------------------------------------
void Flanger::resume ()
{
	memset (buffer, 0, sr * sizeof (float));
	AudioEffectX::resume ();
}
//-----------------------------------------------------------------------------------------
void Flanger::setDelay(float fdelay) {
	fDelay = fdelay;
	delay = (sr)*fdelay;	
}
//------------------------------------------------------------------------------------------
void Flanger::setSweep(float fsweep) {
	fSweep = fsweep;
	sweep = (sr)*fsweep;
}
//------------------------------------------------------------------------------------------
void Flanger::setSpeed(float fspeed){ //speed = kolko vln za sekundu, frekvencia
	fSpeed = fspeed; //maximalne speed =1
	speed = fspeed/3;
}
//------------------------------------------------------------------------------------------
void Flanger::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    float* in1  =  inputs[0];
    float* out1 = outputs[0];
	float* out2 = outputs[1];
	float act_sweep = 0;	//aktualny sweep vypocitany podla aktualneho lfo
	float act_delay = 0;	//aktualny delay
	float max_counter;
	long pom1 = 0, pom2 = 0;
	float lfo, x1, a1, b1;
    while (--sampleFrames >= 0)
    {

		max_counter = sr/speed; //kolko samplov v jednej vlne lfo
		if (counter < (max_counter-1)) {
			counter++;	//aktualna pozicia v case na lfo...pocet samplov od zaciatku
		}	
		else { 
			counter=0; //reset pocitadla, sma na konci
		}
		
		lfo = 1 + sin(2 * pi * (counter/max_counter));	//aktualna amplituda lfo
		act_sweep = sweep*sweep_sucinitel * lfo;					//aktualny sweep
		act_delay = delay*delay_sucinitel + act_sweep;			//aktualny celkovy delay

		x1 = *in1++;
		//x2 = *in2++;
		
		//linearna interpolacia
		buffer[kdesom]=x1;
		pom1 = kdesom-floor(act_delay);
		if (pom1 < 0) pom1 += (max_delay);
		pom2 = kdesom-floor(act_delay)-1;
		if (pom2 < 0) pom2 += (max_delay);

		a1 = (1-(act_delay-floor(act_delay)))*buffer[pom1];
		b1 = ((act_delay-floor(act_delay)))*buffer[pom2];
		
		if (kdesom < (max_delay-1)) kdesom++; //pozicia v bufferi
		else kdesom=0;
		
		(*out1++) = x1 + fMix*(a1+b1);
		(*out2++) = x1 + fMix*(a1+b1);
		
     }
}

//-----------------------------------------------------------------------------------------
