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