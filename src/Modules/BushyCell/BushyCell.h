#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
//#include "Rothman93Compartment.h"
//#include "Brughera96Compartment.h"
#include "HHCompartmentFactory.h"
#include "HHSynapse.h"
#include <time.h>
#include "EarlabModule.h"

//#define BUSHY_CELL_TYPE Rothman93Compartment
//#define BUSHY_CELL_TYPE Brughera96Compartment

#define ENUMED_RESPONSE_SET Enumed93ResponseType

class BushyCell : public EarlabModule
{
public:
    BushyCell();
    ~BushyCell();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	double mSampleRate_Hz;
	double mFrameLength_Seconds;
	double mSynapticStrength;
	double mSynapticTau;
	double mCellTemperature_DegreesC;
	int SamplesPerFrame;

	int mResponseType;	// 1=Type_I, 2=Type_II
						// All types may be made adaptive by setting 
						// non-zero values for mTauAHP_Seconds and mAHPIncrement_Siemens
	ENUMED_RESPONSE_SET newmResponseType;

	//BUSHY_CELL_TYPE ***BushyCells;
	HHCompartment ***BushyCells;
	char mCompartmentClassName[256];

	HHSynapse ***Synapses;
	bool **WasUnderThreshold;

	int ***SynapseList;

	int NumChannels;
	int NumInputReps, NumOutputReps;
	int mMaxSynapseCount;

	char mgeFilename[256], mgiFilename[256], mgNaFilename[256], mgKHTFilename[256], mgKLTFilename[256];
	FILE *mgeFile, *mgiFile, *mgNaFile, *mgKHTFile, *mgKLTFile;
	char mVmemFilename[256];
	FILE *mVmemFile;
};
