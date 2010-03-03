#include "stdafx.h"
#include "VstHost.h"
#include <String>

VstHost::VstHost(){
	message = gcnew String("VstHost > VstHost initialized");
}

VstHost::~VstHost(){
	delete drySound;
	delete wetSound;
	delete loader;
}

void VstHost::openDryFile(System::String ^name) {
	drySound = gcnew WaveReader(name);
	message = gcnew String("VstHost > dry file  '" + name + "'  loaded");
}

void VstHost::openWetFile(System::String ^ name) {
	wetSound = gcnew WaveWriter(name, drySound->getHeader(), drySound->getDataLength());
	message = gcnew String("VstHost > wet file  '" + name + "'  loaded");
}

void VstHost::openEffectFile(String ^ fileName) {
	loader = new VstLoader();
	String ^ message1;
	//konverzia z String na wchar_t* koli loadEffect(wchar_t*)
	wchar_t* chars = reinterpret_cast<wchar_t*>((Runtime::InteropServices::Marshal::StringToHGlobalUni(fileName)).ToPointer());
	
	try {
		loader->loadEffect(chars);
	}
	catch (exception e) {
		message = gcnew String("VstHost > " + e.what());
		throw gcnew Exception(message);
	}

	message = gcnew String(string("VstHost > ").append(loader->message.c_str()).c_str());
	
	
}