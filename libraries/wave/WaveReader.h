// by bFooz

#include <fstream>
#include <String>

using namespace std;

typedef unsigned char byte;


class WaveReader {

private:
	int num_chan;			//pocet kanalov
	float sample_rate;		//sample rate
	int length;				//velkost bufferu
	int bytes_per_sample;	//kolko bytov v 1 sampli
	byte * header;			//hlavicka
	byte * buffer;			//ulozisko pre nacitany subor
	float getSample(int pos);	//vracia 1 vzorku
	float * current;		//aktualne sample pre vsetky kanaly
	int act_pos;			//aktualna pozicia
	long samples_length;		//pocet samplev na jeden kanal
	ifstream input;
	long int max_value;			//maximalna hodnota samplu podla bitovej hlbky

public:
	WaveReader(String ^ name);		//konstruktor read
	~WaveReader();					//destruktor
	int getNumChan();		//vracia pocet kanalov
	float getSampleRate();	//vracia sample rate
	long getDataLength();	//vracia pocet samplov na jeden kanal
	byte * getHeader();		//vracia hlavicku
	float getCurrentSample(int channel = 0);	//vrati aktualnu vzorku z urceneho kanalu
	void step();			//prejde na dalsie sample
};

