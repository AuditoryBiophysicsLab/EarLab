#include "MatrixN.h"
#include "Earlab.h"
#include <stdio.h>
#include "LopezPovedaMeddisDualResonanceNonlinear.h"
#include "FirstOrderLowpass.h"
#include "Logger.h"
#include "XmlMetadata.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"

class BasilarMembraneNonlinearGammatone : public EarlabModule
{
public:
    BasilarMembraneNonlinearGammatone();
    ~BasilarMembraneNonlinearGammatone();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	void Initialize(void);  // Called by the constructors to perform common initialization tasks

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
	double InnerHaircellCouplingFrequency_Hz;
	FirstOrderLowpass *mLowpassFilter;
	//FloatMatrixN mCFArray;
	
	LopezPovedaMeddisDualResonanceNonlinear *mDRNL;
	XmlMetadataFile *mMetadataFile;

	double *mCFArray_Hz;

};
