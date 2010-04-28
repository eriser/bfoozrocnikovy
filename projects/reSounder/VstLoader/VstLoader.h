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
	//methods
	VstLoader();
	~VstLoader();
	void		loadEffect(wchar_t * name);
	int			getNumParams();						//vracia pocet parametrov
	float		getParam(int i);					//vracia hodnotu parametru i
	void		setParam(int param, float value);	//nastavi parameter na value
	std::string getParamName(int i);		//vracia meno parametru i
	int			getNumChan();						//vracia pocet kanalov efektu
	const char* getUniqueID();				//vrati UniqID efektu
	const char* getFxVersion();			//vracia verziu efektu
	void		process(float ** dry, float** effected, long length);			//vracia zefektovany zvuk
	std::string	getParamDisplay(int i);			//vracia zobrazovanu hodnotu parametru i
	std::string getLabel(int i);				//vracia label (nazov jednotky v akej zobrazuje) parametru i
	void		reset();					//znovu nacita plugin

	//properties
	std::string message;		//stavova sprava

private:
	//methods

	//properties
	PluginLoader * loader;
	PluginEntryProc mainEntry;
	AEffect* effect;
	wchar_t * name;		//cesta efektu
	static VstIntPtr VSTCALLBACK HostCallback (AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt);

};