#include "MatrixN.h"
#include "Earlab.h"
#include "FirstOrderLowpass.h"
#include "Logger.h"

class CompoundActionPotentialIFR
{
public:
    CompoundActionPotentialIFR();
    ~CompoundActionPotentialIFR();

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

	FirstOrderLowpass *mLowpassFilter;
	double mLowpassCornerFrequency_Hz;
	int mLowpassFilterOrder;

	double mOldSample;
	double mDeltaT;

	double mSampleRate_Hz;
	int mFrameSize_Samples;
	int mNumChannels;
};
