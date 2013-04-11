#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"

class MSOQuarterWave
{
public:
    MSOQuarterWave();
    ~MSOQuarterWave();

    int ReadParameters(char *ParameterFileName);
    int ReadParameters(char *ParameterFileName, char *SectionName);

    int Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions]);
    int Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

	void SetModuleName(char *ModuleName);
	void SetLogger(Logger *TheLogger);

private:
	Logger *mLogger;
	char *mModuleName;
	char mCFFileName[256];
    int mFrameSize_Samples, mSampleRate_Hz, mNumChannels, mNumDelays_Samples;
	int mMaxLag_Samples;
	double mMaxLag_us, mDeltaT;
	double *mSampLeft, *mSampRight, *mLag;
	double *mCFArray, *mScale;
	int *mQLags;
	DoubleMatrixN mFrameBuf[2];
};
