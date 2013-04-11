#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "XmlMetadata.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"
#include "NthOrderLowpass.h"
#include <math.h>

class HalfWaveRectifier
{
public:
	HalfWaveRectifier() {};
	inline float Step(float InputValue) {return InputValue < 0.0f ? 0.0f : InputValue;}
private:
};

class LogCompressor
{
public:
	LogCompressor() {mA = 1.0; mB = 1.0;}
	LogCompressor(double a, double b) {mA = a; mB = b;}
	inline void SetA(double a) {mA = a;}
	inline void SetB(double b) {mB = b;}
	inline double Step(double InputValue) 
	{
		if (InputValue > 0)
			return mB * ( log(1 + (mA * InputValue)));
		else
			return mB * (-log(1 - (mA * InputValue)));
	}
private:
	double mA, mB;
};

class EnvelopeExtractor : public EarlabModule
{
public:
    EnvelopeExtractor();
    ~EnvelopeExtractor();

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
	double mEnvelopeFilterCornerFrequency_Hz;
	int mNumChannels, mFrameSize_Samples, mEnvelopeFilterOrder;

	HalfWaveRectifier *HalfWaveRectifiers;
	NthOrderLowpass *NthOrderLowpassFilters;
	LogCompressor *LogCompressors;

	char mCFFileName[256];
	bool mUseCompression;
	double mCompressorA, mCompressorB;
};

