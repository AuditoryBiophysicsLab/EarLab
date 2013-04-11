#include <iostream>
#include "Earlab.h"
#include "CIS.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"

using namespace std;

CIS::CIS()
{
    memset(this, 0, sizeof(CIS));
	SetModuleName("CIS");
}

CIS::~CIS()
{
	if (mMetadataFile != NULL)
		delete mMetadataFile;
	if (CIS_Compressors != NULL)
		delete [] CIS_Compressors;
	if (CIS_Pulsetrains != NULL)
		delete [] CIS_Pulsetrains;
}

int CIS::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	    return ReadParameters(ParameterFileName, "CIS");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int CIS::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, mSampleRate_Hz);
    Status = theParamFile.GetParameter(SectionName, "StimulationRate_Hz", mStimulationRate_Hz, 10000.0);
    Status = theParamFile.GetParameter(SectionName, "OutputGain", mGain, 10);
 	Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), "");
    return 1;
}

int CIS::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	int i;
	char MetadataFileName[256];
	double InitialPulseLag_Seconds, PulseTimePerChannel;

	Log("    Start: %s", ModuleName());

    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
    if (NumInputs != 1)
	throw EarlabException("%s: Currently this module can only handle one input stream.  Sorry!", ModuleName());

    if (NumOutputs != 1)
	throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", ModuleName());

	if (InputTypes[0] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform input data streams.  Sorry!", ModuleName());

	if (OutputTypes[0] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform output data streams.  Sorry!", ModuleName());

    if (InputSize[0][0] != OutputSize[0][0])
	throw EarlabException("%s: Input and output frame lengths must be identical.  Sorry!", ModuleName());

    if (InputSize[0][1] != OutputSize[0][1])
	throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", ModuleName());

    if (InputSize[0][2] != 0)
	throw EarlabException("%s: Input data must be two-dimensional array.  Sorry!", ModuleName());

    if (OutputSize[0][2] != 0)
	throw EarlabException("%s: Output signal must be two-dimensional array.  Sorry!", ModuleName());

	OutputElementCounts[0] = OutputSize[0][0] * OutputSize[0][1];

    mFrameSize_Samples = OutputSize[0][0];
    mNumChannels = OutputSize[0][1];

	sprintf(MetadataFileName, "%s.1.metadata", ModuleName());
	mMetadataFile = new XmlMetadataFile(MetadataFileName);
	mMetadataFile->SetSampleRate_Hz(mSampleRate_Hz);
	mMetadataFile->Add(new XmlParameter("Units", "IFR (Spikes/S)"));
	//mMetadataFile->Add(new XmlParameter("FrameSize", mFrameSize_Samples));
	//mMetadataFile->Add(new XmlParameter("SampleRate_Hz", mSampleRate_Hz));
	mMetadataFile->Add(new XmlDimension("Frequency (Hz)", mNumChannels, mCFFileName));
	mMetadataFile->SetSource(new XmlSource(ModuleName(), "CIS_0.1.0.0"));
	mMetadataFile->Write();

	mPulseRatePerChannel_Hz = mStimulationRate_Hz / mNumChannels;
	Log("Each channel will pulse every %f seconds", 1.0 / mPulseRatePerChannel_Hz);
	PulseTimePerChannel = 1.0 / mPulseRatePerChannel_Hz;
	CIS_Compressors = new CIS_Compressor [mNumChannels];
	CIS_Pulsetrains = new CIS_Pulsetrain [mNumChannels];
	for (i = 0; i < mNumChannels; i++)
	{
		InitialPulseLag_Seconds = (PulseTimePerChannel / mNumChannels) * i;
		Log("Channel %d will delay %f sec before first pulse", i, InitialPulseLag_Seconds);
		CIS_Pulsetrains[i].Init(mSampleRate_Hz, mPulseRatePerChannel_Hz, InitialPulseLag_Seconds);
	}

    return 1;
}

int CIS::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
    int i, j, counter;
	float CurOutputSample;
	FloatMatrixN *Input, *Output;

	Log("    Advance: %s", ModuleName());

	Input = ((EarlabWaveformStream *)InputStream[0])->GetData();    // Only supporting one output at the present moment
	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();  // Only supporting one output at the present moment

	if ((Input->Rank(0) != mFrameSize_Samples) || (Input->Rank(1) != mNumChannels))
		throw EarlabException("%s: Input size mismatch with Start()", ModuleName());
	if ((Output->Rank(0) != mFrameSize_Samples) || (Output->Rank(1) != mNumChannels))
		throw EarlabException("%s: Output size mismatch with Start()", ModuleName());

    counter = 0;

	for (j = 0; j < mFrameSize_Samples; j++)
    {
		for (i = 0; i < mNumChannels; i++)
		{
			//CurOutputSample = (float)(mGain * CIS_Pulsetrains[i].Step(i) * CIS_Compressors[i].Step(Input->Data(j, i)));
			CurOutputSample = (float)(mGain * CIS_Pulsetrains[i].Step(i) * Input->Data(j, i));
			mMetadataFile->UpdateMinMax(CurOutputSample);
			Output->Data(j, i) = CurOutputSample;
			counter++;
		}
		mMetadataFile->AddSample();
    }
    return counter;
}

