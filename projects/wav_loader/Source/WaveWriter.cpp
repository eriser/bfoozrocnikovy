// by bFooz

#include "WaveWriter.h"
#include <string>
#include <sstream>


WaveWriter::WaveWriter(char * name, byte * header1, long DataLength) {
	
	header = new byte[44];
	memcpy(header, header1, 44);
	bits_per_sample = 0;

	//otvorime subor
	output.open(name, ios::binary);

	//zistime bites_per_sample
	bits_per_sample = (bits_per_sample << 8) + header[35];
	bits_per_sample = (bits_per_sample << 8) + header[34];

	//zistime num_chan
	int num_chan = 0;
	num_chan = (num_chan << 8) + header[23];
	num_chan = (num_chan << 8) + header[22];

	//upravia sa vsetky hodnoty spojene s bits_per_sample alebo num_chan
	if (bits_per_sample!=16 || num_chan!=2) {
		////upravime hlavicku

		//zapiseme num_chan 2
		header[23] = 0;
		header[22] = 2;
		
		//zistime sample_rate
		int sample_rate = 0;
		sample_rate = (sample_rate << 8) + header[27];
		sample_rate = (sample_rate << 8) + header[26];
		sample_rate = (sample_rate << 8) + header[25];
		sample_rate = (sample_rate << 8) + header[24];
		
		//spocitame byte_rate
		int byte_rate = sample_rate * 2 * 2;
		//zapiseme byte_rate ho hlavicky
		header[31] = (byte_rate)        >> 24;
		header[30] = (byte_rate <<  8 ) >> 24;
		header[29] = (byte_rate << 16 ) >> 24;
		header[28] = (byte_rate << 24 ) >> 24;

		//spocitame block_align
		int block_align = 2 * 2;
		//zapiseme block align do hlavicky
		header[33] = (block_align)      >> 8;
		header[32] = (block_align << 8) >> 8;

		//zapiseme novy bites_per_sample
		header[35] = 0;
		header[34] = 16;

		//spocitame Subchunk2Size
		int subchunk2size = 0;
		subchunk2size = (subchunk2size << 8) + header[43];
		subchunk2size = (subchunk2size << 8) + header[42];
		subchunk2size = (subchunk2size << 8) + header[41];
		subchunk2size = (subchunk2size << 8) + header[40];
		//spocitame novu subchun2size
		//subchunk2size /= num_chan * (bits_per_sample / 8);
		//subchunk2size *= 2 * 16;
		subchunk2size = DataLength * 2 * 2;
		//zapiseme novy subchunk2size
		header[43] = (subchunk2size)        >> 24;
		header[42] = (subchunk2size <<  8 ) >> 24;
		header[41] = (subchunk2size << 16 ) >> 24;
		header[40] = (subchunk2size << 24 ) >> 24;

		//spocitame chunksize
		int chunksize = 36 + subchunk2size;
		//zapiseme chunksize
		header[7] = (chunksize)        >> 24;
		header[6] = (chunksize <<  8 ) >> 24;
		header[5] = (chunksize << 16 ) >> 24;
		header[4] = (chunksize << 24 ) >> 24;
	}


	//zapiseme hlavicku
	output.write((char *)header, 44);
	

}

WaveWriter::~WaveWriter() {
	output.close();
	delete [] header;
}

void WaveWriter::write(float * left_in, float * right_in, int size) {
	
	short left, right;
	char * pole = new char[4];
	unsigned char c;
	bool minus_l=false, minus_r=false;

	for (int i=0; i<size; i++) {
		
		//spracujeme lavy kanal
		left = left_in[i] * 0x8000;
		if (left_in[i]<0) {
			left *= -1;
			c = (left << 8) >> 8;
			pole[0] = ~c;
			c = left        >> 8;
			pole[1] = ~c + 0x8000;
		}
		else {
			pole[0] = (left << 8) >> 8;
			pole[1] = left        >> 8;
		}
		
		//spracujeme pravy kanal
		right = right_in[i] * 0x8000;
		if (right_in[i]<0) {
			right *= -1;
			c = (right << 8) >> 8;
			pole[2] = ~c;
			c = right        >> 8;
			pole[3] = ~c + 0x8000;
		}
		else {
			pole[2] = (right << 8) >> 8;
			pole[3] = right        >> 8;
		}
		
		//zapiseme do suboru
		output.write(pole, 4);
	}

	delete [] pole;
}