#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "FirstOrderLowpass.h"
#include "EarlabModule.h"
#include "TimerQueue.h"
#include "Random.h"

class IntegrateAndFireNeuron
{
public:
	IntegrateAndFireNeuron();
	IntegrateAndFireNeuron(double SampleRate_Hz, double FilterTimeConstant_Seconds, double RestingPotential_Volts, double AbsoluteRefractoryPeriod_Seconds, double RelativeRefractoryPeriod_Sec, double Threshold_Volts, double MaxThreshold_Volts);
	~IntegrateAndFireNeuron();
	void Initialize(double SampleRate_Hz, double FilterTimeConstant_Seconds, double RestingPotential_Volts, double AbsoluteRefractoryPeriod_Seconds, double RelativeRefractoryPeriod_Sec, double Threshold_Volts, double MaxThreshold_Volts);
	bool Step(float MembranePotential_Volts);
private:
	bool Initialized;
	double RestingPotential_Volts;
	double Threshold_Volts;
	double ThresholdBoost_Volts;
	double CurThresholdBoost_Volts;
	double ThresholdReductionFactor;
	double AbsoluteRefractoryPeriod_Seconds;
	bool InAbsoluteRefractoryPeriod;
	TimerQueue *AbsoluteRefractoryTimerQueue;
	FirstOrderLowpass *Integrator;
};

class IntegrateAndFire : public EarlabModule
{
public:
    IntegrateAndFire();
    ~IntegrateAndFire();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	int FrameLength_Samples;
	int NumChannels;

	double mSampleRate_Hz;
	double mFilterTimeConstant_Seconds;
	double mThresholdVoltage_V, mMaxThresholdVoltage_V;
	double mRestingPotential_V;
	double mInputGain, mNoiseGain;
	double mAbsoluteRefractoryPeriod_Seconds;
	double mRelativeRefractoryPeriod_Seconds;
	Random random;
	IntegrateAndFireNeuron *Neurons;
};
