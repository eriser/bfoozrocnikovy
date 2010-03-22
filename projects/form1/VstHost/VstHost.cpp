#include "stdafx.h"
#include "VstHost.h"
#include <String>

VstHost::VstHost(){
	message = gcnew String("VstHost > VstHost initialized");
	ok_dry = ok_wet = ok_effect = false;
	//normalized = {false, false};
	processed = false;

	dry = new float*[2];	//stereo
	wet = new float*[2];	//stereo
	dry[0] = new float[0];
	dry[1] = new float[0];
	wet[0] = new float[0];
	wet[1] = new float[0];
}

VstHost::~VstHost(){
	delete drySound;
	delete wetSound;
	delete loader;
	delete [] dry[0];
	delete [] dry[1];
	delete [] wet[0];
	delete [] wet[1];
	delete [] dry;
	delete [] wet;
	delete paramsValues;
}

void VstHost::openDryFile(System::String ^name) {
	drySound = gcnew WaveReader(name);
	sampleRate = drySound->getSampleRate();
	message = gcnew String("VstHost > dry file  '" + name + "'  loaded");
	ok_dry = true;
}

void VstHost::openWetFile(System::String ^ name) {
	wetSound = gcnew WaveReader(name);
	message = gcnew String("VstHost > wet file  '" + name + "'  loaded");
	ok_wet = true;
}

void VstHost::openEffectFile(String ^ fileName) {
	loader = new VstLoader();
	//konverzia z String na wchar_t* koli loadEffect(wchar_t*)
	wchar_t* chars = reinterpret_cast<wchar_t*>((Runtime::InteropServices::Marshal::StringToHGlobalUni(fileName)).ToPointer());
	
	try {
		loader->loadEffect(chars);
	}
	catch (exception e) {
		message = gcnew String(string("VstHost > ").append(e.what()).c_str());
		throw gcnew Exception(message);
	}

	message = gcnew String(string("VstHost > ").append(loader->message.c_str()).c_str());
	ok_effect = true;
}

int VstHost::getNumParams(){
	return loader->getNumParams();
}

String ^ VstHost::getParamName(int i) {
	String ^ name = gcnew String(loader->getParamName(i).c_str());
	return name;
}

bool VstHost::getOk() {
	return (ok_dry & ok_wet & ok_effect);
}

void VstHost::fillArray(WaveReader ^reader, float *left, float *right) {

	if (reader->getNumChan() == 1) {	//mono
		for (int i=0; i<dataLength; i++) {
			left[i] = right[i] = reader->getCurrentSample(0);
			reader->step();
		}
	}
	else if (reader->getNumChan() == 2) {	//stereo
		for (int i=0; i<dataLength; i++) {
			left[i] = reader->getCurrentSample(0);
			right[i] = reader->getCurrentSample(1);
			reader->step();
		}
	}
}

void VstHost::process(int type, int* selectedParams, int numSelectedParams, System::Windows::Forms::ProgressBar^ progressBar) {
	
	//System::Threading::Thread::Sleep(100);

	dataLength = min(drySound->getDataLength(), wetSound->getDataLength()-1);
	//naplnime floatove polia so samplami
	dry[0] = new float[dataLength];
	dry[1] = new float[dataLength];
	wet[0] = new float[dataLength];
	wet[1] = new float[dataLength];
	fillArray(drySound, dry[0], dry[1]);
	fillArray(wetSound, wet[0], wet[1]);

	//nastavime premenne
	this->progressBar = progressBar;
	this->selectedParams = selectedParams;
	this->numSelectedParams = numSelectedParams;
	paramsValues = new float[numSelectedParams];

	//spustime process
	switch(type) {
		case tVolume	:	processVolume(); break;
		case tDelay		:	processDelay(); break;
	}


//	saveProgram();
	message = gcnew String("VstHost > process complete");
	processed = true;
	
}

