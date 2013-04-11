#include <iostream>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <math.h>
#include "Earlab.h"
#include "IntegrateAndFire.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "EarlabException.h"
#include "FPException.h"

using namespace std;

IntegrateAndFire::IntegrateAndFire()
{
    memset(this, 0, sizeof(IntegrateAndFire));
	SetModuleName("IntegrateAndFire");
}

IntegrateAndFire::~IntegrateAndFire()
{
	if (Neurons != NULL)
		delete [] Neurons;
}

int IntegrateAndFire::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	    return ReadParameters(ParameterFileName, "IntegrateAndFire");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int IntegrateAndFire::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0.0);
    Status = theParamFile.GetParameter(SectionName, "FilterTimeConstant_Seconds", mFilterTimeConstant_Seconds, 0.5);
    Status = theParamFile.GetParameter(SectionName, "RestingPotential_V", mRestingPotential_V, -60.0);
    Status = theParamFile.GetParameter(SectionName, "ThresholdVoltage_V", mThresholdVoltage_V, 0.0);
    Status = theParamFile.GetParameter(SectionName, "MaxThresholdVoltage_V", mMaxThresholdVoltage_V, -25.0);
    Status = theParamFile.GetParameter(SectionName, "AbsoluteRefractoryPeriod_Seconds", mAbsoluteRefractoryPeriod_Seconds, 0.0005);
    Status = theParamFile.GetParameter(SectionName, "RelativeRefractoryPeriod_Seconds", mRelativeRefractoryPeriod_Seconds, 0.001);
    Status = theParamFile.GetParameter(SectionName, "InputGain", mInputGain, 1.0);
    return 1;
}

int IntegrateAndFire::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount])
{
	double FrameLength_mS;
	int CellCount;
	int Channel;

	cout << "    Start: " << ModuleName() << endl;

    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
    if (NumInputs != 1)
        throw EarlabException("%s: Currently this module can only handle one input stream.  Sorry!", ModuleName());

    if (NumOutputs != 1)
        throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", ModuleName());

	if (InputTypes[0] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform input data streams.  Sorry!", ModuleName());

	if (OutputTypes[0] != SpikeData)
		throw EarlabException("%s: Currently this module can only handle spike output data streams.  Sorry!", ModuleName());

    if (InputSize[0][1] != OutputSize[0][0])
        throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", ModuleName());

    if (InputSize[0][2] != 0)
        throw EarlabException("%s: Input data must be two-dimensional array.  Sorry!", ModuleName());

    if (OutputSize[0][1] != 0)
        throw EarlabException("%s: Output signal must be one-dimensional array.  Sorry!", ModuleName());

	CellCount = OutputSize[0][0];

	FrameLength_Samples = InputSize[0][0];
	NumChannels = InputSize[0][1];
	FrameLength_mS = ((1.0 / mSampleRate_Hz) * FrameLength_Samples) * 1000;

	OutputElementCounts[0] = (int)(10 * CellCount * FrameLength_mS);

	Neurons = new IntegrateAndFireNeuron [NumChannels];
	for (Channel = 0; Channel < NumChannels; Channel++)
		Neurons[Channel].Initialize(mSampleRate_Hz, mFilterTimeConstant_Seconds, mRestingPotential_V, 
									mAbsoluteRefractoryPeriod_Seconds, mRelativeRefractoryPeriod_Seconds, 
									mThresholdVoltage_V, mMaxThresholdVoltage_V);

	return 1;
}

int IntegrateAndFire::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int counter = 0;
	FloatMatrixN *Input;
	SpikeStream *Output;
	int Sample, Channel;

	Log("    Advance: %s", ModuleName());

	Input = ((EarlabWaveformStream *)InputStream[0])->GetData();	// Only supporting one input at the present moment
	Output = ((EarlabSpikeStream *)OutputStream[0])->GetData();	// Only supporting one output at the present moment
	Output->SetSampleRate_Hz(mSampleRate_Hz);
	Output->NewFrame();

	for (Sample = 0; Sample < FrameLength_Samples; Sample++)
	{
		for (Channel = 0; Channel < NumChannels; Channel++)
		{
			if (Neurons[Channel].Step(Input->Data(Sample, Channel)))
			{
				Output->Fire(Channel);
				counter++;
			}
		} // for (Channel)
		Output->Step();
	} // for (Sample)

	return counter + 1;
}

