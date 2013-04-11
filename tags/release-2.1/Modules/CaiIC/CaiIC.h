#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "Cai98Compartment.h"
#include "HHSynapse.h"
#include <time.h>
#include "EarlabModule.h"

class CaiIC : public EarlabModule
{
public:
    CaiIC();
    ~CaiIC();

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
	double mExSynapticStrength;
	double mExSynapticTau;
	double mInhSynapticStrength;
	double mInhSynapticTau;
	double mCellTemperature_DegreesC;
	double mInhInputDelay_Seconds;
	int SamplesPerFrame;

	Cai98Compartment ***ICCells;
	HHSynapse ***ExcitatorySynapses, ***InhibitorySynapses;
	bool **WasUnderThreshold;
	// SpikeStream *TempInputContra;
	SpikeStream *InputContra;

	int NumChannels;
	int NumInputReps, NumLags;
	int InputCellCount, OutputCellCount, mExcitatoryMSOLagNumber;

	SpikeDelayBuffer *InhInputDelayBuffer;
};