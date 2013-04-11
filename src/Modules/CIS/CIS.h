#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "XmlMetadata.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"

class CIS_Compressor
{
public:
	CIS_Compressor();
	float Step(float InputValue);
private:
	double p;
	double MaxCurrentLevel_uAmps;	// Maximum current level, in microAmps
	double ThresholdLevel_uAmps;	// Compression threshold, in microAmps
	double MinEnv;					// Minimum envelope output
	double MaxEnv;					// Maximum envelope output
	double A, B;
	double Denominator;
};

class CIS_Pulsetrain
{
public:
	CIS_Pulsetrain() {}
	CIS_Pulsetrain(double SampleRate_Hz, double PulseWidth_Seconds, double PulseRate_Hz, double FirstPulseTime_Seconds);
	CIS_Pulsetrain(double SampleRate_Hz, double PulseRate_Hz, double FirstPulseTime_Seconds);
	void Init(double SampleRate_Hz, double PulseRate_Hz, double FirstPulseTime_Seconds);
	void Init(double SampleRate_Hz, double PulseWidth_Seconds, double PulseRate_Hz, double FirstPulseTime_Seconds);
	float Step(int i);
private:

	bool mDebug;
	int HalfPulseWidth_Samples, InterPulseInterval_Samples, StateDuration_Samples;
	double SamplePeriod_Seconds;
	int CurState;
};

class CIS : public EarlabModule
{
public:
    CIS();
    ~CIS();

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
	double mStimulationRate_Hz;
	double mPulseRatePerChannel_Hz;
	double mGain;
	int mNumChannels, mFrameSize_Samples;

	CIS_Compressor *CIS_Compressors;
	CIS_Pulsetrain *CIS_Pulsetrains;
	char mCFFileName[256];
};

