// by bFooz

#include <string>
#include <fstream>

using namespace std;

typedef unsigned char byte;


class WaveWriter {

private:
	byte * header;			//hlavicka
	int bits_per_sample;
	ofstream output;


public:
	WaveWriter(char * name, byte * header, long DataLength);	//konstruktor write
	~WaveWriter();		//destruktor

	void write(float * left_in, float * right_in, int size); //16-bit, stereo

};

