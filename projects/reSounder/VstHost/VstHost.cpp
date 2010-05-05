#include "stdafx.h"
#include "VstHost.h"
#include <String>
#include <cmath>
//#include "libraries/fft/fft.h"
#include "libraries/FftConv/FftConv.h"
#include "libraries/source/audioeffect.h"
#include "libraries/NoiseGenerator/NoiseGenerator.h"


using namespace std;

VstHost::VstHost(){
	message = gcnew String("VstHost > VstHost initialized");
	ok_dry = ok_wet = ok_effect = false;
	processed = false;
	logFile = new ofstream("c:/ReSounder_log.txt");
	logFile->precision(16);
	logFile->setf(ios::fixed);
	fftSize = rmsWindowSize = 1024;
	rmsOverlap = 0;
	segmentLength = rmsWindowSize;
	nl = gcnew String(Environment::NewLine);
	defaultParamValues = new float[9];
}

VstHost::~VstHost(){
	logFile->close();
	delete logFile;
	delete drySound;
	delete wetSound;
	delete loader;
	delete defaultParamValues;
}

void VstHost::openDryFile(System::String ^name) {
	drySound = gcnew WaveReader(name);
	sampleRate = drySound->getSampleRate();
	message = gcnew String("VstHost > clean file  '" + name + "'  loaded");
	ok_dry = true;
}

void VstHost::openWetFile(System::String ^ name) {
	wetSound = gcnew WaveReader(name);
	message = gcnew String("VstHost > wanted file  '" + name + "'  loaded");
	ok_wet = true;
}

void VstHost::openEffectFile(String ^ fileName) {
	if (ok_effect) {
		delete loader;
		ok_effect = false;
	}
	loader = new VstLoader();
	//konverzia z String na wchar_t* koli loadEffect(wchar_t*)
	wchar_t* chars = reinterpret_cast<wchar_t*>((Runtime::InteropServices::Marshal::StringToHGlobalUni(fileName)).ToPointer());
	
	try {
		loader->loadEffect(chars);
	}
	catch (exception e) {
		message = gcnew String(string("VstHost > ").append(e.what()).c_str());
		throw gcnew Exception(message);
	}

	//ulozime defaultne hodnoty parametrov
	for (int i=0; i<loader->getNumParams(); i++) {
		defaultParamValues[i] = loader->getParam(i);
	}

	message = gcnew String(string("VstHost > ").append(loader->message.c_str()).c_str());
	ok_effect = true;
}

int VstHost::getNumParams(){
	return loader->getNumParams();
}

String ^ VstHost::getParamName(int i) {
	String ^ name = gcnew String(loader->getParamName(i).c_str());
	return name;
}

bool VstHost::getOk() {
	return (ok_dry & ok_wet & ok_effect);
}

void VstHost::fillArray(WaveReader ^reader, float *left, float *right) {

	reader->resetPosition();
	if (reader->getNumChan() == 1) {	//mono
		for (int i=0; i<dataLength; i++) {
			left[i] = right[i] = reader->getCurrentSample(0);
			reader->step();
		}
	}
	else if (reader->getNumChan() == 2) {	//stereo
		for (int i=0; i<dataLength; i++) {
			left[i] = reader->getCurrentSample(0);
			right[i] = reader->getCurrentSample(1);
			reader->step();
		}
	}
}

void VstHost::process(int type, int* selectedParams, bool* volType, int* stepSize, int numSelectedParams, bool fastMethod, System::Windows::Forms::ProgressBar^ progressBar) {
	
	dataLength = min(drySound->getDataLength(), wetSound->getDataLength());
	//segmentLength = dataLength;
	
	//floatove polia so samplami
	dry = new float*[2];	//stereo
	wet = new float*[2];	//stereo
	dry[0] = new float[dataLength];
	dry[1] = new float[dataLength];
	wet[0] = new float[dataLength];
	wet[1] = new float[dataLength];
	fillArray(drySound, dry[0], dry[1]);	//ziskame sample z dry signalu
	fillArray(wetSound, wet[0], wet[1]);	//ziskame sample z wet signalu

	//nastavime premenne
	this->type = type;
	this->progressBar = progressBar;
	this->selectedParams = selectedParams;
	this->numSelectedParams = numSelectedParams;
	this->stepSize = stepSize;
	this->volType = volType;
	this->fastMethod = fastMethod;

	segmentNum = dataLength / segmentLength;		//pocet segmentov

	
	////------------   RMS inicializacia

			//zistime, ci aspon jeden parameter je volume-type
			bool vol = false;		//true == aspon jeden parameter je volume-type
			for (int i=0; i<numSelectedParams; i++) {
				if (volType[i]) { vol = true; }
			}

			if (vol) { //ak ano
				
				segmentRmsLength = segmentLength / (rmsWindowSize-rmsOverlap);	//dlzka jedneho segmentu v segmentRmsWet
				segmentRmsEff = new float[segmentRmsLength];		//ulozene RMS pre jeden segment efektovaneho zvuku
				segmentRmsWet = new float*[segmentNum];				//ulozene RMS pre cely wet signal, uz rozdelene do segmentov
				for (int i=0; i<segmentNum; i++) {
					segmentRmsWet[i] = new float[segmentRmsLength];
				}
				float* segmentRmsPom = new float[segmentLength];	//pomocne pole

				//spocitame segmentRmsWet
				for (int segment=0; segment<segmentNum; segment++) {	//cez vsetky segmenty
					//zoberieme sample z wet signalu aktualneho segmentu
					for (int pos=0; pos<segmentLength; pos++) {	//cez cely aktualny semgent
							segmentRmsPom[pos] = wet[0][(segment*segmentLength)+pos] + wet[1][(segment*segmentLength)+pos];	//odhryzneme segment z wet signalu
					}
					calculateRms(segmentRmsPom, segmentLength, segmentRmsWet[segment]);	//spocitame RMS pre aktualy segment
				}
				delete [] segmentRmsPom;
			}

	////------------- / RMS inicializacia


	//nastavime efektu vsetky parametre na defaultne hodnoty
	{
		int numParams = loader->getNumParams();
		for (int i=0; i<numParams; i++) {
			loader->setParam(i, defaultParamValues[i]);
		}
	}

	//spustime process

	vector<float>*	bestValues = new vector<float>();
	message = gcnew String("result:" + nl);
	switch(type) {
		case tGeneral	:	processGeneral(numSelectedParams-1, bestValues); break;
		case tSingle	:	processSingle(bestValues); break;
		case tDelay		:	processGeneral(numSelectedParams-1, bestValues); break;
		case tEq		:	processEq(numSelectedParams-1, bestValues); break;
		case tLimiter	:	processLimiter(bestValues); break;
		case tComp		:	processComp(bestValues); break;
		case tPitchShift:	processPitch(bestValues); break;
	}
	
	progressBar->Value = 100;
	

	//nastavime efektu vsetky parametre na defaultne hodnoty
	{
		int numParams = loader->getNumParams();
		for (int i=0; i<numParams; i++) {
			loader->setParam(i, defaultParamValues[i]);
		}
	}
	//nastavime efektu vybrane parametre na vysledne hodnoty
	for (int i=0; i<bestValues->size(); i++) {
		loader->setParam(selectedParams[i], (*bestValues)[i]);
	}

	//vypis
	for (int i=0; i<bestValues->size(); i++) {
		(*logFile) << "parameter " << selectedParams[i] << " == " << (*bestValues)[i] << endl;
		String^ name = gcnew String(loader->getParamName(selectedParams[i]).c_str());
		String^ value = gcnew String( (loader->getParamDisplay(selectedParams[i])).c_str());
		String^ label = gcnew String((loader->getLabel(selectedParams[i])).c_str());
		message = message->Insert(message->Length,(name + "\t=\t" + value + " " + label + "\t(" + (*bestValues)[i] + ") ;" + nl ));
	}

	delete bestValues;

	delete [] dry[0];	delete [] dry[1]; delete [] dry;
	delete [] wet[0];	delete [] wet[1]; delete [] wet;

	if (vol) {
		delete [] segmentRmsEff;
		for (int i=0; i<segmentNum; i++) {
			delete [] segmentRmsWet[i];
		}
		delete [] segmentRmsWet;
	}

	//message = gcnew String("VstHost > process complete");
	processed = true;
}

