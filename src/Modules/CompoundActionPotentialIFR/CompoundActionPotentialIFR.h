#include "MatrixN.h"
#include "Earlab.h"
#include "FirstOrderLowpass.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"
#include "FiniteImpulseResponse.h"

class CompoundActionPotentialIFR : public EarlabModule
{
public:
    CompoundActionPotentialIFR();
    ~CompoundActionPotentialIFR();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	char mImpulseResponseFilename[256];
	FiniteImpulseResponse *mFIRFilter;
	//FirstOrderLowpass *mLowpassFilter;
	double mLowpassCornerFrequency_Hz;
	int mLowpassFilterOrder;

	double mOldSample;
	double mDeltaT;
	double mGain;

	double mSampleRate_Hz;
	int mFrameSize_Samples;
	int mNumChannels;
};
