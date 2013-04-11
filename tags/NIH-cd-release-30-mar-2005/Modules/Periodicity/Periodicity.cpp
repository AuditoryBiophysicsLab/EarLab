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
    SetModuleName("Periodicity");
}

Periodicity::~Periodicity()
{
	int i;

	if (mMetadataFile != NULL)
		delete mMetadataFile;
	for (i = 0; i < mNumChannels; i++)
		delete mFrameBuffer[i];
	if (mBMFArray != NULL)
		delete [] mBMFArray;
	if (mBMFArray_Hz != NULL)
		delete [] mBMFArray_Hz;
	delete mFrameBuffer;
}

int Periodicity::ReadParameters(const char *ParameterFileName)
{
    return ReadParameters(ParameterFileName, ModuleName());
}

int Periodicity::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0);
    Status = theParamFile.GetParameter(SectionName, "LowBestModulationFreq_Hz", mLowBMF_Hz, 20.0);
    Status = theParamFile.GetParameter(SectionName, "HighBestModulationFreq_Hz", mHighBMF_Hz, 1000.0);
    Status = theParamFile.GetParameter(SectionName, "ExcitatoryFilterOrder", mExcitatoryFilterOrder, 4);
    Status = theParamFile.GetParameter(SectionName, "ExcitatoryFilterGain", mExcitatoryGain, 1.0);
    Status = theParamFile.GetParameter(SectionName, "InhibitoryFilterOrder", mInhibitoryFilterOrder, 4);
    Status = theParamFile.GetParameter(SectionName, "InhibitoryFilterGain", mInhibitoryGain, 1.0);
    Status = theParamFile.GetParameter(SectionName, "InhibitoryToExcitatoryFrequencyRatio", mInhibitoryFrequencyRatio, 0.95);
    Status = theParamFile.GetParameter(SectionName, "ExcitatoryThreshold", mExcitatoryThreshold, 1);
 	Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), "");

	return 1;
}

int Periodicity::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount])
{
	int i;
	char MetadataFileName[256];

    // Because this module produces data in a "Delay-Frequency-Time" format, the following statements apply:
    // OutputSize[i][0] is the number of samples in a frame (the term "samples" includes data for all frequency channels at one time index)
    // OutputSize[i][1] is the number of frequency channels provided
    // OutputSize[i][2] is the number of delay channels provided
    // OutputSize[i][3] must be 0 to mark the end of the dimension count

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

    //if (InputSize[0][0] != OutputSize[0][0])
    //    throw EarlabException("%s: Input and output frame lengths must be identical.  Sorry!", ModuleName());

	if (InputSize[0][1] != OutputSize[0][1])
        throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", ModuleName());

	if ((InputSize[0][0] == 0) || (InputSize[0][1] == 0))
        throw EarlabException("%s: Input signal must be two-dimensional array.  Sorry!", ModuleName());

	if ((OutputSize[0][0] == 0) || (OutputSize[0][1] == 0) || (OutputSize[0][2] == 0) || (OutputSize[0][3] != 0))
        throw EarlabException("%s: Output signal must be three-dimensional array.  Sorry!", ModuleName());

	OutputElementCounts[0] = OutputSize[0][0] * OutputSize[0][1] * OutputSize[0][2];


    mFrameSize_Samples = InputSize[0][0];
    mNumChannels = InputSize[0][1];
    mNumBMFChannels = OutputSize[0][2];
	mDeltaT = 1.0 / mSampleRate_Hz;
	mBMFStep_Hz = log(mHighBMF_Hz / mLowBMF_Hz);
	mBMFArray = new double[mNumBMFChannels];
	mBMFArray_Hz = new double[mNumBMFChannels];
	mExcitatoryFilters = new NthOrderLowpass[mNumBMFChannels];
	mInhibitoryFilters = new NthOrderLowpass[mNumBMFChannels];

	mFrameBuffer = new double *[mNumChannels];
	for (i = 0; i < mNumChannels; i++)
		mFrameBuffer[i] = new double [mNumBMFChannels];

	for (i = 0; i < mNumBMFChannels; i++)
	{
		// Calculate the Best Modulation Frequency for the current delay channel
		if (mNumBMFChannels > 1)
			mBMFArray_Hz[i] = mLowBMF_Hz * exp((mBMFStep_Hz * (double)i)/((double)(mNumBMFChannels - 1)));
		else
			mBMFArray_Hz[i] = mLowBMF_Hz;

		mBMFArray[i] = mBMFArray_Hz[i] * 2.0 * PI;

		// Initialize the excitatory and inhibitory filters for the current delay channel
		mExcitatoryFilters[i].SetFilterOrder(mExcitatoryFilterOrder);
		mExcitatoryFilters[i].SetSampleRate_Hz(mSampleRate_Hz);
		mExcitatoryFilters[i].SetCornerFrequency_Hz(mBMFArray[i]);

		mInhibitoryFilters[i].SetFilterOrder(mInhibitoryFilterOrder);
		mInhibitoryFilters[i].SetSampleRate_Hz(mSampleRate_Hz);
		mInhibitoryFilters[i].SetCornerFrequency_Hz(mBMFArray[i] * mInhibitoryFrequencyRatio);
	}


	sprintf(MetadataFileName, "%s.1.metadata", ModuleName());
	mMetadataFile = new XmlMetadataFile(MetadataFileName);
	mMetadataFile->SetSampleRate_Hz(mSampleRate_Hz);
	mMetadataFile->Add(new XmlParameter("Units", "IFR (Spikes/S)"));
	//mMetadataFile->Add(new XmlParameter("LagMin_uS", mLowBMF_Hz));
	//mMetadataFile->Add(new XmlParameter("LagMax_uS", mHighBMF_Hz));
	//mMetadataFile->Add(new XmlParameter("LagStep_uS", mBMFStep_Hz));
	//mMetadataFile->Add(new XmlParameter("SampleRate_Hz", mSampleRate_Hz / mFrameSize_Samples));
	mMetadataFile->Add(new XmlDimension("Frequency (Hz)", mNumChannels, mCFFileName));
	mMetadataFile->Add(new XmlDimension("Best Modulation Frequency (Hz)", mNumBMFChannels, mBMFArray_Hz));
	mMetadataFile->SetSource(new XmlSource(ModuleName(), "Periodicity_0.1.0.0"));
	mMetadataFile->Write();
    return 1;
}

