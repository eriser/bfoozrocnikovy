#include "Wave.h"
#include "libraries/soundfile++/include/soundfile.h"
#include <iostream>
#include <cmath>
#include <cstring>
#include <sstream>
#include <array>

int main() {

	/*
	WaveReader in("cymbal3.wav");
	WaveWriter out("out.wav", in.getHeader(), in.getDataLength());
	
	float * left = new float[in.getDataLength()];
	float * right = new float[in.getDataLength()];
	memset(left , 0, in.getDataLength() * sizeof(float));
	memset(right, 0, in.getDataLength() * sizeof(float));

	//for (int i=in.getDataLength()-1; i>=0; i--) {
	for (int i=0; i<in.getDataLength(); i++) {
		left[i] = in.getCurrentSample(0);
		right[i] = in.getCurrentSample(1);
		in.step();		
	}


	out.write(left, right, in.getDataLength());

	delete [] left;
	delete [] right;*/

	char c = 0;
	char d = ~c;

	return 0;
}