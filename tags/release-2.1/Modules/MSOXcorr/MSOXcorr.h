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
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	int HalfCorrelate(double ipsi[], double contra[], long n, double lag[], long shiftmax);
	double ComputeLag(double ipsi[], double contra[], long n);

	char mCFFileName[256], mLagFileName[256];
    int mFrameSize_Samples, mNumChannels, mMaxLag_Samples;
	double mSampleRate_Hz;
	double mMaxITD_uS;
	double *mIpsilateral, *mContralateral, *mLag;
	double mDeltaT;
	double mMinimum, mMaximum;
	DoubleMatrixN mFrameBuf[2];
	XmlMetadataFile *mMetadataFile;
};
