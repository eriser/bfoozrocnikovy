#include "../Wave/Wave.h"
#include "../VstLoader/VstLoader.h"
#include "libraries/interfaces/aeffectx.h"
#include <cstring>

public ref class VstHost {

public:
	VstHost();
	~VstHost();
	String ^ message;
	void openDryFile(System::String ^ name);
	void openWetFile(System::String ^ name);
	void openEffectFile(System::String ^ name);

private:
	WaveReader ^ drySound;
	WaveWriter ^ wetSound;
	VstLoader * loader;
};

