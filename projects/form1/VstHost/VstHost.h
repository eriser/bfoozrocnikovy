#include "../Wave/Wave.h"
#include "../VstLoader/VstLoader.h"
#include "../VstProgram/VstProgram.h"
#include "libraries/interfaces/aeffectx.h"
#include <cstring>

enum {
	tVolume,
	tDelay,
	tFlanger,
	tCompressor,
	tTremolo,
};

public ref class VstHost {

public:
	//methods
	
	//..inicializacne
	VstHost();
	~VstHost();
	void	openDryFile(System::String ^ name);	//otvori subor s dry zvuko
	void	openWetFile(System::String ^ name);	//otvori subor s wet zvukom
	void	openEffectFile(System::String ^ name);	//otvori efekt

	//..informacne
	int		getNumParams();							//vracia pocet parametrov efektu
	String^ getParamName(int i);				//vracia mena parametru i
	bool	getOk();								
	bool	getProcessed();						//vracia hodnotu 'processsed'

	//..vypoctove
	void	process(int type, int* selectedParams,int numSelectedParams, System::Windows::Forms::ProgressBar^ progressBar);						
	void	saveProgram(String^ fileName);		//ulozi nastavenie do fxp suboru

	//properties
	String^	message;			//sprava o stave

private:
	//methods
	
	//..hlavne
	void	processDelay();
	void	processVolume();

	//..pomocne
	void	fillArray(WaveReader ^ reader, float * left, float * right);	//naplni floatove polia so samplami z reader
	float**	normalize(int i);	//vracia znormalizovany dry (i==0) alebo wet(i==1);
	float	getPeak(float * * in, long length);	//vracia hodnotu samplu s najvacsou absolutnou hodnotou
	float	abs(float f);

	//properties
	//..hlavne
	WaveReader^ drySound;
	WaveReader^ wetSound;
	VstLoader*	loader;
	float* *	dry;						//pole so samplami z dry
	float* *	wet;						//pole so samplami z wet
	float* *	dry_normalized;				//normalizovane dry
	float* *	wet_normalized;				//normalizovane wet
	System::Windows::Forms::ProgressBar^	progressBar;

	//..informacne
	long		dataLength;					//dlzka dat v drySound
	bool		ok_dry, ok_wet, ok_effect;	//true == nacitane
	bool		processed;					//true == najdene nastavenia, mozu sa ulozit
	long		sampleRate;
	//bool []		normalized;					//bola vykonana normalizacia [dry, wet]
	
	//..dalsie
	int*		selectedParams;
	int			numSelectedParams;			//pocet vybranych paramterov
	float*		paramsValues;				//nastavene hodnoty parametrov
	VstProgram*	program;					//sluzi na ulozenie nastavenia efektu do fxp suboru
};

