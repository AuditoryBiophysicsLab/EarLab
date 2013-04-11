#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "CarneySpikeGenerator.h"
#include "EarlabModule.h"

class SpikeToPulsetrain : public EarlabModule
{
public:
    SpikeToPulsetrain();
    ~SpikeToPulsetrain();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	XmlMetadataFile *mMetadataFile;

	int FrameLength_Samples;
	int NumChannels;
	int NumReps;

	double mSampleRate_Hz;
	double mGain;
	char mCFFileName[256];
	int mCellIDs[100];
};
