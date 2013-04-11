#include "MatrixN.h"
#include "Earlab.h"
#include <stdio.h>
#include "Logger.h"
#include "EarlabDataStream.h"
#include "XmlMetadata.h"
#include "EarlabModule.h"

class MSOCombiner : public EarlabModule
{
public:
    MSOCombiner();
    ~MSOCombiner();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	double mSampleRate_Hz;
	int mNumChannels, mMaxLag_Samples, mZeroLag_Index;
	double mMaxITD_uS, mLagResolution_uS;
	int mInputLagCount;
	double *mCFArray_Hz;
	double *mChannelBuffer;
	XmlMetadataFile *mMetadataFile;
	char mCFFileName[256];
};
