#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "XmlMetadata.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"

class VocoderChannel
{
public:
	VocoderChannel(double SampleRate_Hz, double Frequency_Hz, double Gain);
	float Step(float CurSample);
private:
	double mSampleRate_Hz, mGain, mDeltaT_Sec, mFrequency_Hz;
	int mStepCount;
};

class Vocoder : public EarlabModule
{
public:
    Vocoder();
    ~Vocoder();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	double mSampleRate_Hz;
	int mNumChannels, mFrameSize_Samples, mSampleCount, mCFCount;
	double mGain;

	float *mCFArray_Hz;
	XmlMetadataFile *mMetadataFile;

	VocoderChannel **Vocoders;
	char mCFFileName[256];
};