void VstHost::processEq(int actParam, vector<float>* bestValues) {

		float fftSize2 = fftSize/2;
		float PI  = 3.1415926536f;
		float freqPerBin = sampleRate / fftSize;		//kolko Hz je v jednom bin-e
		int	bandsNum = 3;								//pocet pasiem
		int bandsFreq [] = {100, 1000, 10000};			//frekvencie pasiem
		float gain = 0.5;								//hodnota parametru gain
		long fftSegmentNum = dataLength / fftSize;		//pocet fft segmentov

		float* fftWet = new float[fftSize2];	//ulozena eq krivka wet signalu
		float* fftDry = new float[fftSize2];	//ulozena eq krivka dry signalu
		float* fftDiff = new float[fftSize2];	//ulozeny rozdiel magnitud wet - dry
		float* fftEff = new float[fftSize2];	//eq krivka zefektovaneho signalu
		float* fftSegment = new float[2*fftSize];	//pouzivane pre ukladanie segmentu wet alebo dry signalu pre fft
		memset(fftWet, 0, (fftSize2) * sizeof(float));
		memset(fftDry, 0, (fftSize2) * sizeof(float));
		memset(fftDiff, 0, (fftSize2) * sizeof(float));
		memset(fftEff, 0, (fftSize2) * sizeof(float));

			
		
		
//--- vypocet -----------------------------------------------------------------------------------------------------

		for (int segment=0; segment<fftSegmentNum; segment++) { //cez vsetky fft segmenty
			
			//------  dry signal
			memset(fftSegment, 0, 2 * fftSize * sizeof(float));
			//nacitame dry segment
			for (long i=0; i<fftSize; i++) {
				fftSegment[2*i] = dry[0][segment*fftSize + i] + dry[1][segment*fftSize + i];	//left + right
			}
			//spocitame fft segmentu
			Fft(fftSegment, fftSize, -1);
			//zvysime fftDry o vypocitane hodnoty magnitudy
			for	(long i=0; i<fftSize2; i++) {	//cez cely dlzku segmentu
				fftDry[i]+= sqrt( (fftSegment[2*i]*fftSegment[2*i] + fftSegment[2*i+1]*fftSegment[2*i+1]));
			}

			//--------  wet signal
			memset(fftSegment, 0, 2 * fftSize * sizeof(float));
			//nacitame wet segment
			for (long i=0; i<fftSize; i++) {
				fftSegment[2*i] = wet[0][segment*fftSize + i] + wet[1][segment*fftSize + i];	//left + right
			}
			//spocitame fft segmentu
			Fft(fftSegment, fftSize, -1);
			//zvysime fftWet o vypocitane hodnoty magnitudy
			for	(long i=0; i<fftSize2; i++) {	//cez cely dlzku segmentu
				fftWet[i]+= sqrt(fftSegment[2*i]*fftSegment[2*i] + fftSegment[2*i+1]*fftSegment[2*i+1] );
			}

			progressBar->Value = (int)(100* ((float)segment/fftSegmentNum));
		}

		/*
		//spocitame rozdiel wet - dry
		subtract(fftWet, fftDry, fftSize2, fftDiff);

		////znormalizujeme fftWet na uroven fftDry
		//nejdeme frekvenciu ktora je v dry a wet na co najpodobnejsej urovni, tzn. abs(fftWet - fftDry) je co najblizsie k 0
		long binSimilar = -1;	//frekvencie, v ktorej su dry a wet najpodobnejsie
		float diff = float::MaxValue;	//najmensi rozdiel wet-dry
		for (long bin=0; bin<fftSize2; bin++) {		//cez vsetky biny
			if ( (abs(fftDiff[bin])) < diff) {		//ak sme nasli zatal najpodobnejsiu frekvenciu
				binSimilar = bin;
				diff = abs(fftDiff[bin]);
			}
		}
		//zistime hodnotu gain
		gain = fftWet[binSimilar] / fftDry[binSimilar];
		*/
		
		//zistime pomery v pasmach z bandsFreq
		for (int i=0; i<numSelectedParams; i++) {
			if (selectedParams[i]==3) {
				bestValues->push_back(0.5); //gain
			}
			else {
				int param = selectedParams[i];
				long bin = bandsFreq[param] / freqPerBin;
				float x = 0;
				for (int j=-1; j<=1; j++) {	//okolie
					x+=fftWet[bin+j] / fftDry[bin+j];	
				}
				x /= 3;
				bestValues->push_back((log(x)/2)+0.5);	//inverzny vzorec z equalizer.cpp
			}
		}
		
		/*
		////vypis do logu
		//-- konvolucia
		int filterLength = 4;
		float* convFilter = new float[filterLength];
		for (int i=0; i<filterLength; i++) {
			convFilter[i] = 0.25;
		}
		float* fftDiff2 = new float[fftSize/2];
		float* pom = new float[fftSize];
		float* pom2 = new float[fftSize];
		memset(pom, 0, fftSize * sizeof(float));
		memset(pom, 0, fftSize * sizeof(float));
		memset(fftDiff2, 0, fftSize/2 * sizeof(float));

		FftConv(fftDiff, fftSize/2 - filterLength , convFilter, filterLength, fftDiff2, 1);	//konvolucia
		//vupis
		for (long i=0; i<fftSize/2; i++) {
				(*logFile) << abs(fftDiff2[i]);
				(*logFile) << "\t" << fftDiff2[i];
				//(*logFile) << "\t" << sqrt(whiteNoiseFft[2*i]*whiteNoiseFft[2*i] + whiteNoiseFft[2*i+1]*whiteNoiseFft[2*i+1]) / fftSize;
				(*logFile) << endl;
				
		}
		delete [] convFilter;
		delete [] fftDiff2;
		delete [] pom;
		delete [] pom2;
		*/

		

		delete [] fftWet;
		delete [] fftDry;
		delete [] fftDiff;
		delete [] fftEff;
		delete [] fftSegment;

}

