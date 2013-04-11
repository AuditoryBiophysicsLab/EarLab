#include "MatrixN.h"
#include "Earlab.h"
#include <stdio.h>
#include "Logger.h"
#include "XmlMetadata.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"

class MSOXcorr : public EarlabModule
{
public:
    MSOXcorr();
    ~MSOXcorr();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	int HalfCorrelate(double ipsi[], double contra[], int n, double lag[], int shiftmax);
	double ComputeLag(double ipsi[], double contra[], int n);

	char mCFFileName[256], mLagFileName[256];
    int mFrameSize_Samples, mNumChannels, mMaxLag_Samples, mSamplesPerITDStep, mNumITDSteps;
	double mSampleRate_Hz;
	double mMaxITD_uS, mITDStepSize_uS;
	double *mIpsilateral, *mContralateral, *mLag;
	double mDeltaT;
	double mMinimum, mMaximum;
	DoubleMatrixN mFrameBuf[2];
	XmlMetadataFile *mMetadataFile;
};
