#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "XmlMetadata.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"

class AuditoryNerve
{
public:
	AuditoryNerve(double SampleRate_Hz);
	float Step(float InputValue);
private:
	double mSampleRate_Hz;

	double mANB, mOldB, mNMax;
	double mR, mOldR, mANFilter[3], mOldANFilter[3];
	double mDeltaT, mTauCA;
	double ks,v0,k3,k4,kr,kp,n0,Thr_spont;
	double ka1,kb1;
	double ka2,kb2;
	double ka3,kb3;
};

class AuditoryNerveIFR : public EarlabModule
{
public:
    AuditoryNerveIFR();
    ~AuditoryNerveIFR();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	XmlMetadataFile *mMetadataFile;

	double mSampleRate_Hz;
	double mMinimum, mMaximum;
	double mGain;
	int mNumChannels, mFrameSize_Samples;

	AuditoryNerve **ANFibers;
	char mCFFileName[256];
};
