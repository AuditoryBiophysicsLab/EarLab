#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "BinaryWriter.h"
#include "EarlabDataStream.h"

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

class AuditoryNerveIFR
{
public:
    AuditoryNerveIFR();
    ~AuditoryNerveIFR();

    int ReadParameters(char *ParameterFileName);
    int ReadParameters(char *ParameterFileName, char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

	void SetModuleName(char *ModuleName);
	void SetLogger(Logger *TheLogger);

private:
	Logger *mLogger;
	char *mModuleName;
	BinaryWriter *mWriter;

	double mSampleRate_Hz;
	int mNumChannels, mFrameSize_Samples, mSampleCount;

	AuditoryNerve **ANFibers;
};
