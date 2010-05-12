#include "../Wave/Wave.h"
#include "../VstLoader/VstLoader.h"
#include "../VstProgramSaver/VstProgramSaver.h"
#include "libraries/vst/aeffectx.h"
#include <cstring>
#include <fstream>
#include <vector>
#include <algorithm>

enum {
	tGeneral,
	tSingle,
	tDelay,
	tEq,
	tLimiter,
	tComp,
	tPitchShift,
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
	//void	closeEffectFile();		//zatvori predtym otvoreny efektovy subor

	//..informacne
	int		getNumParams();							//vracia pocet parametrov efektu
	String^ getParamName(int i);				//vracia mena parametru i
	bool	getOk();								
	bool	getProcessed();						//vracia hodnotu 'processed'
	void	setProcessed(bool b);				//nasavi hodnotu 'processed'

	//..vypoctove
	void	process(int type, int* selectedParams, bool* volType, int* stepSize, int numSelectedParams, bool fast, System::Windows::Forms::ProgressBar^ progressBar);						
	void	saveProgram(String^ fileName);		//ulozi nastavenie do fxp suboru

	//properties
	String^	message;			//sprava o stave

private:
	//methods
	
	//..hlavne
	void	processGeneral(int actParams, std::vector<float>* bestValues);
	void	processSingle(std::vector<float>* bestValues);
	void	processDelay(std::vector<float>* bestValues);
	void	processEq(int actParam, std::vector<float>* bestValues);
	void	processLimiter(std::vector<float>* bestValues);
	void	processComp(std::vector<float>* bestValues);
	void	processPitch(std::vector<float>* bestValues);

	//..pomocne
	void	fillArray(WaveReader ^ reader, float * left, float * right);	//naplni floatove polia so samplami z reader
	void	normalize(float** in, float** out, int length);	//vracia znormalizovane pole samplov in s max hodnotou 1
	float**	normalize_mul(float** input, int length, float mul);	//vracia vynasobene pole in s hodnotou mul
	float	getPeak(float * * in, long length);	//vracia hodnotu samplu s najvacsou absolutnou hodnotou
	float	abs(float f);
	void	abs(float* f, long length);						//vtrvori pole absolutnych hdnot
	void	subtract(float** in1, float** in2, long length, float** out);	//out==in1 minus in2, length == dlzka kratcieho vstupu
	void	divide(float* in1, float* in2, float* out, long length);		//out==in1/in2
	void	subtract(float* in1, float* in2, long length, float* out);
	float	getRating(float* in1, float* in2, long length);	//vracia rating rovnakosti in1 a in2 z intervalu <0...); cim nizsi, tym podobnejsie
	float	getRating(float** in1, float** in2, long length);	//vracia rating rovnakosti in1 a in2 z intervalu <0...); cim nizsi, tym podobnejsie
	float	getRatingRms(float* rms1, float* rms2, long length);	//porovnava hodnoty pola 'rms[]' s hodnotami vypocitaneho rms pole 'in[]'
	long	calculateRms(float* in, long inLength, float* out);	//do out[] vypocita rms hodnoty vstupneho signalu, vracia dlzku out[]
	long	calculateRms(float** in, long inLength, float* out);	//do out[] vypocita rms hodnoty vstupneho signalu, vracia dlzku out[]
	float	getRatingFft(float* fftSignal1, float* fftSignal2, long length);	//vypocita rating frekvencych obrazov
	void	processDelay_insert(std::vector<std::pair<float,float>>* params_best, float index, float value);	//v params_best najde majvacsiu hodnotu a ak je index mansi ako ona, nahradi
	float	EX(std::vector<float>* v);			//spocita strednu hodnotu (priemer)
	float	EX(float** in, long length);
	float	EX(float* in, long length);
	float	odch(std::vector<float>* v, float ex);	//spocita priemernu odchylku
	bool	isPeak(int pos);						//zisti, ci je na pozicii pos v dry signale peak
	//bool	testParam(int param);					//otestuje parameter, true ak je hlasitostneho typu, false ak nie je

	//properties
	//..hlavne
	WaveReader^ drySound;
	WaveReader^ wetSound;
	VstLoader*	loader;
	float* *	dry;						//pole so samplami z dry
	float* *	wet;						//pole so samplami z wet
	float**		segmentRmsWet;				//pole s vypocitanymi hodnotami RMS wet signalu, uz rovno rozdelene na segmenty
	float**		fftSegmentWet;				//vypocitane FFT segmentov wet signalu (left + right);
	float* *	dry_normalized;				//normalizovane dry
	float* *	wet_normalized;				//normalizovane wet
	System::Windows::Forms::ProgressBar^	progressBar;
	int			type;						//zvoleny typ efektu

	//..informacne
	long		dataLength;					//dlzka dat v drySound
	bool		ok_dry, ok_wet, ok_effect;	//true == nacitane
	bool		processed;					//true == najdene nastavenia, mozu sa ulozit
	long		sampleRate;
	
	//..dalsie
	int*		selectedParams;
	int			numSelectedParams;			//pocet vybranych paramterov
	int*		stepSize;					//velkost krokov pre vybrane parametre, v percentach [1..100]
	bool*		volType;					//ci je dany parameter typu volume
	float*		paramsValues;				//nastavene hodnoty parametrov
	VstProgramSaver*	program;					//sluzi na ulozenie nastavenia efektu do fxp suboru
	ofstream*	logFile;					//log file
	int			rmsWindowSize;				//velkost rms okna
	int			rmsOverlap;					//velkost prekrytia v RMS kalkulacii
	long		segmentLength;				//dlzka segmentov v procesovani
	long		fftSize;					//velkost FFT
	long		segmentNum;					//pocet segmentov, zavisi na segmentLength a dlzke signalu
	float*		segmentRmsEff;				//vypocitane RMS zo segmentu efektovaneho signalu
	long		segmentRmsLength;			//dlzka jedneho segmentu RMS vysledku
	bool		fastMethod;					//true == pouzije sa orezavanie
	String^		nl;							//new line for String
	float*		defaultParamValues;			//defaultne hodnoty parametrov
};

