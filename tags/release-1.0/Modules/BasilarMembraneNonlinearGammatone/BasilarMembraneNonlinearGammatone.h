#include "MatrixN.h"
#include "Earlab.h"
#include <stdio.h>
#include "LopezPovedaMeddisDualResonanceNonlinear.h"
#include "FirstOrderLowpass.h"
#include "Logger.h"
#include "BinaryWriter.h"

class BasilarMembraneNonlinearGammatone
{
public:
    BasilarMembraneNonlinearGammatone();
    ~BasilarMembraneNonlinearGammatone();

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
	void Initialize(void);  // Called by the constructors to perform common initialization tasks

	char *mModuleName;
	
	BinaryWriter *mWriter;
	int mSampleCount;

	bool mDisableNonlinearPath;

	bool mProduceOutputFile, mBinaryOutput;
	char mCFFileName[256];
	FILE *mOutputFile;

	int mNumChannels, mCounter;
    int mFrameSize_Samples;
	double mSampleRate_Hz;

	// Greenwood frequency placemap parameters and support variables
	double mA, mAlpha, mK, mCochleaLength_mm;
	double mBMStartPercent, mBMEndPercent;
	double mCochleaStartPos_mm, mCochleaEndPos_mm;
	double mCochleaModeledLength_mm;

	double mInputScaleFactor, mOutputScaleFactor;
	FirstOrderLowpass *mLowpassFilter;
	FloatMatrixN mCFArray;
	
	LopezPovedaMeddisDualResonanceNonlinear *mDRNL;
};
