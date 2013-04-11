#include "MatrixN.h"
#include "Earlab.h"
#include "FirstOrderHighpass.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "XmlMetadata.h"
#include "EarlabModule.h"

class MiddleEarSimpleFilter : public EarlabModule
{
public:
    MiddleEarSimpleFilter();
    ~MiddleEarSimpleFilter();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	FirstOrderHighpass *mHighpassFilter;
	double mHighpassCornerFreq_Hz;
	int mHighpassFilterOrder;

	int mFrameSize_Samples;

	double FrameDuration_ms;
	double mSampleRate_Hz;
	double mGain;
	int SamplesPerFrame;
	XmlMetadataFile *mMetadataFile;
};