// Note: We assume here that Input 0 is Ipsilateral and Input 1 is Contralateral
int Periodicity::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int CurFreqChannel, CurSample, CurBMFChannel, ItemCount = 0;
	double Average;
	int OutputLagNumber = 0;
	double CurDataPoint, ExcitatoryPotential, InhibitoryPotential, MembraneVoltage;
	FloatMatrixN *Input, *Output;

	Log("    Advance: %s", ModuleName());

	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();
	Input = ((EarlabWaveformStream *)InputStream[0])->GetData();	

	if ((Input->Rank(0) != mFrameSize_Samples) || (Input->Rank(1) != mNumChannels))
		throw EarlabException("%s: Input size mismatch with Start(): %d %d %d %d", ModuleName(), 
			Input->Rank(0), Input->Rank(1), 
			mFrameSize_Samples, mNumChannels);
	if ((Output->Rank(1) != mNumChannels) || (Output->Rank(2) != mNumBMFChannels))
		throw EarlabException("%s: Output size mismatch with Start()", ModuleName());

	for (CurSample = 0; CurSample < mFrameSize_Samples; CurSample++)
	{
		for (CurFreqChannel = 0; CurFreqChannel < mNumChannels; CurFreqChannel++)
		{
			//Log("Processing channel %d of %d", CurFreqChannel + 1, mNumChannels);
			CurDataPoint = Input->Data(CurSample, CurFreqChannel);
			for (CurBMFChannel = 0; CurBMFChannel < mNumBMFChannels; CurBMFChannel++)
			{
				if (CurSample == 0)
					mFrameBuffer[CurFreqChannel][CurBMFChannel] = 0;
				ExcitatoryPotential = mExcitatoryFilters[CurBMFChannel].Filter(CurDataPoint);
				InhibitoryPotential = mInhibitoryFilters[CurBMFChannel].Filter(CurDataPoint);
				MembraneVoltage = (mExcitatoryGain * ExcitatoryPotential) - (mInhibitoryGain * InhibitoryPotential);
				MembraneVoltage -= mExcitatoryThreshold;
				MembraneVoltage = MembraneVoltage > 0.0 ? MembraneVoltage : 0.0;
				mFrameBuffer[CurFreqChannel][CurBMFChannel] += (float)(MembraneVoltage * CurDataPoint);
			}
		}
		OutputLagNumber++;
	}
	for (CurFreqChannel = 0; CurFreqChannel < mNumChannels; CurFreqChannel++)
	{
		for (CurBMFChannel = 0; CurBMFChannel < mNumBMFChannels; CurBMFChannel++)
		{
			Average = mFrameBuffer[CurFreqChannel][CurBMFChannel] / mFrameSize_Samples;
			mMetadataFile->UpdateMinMax(Average);
			Output->Data(0, CurFreqChannel, CurBMFChannel) = (float)Average;
			ItemCount++;
		}
	}
	mMetadataFile->AddSample();
	return ItemCount;
}

int Periodicity::Stop(void)
{
	Log("    Stop: %s", ModuleName());
	mMetadataFile->Write();
    return 1;
}

int Periodicity::Unload(void)
{
	Log("    Unload: %s", ModuleName());
    return 1;
}
