#include "MatrixN.h"
#include "Earlab.h"
#include <stdio.h>
#include "Logger.h"
#include "BinaryWriter.h"

class MSOXcorr
{
public:
    MSOXcorr();
    ~MSOXcorr();

    int ReadParameters(char *ParameterFileName);
    int ReadParameters(char *ParameterFileName, char *SectionName);

    int Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions]);
    int Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

	void SetModuleName(char *ModuleName);
	void SetLogger(Logger *TheLogger);

private:
	int HalfCorrelate(double ipsi[], double contra[], long n, double lag[], long shiftmax);
	double ComputeLag(double ipsi[], double contra[], long n);

	Logger *mLogger;
	char *mModuleName;
	char mCFFileName[256], mLagFileName[256];
    int mFrameSize_Samples, mNumChannels, mMaxLag_Samples, mFrameCount;
	double mSampleRate_Hz;
	double mMaxITD_uS;
	double *mIpsilateral, *mContralateral, *mLag;
	double mDeltaT;
	DoubleMatrixN mFrameBuf[2];
	BinaryWriter *mWriter;
};
