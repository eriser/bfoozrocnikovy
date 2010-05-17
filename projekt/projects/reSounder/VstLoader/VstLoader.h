#include "libraries/vst/aeffectx.h"
#include <string>

typedef AEffect* ( *PluginEntryProc) (audioMasterCallback audioMaster);

///\brief Struktura sluziaca na nacitavanie VST efektu. (podla VST SDK)
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

///\brief Sluzi na komunikaciu s VST efektom.
class VstLoader {

public:
	//methods
	VstLoader();
	~VstLoader();
	///\brief Nacita VST efekt.
	///\param name cesta k suboru efektu
	void		loadEffect(wchar_t * name);
	///\brief Vracia pocet parametrov efektu.
	int			getNumParams();						//vracia pocet parametrov
	///\brief Vracia hodnotu parametru i.
	///\param i cislo parametru
	float		getParam(int i);					//vracia hodnotu parametru i
	///\brief Nastavi parameter na hodnotu.
	///\param param cislo nastavovaneho parametru
	///\param value hodnota, na ktoru sa ma nastavit
	void		setParam(int param, float value);	//nastavi parameter na value
	///\brief Vracia nazov parametru.
	///\param i cislo parametru
	std::string getParamName(int i);		//vracia meno parametru i
	///\brief Vracia pocet kanalov efektu.
	int			getNumChan();						//vracia pocet kanalov efektu
	///\brief Vracia ID efektu.
	const char* getUniqueID();				//vrati UniqID efektu
	///\brief Vracia verziu efektu.
	const char* getFxVersion();			//vracia verziu efektu
	///\brief Spracuje signal.
	///\param dry vstupne pole s cistm signalom
	///\param wet vystupne pole so zefektovanym signalom
	///\param length dlzka dry
	void		process(float ** dry, float** effected, long length);			//vracia zefektovany zvuk
	///\brief Vracia zobrazovanu hodnotu parametru.
	///\param i cislo parametru
	std::string	getParamDisplay(int i);			//vracia zobrazovanu hodnotu parametru i
	///\brief Vracia jednotku v akej za zobrazuje parameter
	///\param i cislo parametru
	std::string getLabel(int i);				//vracia label (nazov jednotky v akej zobrazuje) parametru i
	///\brief Znovu nacita VST efekt.
	void		reset();					//znovu nacita plugin

	//properties
	///\brief Sprava o stave.
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