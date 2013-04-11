#include "MatrixN.h"
#include "Earlab.h"
#include <stdio.h>
#include "Logger.h"
#include "XmlMetadata.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"

class LSO : public EarlabModule
{
public:
    LSO();
    ~LSO();

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

	char mCFFileName[256], mIIDFileName[256];
    int mFrameSize_Samples, mNumChannels, mMaxLag_Samples;
	double mSampleRate_Hz;
	double MaxIID_RateDelta_SpikesSec;
	double *mIpsilateral, *mContralateral, *mLag;
	double mDeltaT;
	double mMinimum, mMaximum;
	DoubleMatrixN mFrameBuf[2];
	XmlMetadataFile *mMetadataFile;
};