void VstHost::processGeneral(int actParam, vector<float>* bestValues) {
	if (actParam!=-1) { //ak mame este nejaky parameter na spracovanie, inak koniec rekurzie
		float** effected = new float*[2];		//bude ukladany zefektovany signal
		for (int i=0; i<2; i++) {
			effected[i] = new float[dataLength];
		}

		float*	segmentEff = new float[segmentLength];	//left + right
		float*	segmentWet = new float[segmentLength];
		
		int stepIters = (100. /stepSize[actParam]) + 1;	//pocet iteracii ktorymi prejdeme vsetky hodnoty parametru
		float value = 0;		//hodnoty parametru
		float segmentRating = 0;	//rating aktualneho segmentu
		float stepRating = 0;		//rating aktualneho kroku (aktualnej hodnoty parametru)
		float lastStepRating = 0;	//stepRating v predchadzajucom kroku
		float last2StepRating = 0;	//stepRating v pred-predchadzajucom kroku
		float bestValue = -1;		//najlepsia hodnota parametru
		float lastValue = 0;		//value v predchadzajucej iteracii
		float resultRating = float::MaxValue;	//vysledny rating
		long stepCounter = 0;					//pocita, kolko krokov sme spravili
		vector<float>* bestValuesStep = new vector<float>();	//bestValues aktualneho kroku
		vector<float>* bestValuesResult = new vector<float>();	//vysledne bestValues

//--- vypocet -----------------------------------------------------------------------------------------------------

		for (int step=0; step<stepIters; step++) {
			stepCounter++;
			loader->setParam(selectedParams[actParam], value);	//nastavime parameter actParam
			bestValuesStep->clear();
			processGeneral(actParam-1, bestValuesStep);		//rekurzia
			
			//nastavime parametre, ktore sme vypocitali rekurziou
			for (int i=0; i<bestValuesStep->size(); i++) {
				loader->setParam(i, (*bestValuesStep)[i]);
			}

			loader->process(dry, effected, dataLength);			//zefektujeme zvuk

			stepRating = 0;

			for (int segment=0; segment<segmentNum; segment+=2) {	//cez (vsetky) segmenty
				
					//nacitame aktualne spracovavany segment
					for (int i = 0; i<segmentLength; i++) {
							segmentEff[i] = effected[0][(segment*segmentLength)+i] + effected[1][(segment*segmentLength)+i];
							if (!volType[actParam]) segmentWet[i] = wet[0][(segment*segmentLength)+i] + wet[1][(segment*segmentLength)+i];	//ak aktualny parameter nie je volume-type
					}

					//spocitame rating aktualneho segmentu
					if (volType[actParam]) {	//ak je parameter volume-type
						//spocitame RMS aktualneho segmentu efektovaneho signalu
						calculateRms(segmentEff, segmentLength, segmentRmsEff);
						//spocitame RMS rating
						segmentRating = getRatingRms(segmentRmsWet[segment], segmentRmsEff, segmentRmsLength);
					}
					else {
						//spocitame obycajny rating
						segmentRating = getRating(segmentEff, segmentWet, segmentLength);
					}	

					//zvysime celkovy rating kroku
					stepRating += segmentRating;
			}

			if (actParam==numSelectedParams-1){			
				(*logFile) << stepRating << "\t" << value << "\t" << endl;
			}

			if (stepRating<resultRating && stepRating>0) {	//ak sme nasli doteraz najlepsi rating
				resultRating = stepRating;
				bestValuesResult->assign(bestValuesStep->begin(), bestValuesStep->end());	//pridame vector z rekurzie
				bestValuesResult->push_back(value);			//pridame vypocitanu hodnotu
			}

			lastValue = value;
			value+=((double)stepSize[actParam])/100;

			//uprava progress baru, iba najvyssou urovnou rekurzie
			if (actParam==numSelectedParams-1) {
				progressBar->Value = 100 * ((step+1.)/stepIters);
			}

			//----- orezavanie
			if (fastMethod) {	//ak je zvolena fast method
				if (!volType[actParam]) { //ak nie je parameter volume-type
					if ( (stepRating) <= 0.6 * lastStepRating ) { //ak je aktualny stepRating ovela mensi ako lastStepRating
						break;
					}
				}
				else {	//ak je parameter volume-type
					if ( (last2StepRating>lastStepRating) && (lastStepRating<stepRating) )
						break;
				}
			}		
			//----- /orezavanie

			last2StepRating = lastStepRating;
			lastStepRating = stepRating;
		}

		bestValues->insert(bestValues->end(), bestValuesResult->begin(), bestValuesResult->end());


		for (int i=0; i<2; i++) {
			delete [] effected[i];
		}
		delete [] effected;	
		delete [] segmentWet;
		delete [] segmentEff;
		delete bestValuesStep;
		delete bestValuesResult;
	}
}

void VstHost::processSingle(vector<float>* bestValues) {
	
	long stepIters = (long)(100 / stepSize[0]) + 1;	//pocet krokov v nastavovani parametru
	float value = 0;			//aktualna hodnota parametru
	float** effected = new float*[2];	//efektovany signal
		effected[0] = new float[dataLength];
		effected[1] = new float[dataLength];
	float rmsLength = 0;	//dlzka rms pola
	float* rmsEff = new float[dataLength];
	float* rmsWet = new float[dataLength];
	float resultRating = float::MaxValue;	//min. (najlepsi) rating
	float resultValue = float::MaxValue;	//vysledna hodnota parametru
	float lastStepRating = 0;
	float lastStepRating2 = 0;

	if (volType[0]) {	//vypocitame rms wet signalu
		rmsLength = calculateRms(wet, dataLength, rmsWet);
	}

	for (int step=0; step<stepIters; step++) {
		float stepRating;	//rating aktualneho kroku
		loader->setParam(selectedParams[0], value);
		loader->process(dry, effected, dataLength);	//spracujeme signal
			
		if (!volType[0]) {		//aktualny parameter nie je volume type
			stepRating = getRating(wet, effected, dataLength);
		}
		else {	//je volume type
			calculateRms(effected, dataLength, rmsEff);	//spocitame rms effektovaneho signalu
			stepRating = getRatingRms(rmsWet, rmsEff, rmsLength);	//spocitame rating
		}

		if (stepRating<resultRating) {
			resultRating = stepRating;	
			resultValue = value;
		}

		(*logFile) << stepRating << "\t" << value << endl;

		progressBar->Value = (int)( ((float)step/stepIters)*100 );
		value+=((double)stepSize[0])/100;

		//----- orezavanie
			if (fastMethod) {	//ak je zvolena fast method
				if (!volType[0]) { //ak nie je parameter volume-type
					if ( (stepRating) <= 0.4 * lastStepRating) { //ak je aktualny stepRating ovela mensi ako lastStepRating
						break;
					}
				}
				else {	//ak je parameter volume-type
					if ( (lastStepRating2>lastStepRating) && (lastStepRating<stepRating) )
						break;
				}
			}		
		//----- /orezavanie
		lastStepRating2 = lastStepRating;
		lastStepRating = stepRating;
	}

	//(*logFile) << "vysledok == " << resultValue << endl;

	bestValues->push_back(resultValue);

	delete [] effected[0]; delete [] effected[1]; delete [] effected;
	delete [] rmsEff;
	delete [] rmsWet;
}

