#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "XmlMetadata.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"
#include "FiniteImpulseResponse.h"

class FIRfilterbank : public EarlabModule
{
public:
    FIRfilterbank();
    ~FIRfilterbank();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	XmlMetadataFile *mMetadataFile;

	double mSampleRate_Hz;
	double mMinimum, mMaximum;
	double mGain;
	int mNumChannels, mFrameSize_Samples;

	bool mOneDimensionalOutput;
	FiniteImpulseResponse **mFIRfilters;
	char mCFFileName[256];
	char mImpulseResponseFilename[256];
};
