#include <iostream>
#include "Earlab.h"
#include "FIRfilterbank.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"

using namespace std;

FIRfilterbank::FIRfilterbank()
{
    memset(this, 0, sizeof(FIRfilterbank));
	SetModuleName("FIRfilterbank");
}

FIRfilterbank::~FIRfilterbank()
{
	if (mMetadataFile != NULL)
		delete mMetadataFile;
	if (mFIRfilters != NULL)
		delete [] mFIRfilters;
}

int FIRfilterbank::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	    return ReadParameters(ParameterFileName, "FIRfilterbank");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int FIRfilterbank::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, mSampleRate_Hz);
    Status = theParamFile.GetParameter(SectionName, "Gain", mGain, 10);
 	Status = theParamFile.GetParameter(SectionName, "ImpulseResponseFileName", mImpulseResponseFilename, sizeof(mImpulseResponseFilename), "");
 	//Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), "");
    return 1;
}

int FIRfilterbank::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	int i;
	//char MetadataFileName[256];

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
		throw EarlabException("%s: Input signal must be one- or two-dimensional array.  Sorry!", ModuleName());

    if (OutputSize[0][2] != 0)
		throw EarlabException("%s: Output signal must be one or two-dimensional array.  Sorry!", ModuleName());

	if (OutputSize[0][1] == 0)
	{
		mOneDimensionalOutput = true;
		OutputElementCounts[0] = OutputSize[0][0];
	    mNumChannels = 1;
	}
	else
	{
		mOneDimensionalOutput = false;
		OutputElementCounts[0] = OutputSize[0][0] * OutputSize[0][1];
	    mNumChannels = OutputSize[0][1];
	}

    mFrameSize_Samples = OutputSize[0][0];

	//sprintf(MetadataFileName, "%s.1.metadata", ModuleName());
	//mMetadataFile = new XmlMetadataFile(MetadataFileName);
	//mMetadataFile->SetSampleRate_Hz(mSampleRate_Hz);
	//mMetadataFile->Add(new XmlParameter("Units", "IFR (Spikes/S)"));
	//mMetadataFile->Add(new XmlParameter("FrameSize", mFrameSize_Samples));
	//mMetadataFile->Add(new XmlParameter("SampleRate_Hz", mSampleRate_Hz));
	//mMetadataFile->Add(new XmlDimension("Frequency (Hz)", mNumChannels, mCFFileName));
	//mMetadataFile->SetSource(new XmlSource(ModuleName(), "FIRfilterbank_0.1.0.0"));
	//mMetadataFile->Write();

	mFIRfilters = new FiniteImpulseResponse *[mNumChannels];
	for (i = 0; i < mNumChannels; i++)
		mFIRfilters[i] = new FiniteImpulseResponse(mImpulseResponseFilename);

    return 1;
}

int FIRfilterbank::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
    int i, j, counter;
	float CurOutputSample;
	FloatMatrixN *Input, *Output;

	Log("    Advance: %s", ModuleName());

	Input = ((EarlabWaveformStream *)InputStream[0])->GetData();    // Only supporting one output at the present moment
	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();  // Only supporting one output at the present moment

	if (mOneDimensionalOutput)
	{
		if (Input->Rank(0) != mFrameSize_Samples)
			throw EarlabException("%s: Input size mismatch with Start()", ModuleName());
		if (Output->Rank(0) != mFrameSize_Samples)
			throw EarlabException("%s: Output size mismatch with Start()", ModuleName());
	}
	else
	{
		if ((Input->Rank(0) != mFrameSize_Samples) || (Input->Rank(1) != mNumChannels))
			throw EarlabException("%s: Input size mismatch with Start()", ModuleName());
		if ((Output->Rank(0) != mFrameSize_Samples) || (Output->Rank(1) != mNumChannels))
			throw EarlabException("%s: Output size mismatch with Start()", ModuleName());
	}

    counter = 0;

	for (j = 0; j < mFrameSize_Samples; j++)
    {
		for (i = 0; i < mNumChannels; i++)
		{
			if (mOneDimensionalOutput)
				CurOutputSample = Input->Data(j);
			else
				CurOutputSample = Input->Data(j, i);
			CurOutputSample = (float)(mGain * mFIRfilters[i]->Filter(CurOutputSample));
			//mMetadataFile->UpdateMinMax(CurOutputSample);
			Output->Data(j, i) = CurOutputSample;
			counter++;
		}
		//mMetadataFile->AddSample();
    }
    return counter;
}

int FIRfilterbank::Stop(void)
{
	Log("    Stop: %s", ModuleName());
	//mMetadataFile->Write();
    return 1;
}

int FIRfilterbank::Unload(void)
{
	Log("    Unload: %s", ModuleName());

    return 1;
}
