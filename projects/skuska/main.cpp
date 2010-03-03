#include "libraries/fft2/FFTReal.h"
#include "math.h"
#include <stdio.h>
#include "string.h"

float pi = 3.141592653589793238462643383279502;
long sr = 44100;
const long fft_size2 = 128;
const long fft_size = 64;
int oversamp = 4;
float phase_expect = 2*pi/oversamp;

void analyse(FFTReal::flt_t *signal_fft, float *magn_in, float *freq_in) { //spocita magnitudu a skutocnu frekvenciu pre kazdy bin
	FFTReal::flt_t real, imag; 
	float magn, phase, tmp;
	long qpd;
	float * phase_old = new float[fft_size2];
	
	
	memset(phase_old, 0, (fft_size) * sizeof(float));

	for (int i=0; i<fft_size; i++) {
		real = signal_fft[i];
		if (i==0) imag=0;
		else imag = signal_fft[i+fft_size];
		
		magn = 2*sqrt(real*real + imag*imag);
		phase = atan2(imag,real);
		
		tmp = phase - phase_old[i];
		phase_old[i] = phase;
		tmp -= i*phase_expect;
		qpd = tmp/pi;
		if (qpd >= 0) qpd += qpd&1;
		else qpd -= qpd&1;
		tmp -= pi*qpd;
		tmp = oversamp*tmp/(2*pi);
		tmp = i*(sr/fft_size) + tmp*(sr/fft_size); //k-ta skutocna freqencia
		
		//magnituda a skutocna frekvencia
		magn_in[i] = magn;
		freq_in[i] = tmp;
	}
}

void shift_right(FFTReal::flt_t pole[], int i){
	int pom [fft_size];
	int j=0;
	int ciel;
	while ((ciel=floor(((j+1)*i)+0.5))<fft_size) {
		pom[ciel] = pole[j++];
	}
	for (int j=0; j<fft_size; j++){
		pole[j] = pom[j];
	}
}


int main () {

	
	
	float pi = 	3.141592653589793238462643383279502;

	FFTReal::flt_t a[fft_size2];
	FFTReal::flt_t b[fft_size2];
	FFTReal::flt_t c[fft_size2];
	float magn[fft_size], freq[fft_size];

	memset(magn, 0, fft_size * sizeof(float));
	memset(freq, 0, fft_size * sizeof(float));

	FFTReal fft(fft_size2);
	for (int i=0; i<fft_size2; i++) {
		a[i++] = pow((float)(-1),i/2);
		a[i] = 0;
	}
	
	fft.do_fft(b, a);
	analyse(b, magn, freq);
	shift_right(b, 1);

	fft.do_ifft(b,c);
	fft.rescale(c);
	
	

	return 0;
}