void VstHost::processDelay(vector<float>* bestValues){

	
	float	stepSize = 0.05;			//krok v anstavovani parametrov
	long	intervalLength = dataLength;		//dlzka spracovavaneho itervalu v samploch

	float	value = 0.;			//aktualna hodnota parametru
	long	stepIters = (long)(1. / stepSize) + 1;	//pocet iteracii ktorymi prejdeme vsetky hodnoty parametru
	long	intervalNum = dataLength / intervalLength;	//pocet celych intervalov

	float*	intervalEff = new float[intervalLength];	//pole na ukladanie aktualne spracovavaneho intervalu efektovaneho zvuku
	float*	intervalWet = new float[intervalLength];
	float*	intervalRmsPom = new float[intervalLength];	//pomocne pole pre vypocet intervalRms[]
	float** effected = new float*[2];		//bude ukladany zefektovany signal
		for (int i=0; i<2; i++) {
			effected[i] = new float[dataLength];
		}

	float** intervalRmsWet = new float*[intervalNum];	//bude ulozene hodnoty RMS wet signalu, uz rozsekane na intervaly
	long	intervalRmsWetLength = intervalLength / (rmsWindowSize-rmsOverlap);	//dlzka jedneho podpola v intervalRmsWet
		for (int i=0; i<intervalNum; i++) {
			intervalRmsWet[i] = new float[intervalRmsWetLength];
		}

	float*	intervalRmsEff = new float[intervalRmsWetLength];	//na ukladanie hodnot RMS eff signalu pre aktualny interval


	vector<float>* results = new vector<float>();//vysledne parametre vsetkych intervalov
	float result = 0;		//uplne vysledny paramter

	pair<float,float>	pair_floatmax(float::MaxValue, float::MaxValue);	//par <index,value>
	int					params_best_size = (int)( (1. / stepSize) * (0.02) );
	vector<pair<float,float>>*	params_best = new vector<pair<float,float>>(params_best_size, pair_floatmax);	//budu ulozene najnizsie indexy s hodnotami


//--------  vypocet  ------------------------------------------------------------------------
	int paramPom = 0;
	loader->setParam(0,0.1);		//delay
	loader->setParam(1, 1);	//wet
	loader->setParam(2, 1);	//dry

	params_best->push_back(pair_floatmax);

	//vypocitame RMS wet signalu a rovno rozdelime do intervalov
	for (int interval=0; interval<intervalNum; interval++) {	//cez vsetky intervaly
		//zoberieme sample z wet signalu aktualneho intervalu
		for (int pos=0; pos<intervalLength; pos++) {	//cez cely aktualny interval
				intervalRmsPom[pos] = wet[0][(interval*intervalLength)+pos] + wet[1][(interval*intervalLength)+pos];
		}
		float x = calculateRms(intervalRmsPom, intervalLength, intervalRmsWet[interval]);	//spocitame RMS pre aktualy interval
	}

	for (int i=0; i<dataLength; i+=dataLength/100) {
		//(*logFile) << (rmsWet[0][i]+rmsWet[1][i])/2 << endl;
	}
	(*logFile) << endl << endl<<endl;

	for (int step=0; step<stepIters; step++) {	//cez vsetky hodnoty intervalu

		loader->setParam(paramPom, value);

		loader->process(dry, effected, dataLength);	//zefektujeme zvuk
		float ratingSum = 0;	//suma ratingov pre aktuane nastavenie parametru

		//spracujeme intervaly
		for (int interval=0; interval<intervalNum; interval++) {	//cez (vsetky) intervaly
			//(*logFile) << "interval: " << interval << endl;

			float rating = 0;	//rating pre aktualny interval

			//nacitame aktualne spracovavany interval
			for (int i = 0; i<intervalLength; i++) {
					intervalEff[i] = effected[0][(interval*intervalLength)+i] + effected[1][(interval*intervalLength)+i];
					intervalWet[i] = wet[0][(interval*intervalLength)+i] + wet[1][(interval*intervalLength)+i];
			}
			
			//spocitame RMS aktualneho intervalu eff signalu
			calculateRms(intervalEff, intervalLength, intervalRmsEff);

			//spocitame rating aktualneho intervalu
			//rating = getRatingRms(intervalRmsWet[interval], intervalRmsEff, intervalRmsWetLength);
			//rating = getRating(intervalEff, intervalWet, intervalLength);

			rating = getRating(intervalEff, intervalWet, intervalLength);
			ratingSum += rating;
		}
		
		(*logFile) << ratingSum << "\t" << value << endl;

		processDelay_insert(params_best, ratingSum, value);
		
		value+=stepSize;

		progressBar->Value = (int)( 100 * ((double)step/stepIters) );
		
	}

	//spocitame result
	for (int i=0; i<params_best->size(); i++) {
		if ((*params_best)[i].first<pair_floatmax.first) {
			pair_floatmax = (*params_best)[i];
		}
	}
	result = pair_floatmax.second;
	
	(*logFile) << "vysledok " << result <<endl;
	
	message = gcnew String("vysledok = ");
	bestValues->push_back(result);

	progressBar->Value = 100;

//- deleting ---------------------------------------------------------------
	delete params_best;

	for (int i=0; i<2; i++) {
		delete [] effected[i];
	}
	delete [] intervalWet;
	delete [] intervalEff;
	delete [] intervalRmsPom;
	delete [] effected;

	for (int i=0; i<intervalNum; i++) {
		delete [] intervalRmsWet[i];
	}
	delete [] intervalRmsWet;

	delete [] intervalRmsEff;
	delete results;
//- /deleting -----------------------------------------------------------
}

void VstHost::processLimiter(std::vector<float> *bestValues) {

	float* ratio = new float[dataLength];
	float tresh = 0;		//najdeny treshold

	//spocitame pomer wet / dry
	divide(wet[0], dry[0], ratio, dataLength);

	//najdeme taku poziciu, kde je pomer==1 a zaroven velkost samplu v dry na tejto pozicii je najvacsia
	float treshSample = 0;	//zatial najvacsia hodnota vhodneho samplu
	for (long i=0; i<dataLength; i++) {
		if (ratio[i]==1  && abs(dry[0][i])>treshSample) {
			treshSample = abs(dry[0][i]);
		}
	}

	//zapiseme
	bestValues->push_back(treshSample);
	
	//spocitame treshold v dB
	//tresh = 20. * log10((double)treshSample);
	//String^ treshMessage = gcnew String("" + tresh + nl);
	//message = message->Insert(message->Length, treshMessage);

	delete ratio;
}

