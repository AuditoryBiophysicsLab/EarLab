#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"

class MSOQuarterWave : public EarlabModule
{
public:
    MSOQuarterWave();
    ~MSOQuarterWave();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	char mCFFileName[256];
    int mFrameSize_Samples, mSampleRate_Hz, mNumChannels, mNumDelays_Samples;
	int mMaxLag_Samples;
	double mMaxLag_us, mDeltaT;
	double *mSampLeft, *mSampRight, *mLag;
	double *mCFArray, *mScale;
	int *mQLags;
	DoubleMatrixN mFrameBuf[2];
};
