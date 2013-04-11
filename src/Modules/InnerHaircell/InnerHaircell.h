#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "XmlMetadata.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"

class IHC
{
public:
	IHC(double SampleRate_Hz, double Gain, double Tau);
	float Step(float Displacement_nM);
private:
	double mSampleRate_Hz, mGain;
	double ga0;
	double mGA, mOldGA[4];
	double mIHCFilter[4], mOldIHCFilter[4];
	double ihca,ihcb;   
	double x0,x1;
	double sx0,sx1;
	double state_0;
};

class InnerHaircell : public EarlabModule
{
public:
    InnerHaircell();
    ~InnerHaircell();

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
	int mNumChannels, mFrameSize_Samples, mSampleCount;
	double mGain, mTau;

	XmlMetadataFile *mMetadataFile;

	IHC **IHCs;
	char mCFFileName[256];
};
