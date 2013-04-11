#include "MatrixN.h"
#include "Earlab.h"
#include <stdio.h>
#include "Logger.h"
#include "XmlMetadata.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"

class BasilarMembraneLinearGammatone : public EarlabModule
{
public:
    BasilarMembraneLinearGammatone();
    ~BasilarMembraneLinearGammatone();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	void Initialize(void);  // Called by the constructors to perform common initialization tasks

	XmlMetadataFile *mMetadataFile;

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

	double *mCFArray_Hz;

	DoubleMatrixN mCFArray, mTauArray, mQArray;

	DoubleMatrixN mBMA, mBMB, mCFShift;

	ComplexMatrixN mShiftIn, mOldIn;
	ComplexMatrixN mBMFilter, mOldBMFilter;
	ComplexMatrixN mHold;
};