void VstHost::processComp(std::vector<float> *bestValues){

	int peaksPrecision = 1000;				//pocet roznych vstupnych hodnot peakov
	int peakNum = 10000;					//pocet zaznamov pre jednu vstupnu hodnotu peaku
	float** peaks = new float*[peaksPrecision];	//zoznam pomerov velkosti peakov
	for (int i=0; i<peaksPrecision; i++) {
		peaks[i] = new float[peakNum];
	}
	peaks[0][0] = 1;
	int* peaksCalculated = new int[peaksPrecision];	//pocet vypocitanych pomerov peakov jednotlivo
		memset(peaksCalculated, 0, peaksPrecision * sizeof(float));
	int peaksCalculatedSum = 0;							//celkovy pocet vypocitanych pomerov peakov
	int peaksCalculatedMax = peaksPrecision * peakNum;	//maximalna hodnota peaksCalculatedSum
	int inPeak = 0;										//umiestnenie aktualnej vstupnej velkosti peaku v zozname 'peaks'
	float* rmsDry = new float[dataLength];			//vypocitane RMS dry signalu
		memset(rmsDry, 0, dataLength);				//vypocitane RMS wet signalu
	float* rmsWet = new float[dataLength];
		memset(rmsWet, 0, dataLength);
	long rmsLength = 0;
	float* ratioPeak = new float[dataLength];			//pomer wet / dry
	float* ratioRms = new float[dataLength];			//pomer rmsWet / rmsDry
	long pos = 0;							//pomocna
	int treshNum = 0;						//pomocna
	float tresh = 0;						//treshold
	long maxAttack = sampleRate * 0.1;				//maximalny attack
	int minAttack = sampleRate * 0.001;		//minimalny attack time == rms window size
	long maxRelease = sampleRate * 0.5;		//maximalny release
	vector<float>* attackTimes = new vector<float>();	//ulozene casy attacku
	float attack = 0;						//vysledny attack
	float attackParam = 0;					//hodnota pre nastavenie parametru
	float releaseParam = 0;					//hodnota pre nastavenie parametru
	float release = 0;						//vysledny release
	int sectionNum = 4;						//pocet vypoctovych "sekcii", koli progressBaru
	float actSection = 0;					//aktualna sekcia
	float compressRatio = float::MaxValue;				//vysledny kompresny pomer

	float* params = new float[loader->getNumParams()];
	

	//--------------  vypocet

	//spocitame rmsWet aj rmsDry, ako velkost RMS okna pouzijeme minimalny attack time
	int defaultRmsWindowSize = rmsWindowSize;
	rmsWindowSize = minAttack;
	rmsLength = min(calculateRms(dry, dataLength, rmsDry), calculateRms(wet, dataLength, rmsWet) );
	rmsWindowSize = defaultRmsWindowSize;


	//spocitame pomer wet / dry
	divide(wet[0], dry[0], ratioPeak, dataLength);
	abs(ratioPeak, dataLength);
	//spocitame pomer rmsWet / rmsDry
	divide(rmsWet, rmsDry, ratioRms, rmsLength);


	//sekcia COMPRESS RATIO------------------------------
	actSection++;

	/*
	//naplnime 'peaks[]'
	pos = 0;
	while ( (pos<dataLength) && (peaksCalculatedSum < peaksCalculatedMax) ) {	//kym mame vstupne data alebo kym sme nenasli dostatok pomerov peakov
		inPeak = (int)(abs(dry[0][pos] * peaksPrecision));		//na ktorej pozicii v zozname 'peaks' sa nachadza aktualny vstupny peak
		if (inPeak > 0) {
			if (peaksCalculated[inPeak]<peakNum) {		//ak este nemam vypocitanych dost pomerov pre aktualnu velksot vstupneho peaku
				peaks[inPeak][peaksCalculated[inPeak]] = abs(wet[0][pos] / dry[0][pos]);	//vypocitame pomer
				peaksCalculated[inPeak]++;				//zvysime pocet vypocitanych pomerov pre aktualny vstupny peak
				peaksCalculatedSum++;					//zvysime celkovy pocet vypocitanych pomerov
			}
		}
		
		progressBar->Value =(int)( 50 + (50 * ((float)pos) / dataLength) );
		pos++;
	}*/


	//pomocou RMS
	pos = 0;
	while ( (pos<rmsLength) && (peaksCalculatedSum < peaksCalculatedMax) ) {	//kym mame vstupne data alebo kym sme nenasli dostatok pomerov peakov
		inPeak = (int)(rmsDry[pos] * peaksPrecision);							//na ktorej pozicii v zozname 'peaks' sa nachadza aktualny vstupny peak
		if (inPeak > 0) {
			if (peaksCalculated[inPeak]<peakNum) {								//ak este nemam vypocitanych dost pomerov pre aktualnu velksot vstupneho peaku
				peaks[inPeak][peaksCalculated[inPeak]] = ratioRms[pos];			//vypocitame pomer
				peaksCalculated[inPeak]++;										//zvysime pocet vypocitanych pomerov pre aktualny vstupny peak
				peaksCalculatedSum++;											//zvysime celkovy pocet vypocitanych pomerov
			}
		}
		progressBar->Value = (int)( (100*((actSection-1)/sectionNum)) + ((100./sectionNum)*((float)pos/rmsLength)) );
		pos++;
	}
	
	//spocitame premerne pomery v 'peaks[]' a zaroven najdeme najmensi pomer == compressRatio
	for (int i=1; i<peaksPrecision; i++) {	//cez cele vrchne pole 'peaks'
		if (peaksCalculated[i]!=0) {		//ak sme pre aktualnu hodnotu peaku vypocitali nejake pomery
			//float avg = EX(peaks[i], peaksCalculated[i]);	//spocitame priemer
			float avg = float::MaxValue;
			for (int j=0; j<peaksCalculated[i]; j++) {
				if (peaks[i][j] < avg) {
					avg = peaks[i][j];
				}
			}
			peaks[i][0] = avg;								//vysledny priemer ulozime
		}
		else {	//ak sme pre aktualnu hodnotu peaku nespocitali nijake pomery
			peaks[i][0] = peaks[i-1][0];	//doplnime hodnotu z nizsej hodnoty peaku
		}
		if (peaks[i][0]<compressRatio) {
			compressRatio = peaks[i][0];
		}
	}

	//sekcia TRESHOLD ---------------------------------------------
	actSection++;

	/*
	//pomocou peakov
	for (long i=1; i<dataLength; i++) {
		if (ratioPeak[i-1]==1 && ratioPeak[i]<1 && abs(dry[0][i-1]!=0)) {
			tresh += abs(dry[0][i-1]);
			treshNum++;
		}
	}
	tresh/=treshNum;
	*/

	
	//pomocou RMS
	tresh = 0;
	treshNum=0;
	for (long i=1; i<rmsLength; i++) {
		if (ratioRms[i-1]>(compressRatio + (1-compressRatio)/2 ) && ratioRms[i]<=(compressRatio + (1-compressRatio)/2 ) && rmsDry[i-1]!=0) {
			//if (rmsDry[i]<tresh && rmsDry[i]>0) {
				tresh+=rmsDry[i];
				treshNum++;
			//}
		}
		
		progressBar->Value = (int)( (100*((actSection-1)/sectionNum)) + ((100./sectionNum)*((float)i/rmsLength)) );

	}
	tresh/=treshNum;
	
	//prevedieme na dB
	float treshdB = 20. * log10(tresh);


	//sekcia ATTACK TIME -------------------------------------------
	actSection++;
	pos = 0;
	while (pos<rmsLength) {	//cez celu dlzku ratioRms[]
		
		//najdeme zaciatok useku, ktory je komprimovany
		while ( (ratioRms[pos]>0.96 || ratioRms[pos]==0) && pos<rmsLength) {
			pos++;
		}
		//najdeme attack time, v rozpati (pos-maxAttac, pos) najdeme prvu rms hodnotu s hodnotou vacsou a treshold
		float maxSample = 0;	//hodnota hladaneho samplu
		long  maxSamplePos = -1;	//pozicia hladaneho samplu v rmsDry[]
		for (int reverse = pos-1; reverse>(int)(pos-(maxAttack/minAttack)); reverse--) {		//cez cele rozpatie, ideme spatne, hladame v rmsDry, preto maxAttack/attack == pocet rms okien za dobu maxAttack
			if (reverse<0 || ratioRms[reverse]<=0.96 ) break;	//proti preteceniu alebo zasiahnutiu do predchadzajuceho skomprimovaneho useku
			if (rmsDry[reverse]>tresh) {
				maxSample = rmsDry[reverse];
				maxSamplePos = reverse;
			}
		}
		if (maxSamplePos!=-1) {	//ak sme nieco nasli
			attackTimes->push_back((float)( minAttack * (pos-maxSamplePos) ));	//ulozime najdeny attack time, hodnota v samploch
		}

		//posunieme sa na zaciatok dalsieho useku jednotiek
		while (ratioRms[pos]<=0.96 && pos<rmsLength) {
			pos++;
		}
		pos++;
		progressBar->Value = (int)( (100*((actSection-1)/sectionNum)) + ((100./sectionNum)*((float)pos/rmsLength)) );
	}
	
	attack = EX(attackTimes);
	attack /= sampleRate;	//attack v samploch
	int attackMs = floor( (1000 * attack) + 0.5 );	//v milisekundach
	
	//najdeme hodnotu parametru z intervalu (0..1) pre najdeny attack v milisekundach
	{
		float lastDiff = float::MaxValue;	//velkost posledneho rozdielu
		float actDiff = float::MaxValue;	//velkost aktualneho rozdielu
		float lastDisp = 0;					//posledna zobrazovana hodnota
		float value = 0;	//skutocna hodnota parametru
		float disp;		//zobrazovana hodnota
		while (value<=1){
			loader->setParam(2, value);
			disp = atof(loader->getParamDisplay(2).c_str());
			actDiff = abs(attackMs-disp);
			if (lastDiff < actDiff) {	//ak sa rozdiel zacal zvacsovat to znamena, ze sme uz hladanu hodnotu presli
				attackParam = value-0.01;
				break;
			}
			else {
				lastDiff = actDiff;
				value+=0.01;	//zvysime o 1 percento
			}
		}
	}

	//sekcia RELEASE TIME-----------------------------------------------------
	attackTimes->clear();
	actSection++;
	pos = 0;
	while (pos<rmsLength) {	//cez celu dlzku ratioRms[]
		
		//najdeme koniec useku, ktory je komprimovany
		while ( (ratioRms[pos]<=0.96 || ratioRms[pos]==0) && pos<rmsLength) {
			pos++;
		}
		//najdeme release time, v rozpati (pos-maxRelease, pos) najdeme prvu rms hodnotu s hodnotou vacsou ako treshold
		long  searchedPos = -1;	//pozicia hladaneho samplu v rmsDry[]
		for (int reverse = pos-1; reverse>(int)(pos-(maxRelease/minAttack)); reverse--) {		//cez cele rozpatie, ideme spatne, hladame v rmsDry, preto maxRelease/minAttack == pocet rms okien za dobu maxRelease
			//hladame prvu hodnotu, ktora je vacsia ako treshold
			if (reverse<0) break;	//proti preteceniu
			if (rmsDry[reverse]>tresh) {
				searchedPos = reverse;
				break;
			}
		}
		if (searchedPos!=-1) {	//ak sme nieco nasli
			attackTimes->push_back((float)( minAttack * (pos-searchedPos) ));	//ulozime najdeny release time, hodnota v samploch; pouzivame vektor z predchadzajucej sekcie
		}

		//posunieme sa na zaciatok dalsieho komprimovaneho useku, tzn. na koniec aktualneho nekomprimovaneho useku
		while ((ratioRms[pos]>0.96 || ratioRms[pos]==0) && pos<rmsLength) {
			pos++;
		}
		pos++;
		progressBar->Value = (int)( (100*((actSection-1)/sectionNum)) + ((100./sectionNum)*((float)pos/rmsLength)) );
	}
	
	release = EX(attackTimes);
	release /= sampleRate;
	int releaseMs = floor( (1000 * release) + 0.5 );
	//najdeme hodnotu parametru z intervalu (0..1) pre najdeny release v milisekundach
	{
		float lastDiff = float::MaxValue;	//velkost posledneho rozdielu
		float actDiff = float::MaxValue;	//velkost aktualneho rozdielu
		float lastDisp = 0;					//posledna zobrazovana hodnota
		float value = 0;	//skutocna hodnota parametru
		float disp;		//zobrazovana hodnota
		while (value<=1){
			loader->setParam(3, value);
			disp = atof(loader->getParamDisplay(3).c_str());
			actDiff = abs(releaseMs-disp);
			if (lastDiff < actDiff) {	//ak sa rozdiel zacal zvacsovat to znamena, ze sme uz hladanu hodnotu presli
				releaseParam = value-0.01;
				break;
			}
			else {
				lastDiff = actDiff;
				value+=0.01;	//zvysime o 1 percento
			}
		}
	}

	//---- ZAPIS VYSLEDKOV ----------------------------------------------------------------------

	
	/*
	message = message->Insert(message->Length, gcnew String("attack: " + attackMs + " ms" + nl));
	message = message->Insert(message->Length, gcnew String("release: " + releaseMs + " ms" + nl));
	message = message->Insert(message->Length, gcnew String("ratio: " + compressRatio + "" + nl));
	message = message->Insert(message->Length, gcnew String("treshold: " + treshdB + " dB" + nl));
	*/
	//zapis do logu
	(*logFile) << "attack: " << (int)(attack*1000) << endl;
	(*logFile) << "release: " << (int)(release*1000) << endl;
	(*logFile) << "ratio: " << compressRatio << endl;
	(*logFile) << "treshold: " << tresh << endl;
	for (int i=0; i<peaksPrecision; i++) {
		(*logFile) << peaksCalculated[i] << "\t" << peaks[i][0] << endl;
	}

	params[0] = 0.5;	//default input
	params[1] = 0;		//kompresor, nie limiter
	params[2] = attackParam;
	params[3] = releaseParam;
	params[4] = ((1/compressRatio) - 1 ) /9;	//inverzny vzorec z compressor.cpp
	params[5] = tresh;
	params[6] = 0;		//auto makeup off
	params[7] = 0.5;	//default output

	
	for (int i=0; i<numSelectedParams; i++) {
		bestValues->push_back(params[selectedParams[i]]);
	}

	//---  DELETOVANIE -------------------------------------------
	for (int i=0; i<peaksPrecision; i++) {
		delete [] peaks[i];
	}
	delete [] peaks;
	delete [] peaksCalculated;
	delete [] ratioPeak;
	delete [] ratioRms;
	delete [] rmsDry;
	delete [] rmsWet;
	delete attackTimes;
	delete params;
}

