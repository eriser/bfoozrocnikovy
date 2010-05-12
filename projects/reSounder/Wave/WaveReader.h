// by bFooz

#include <String>

using namespace std;
using namespace System;
using namespace System::IO;

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
	WaveReader(String ^ name);		//konstruktor read
	int		getNumChan();		//vracia pocet kanalov
	float	getSampleRate();	//vracia sample rate
	long	getDataLength();	//vracia pocet samplov na jeden kanal
	array<Byte> ^ getHeader();		//vracia hlavicku
	float	getCurrentSample(int channel);	//vrati aktualnu vzorku z urceneho kanalu
	void	step();				//prejde na dalsie sample
	void	resetPosition();	//nastavi aktualnu poziciu na zaciatok

	//properties
};

