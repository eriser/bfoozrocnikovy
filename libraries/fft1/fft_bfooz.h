namespace FFT {

class FFT_bfooz {

public:
	float pi;
	void fft(double *data, long size);
	void ifft(double* data, long size);
	void dif_butterfly(double *data,long size);
	void inverse_dit_butterfly(double *data,long size);
	void unshuffle(double *data, long size);
	FFT_bfooz();
};

}