void VstHost::processPitch(std::vector<float> *bestValues){
	
		float fftSize2 = fftSize/2;
		int	  numStep = (100. /stepSize[0]) + 1;		//pocet iteracii ktorymi prejdeme vsetky hodnoty parametru
		float value = 0;						//aktualna hodnota parametru
		float resultValue = 0;					//vysledna hodnota parametru
		float rating = 0;						//aktualny rating
		float lastRating = 0;					//rating v predchazajucej interacii
		float lastRating2 = 0;					//ratign v pred-predchadzajucej iteracii
		float resultRating = float::MaxValue;	//vysledny rating
		long fftSegmentNum = dataLength / fftSize;		//pocet fft segmentov

		float** effected = new float*[2];		//bude ukladany zefektovany signal
		for (int i=0; i<2; i++) {
			effected[i] = new float[dataLength];
		}
		float* fftWet = new float[fftSize2];		//ulozena eq krivka wet signalu
		float* fftEff = new float[fftSize2];		//ulozena eq krivka efektovaneho signalu
		float* fftSegment = new float[2*fftSize];	//pouzivane pre ukladanie segmentu wet alebo dry signalu pre fft
		memset(fftWet, 0, (fftSize2) * sizeof(float));
		memset(fftEff, 0, (fftSize2) * sizeof(float));
		

		//--- VYPOCET -------------------------------------------------------------------------------

		(*logFile) << "processPitch" << endl;

		//zistime frekvencny obraz wet signalu
		for (int segment=0; segment<fftSegmentNum-1; segment++) { //cez vsetky fft segmenty
			memset(fftSegment, 0, 2 * fftSize * sizeof(float));
			//nacitame wet segment
			for (long i=0; i<fftSize; i++) {
				fftSegment[2*i] = wet[0][segment*fftSize + i] + wet[1][segment*fftSize + i];	//left + right
			}
			//spocitame fft segmentu
			Fft(fftSegment, fftSize, -1);
			//zvysime fftWet o vypocitane hodnoty magnitudy
			for	(long i=0; i<fftSize2; i++) {	//cez cely dlzku segmentu
				fftWet[i]+= sqrt(fftSegment[2*i]*fftSegment[2*i] + fftSegment[2*i+1]*fftSegment[2*i+1] );
			}
		}

		for	(int step = 0; step<numStep; step++) {	//cez vsetky hodnoty parametru
			
			loader->setParam(0, value);					//nastavime parameter

			float x = loader->getParam(0);

			loader->process(dry, effected, dataLength); //spracujeme

			//zistime rekvencny obraz zefektovaneho signalu
			memset(fftEff, 0, (fftSize2) * sizeof(float));
			for (int segment=0; segment<fftSegmentNum-1; segment++) { //cez vsetky fft segmenty
				memset(fftSegment, 0, 2 * fftSize * sizeof(float));
				//nacitame eff segment
				for (long i=0; i<fftSize; i++) {
					fftSegment[2*i] = effected[0][segment*fftSize + i] + effected[1][segment*fftSize + i];	//left + right
				}
				//spocitame fft segmentu
				Fft(fftSegment, fftSize, -1);
				//zvysime fftEff o vypocitane hodnoty magnitudy
				for	(long i=0; i<fftSize2; i++) {	//cez cely dlzku segmentu
					fftEff[i]+= sqrt(fftSegment[2*i]*fftSegment[2*i] + fftSegment[2*i+1]*fftSegment[2*i+1] );
					if (fftEff[0] > 1000) {
						float x = fftEff[0];
					}
				}
			}

			//spocitame rating
			rating = getRatingFft(fftWet, fftEff, fftSize2);	//porovname wet a zefektovany signal
			if (rating<resultRating) {
				resultRating = rating;
				resultValue = value;
			}

			//zapis do log
			(*logFile) << rating << "\t" << value << endl;

			//-- orezavanie -----------------------------------------
			if (fastMethod) {		//ak je zvolena fastMethod
				if (lastRating2>lastRating  && lastRating<rating) {	//ak sme presli minimom v postupnosti ratingov
					break;
				}
			}

			lastRating2 = lastRating;
			lastRating = rating;
			value+=((double)stepSize[0])/100;
			progressBar->Value = (int)(((float)step/numStep)*100);
		}

		bestValues->push_back(resultValue);


		//---- DELETOVANIE -----------------------------------------------------------------
		delete [] effected[0]; delete [] effected[1]; delete [] effected;
		delete [] fftWet;
		delete [] fftEff;
		delete [] fftSegment;
}