void VstHost::processDelay(){
		
	long	delaySamples  = 0;		//pocet samplov o kolke je delayovane
	float	delayTime = 0;			//sekundy o kolko je delayovane
	long	startDry = 0;			//dlzka zaciatocneho useku samych nul v dry[] (v samploch)
	long	startDelayed = 0;		//dlzka zaciatocneho useku samych nul v delayed_L[] && delayed_R[] (v samploch)
	
	float * * dry_n = normalize(0);	//normalizovany dry
	float * * wet_n = normalize(1);	//normalizovany wet
	float* * delayed = new float*[2];	//cisty oneskoreny signal
	delayed[0] = new float[dataLength];
	delayed[1] = new float[dataLength];

	//odcitame wet - dry
	for (int i=0; i<dataLength; i++) {
		delayed[0][i] = wet_n[0][i] - dry_n[0][i];
		delayed[1][i] = wet_n[1][i] - dry_n[1][i];
	}

	//najdeme dlzku zaciatocneho useku samych nul v dry[]
	while ( (dry_n[0][startDry]==0) && (dry_n[1][startDry]==0) ) {
		startDry++;
	}

	//najdeme dlzku zaciatocneho useku nul v delayed_L[] a delayed_R[]
	while ( (delayed[0][startDelayed]==0) && (delayed[1][startDelayed]==0) ) {
		startDelayed++;
	}

	delaySamples = startDelayed - startDry;
	delayTime = (double)delaySamples / sampleRate;

	delete [] delayed[0];	delete [] delayed[1];	delete [] delayed;
	delete [] dry_n[0];		delete [] dry_n[1];		delete [] dry_n;
	delete [] wet_n[0];		delete [] wet_n[1];		delete [] wet_n;
}

void VstHost::processVolume() {
	
	float	sum = 0;	//sucet pomerov volume vsetkych samplov wet/dry
	float	sum_old = 0;	//sum v predchadzajucej iteracii
	float	average = 0;
	int		testLength = min(dataLength, long::MaxValue);	//pocet samplov ktore sa otestuju
	int		realLength = 0;				//skutocny pocet porovnanych samplov
	array<float^>^	change = gcnew array<float^>(testLength);	//ulozene rozdiely hlasitosti

	for (long i=0; i<testLength; i++) {

		change[i] =( ( wet[0][i] / dry[0][i] ) + (wet[1][i] / dry[1][i]) )/2;

		if (!float::IsNaN(*(change[i]))) {
			sum_old = sum;
			sum = sum + *(change[i]);
			if (float::IsInfinity(sum)) {	//ak je sum uz prilis velke cislo aby sa zmestilo do float
				sum = sum_old / realLength;
				realLength = 0;
				sum_old = 0;
			}
			realLength++;
		}
		
		progressBar->Value = (int)( ((float)i/testLength) * 100 );
	}

	average = sum / realLength;

	paramsValues[0] = average;
}

void VstHost::saveProgram(String^ fileName) {
	
	if (!processed) {
		message = gcnew String("VstHost > Parameters values not processed, cannot save!");
		throw gcnew Exception();
	}
	else {

		int numParams = loader->getNumParams();
		float * params = new float[numParams];		//hodnoty parametrov

		//nastavime vsetky default hodnoty parametrov
		for(int i=0; i<numParams; i++) {
			params[i] = loader->getParam(i);
		}

		//nastavime hodnoty zmenenych parametrov
		for (int i=0; i<numSelectedParams; i++) {
			params[selectedParams[i]] = paramsValues[i];
		}

		//----
		params[0] = 1;
		//----

		//z fileName vytvorime nazov presetu, prevedieme zo String^ na char*
		int pos1 = fileName->LastIndexOf("\\") + 1;
		int pos2 = fileName->LastIndexOf(".");
		String^ name = gcnew String(fileName->Substring(pos1, pos2-pos1));

		char* fileNameChar = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(fileName);
		char* nameChar	= (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(name);

		//ulozime VstProgram
		VstProgram(loader->getUniqueID(), loader->getFxVersion(), numParams, fileNameChar, nameChar, name->Length, params);
		message = gcnew String("VstHost > Saved to file " + fileName);
	}
}

bool VstHost::getProcessed(){
	return processed;
}

float * * VstHost::normalize (int i) {
	/*if (normalized[i]==true) {
		switch (i) {
			case 0	:	return normalized_dry;	break;
			case 1	:	return normalized_wet;	break;
		}
	}
	else {*/
		float peak;
		float mul;	//multiplikator
		float * * input;
		float * * output = new float*[2];
		output[0] = new float[dataLength];
		output[1] = new float[dataLength];

		switch(i) {
			case 0	:	input = dry; break;
			case 1	:	input = wet; break;
			default	:	{};
		}

		peak = getPeak(input, dataLength);
		mul = 1. / peak;

		for (long j=0; j<dataLength; j++) {
			output[0][j] = input[0][j] * mul;
			output[1][j] = input[1][j] * mul;
		}

		return output;
	//}
}

float VstHost::getPeak(float * * in, long length) {
	float peak = 0;
	for (long i=0; i<length; i++ ) {
		float maxpom = max(abs(in[0][i]), abs(in[1][i]));
		peak = max(peak, maxpom);
	}
	return peak;
}

float VstHost::abs(float f) {
	if (f<0) {
		return -f;
	}
	else {
		return f;
	}
}

