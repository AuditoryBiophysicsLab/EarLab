#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"

const int MidearMaxFilterSize = 32;

class MiddleEarBilinear
{
public:
    MiddleEarBilinear();
    ~MiddleEarBilinear();

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

	void MatBilFun(double *bd, double *ad, double *b, double *a, double sampleRate);
	int GetBilinear(float SampleRate);

	int mFrameSize_Samples;

	double den[MidearMaxFilterSize];
	double a[MidearMaxFilterSize];
	double num[MidearMaxFilterSize];
	double b[MidearMaxFilterSize];
	double *z; /* the state array will be allocated at run time */
	int FilterSize;
	double FrameDuration_ms;
	double mSampleRate_Hz;
	int SamplesPerFrame;
};