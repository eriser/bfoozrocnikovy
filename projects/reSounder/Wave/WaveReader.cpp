// by bFooz
// vstup ...	16 bit, mono
//				16 bit, stereo
//				24 bit, mono
//				24 bit, stereo
//vystup ...	float

#include "stdafx.h"
#include "WaveReader.h"


WaveReader::WaveReader(String ^ name) {	//konstruktor

	this->name = name;
	num_chan = 0;
	sample_rate = 0;
	length = 0;
	bytes_per_sample = 0;
	act_pos = 44; //zaciatok zvukovych dat
	header = gcnew array<Byte>(44);

	//otvorime subor
	input = gcnew BinaryReader(File::Open(name, FileMode::Open));

	//alokujeme pamat
	length = input->BaseStream->Length;
	buffer = gcnew array<Byte>(length);

	//precitame vsetky data
	for (int i=0; i<length; i++) {
		buffer[i] = input->ReadByte();
	}
	input->Close();

	//nastavime hlavicku
	Array::Copy(buffer, header, 44);

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
	samples_length = (samples_length << 8) + buffer[43];
	samples_length = (samples_length << 8) + buffer[42];
	samples_length = (samples_length << 8) + buffer[41];
	samples_length = (samples_length << 8) + buffer[40];

	samples_length = (double)samples_length / ( num_chan * bytes_per_sample);

	//zistime max_value
	if (bytes_per_sample == 2) {
		max_value = (long)0x8000;
	}
	else if (bytes_per_sample == 3) {
		max_value = (long)0x800000;
	}
	/*else if (bytes_per_sample == 4) {
		max_value = 2147483647;
	}*/

	step();
}

void WaveReader::resetPosition() {
	act_pos = 44;
}

array<Byte> ^ WaveReader::getHeader() {
	return header;
}

int WaveReader::getNumChan() { //vracia pocet kanalov
	return num_chan;
}

float WaveReader::getSampleRate() {	//vracia sample rate
	return (float)sample_rate;
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
	
	int sample = 0;
	bool minus = false;
	unsigned char c;

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