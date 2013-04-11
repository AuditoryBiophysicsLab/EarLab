#include "MatrixN.h"
#include "Earlab.h"
#include "FirstOrderHighpass.h"
#include "Logger.h"

class MiddleEarSimpleFilter
{
public:
    MiddleEarSimpleFilter();
    ~MiddleEarSimpleFilter();

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

	FirstOrderHighpass *mHighpassFilter;
	double mHighpassCornerFreq_Hz;
	int mHighpassFilterOrder;

	int mFrameSize_Samples;

	double FrameDuration_ms;
	double mSampleRate_Hz;
	double mGain;
	int SamplesPerFrame;
};
