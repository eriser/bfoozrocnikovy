#include <fstream>
#include "string.h"

using namespace std;



int main() {

	float f=-0.00050;
	float g= 0.00045;

	float a = (float)abs(g);
	float b = (float)abs(f);
	float k  = max(a,b);


	return 0;

}