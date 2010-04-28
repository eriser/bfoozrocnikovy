//bFooz
//FFT konvolucia

#include "libraries/fft/fft.h"

class FftConv {
public:
	FftConv(float* input, long input_size, float* filter, long filter_size, float* output, int sign) {//podla znamienka vypocita konvoluciu(1) alebo korelaciu(-1)
		//input_size + filter_size musi byt mocninou 2
	if ((input_size + filter_size)%2 == 0) {

		long	fftSize = 2*(input_size + filter_size);
		float*	fftMulti = new float[2 * fftSize];	///pole pre vynasobene FFT rozklady
		memset(fftMulti, 0, 2 * fftSize * sizeof(float));
		
		//nove polia pouzite pre FFT
		float* fftInput = new float[2*fftSize];
		float* fftFilter = new float[2*fftSize];
		memset(fftInput, 0, 2 * fftSize * sizeof(float));
		memset(fftFilter, 0, 2 * fftSize * sizeof(float));

		//naplnime fft polia
		for (long i=0; i<input_size; i++) {
			fftInput[2*i] = input[i];
		}
		for (long i=0; i<filter_size; i++) {
			if (sign==1) {	//robime konvoluciu
				fftFilter[2*i] = filter[i];
			}
			else { //robime korelaciu
				fftFilter[2*i] = filter[filter_size - i - 1];
			}
		}

		//spocitame fft
		Fft(fftInput, fftSize, -1);
		Fft(fftFilter, fftSize, -1);

		//vynasobime frekvencie
		for (int j=0; j<2*fftSize; j++) {
			fftMulti[j] = fftInput[j] * fftFilter[j];
		}

		//inverzna FFT
		Fft(fftMulti, fftSize, 1);
		
		//vysledok
		for (long i=0; i<fftSize/2; i++) {
			output[i] = fftMulti[2*i] / fftSize;
		}
		
		delete [] fftInput;
		delete [] fftFilter;
		delete [] fftMulti;
	}
	}
	
};