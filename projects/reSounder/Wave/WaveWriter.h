// by bFooz

#include <string>
#include <fstream>

using namespace std;
using namespace System;
using namespace System::IO;

public ref class WaveWriter {

private:
	int bits_per_sample;
	BinaryWriter ^ output;


public:
	WaveWriter(String ^ name, array<Byte> ^ header, long DataLength);	//konstruktor write
	void write(float * left_in, float * right_in, int size); //16-bit, stereo

};

