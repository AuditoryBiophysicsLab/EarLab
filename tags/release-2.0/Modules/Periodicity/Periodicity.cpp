#include <iostream>
#include "Periodicity.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "Earlab.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"
#include "Logger.h"

using namespace std;

Periodicity::Periodicity()
{
    memset(this, 0, sizeof(Periodicity));
	mLogger = new Logger();
    SetModuleName("Periodicity");
}

Periodicity::~Periodicity()
{
    if (mModuleName != NULL)
        delete [] mModuleName;
}

int Periodicity::ReadParameters(char *ParameterFileName)
{
    return ReadParameters(ParameterFileName, mModuleName);
}

int Periodicity::ReadParameters(char *ParameterFileName, char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	mLogger->Log("    ReadParameters: %s \"%s\"", mModuleName, ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0);
    Status = theParamFile.GetParameter(SectionName, "LowBestModulationFreq_Hz", mLowBMF, 20.0);
    Status = theParamFile.GetParameter(SectionName, "HighBestModulationFreq_Hz", mHighBMF, 1000.0);
    Status = theParamFile.GetParameter(SectionName, "ExcitatoryFilterOrder", mExcitatoryFilterOrder, 4);
    Status = theParamFile.GetParameter(SectionName, "ExcitatoryFilterGain", mExcitatoryGain, 1.0);
    Status = theParamFile.GetParameter(SectionName, "InhibitoryFilterOrder", mInhibitoryFilterOrder, 4);
    Status = theParamFile.GetParameter(SectionName, "InhibitoryFilterGain", mInhibitoryGain, 1.0);
    Status = theParamFile.GetParameter(SectionName, "InhibitoryToExcitatoryRatio", mInhibitoryFrequencyRatio, 0.95);
    Status = theParamFile.GetParameter(SectionName, "ExcitatoryThreshold", mExcitatoryThreshold, 1);

	return 1;
}

