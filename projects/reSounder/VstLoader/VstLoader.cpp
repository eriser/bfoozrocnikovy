#include "stdafx.h"
#include "VstLoader.h"
#include <string>

using namespace std;

VstLoader::VstLoader() {
	loader = new PluginLoader();
}

VstLoader::~VstLoader() {
	delete loader;
}

void VstLoader::loadEffect(wchar_t * name) {

	this->name = name;
	
	if (!loader->loadLibrary (name)) {
		throw new std::exception("VstLoader > Failed to load VST Plugin library!");
	}

	mainEntry = loader->getMainEntry ();
	if (!mainEntry) {
		throw new std::exception("VstLoader > VST Plugin main entry not found!");
	}

	effect = (mainEntry (HostCallback));
	if (!effect) {
		throw new std::exception("Vstloader > Failed to create effect instance!");
	}

	size_t origsize = wcslen(name) + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    char nstring[newsize];
    wcstombs_s(&convertedChars, nstring, origsize, name, _TRUNCATE);

	message.assign("VstLoader >  '");
	message.append(nstring);
	message.append("'  loaded");
}

VstIntPtr VSTCALLBACK VstLoader::HostCallback (AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
{
	VstIntPtr result = 0;

	// Filter idle calls...
	bool filtered = false;
	if (opcode == audioMasterIdle)
	{
		static bool wasIdle = false;
		if (wasIdle)
			filtered = true;
		else
		{
			printf ("(Future idle calls will not be displayed!)\n");
			wasIdle = true;
		}
	}

	if (!filtered)
		printf ("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void> (value), ptr, opt);

	switch (opcode)
	{
		case audioMasterVersion :
			result = kVstVersion;
			break;
	}

	return result;
}

void VstLoader::process(float **dry, float** effected, long length){
	effect->processReplacing(effect, dry, effected, length);
}

void VstLoader::setParam(int param, float value){
	effect->setParameter(effect, param, value);
}

int VstLoader::getNumParams(){
	return effect->numParams;
}

string VstLoader::getParamName(int i) {
	char name[256] = {0};
	effect->dispatcher (effect, effGetParamName, i, 0, name, 0);
	return string(name);
}

float VstLoader::getParam(int i){
	return effect->getParameter(effect, i);
}

const char * VstLoader::getUniqueID() {
	char * pole = new char[4];
	int id = effect->uniqueID;
	pole[0] = id         >> 24;
	pole[1] = (id << 8 ) >> 24;
	pole[2] = (id << 16) >> 24;
	pole[3] = (id << 24) >> 24;
	return (const char *)pole;
}

const char * VstLoader::getFxVersion(){
	int id = effect->version;
	char * pole = new char[4];
	pole[0] = id         >> 24;
	pole[1] = (id << 8 ) >> 24;
	pole[2] = (id << 16) >> 24;
	pole[3] = (id << 24) >> 24;
	return (const char *)pole;
}

string VstLoader::getParamDisplay(int i) {
	char x[256] = {0};
	effect->dispatcher (effect, effGetParamDisplay, i, 0, x, 0);
	return (string) x;
}

string VstLoader::getLabel(int i) {
	char x[256] = {0};
	effect->dispatcher (effect, effGetParamLabel, i, 0, x, 0);
	return (string) x;
}

void VstLoader::reset(){
	delete loader;
	loader = new PluginLoader();
	loadEffect(name);
}