#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "HHCompartment.h"
#include "HHSynapse.h"
#include <time.h>

class BrugheraMSO
{
public:
    BrugheraMSO();
    ~BrugheraMSO();

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
	double mMaxLag_Seconds;
	int SamplesPerFrame;

	HHCompartment ***MSOCells;
	HHSynapse ***Synapses;
	bool **WasUnderThreshold;
	int **SynapseList;
	SpikeStream **InputIpsi;

	int NumChannels;
	int NumInputReps, NumLags;
	int InputCellCount, OutputCellCount;

	SpikeDelayBuffer **DelayBuffers;
};