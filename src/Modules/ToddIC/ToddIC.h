#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "Rothman03Compartment.h"
//#include "Rothman03AHPCompartment.h"
#include "HHSynapse.h"
#include <time.h>
#include "EarlabModule.h"
#include "ParameterStatus.h"

#define COMPARTMENT_TYPE Rothman03Compartment
//#define COMPARTMENT_TYPE Rothman03AHPCompartment


class ToddIC : public EarlabModule
{
public:
    ToddIC();
    ~ToddIC();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount], int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	double mSampleRate_Hz;
	double mFrameLength_Seconds;
	double mCellTemperature_DegreesC;

	double mSynapticStrength[EarlabMaxIOStreamCount];
	double mSynapticTau_Seconds[EarlabMaxIOStreamCount];
	double mInputDelay_Seconds[EarlabMaxIOStreamCount];
	int mSynapseFiberNumber[EarlabMaxIOStreamCount];
	int mIsExcitatory[EarlabMaxIOStreamCount];

	int mSynapticStrengthLength;
	int mSynapticTau_SecondsLength;
	int mInputDelay_SecondsLength;
	int mSynapseFiberNumberLength;
	int mIsExcitatoryLength;
	
	double mTauAHP_Seconds;
	double mAHPIncrement_Siemens;
		
	int mResponseType;// 1=TypeI-c, 11=TypeI-t, 12=TypeI-II, 21=TypeII-I, 2=TypeII, 19=Zhou06Gmaxs
						// All types may be made adaptive by setting 
						// non-zero values for mTauAHP_Seconds and mAHPIncrement_Siemens
	EnumedResponseType newmResponseType;

	int mNumChannels, mNumInputs, mExcitCount;
	int SamplesPerFrame;	

	bool *WasUnderThreshold;

	COMPARTMENT_TYPE **ICCells;
	HHSynapse ***ExcitatorySynapses, ***InhibitorySynapses;

	SpikeDelayBuffer **InputDelayBuffer;
	
	char mgeFilename[256], mgiFilename[256], mgNaFilename[256], mgKAFilename[256], mgKHTFilename[256], mgKLTFilename[256], mghFilename[256], mgAHPFilename[256];
	FILE *mgeFile, *mgiFile, *mgNaFile, *mgKAFile, *mgKHTFile, *mgKLTFile, *mghFile, *mgAHPFile;
	char mVmemFilename[256];
	FILE *mVmemFile;
	
};
