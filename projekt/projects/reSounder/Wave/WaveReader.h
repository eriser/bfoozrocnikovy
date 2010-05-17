// by bFooz

#include <String>

using namespace std;
using namespace System;
using namespace System::IO;

///\brief Sluzi na nacitavanie wav suboru.
public ref class WaveReader {

private:
	//methods
	float getSample(int pos);	//vracia 1 vzorku

	//properties
	int num_chan;			//pocet kanalov
	int sample_rate;		//sample rate
	int length;				//velkost bufferu
	int bytes_per_sample;	//kolko bytov v 1 sampli
	array<Byte> ^ header;			//hlavicka
	array<Byte> ^ buffer;			//ulozisko pre nacitany subor
	float * current;		//aktualne sample pre vsetky kanaly
	int act_pos;			//aktualna pozicia
	long samples_length;		//pocet samplev na jeden kanal
	BinaryReader ^ input;		//input file
	long int max_value;			//maximalna hodnota samplu podla bitovej hlbky
	String ^ name;


public:
	//methods
	///\brief Konstruktor.
	///\param name cesta k suboru
	WaveReader(String ^ name);		//konstruktor read
	///\brief Vracia pocet kanalov suboru.
	int		getNumChan();		//vracia pocet kanalov
	///\brief Vracia vzorkovaciu frekvenciu suboru.
	float	getSampleRate();	//vracia sample rate
	///\brief Vracia pocet samplov jedneho kanalu
	long	getDataLength();	//vracia pocet samplov na jeden kanal
	///\brief Vracia hlavicku suboru.
	array<Byte> ^ getHeader();		//vracia hlavicku
	///\brief Vracia aktualnu vzorku zo zvoleneho kanalu.
	///\param channel kanal, ktoreho vzorku chceme
	float	getCurrentSample(int channel);	//vrati aktualnu vzorku z urceneho kanalu
	///\brief Prejde na dalsie vzorky.
	void	step();				//prejde na dalsie sample
	///\brief Nastavi aktualnu poziciu na zaciatok zvukovych dat.
	void	resetPosition();	//nastavi aktualnu poziciu na zaciatok

	//properties
};

