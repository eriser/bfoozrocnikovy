#include <fstream>
#include <iostream>
#include "string.h"
#include "libraries/FftConv/FftConv.h"

using namespace std;

int main() {

	int size=65;

	float** a = new float*[2];
	float** b = new float*[2];
	float** c = new float*[2];
	a[0] = new float[size];
	a[1] = new float[size];
	b[0] = new float[size];
	b[1] = new float[size];
	c[0] = new float[128];
	c[1] = new float[128];

	for (int i=0; i<size; i++) {
		a[0][i] = a[1][i] = b[0][i] = b[1][i] =(int) sin((3.141592653589793238462643383279502/2) * i);
	}

	FftConv (a, 63, b, 65, c, -1);
	
	for (int i=0; i<64; i++) {
		cout << c[0][i] << " ";
	}
	cout << endl;

	delete [] a[0]; delete [] a[1]; delete [] a;
	delete [] b[0]; delete [] b[1]; delete [] b;
	delete [] c[0]; delete [] c[1]; delete [] c;
	return 0;
}