//generator sumu

#include <cmath>

class NoiseGenerator {
public:
	static void GenerateWhite(float* out, long length) {	//generuje biely sum
		
		float PI = 3.141592653589793238462643383279502;
		float R1, R2; 

		for (int i=0; i<length; i++) {
			R1 = (float) rand() / (float) RAND_MAX;
			R2 = (float) rand() / (float) RAND_MAX;

			out[i] = (float) sqrt( -2.0f * log( R1 )) * cos( 2.0f * PI * R2 );
		}
		

	}
};