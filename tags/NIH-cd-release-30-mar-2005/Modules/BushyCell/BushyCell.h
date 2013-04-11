#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
//#include "Rothman93Compartment.h"
#include "Brughera96Compartment.h"
#include "HHSynapse.h"
#include <time.h>
#include "EarlabModule.h"

class BushyCell : public EarlabModule
{
public:
    BushyCell();
    ~BushyCell();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount]);
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

	//Rothman93Compartment ***BushyCells;
	Brughera96Compartment ***BushyCells;
	HHSynapse ***Synapses;
	bool **WasUnderThreshold;

	int ***SynapseList;

	int NumChannels;
	int NumInputReps, NumOutputReps;
	int mMaxSynapseCount;
};
