#include "MatrixN.h"
#include "Earlab.h"
#include <stdio.h>
#include "Logger.h"
#include "NthOrderLowpass.h"
#include "XmlMetadata.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"

class Periodicity : public EarlabModule
{
public:
    Periodicity();
    ~Periodicity();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
    int mFrameSize_Samples, mNumChannels;
	double mDeltaT;
	double **mFrameBuffer;
	int mNumBMFChannels;
	double mSampleRate_Hz; 
	double mLowBMF_Hz, mHighBMF_Hz, mBMFStep_Hz;
	double mInhibitoryFrequencyRatio;
	int mExcitatoryFilterOrder;
	int mInhibitoryFilterOrder;
	double mExcitatoryGain;
	double mInhibitoryGain;
	double mExcitatoryThreshold;
	double *mBMFArray, *mBMFArray_Hz;
	NthOrderLowpass *mExcitatoryFilters, *mInhibitoryFilters;
	XmlMetadataFile *mMetadataFile;
	char mCFFileName[256];
};