/*
bool VstHost::testParam(int param) {
	//nastavi vsetky parametre na 0.5, potom spocita dva zefektovane signaly, zakazdym ina hodnota zvoleneho parametru, potom porovna znormalizovane zefektovane signaly

	bool result;
	float rating;
	float avgRms1;	//priemerne RMS
	float avgRms2;

	float** eff1 = new float*[2];	//1. efektovany signal 
	float** eff2 = new float*[2];	//2. efektovany signal
	float**	effPom = new float*[2];	//pomocna
	for (int i=0; i<2; i++) {
		eff1[i] = new float[dataLength];
		eff2[i] = new float[dataLength];
		effPom[i] = new float[dataLength];
	}
	float*	rms1 = new float[dataLength];
	float*	rms2 = new float[dataLength];
	long	rmsLength;

	for (int i=0; i<numSelectedParams; i++) {	//cez vsetky zvolene parametre
		loader->setParam(selectedParams[i], 0.25);	//nastavime ich
	}

	//zefektujeme s prvou hodnotou vybraneho parametru
	loader->setParam(param, 0.0);
	loader->process(dry, effPom, dataLength);
	normalize(effPom, eff1, dataLength);

	//zefektujeme s druhou hodnotou vybraneho parametru
	loader->setParam(param, 1.0);
	loader->process(dry, effPom, dataLength);
	normalize(effPom, eff2, dataLength);

	//spocitame RMS prveho signalu
	rmsLength = calculateRms(eff1, dataLength, rms1);

	//spocitame RMS druheho signalu
	calculateRms(eff2, dataLength, rms2);

	rating = getRatingRms(rms1, rms2, rmsLength);
	avgRms1 = EX(rms1, rmsLength);
	avgRms2 = EX(rms2, rmsLength);

	(*logFile) << endl << "rating== " << rating << endl;
	(*logFile) << "avg rms1 == " << avgRms1 << endl;
	(*logFile) << "avg rms2 == " << avgRms2 << endl;

	for (int i=0; i<2; i++) {
		delete [] eff1[i];
		delete [] eff2[i];
		delete [] effPom[i];
	}
	delete [] eff1;
	delete [] eff2;
	delete [] effPom;
	delete [] rms1;
	delete [] rms2;

	return true;
}
*/

float VstHost::EX(vector<float>* v) {
	float ex = 0;
	for (int i=0; i<v->size(); i++) {
		ex+=(*v)[i];
	}
	ex/=v->size();
	return ex;
}

float VstHost::EX(float** in, long length) {
	
	float sum = 0;

	for (long i=0; i<length; i++) {
		sum+=in[0][i] + in[1][i];
	}
	
	return (sum/(2*length));
}

