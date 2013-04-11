#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "CarneySpikeGenerator.h"
#include "EarlabModule.h"

class SimpleSpikeGenerator : public EarlabModule
{
public:
    SimpleSpikeGenerator();
    ~SimpleSpikeGenerator();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	int FrameLength_Samples;
	int NumChannels;
	int NumReps;

	double mSampleRate_Hz;
    double c0;		// s0 weighting factor [ Carney default c0 = 0.55] 
    double c1;		// s1 weighting factor [ Carney default c1 = 0.45] 
    double s0;		// Time constant ms [ Carney default s0 = 0.8] 
    double s1;		// Time constant ms [ Carney default s1 = 25] 
    double Ra;		// Absolute refractory period ms  [ Carney default Ra = 0.75] 
    double Scale;	// Scaling factor used to multiply the firing rate sample step
    double Offset;	// Base probability of given cell producing a spike for each sample interval

	CarneySpikeGenerator **Cells;
};
