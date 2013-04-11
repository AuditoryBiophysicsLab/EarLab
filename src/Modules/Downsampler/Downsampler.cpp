#include <iostream>
#include "Earlab.h"
#include "Downsampler.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"

using namespace std;

Downsampler::Downsampler()
{
    memset(this, 0, sizeof(Downsampler));
	SetModuleName("Downsampler");
}

Downsampler::~Downsampler()
{
	if (mMetadataFile != NULL)
		delete mMetadataFile;
	if (mDownsampleBuffer != NULL)
		delete [] mDownsampleBuffer;
}

int Downsampler::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	    return ReadParameters(ParameterFileName, "Downsampler");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int Downsampler::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "InputSampleRate_Hz", mInputSampleRate_Hz, mInputSampleRate_Hz);
    Status = theParamFile.GetParameter(SectionName, "OutputGain", mOutputGain, 1);
 	Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), "");
    return 1;
}

int Downsampler::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	char MetadataFileName[256];

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

    if (InputSize[0][0] == 1)
		throw EarlabException("%s: Input frame length must be greater than one.  Sorry!", ModuleName());

    if (InputSize[0][2] != 0)
		throw EarlabException("%s: Input data must be two-dimensional array.  Sorry!", ModuleName());

    if (OutputSize[0][2] != 0)
		throw EarlabException("%s: Output data must be one- or two-dimensional array.  Sorry!", ModuleName());

    if (OutputSize[0][1] != 0)
	{
		if (OutputSize[0][0] != 1)
			throw EarlabException("%s: Output signal must be one-dimensional array, or low-order dimension must be 1.  Sorry!", ModuleName());
		else
		{
			if (InputSize[0][1] != OutputSize[0][1])
				throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", ModuleName());

			OutputElementCounts[0] = OutputSize[0][1];
		    mNumChannels = OutputSize[0][1];
			nDims = 2;
		}
	}
	else
	{
		if (InputSize[0][1] != OutputSize[0][0])
			throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", ModuleName());

		OutputElementCounts[0] = OutputSize[0][0];
	    mNumChannels = OutputSize[0][0];
		nDims = 1;
	}

    mInputFrameSize_Samples = InputSize[0][0];
	mOutputSampleRate_Hz = mInputSampleRate_Hz / mInputFrameSize_Samples;
	mDownsampleBuffer = new double[mNumChannels];

	sprintf(MetadataFileName, "%s.1.metadata", ModuleName());
	mMetadataFile = new XmlMetadataFile(MetadataFileName);
	mMetadataFile->SetSampleRate_Hz(mOutputSampleRate_Hz);
	mMetadataFile->Add(new XmlParameter("Units", "IFR (Spikes/S)"));
	//mMetadataFile->Add(new XmlParameter("FrameSize", mFrameSize_Samples));
	//mMetadataFile->Add(new XmlParameter("SampleRate_Hz", mSampleRate_Hz));
	mMetadataFile->Add(new XmlDimension("Frequency (Hz)", mNumChannels, mCFFileName));
	mMetadataFile->SetSource(new XmlSource(ModuleName(), "Downsampler_0.1.0.0"));
	mMetadataFile->Write();

    return 1;
}

int Downsampler::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
    int i, j;
	float CurOutputSample;
	FloatMatrixN *Input, *Output;

	Log("    Advance: %s", ModuleName());

	Input = ((EarlabWaveformStream *)InputStream[0])->GetData();    // Only supporting one output at the present moment
	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();  // Only supporting one output at the present moment

	if ((Input->Rank(0) != mInputFrameSize_Samples) || (Input->Rank(1) != mNumChannels))
		throw EarlabException("%s: Input size mismatch with Start()", ModuleName());
	if (nDims == 1)
	{
		if (Output->Rank(0) != mNumChannels)
			throw EarlabException("%s: Output size mismatch with Start()", ModuleName());
	}
	else
	{
		if (Output->Rank(1) != mNumChannels)
			throw EarlabException("%s: Output size mismatch with Start()", ModuleName());
	}

	memset(mDownsampleBuffer, 0, sizeof(double) * mNumChannels);
	for (j = 0; j < mInputFrameSize_Samples; j++)
    {
		for (i = 0; i < mNumChannels; i++)
		{
			mDownsampleBuffer[i] += Input->Data(j, i);
		}
    }
	mMetadataFile->AddSample();
	for (i = 0; i < mNumChannels; i++)
	{
		CurOutputSample = (float)((mDownsampleBuffer[i] / mInputFrameSize_Samples) * mOutputGain);
		mMetadataFile->UpdateMinMax(CurOutputSample);
		if (nDims == 1)
			Output->Data(i) = CurOutputSample;
		else
			Output->Data(0, i) = CurOutputSample;
	}
    return mNumChannels;
}

int Downsampler::Stop(void)
{
	Log("    Stop: %s", ModuleName());
	mMetadataFile->Write();
    return 1;
}

int Downsampler::Unload(void)
{
	Log("    Unload: %s", ModuleName());

    return 1;
}

