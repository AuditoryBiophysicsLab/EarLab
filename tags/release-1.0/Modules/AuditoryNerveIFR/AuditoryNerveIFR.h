#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "BinaryWriter.h"

class AuditoryNerveIFR
{
public:
    AuditoryNerveIFR();
    ~AuditoryNerveIFR();

    int ReadParameters(char *ParameterFileName);
    int ReadParameters(char *ParameterFileName, char *SectionName);

    int Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions]);
    int Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount]);
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
	double mDeltaT, mTauCA;
	double ks,v0,k3,k4,kr,kp,n0,Thr_spont;

	DoubleMatrixN mANB, mOldB, mNMax;
	DoubleMatrixN mR, mOldR, mANFilter, mOldANFilter;

	double ka1,kb1;
	double ka2,kb2;
	double ka3,kb3;
};