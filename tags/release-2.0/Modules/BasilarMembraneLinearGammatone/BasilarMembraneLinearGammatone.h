#include "MatrixN.h"
#include "Earlab.h"
#include <stdio.h>
#include "Logger.h"
#include "BinaryWriter.h"
#include "EarlabDataStream.h"

class BasilarMembraneLinearGammatone
{
public:
    BasilarMembraneLinearGammatone();
    ~BasilarMembraneLinearGammatone();

    int ReadParameters(char *ParameterFileName);
    int ReadParameters(char *ParameterFileName, char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

	void SetModuleName(char *ModuleName);
	void SetLogger(Logger *TheLogger);

private:
	Logger *mLogger;
	void Initialize(void);  // Called by the constructors to perform common initialization tasks

	char *mModuleName;
	int mSampleCount;
	BinaryWriter *mWriter;

	bool mProduceOutputFile, mBinaryOutput;
	char mCFFileName[256];
	FILE *mOutputFile;

	int mNumChannels, mCounter;
    int mFrameSize_Samples;

	// Greenwood frequency placemap parameters and support variables
	double mA, mAlpha, mK, mCochleaLength_mm;
	double mBMStartPercent, mBMEndPercent;
	double mCochleaStartPos_mm, mCochleaEndPos_mm;
	double mCochleaModeledLength_mm;

	double mFilterQ;

	double mSampleRate_Hz;

    double mDeltaT;

	DoubleMatrixN mCFArray, mTauArray, mQArray;

	DoubleMatrixN mBMA, mBMB, mCFShift;

	ComplexMatrixN mShiftIn, mOldIn;
	ComplexMatrixN mBMFilter, mOldBMFilter;
	ComplexMatrixN mHold;
};
