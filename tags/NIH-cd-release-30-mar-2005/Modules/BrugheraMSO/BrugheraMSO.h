#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "Rothman93Compartment.h"
#include "HHSynapse.h"
#include <time.h>
#include "EarlabModule.h"

class BrugheraMSO : public EarlabModule
{
public:
    BrugheraMSO();
    ~BrugheraMSO();

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
	double mMaxLag_Seconds;
	int SamplesPerFrame;

	Rothman93Compartment ***MSOCells;
	HHSynapse ***Synapses;
	bool **WasUnderThreshold;
	int **SynapseList;
	SpikeStream **InputIpsi;

	int NumChannels;
	int NumInputReps, NumLags;
	int InputCellCount, OutputCellCount;

	SpikeDelayBuffer **DelayBuffers;
};