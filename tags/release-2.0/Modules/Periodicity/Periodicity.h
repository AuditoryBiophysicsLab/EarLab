#include "MatrixN.h"
#include "Earlab.h"
#include <stdio.h>
#include "Logger.h"
#include "NthOrderLowpass.h"
#include "EarlabDataStream.h"

class Periodicity
{
public:
    Periodicity();
    ~Periodicity();

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
	char *mModuleName;
    int mFrameSize_Samples, mNumChannels;
	double mDeltaT;
	int mNumBMFChannels;
	double mSampleRate_Hz; 
	double mLowBMF, mHighBMF, mBMFStep;
	double mInhibitoryFrequencyRatio;
	int mExcitatoryFilterOrder;
	int mInhibitoryFilterOrder;
	double mExcitatoryGain;
	double mInhibitoryGain;
	double mExcitatoryThreshold;
	double *mBMFArray;
	NthOrderLowpass *mExcitatoryFilters, *mInhibitoryFilters;
};