int CIS::Stop(void)
{
	Log("    Stop: %s", ModuleName());
	mMetadataFile->Write();
    return 1;
}

int CIS::Unload(void)
{
	Log("    Unload: %s", ModuleName());

    return 1;
}

CIS_Compressor::CIS_Compressor()
{
	p = 0.35;
	MaxCurrentLevel_uAmps = 2000e-6;
	ThresholdLevel_uAmps  =  100e-6;
	MinEnv = 0.00;
	MaxEnv = 0.01;
	A = (MaxCurrentLevel_uAmps - ThresholdLevel_uAmps) / (MaxEnv - MinEnv);
	B = ThresholdLevel_uAmps - (A * pow(MinEnv, p));
	Denominator = (A * pow(6.0e-3, p)) + B;
}

float CIS_Compressor::Step(float InputValue)
{
	double Output, Input;

	Input = InputValue;
	Output = MaxCurrentLevel_uAmps * ((A * pow(Input, p) + B) / Denominator);
	return (float)Output;
}

CIS_Pulsetrain::CIS_Pulsetrain(double SampleRate_Hz, double PulseWidth_Seconds, double PulseRate_Hz, double FirstPulseTime_Seconds)
{
	Init(SampleRate_Hz, PulseWidth_Seconds, PulseRate_Hz, FirstPulseTime_Seconds);
}

CIS_Pulsetrain::CIS_Pulsetrain(double SampleRate_Hz, double PulseRate_Hz, double FirstPulseTime_Seconds)
{
	Init(SampleRate_Hz, 50e-6, PulseRate_Hz, FirstPulseTime_Seconds);
}

float CIS_Pulsetrain::Step(int i)
{
	float RetVal;

	switch (CurState)
	{
	case 0:
		RetVal = 0;
		break;
	case 1:
		//printf("CIS: Channel %d pulse UP\n", i);
		//fflush(stdout);
		RetVal = 1;
		break;
	case 2:
		//printf("CIS: Channel %d pulse DOWN\n", i);
		//fflush(stdout);
		RetVal = -1;
		break;
	default:
		throw EarlabException("CIS_Pulstrain::Step: Invalid state value: %d", CurState);
		break;
	}
	StateDuration_Samples--;
	if (StateDuration_Samples <= 0)
	{
		CurState = (CurState + 1) % 3;
		if (CurState == 0)
			StateDuration_Samples = InterPulseInterval_Samples;
		else
			StateDuration_Samples = HalfPulseWidth_Samples;
	}
	return RetVal;
}

void CIS_Pulsetrain::Init(double SampleRate_Hz, double PulseRate_Hz, double FirstPulseTime_Seconds)
{
	Init(SampleRate_Hz, 50e-6, PulseRate_Hz, FirstPulseTime_Seconds);
}

void CIS_Pulsetrain::Init(double SampleRate_Hz, double PulseWidth_Seconds, double PulseRate_Hz, double FirstPulseTime_Seconds)
{
	if (FirstPulseTime_Seconds < 0.0)
		throw EarlabException("CIS_Pulstrain::Init: Can't pass first pulse time as a negative number, you sent: %f", FirstPulseTime_Seconds);
	if (PulseRate_Hz > (1.0 / PulseWidth_Seconds))
		throw EarlabException("CIS_Pulstrain::Init: Requested pulse rate, in hertz (%f) can't specify a shorter period than the pulse width, in seconds (%f)", PulseRate_Hz, PulseWidth_Seconds);
		
	SamplePeriod_Seconds = 1.0 / SampleRate_Hz;
	HalfPulseWidth_Samples = (int)(((PulseWidth_Seconds * SampleRate_Hz) / 2) + 0.5);
	InterPulseInterval_Samples = (int)((1.0 / PulseRate_Hz) * SampleRate_Hz) - (2 * HalfPulseWidth_Samples);
	if (HalfPulseWidth_Samples == 0)
		HalfPulseWidth_Samples = 1;
	if (FirstPulseTime_Seconds == 0.0)
	{
		mDebug = true;
		CurState = 1;
		StateDuration_Samples = HalfPulseWidth_Samples;
	}
	else
	{
		CurState = 0;
		StateDuration_Samples = (int)((FirstPulseTime_Seconds * SampleRate_Hz) + 0.5);
	}
}

