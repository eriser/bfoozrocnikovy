//ukladanie nastaveni parametrov do .fxp suboru

#include "libraries/vst/aeffect.h"
#include <fstream>
#include <string>

using namespace std;

typedef char*	bytes;

///\brief Struktura sluziaca na ukladanie zloziek fxp suboru (podla VST SDK)
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

///\brief Sluzi na ukladanie nastaveni do fxp suboru.
class VstProgramSaver {
public:
	//methods
	VstProgramSaver();
	///Konstruktor, ktory hned vytvara fxp subor.
	///\param fxID ID efektu
	///\param fxVersion verzia efektu
	///\param numParams pocet parametrov efektu
	///\param fileName nazov fxp suboru, do ktoreho sa uklada
	///\param name pomenovanie nastavenia
	///\param nameLength dlzka name
	///\param params samotne hodnoty parametrov
	VstProgramSaver(const char * fxID, const char * fxVersion, int numParams, const char * fileName, const char * name, int nameLength, float* params);
	~VstProgramSaver();
	//properties

	
private:
	//methods
	char *		intToChar	(int i);	//prevedie zadany int na pole bytov(velkosti 4)
	//properties
	fxProgram*	program;
	ofstream	output;


};