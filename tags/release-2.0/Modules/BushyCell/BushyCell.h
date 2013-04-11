#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "HHCompartment.h"
#include "HHSynapse.h"
#include <time.h>

class BushyCell
{
public:
    BushyCell();
    ~BushyCell();

    int ReadParameters(char *ParameterFileName);
    int ReadParameters(char *ParameterFileName, char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

	void SetModuleName(char *ModuleName);
	void SetLogger(Logger *TheLogger);

private:
	Logger *mLogger;

	char *mModuleName;

	double mSampleRate_Hz;
	double mFrameLength_Seconds;
	double mSynapticStrength;
	double mSynapticTau;
	double mCellTemperature_DegreesC;
	int SamplesPerFrame;

	HHCompartment ***BushyCells;
	HHSynapse ***Synapses;
	bool **WasUnderThreshold;

	int ***SynapseList;

	int NumChannels;
	int NumInputReps, NumOutputReps;
	int mMaxSynapseCount;
};
