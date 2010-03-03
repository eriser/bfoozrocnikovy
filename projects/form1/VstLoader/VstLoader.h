#include "libraries/interfaces/aeffectx.h"
#include <string>

typedef AEffect* ( *PluginEntryProc) (audioMasterCallback audioMaster);

struct PluginLoader
{
	void* module;

	PluginLoader () : module (0) {}

	~PluginLoader () {
		if (module) {
			FreeLibrary ((HMODULE)module);
		}
	}
 
	bool loadLibrary (wchar_t * fileName) {
		module = LoadLibrary (fileName);
		return module != 0;
	}

	PluginEntryProc getMainEntry () {
		PluginEntryProc mainProc = 0;
		mainProc = (PluginEntryProc)GetProcAddress ((HMODULE)module, "VSTPluginMain");
		if (!mainProc)
			mainProc = (PluginEntryProc)GetProcAddress ((HMODULE)module, "main");
		return mainProc;
	}
};


class VstLoader {

public:
	VstLoader();
	~VstLoader();
	void loadEffect(wchar_t * name);
	std::string message;

private:
	PluginLoader * loader;
	PluginEntryProc mainEntry;
	AEffect* effect;
	static VstIntPtr VSTCALLBACK HostCallback (AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt);

};