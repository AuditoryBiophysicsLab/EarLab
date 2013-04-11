#include "MatrixN.h"
#include "Earlab.h"
#include "FirstOrderHighpass.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"

class AudioMixer : public EarlabModule
{
public:
    AudioMixer();
    ~AudioMixer();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	double InputScaleFactorArray[EarlabMaxIOStreamCount];
	int DimSize[EarlabMaxIOStreamCount];
	int mNumInputs;
	bool mScaleFactorsSpecified;
	int mNumSamples;
};
