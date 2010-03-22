#include "stdafx.h"
#include "VstProgram.h"
#include <fstream>

VstProgram::VstProgram(const char * fxID, const char * fxVersion, int numParams, const char * fileName, const char * name, int nameLength, float* params){
	program = new fxProgram();
	int pointer = 0;			//kde s anachadzame v bufferi
	char * buffer = new char[52 + (4*numParams)];	//buffer na vsetky hodnoty, pri zapisovani na disk sa vypise iba on
	memset(buffer, 0, 52 + (4*numParams));

	program->chunkMagic = "CcnK";
	memcpy(&buffer[pointer], program->chunkMagic, 4);
	pointer+=4;

	program->byteSize = intToChar(0);
	memcpy(&buffer[pointer], program->byteSize, 4);
	pointer+=4;

	program->fxMagic = "FxCk";
	memcpy(&buffer[pointer], program->fxMagic, 4);
	pointer+=4;

	program->version = intToChar(0);
	memcpy(&buffer[pointer], program->version, 4);
	pointer+=4;

	program->fxID = (bytes)fxID;
	memcpy(&buffer[pointer], program->fxID, 4);
	pointer+=4;

	program->fxVersion = (bytes)fxVersion;
	memcpy(&buffer[pointer], program->fxVersion, 4);
	pointer+=4;

	program->numParams = intToChar(numParams);
	memcpy(&buffer[pointer], program->numParams, 4);
	pointer+=4;

	program->prgName = (char *)name;
	memcpy(&buffer[pointer], program->prgName, nameLength);
	pointer+=28;

	//prevedieme hodnoty parametrov a
	for (int i=0; i<numParams; i++) {
		bytes pom = (bytes)&(params[i]);
		char * pom2 = new char[4];

		//little-endian to big-endian
		for (int k=0; k<4; k++) {
			pom2[k] = pom[3-k];
		}

		memcpy(&buffer[pointer], pom2, 4);
		pointer+=4;
	}

	ofstream out(fileName, ios::binary);
	out.write(buffer, pointer);
	out.close();
}

VstProgram::~VstProgram(){
	delete program;
	output.close();
}

char * VstProgram::intToChar(int i) {
	char* pole = new char[4];
	pole[0] =  i        >> 24;
	pole[1] = (i << 8 ) >> 24;
	pole[2] = (i << 16) >> 24;
	pole[3] = (i << 24) >> 24;
	return pole;
}