#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
//#include "Rothman93Compartment.h"
//#include "Brughera96Compartment.h"
//#include "Rothman03Compartment.h"
//#include "Rothman03AHPCompartment.h"
#include "HHCompartmentFactory.h"
//#include "HHSynapse.h"
#include <time.h>
#include "EarlabModule.h"

//#define BUSHY_CELL_TYPE Rothman93Compartment
//#define BUSHY_CELL_TYPE Brughera96Compartment
//#define BUSHY_CELL_TYPE Rothman03Compartment
#define BUSHY_CELL_TYPE Rothman03AHPCompartment

//#define ENUMED_RESPONSE_SET Enumed93ResponseType
#define ENUMED_RESPONSE_SET EnumedResponseType


class ANCurrentDrivenSpikes : public EarlabModule
{
public:
    ANCurrentDrivenSpikes();
    ~ANCurrentDrivenSpikes();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:

	int FrameLength_Samples;
	int NumChannels;
	//int NumReps;
	int NumInputReps, NumOutputReps;

	double mSampleRate_Hz;
	//double mFrameLength_Seconds;
	//double mSynapticStrength;
	//double mSynapticTau;
	double mCellTemperature_DegreesC;
	double mAHPIncrement_Siemens;
	double mTauAHP_Seconds;
	//int SamplesPerFrame;


	int mResponseType;// 1=TypeI-c, 11=TypeI-t, 12=TypeI-II, 21=TypeII-I, 2=TypeII, 19=Zhou06Gmaxs
					  // 0 = CUSTOMTYPE which allows custom capacitance and Gmax values with TypeII default values
						// All types may be made adaptive by setting
						// non-zero values for mTauAHP_Seconds and mAHPIncrement_Siemens
	ENUMED_RESPONSE_SET newmResponseType;

	//BUSHY_CELL_TYPE ***ANFibers;
	HHCompartment ***ANFibers;
	char *mCompartmentClassName;

	//HHSynapse ***Synapses;
	bool **WasUnderThreshold;

	int ***SynapseList;

	//int mMaxSynapseCount;

	Rothman03CustomParameterValues mRothman03CustomParameterValues;

	char mgeFilename[256], mgiFilename[256], mgNaFilename[256], mgKAFilename[256], mgKHTFilename[256], mgKLTFilename[256], mghFilename[256], mgAHPFilename[256];
	FILE *mgeFile, *mgiFile, *mgNaFile, *mgKAFile, *mgKHTFile, *mgKLTFile, *mghFile, *mgAHPFile;
	char mVmemFilename[256];
	FILE *mVmemFile;

};
