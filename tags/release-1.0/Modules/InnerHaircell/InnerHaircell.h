#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "BinaryWriter.h"

class InnerHaircell
{
public:
    InnerHaircell();
    ~InnerHaircell();

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
	BinaryWriter *mWriter;

	double mSampleRate_Hz;
	int mNumChannels, mFrameSize_Samples, mSampleCount;
	double ga0;
	DoubleMatrixN mGA, mOldGA;
	DoubleMatrixN mIHCFilter, mOldIHCFilter;
	double ihca,ihcb;   
	double x0,x1;
	double sx0,sx1;
	double state_0;
	double tau, mGain;
};