int Periodicity::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount])
{
	int i;

    // Because this module produces data in a "Delay-Frequency-Time" format, the following statements apply:
    // OutputSize[i][0] is the number of samples in a frame (the term "samples" includes data for all frequency channels at one time index)
    // OutputSize[i][1] is the number of frequency channels provided
    // OutputSize[i][2] is the number of delay channels provided
    // OutputSize[i][3] must be 0 to mark the end of the dimension count

	mLogger->Log("    Start: %s", mModuleName);
    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
    if (NumInputs != 1)
        throw EarlabException("%s: Currently this module can only handle one input stream.  Sorry!", mModuleName);

    if (NumOutputs != 1)
        throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", mModuleName);

	if (InputTypes[0] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform input data streams.  Sorry!", mModuleName);

	if (OutputTypes[0] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform output data streams.  Sorry!", mModuleName);

    if (InputSize[0][0] != OutputSize[0][0])
        throw EarlabException("%s: Input and output frame lengths must be identical.  Sorry!", mModuleName);

	if (InputSize[0][1] != OutputSize[0][1])
        throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", mModuleName);

	if ((InputSize[0][0] == 0) || (InputSize[0][1] == 0))
        throw EarlabException("%s: Input signal must be two-dimensional array.  Sorry!", mModuleName);

	if ((OutputSize[0][0] == 0) || (OutputSize[0][1] == 0) || (OutputSize[0][2] == 0) || (OutputSize[0][3] != 0))
        throw EarlabException("%s: Output signal must be three-dimensional array.  Sorry!", mModuleName);

	OutputElementCounts[0] = OutputSize[0][0] * OutputSize[0][1] * OutputSize[0][2];


    mFrameSize_Samples = InputSize[0][0];
    mNumChannels = InputSize[0][1];
    mNumBMFChannels = OutputSize[0][2];
	mDeltaT = 1.0 / mSampleRate_Hz;
	mBMFStep = log(mHighBMF / mLowBMF);
	mBMFArray = new double[mNumBMFChannels];
	mExcitatoryFilters = new NthOrderLowpass[mNumBMFChannels];
	mInhibitoryFilters = new NthOrderLowpass[mNumBMFChannels];

	for (i = 0; i < mNumBMFChannels; i++)
	{
		// Calculate the Best Modulation Frequency for the current delay channel
		if (mNumBMFChannels > 1)
			mBMFArray[i] = mLowBMF * 2.0 * PI * exp((mBMFStep * (double)i)/((double)(mNumBMFChannels - 1)));
		else
			mBMFArray[i] = mLowBMF * 2.0 * PI;

		// Initialize the excitatory and inhibitory filters for the current delay channel
		mExcitatoryFilters[i].SetFilterOrder(mExcitatoryFilterOrder);
		mExcitatoryFilters[i].SetSampleRate_Hz(mSampleRate_Hz);
		mExcitatoryFilters[i].SetCornerFrequency_Hz(mBMFArray[i]);

		mInhibitoryFilters[i].SetFilterOrder(mInhibitoryFilterOrder);
		mInhibitoryFilters[i].SetSampleRate_Hz(mSampleRate_Hz);
		mInhibitoryFilters[i].SetCornerFrequency_Hz(mBMFArray[i] * mInhibitoryFrequencyRatio);
	}

    return 1;
}

// Note: We assume here that Input 0 is Ipsilateral and Input 1 is Contralateral
int Periodicity::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int CurFreqChannel, CurSample, CurBMFChannel, ItemCount = 0;
	double TestThreshold = 37;
	int OutputLagNumber = 0;
	double CurDataPoint, ExcitatoryPotential, InhibitoryPotential, MembraneVoltage;
	FloatMatrixN *InputIpsi, *InputContra, *Output;

	mLogger->Log("    Advance: %s", mModuleName);

	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();
	InputIpsi = ((EarlabWaveformStream *)InputStream[0])->GetData();	
	InputContra = ((EarlabWaveformStream *)InputStream[1])->GetData();

	if ((InputIpsi->Rank(0) != mFrameSize_Samples) || (InputIpsi->Rank(1) != mNumChannels))
		throw EarlabException("%s: Input size mismatch with Start(): %d %d %d %d %d %d", mModuleName, 
			InputIpsi->Rank(0), InputIpsi->Rank(1), 
			InputContra->Rank(0), InputContra->Rank(1), 
			mFrameSize_Samples, mNumChannels);
	if ((Output->Rank(1) != mNumChannels) || (Output->Rank(2) != mNumBMFChannels))
		throw EarlabException("%s: Output size mismatch with Start()", mModuleName);

	for (CurSample = 0; CurSample < mFrameSize_Samples; CurSample++)
	{
		for (CurFreqChannel = 0; CurFreqChannel < mNumChannels; CurFreqChannel++)
		{
			//mLogger->Log("Processing channel %d of %d", CurFreqChannel + 1, mNumChannels);
			CurDataPoint = InputIpsi->Data(CurSample, CurFreqChannel);
			for (CurBMFChannel = 0; CurBMFChannel < mNumBMFChannels; CurBMFChannel++)
			{
				ExcitatoryPotential = mExcitatoryFilters[CurBMFChannel].Filter(CurDataPoint);
				InhibitoryPotential = mInhibitoryFilters[CurBMFChannel].Filter(CurDataPoint);
				MembraneVoltage = (mExcitatoryGain * ExcitatoryPotential) - (mInhibitoryGain * InhibitoryPotential);
				MembraneVoltage -= mExcitatoryThreshold;
				MembraneVoltage = MembraneVoltage > 0.0 ? MembraneVoltage : 0.0;
				Output->Data(CurSample, CurFreqChannel, CurBMFChannel) = (float)(MembraneVoltage * CurDataPoint);
				ItemCount++;
			}
		}
		OutputLagNumber++;
	}
	return ItemCount;
}

int Periodicity::Stop(void)
{
	mLogger->Log("    Stop: %s", mModuleName);
    return 1;
}

int Periodicity::Unload(void)
{
	mLogger->Log("    Unload: %s", mModuleName);
    return 1;
}

void Periodicity::SetModuleName(char *ModuleName)
{
    if (mModuleName != NULL)
        delete [] mModuleName;

    mModuleName = new char[strlen(ModuleName) + 1];
    strcpy(mModuleName, ModuleName);
}

void Periodicity::SetLogger(Logger *TheLogger)
{
	if (mLogger != NULL)
		delete mLogger;
	mLogger = TheLogger;
}
