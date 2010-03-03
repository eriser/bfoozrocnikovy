// by bFooz

#include "WaveReader.h"
#include <fstream>


WaveReader::WaveReader(const char * name) {	//konstruktor

	num_chan = 0;
	sample_rate = 0;
	length = 0;
	bytes_per_sample = 0;
	act_pos = 44; //zaciatok zvukovych dat
	header = new byte[44];

	//otvorime subor
	input.open(name, ios::binary);

	//zistime dlzku
	input.seekg (0, ios::end);
	length = input.tellg();
	input.seekg (0, ios::beg);

	//alokujeme pamat
	buffer = new byte [length];

	//precitame vsetky data
	input.read ((char *)buffer,length);
	input.close();

	//nastavime hlavicku
	memcpy(header, buffer, 44);
	/*ofstream out_header("header.wav", ios::binary);
	out_header.write((char *)header, 44);
	out_header.close();*/

	//zistime pocet kanalov
	num_chan = (num_chan << 8) + buffer[23];
	num_chan = (num_chan << 8) + buffer[22];
	current = new float[num_chan];

	//zistime vzorkovaciu frekvenciu
	sample_rate = (sample_rate << 8) + buffer[27];
	sample_rate = (sample_rate << 8) + buffer[26];
	sample_rate = (sample_rate << 8) + buffer[25];
	sample_rate = (sample_rate << 8) + buffer[24];

	//zistime bytes_per_sample
	bytes_per_sample = (bytes_per_sample << 8) + buffer[35];
	bytes_per_sample = (bytes_per_sample << 8) + buffer[34];
	bytes_per_sample /= 8;

	//zistime pocet samplov na jeden kanal
	samples_length = (length-44) / (bytes_per_sample * num_chan);

	//zistime max_value
	if (bytes_per_sample == 2) {
		max_value = (long)0x8000;
	}
	else if (bytes_per_sample == 3) {
		max_value = (long)0x80000;
	}
	/*else if (bytes_per_sample == 4) {
		max_value = 2147483647;
	}*/

	step();
}



WaveReader::~WaveReader() {
	delete [] buffer;
	delete [] header;
	delete [] current;
	input.close();
}

byte * WaveReader::getHeader() {
	return header;
}

int WaveReader::getNumChan() { //vracia pocet kanalov
	return num_chan;
}

int WaveReader::getSampleRate() {	//vracia sample rate
	return sample_rate;
}

long WaveReader::getDataLength() {
	return samples_length;
}

float WaveReader::getCurrentSample(int channel) {
	return current[channel];
}

void WaveReader::step() {
	for (int i=0; i<num_chan; i++) {
		current[i] = getSample(act_pos);
		act_pos += bytes_per_sample;
	}
}

float WaveReader::getSample(int pos) {
	
	short sample = 0;
	bool minus = false;
	byte c;

	//spracujeme prvy byte
	if (buffer[pos+bytes_per_sample-1]>=128) {
		minus = true;
		c = (~(buffer[pos+bytes_per_sample-1]<<1))>>1;
	}
	else {
		c = buffer[pos+bytes_per_sample-1];
	}
	sample =  c;

	//spracujeme ostatne bytes
	for (int i=bytes_per_sample-2; i>=0; i--) {
		if (minus) {
			c = (~(buffer[pos+i]<<1))>>1;
		}
		else {
			c = buffer[pos+i];
		}
		sample = (sample << 8) + c;
	}
	
	if (minus) {
		sample *= -1;
	}

	

	return (float)sample / max_value;
}