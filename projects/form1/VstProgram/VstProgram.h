//ukladanie nastaveni parametrov do .fxp suboru

#include "libraries/interfaces/aeffect.h"
#include <fstream>
#include <string>

using namespace std;

typedef char*	bytes;

struct fxProgram
{
	bytes	chunkMagic;			///< 'CcnK'
	bytes	byteSize;			///< size of this chunk, excl. magic + byteSize

	bytes	fxMagic;			///< 'FxCk' (regular) or 'FPCh' (opaque chunk)
	bytes	version;			///< format version (currently 1)
	bytes	fxID;				///< fx unique ID
	bytes	fxVersion;			///< fx version

	bytes	numParams;			///< number of parameters
	//suma == 7 * 4B == 28 B
	char*	prgName;			///< program name (null-terminated ASCII string)
	//suma += 28 == 52 B

};

class VstProgram {
public:
	//methods
	VstProgram();
	VstProgram(const char * fxID, const char * fxVersion, int numParams, const char * fileName, const char * name, int nameLength, float* params);
	~VstProgram();
	//properties

	
private:
	//methods
	char *		intToChar	(int i);	//prevedie zadany int na pole bytov(velkosti 4)
	//properties
	fxProgram*	program;
	ofstream	output;


};