int IntegrateAndFire::Stop(void)
{
	cout << "    Stop: " << ModuleName() << endl;

    return 1;
}

int IntegrateAndFire::Unload(void)
{
	cout << "    Unload: " << ModuleName() << endl;

    return 1;
}

IntegrateAndFireNeuron::IntegrateAndFireNeuron()
{
	memset(this, 0, sizeof(IntegrateAndFireNeuron));
}

IntegrateAndFireNeuron::IntegrateAndFireNeuron(double SampleRate_Hz, double FilterTimeConstant_Seconds, double RestingPotential_Volts, double AbsoluteRefractoryPeriod_Seconds, double RelativeRefractoryPeriod_Seconds, double Threshold_Volts, double MaxThreshold_Volts)
{
	memset(this, 0, sizeof(IntegrateAndFireNeuron));
	Initialize(SampleRate_Hz, FilterTimeConstant_Seconds, RestingPotential_Volts, AbsoluteRefractoryPeriod_Seconds, RelativeRefractoryPeriod_Seconds, Threshold_Volts, MaxThreshold_Volts);
}

void IntegrateAndFireNeuron::Initialize(double SampleRate_Hz, double FilterTimeConstant_Seconds, double RestingPotential_Volts, double AbsoluteRefractoryPeriod_Seconds, double RelativeRefractoryPeriod_Seconds, double Threshold_Volts, double MaxThreshold_Volts)
{
	double SamplePeriod_Seconds = 1.0 / SampleRate_Hz;

	this->RestingPotential_Volts = RestingPotential_Volts;
	this->AbsoluteRefractoryPeriod_Seconds = AbsoluteRefractoryPeriod_Seconds;
	this->Threshold_Volts = Threshold_Volts;
	this->ThresholdBoost_Volts = MaxThreshold_Volts - Threshold_Volts;
	this->InAbsoluteRefractoryPeriod = false;
	this->AbsoluteRefractoryTimerQueue = new TimerQueue(SampleRate_Hz);
	this->ThresholdReductionFactor = (1 - (SamplePeriod_Seconds / RelativeRefractoryPeriod_Seconds));
	Integrator = new FirstOrderLowpass();
	Integrator->SetSampleRate_Hz(SampleRate_Hz);
	Integrator->SetTimeConstant_mS(FilterTimeConstant_Seconds * 1000);
	Integrator->ResetFilterTo(RestingPotential_Volts);
}

IntegrateAndFireNeuron::~IntegrateAndFireNeuron()
{
	delete Integrator;
	delete AbsoluteRefractoryTimerQueue;
}

bool IntegrateAndFireNeuron::Step(float MembranePotential_Volts)
{
	bool FiredThisStep = false;
	double IntegratorOutput;

	if (CurThresholdBoost_Volts > 0)
	{
		// If we're still above our default threshold, perform the exponential reduction in the boost
		CurThresholdBoost_Volts *= ThresholdReductionFactor;
		// Check to see if the boost is very small, and if it is, drop it to zero.  This is a performance enhancement on Pentium processors
		if (IS_ALMOST_DENORMAL(CurThresholdBoost_Volts))
			CurThresholdBoost_Volts = 0;
	}

	// Filter the output through the integrator
	IntegratorOutput = Integrator->Filter(MembranePotential_Volts);

	if (!InAbsoluteRefractoryPeriod)
	{
		// If we're not in the absolute refractory period
		if (IntegratorOutput >= (Threshold_Volts + CurThresholdBoost_Volts))
		{
			FiredThisStep = true;
			Integrator->ResetFilterTo(RestingPotential_Volts);
			AbsoluteRefractoryTimerQueue->Add(new Timer(AbsoluteRefractoryPeriod_Seconds));
			InAbsoluteRefractoryPeriod = true;
		}
	}
	else
	{
		if (AbsoluteRefractoryTimerQueue->Step() != 0)
		{
			CurThresholdBoost_Volts = ThresholdBoost_Volts;
			InAbsoluteRefractoryPeriod = false;
		}
	}

	return FiredThisStep;
}
