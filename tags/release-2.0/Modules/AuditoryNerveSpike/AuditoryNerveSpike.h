#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "BinaryWriter.h"
#include "EarlabDataStream.h"
#include "CarneySpikeGenerator.h"

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

class AuditoryNerveSpike
{
public:
    AuditoryNerveSpike();
    ~AuditoryNerveSpike();

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
	int NumChannels, mFrameSize_Samples, mSampleCount;
	int NumReps;

    double c0;		// s0 weighting factor [ Carney default c0 = 0.55] 
    double c1;		// s1 weighting factor [ Carney default c1 = 0.45] 
    double s0;		// Time constant ms [ Carney default s0 = 0.8] 
    double s1;		// Time constant ms [ Carney default s1 = 25] 
    double Ra;		// Absolute refractory period ms  [ Carney default Ra = 0.75] 
    double Scale;	// Scaling factor used to multiply the firing rate sample step
    double Offset;	// Base probability of given cell producing a spike for each sample interval

	AuditoryNerve **ANFibers;
	CarneySpikeGenerator **Cells;
};