float VstHost::EX(float* in, long length) {
	float sum = 0;

	for (long i=0; i<length; i++) {
		sum+=in[i];
	}

	return (sum/length);
}

float VstHost::odch(vector<float>* v, float ex) {
	float o = 0;
	for (int i=0; i<v->size(); i++) {
		o+=abs((*v)[i]-ex);
	}
	o/=v->size();
	return o;
}


void VstHost::processDelay_insert(vector<pair<float,float>>*	params_best, float index, float value) {
	int max_position = 0;	//pozicia prvku s max. index-om v params_best

	//najdeme kde je prvok s max indexom
	for (int i=0; i<params_best->size(); i++) {
		if ((*params_best)[i].first > (*params_best)[max_position].first) {
			max_position = i;
		}
	}

	//skusime zapisat <index,value>
	if ((*params_best)[max_position].first > index) {
		(*params_best)[max_position].first = index;
		(*params_best)[max_position].second = value;
	}
}


void VstHost::saveProgram(String^ fileName) {
	
	if (!processed) {
		message = gcnew String("VstHost > Parameters values not processed, cannot save!");
		throw gcnew Exception();
	}
	else {

		int numParams = loader->getNumParams();
		float * params = new float[numParams];		//hodnoty parametrov

		//zistime nastavenia parametrov
		for(int i=0; i<numParams; i++) {
			params[i] = loader->getParam(i);
		}

		//z fileName vytvorime nazov presetu, prevedieme zo String^ na char*
		int pos1 = fileName->LastIndexOf("\\") + 1;
		int pos2 = fileName->LastIndexOf(".");
		String^ name = gcnew String(fileName->Substring(pos1, pos2-pos1));

		char* fileNameChar = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(fileName);
		char* nameChar	= (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(name);

		//ulozime VstProgramSaver
		VstProgramSaver(loader->getUniqueID(), loader->getFxVersion(), numParams, fileNameChar, nameChar, name->Length, params);
		message = gcnew String("VstHost > Saved to file " + fileName);

		delete [] params;
	}
}

bool VstHost::getProcessed(){
	return processed;
}

void VstHost::setProcessed(bool b) {
	processed = b;
}

void VstHost::normalize (float** in, float** out, int length) {
		float peak;
		float mul;	//multiplikator

		peak = getPeak(in, length);
		mul = 1 / peak;

		for (long j=0; j<length; j++) {
			out[0][j] = in[0][j] * mul;
			out[1][j] = in[1][j] * mul;
		}
}

float * * VstHost::normalize_mul(float **input, int length, float mul) {
	
	float * * output = new float*[2];
	output[0] = new float[length];
	output[1] = new float[length];

	for (long i=0; i<length; i++) {
		output[0][i] = input[0][i] * mul;
		output[1][i] = input[1][i] * mul;
	}

	return output;
}

float VstHost::getPeak(float * * in, long length) {
	float peak = 0;
	long pos = 0;
	for (long i=0; i<length; i++ ) {
		float maxpom = max(abs(in[0][i]), abs(in[1][i]));
		if (maxpom > peak) {
			peak = maxpom;
			pos = i;
		}
	}
	return peak;
}

float VstHost::abs(float f) {
	if (f<0) {
		return -f;
	}
	else {
		return f;
	}
}

void VstHost::subtract(float **in1, float **in2, long length, float** out){
	for (long i=0; i<length; i++) {
		out[0][i] = in1[0][i] - in2[0][i];
		out[1][i] = in1[1][i] - in2[1][i];
	}
}

void VstHost::subtract(float* in1, float* in2, long length, float* out) {
	for (long i=0; i<length; i++) {
		out[i] = in1[i] - in2[i];
	}
}

void VstHost::divide(float* in1, float* in2, float* out, long length) {
	for (long i=0; i<length; i++) {
		if (in2[i]==0) {
			out[i] = 0;
		}
		else {
			out[i] = in1[i] / in2[i];
		}
	}
}

long VstHost::calculateRms(float* in, long inLength, float* out) {
	float rms = 0;
	int overlap = rmsOverlap;
	int i_in = 0;
	int i_out = 0;

	//memset(out, 0, inLength * sizeof(float));

	for (i_in=0, i_out=0; i_in<=(inLength-rmsWindowSize); i_in+=(rmsWindowSize-overlap), i_out++){	//cez cele pole in[]
		rms = 0;
		for (int ri=0; ri<rmsWindowSize; ri++) {	//cez cele rms okno
			rms+=in[i_in] * in[i_in];
		}
		out[i_out]=sqrt(rms/rmsWindowSize);	
	}

	return i_out;
}

long VstHost::calculateRms(float** in, long inLength, float* out) {

	float* pom = new float[inLength];
	long result;

	for (long i=0; i<inLength; i++) {
		pom[i] = in[0][i] + in[1][i];
	}

	result = calculateRms(in[0], inLength, out);
	return result;
}

float VstHost::getRatingRms(float* rms1, float* rms2, long length) {
	
	float differenceSum = 0;	//sucet rozdielov rms
	
	float* difference = new float[length];		//pole pre rozdiely RMS

	//rozdiely
	subtract(rms1, rms2, length, difference);

	//scitame rozdiely
	for (int i=0; i<length; i++) {	//cez vsetky vypocitane hodnoty RMS
		differenceSum+= abs(difference[i]);			//suma rozdielov
	}

	//priemerny rozdiel rms
	differenceSum /= length;

	delete [] difference;

	return differenceSum;
}


float VstHost::getRating(float *in1, float *in2, long length){

	float mul;	//vynasobene dva sample
	float mulSum = 0;	//suma zapornych 'mul'

	for (long i=0; i<length; i++) {
			if ( (in1[i]<0 && in2[i]>0) || (in1[i]>0 && in2[i]<0) ) {
				mulSum+=in1[i] * in2[i];
			}
	}
	return abs(mulSum);
}

float VstHost::getRating(float** in1, float** in2, long length) {
	float * pom1 = new float[length];
	float * pom2 = new float[length];
	float rating = float::MaxValue;

	for (long i=0; i<length; i++) {
		pom1[i] = in1[0][i] + in1[1][i];
		pom2[i] = in2[0][i] + in2[1][i];
	}

	rating = getRating(pom1, pom2, length);

	delete [] pom1;
	delete [] pom2;

	return rating;
}

float VstHost::getRatingFft(float* fftSignal1, float* fftSignal2, long length) {

	float * subtracted = new float[length];
	float difference = 0;

	subtract(fftSignal1, fftSignal2, length, subtracted);
	for (int i=0; i<length; i++) {
		difference+=abs(subtracted[i]);
	}

	delete subtracted;	

	return difference;
}

void VstHost::abs(float *f, long length) {
	for (long i=0; i<length; i++) {
		if (f[i]<0) {
			f[i]*=-1;
		}
	}
}

bool VstHost::isPeak(int pos) {
	bool result = false;
	if (pos>0 && pos<dataLength-1) {
		if ( (abs(dry[0][pos]) > abs(dry[0][pos-1])) && (abs(dry[0][pos]) < abs(dry[0][pos+1])) ) {
			result = true;
		}
	}
	return result;
}