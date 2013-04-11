#include <iostream>
#include "Earlab.h"
#include "EnvelopeExtractor.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"

using namespace std;

EnvelopeExtractor::EnvelopeExtractor()
{
    memset(this, 0, sizeof(EnvelopeExtractor));
	SetModuleName("EnvelopeExtractor");
}

EnvelopeExtractor::~EnvelopeExtractor()
{
	if (mMetadataFile != NULL)
		delete mMetadataFile;
	if (HalfWaveRectifiers != NULL)
		delete [] HalfWaveRectifiers;
	if (NthOrderLowpassFilters != NULL)
		delete [] NthOrderLowpassFilters;
	if (LogCompressors != NULL)
		delete [] LogCompressors;
}

int EnvelopeExtractor::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	    return ReadParameters(ParameterFileName, "EnvelopeExtractor");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int EnvelopeExtractor::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, mSampleRate_Hz);
    Status = theParamFile.GetParameter(SectionName, "EnvelopeFilterCornerFrequency_Hz", mEnvelopeFilterCornerFrequency_Hz, 400.0);
    Status = theParamFile.GetParameter(SectionName, "EnvelopeFilterOrder", mEnvelopeFilterOrder, 3);
    Status = theParamFile.GetParameter(SectionName, "UseCompression", mUseCompression, false);
    Status = theParamFile.GetParameter(SectionName, "CompressorA", mCompressorA, 1);
    Status = theParamFile.GetParameter(SectionName, "CompressorB", mCompressorB, 1);
 	Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), "");
    return 1;
}

int EnvelopeExtractor::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	int i;
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
	mMetadataFile->SetSource(new XmlSource(ModuleName(), "EnvelopeExtractor_0.1.0.0"));
	mMetadataFile->Write();

	HalfWaveRectifiers = new HalfWaveRectifier [mNumChannels];
	NthOrderLowpassFilters = new NthOrderLowpass [mNumChannels];
	if (mUseCompression)
		LogCompressors = new LogCompressor[mNumChannels];
	for (i = 0; i < mNumChannels; i++)
	{
		NthOrderLowpassFilters[i].SetSampleRate_Hz(mSampleRate_Hz);
		NthOrderLowpassFilters[i].SetCornerFrequency_Hz(mEnvelopeFilterCornerFrequency_Hz);
		NthOrderLowpassFilters[i].SetFilterOrder(mEnvelopeFilterOrder);
		if (mUseCompression)
		{
			LogCompressors[i].SetA(mCompressorA);
			LogCompressors[i].SetB(mCompressorB);
		}
	}

    return 1;
}

int EnvelopeExtractor::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
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
			if (mUseCompression)
				CurOutputSample = (float)LogCompressors[i].Step(NthOrderLowpassFilters[i].Filter(HalfWaveRectifiers[i].Step(Input->Data(j, i))));
			else
				CurOutputSample = (float)NthOrderLowpassFilters[i].Filter(HalfWaveRectifiers[i].Step(Input->Data(j, i)));
			mMetadataFile->UpdateMinMax(CurOutputSample);
			Output->Data(j, i) = CurOutputSample;
			counter++;
		}
		mMetadataFile->AddSample();
    }
    return counter;
}

int EnvelopeExtractor::Stop(void)
{
	Log("    Stop: %s", ModuleName());
	mMetadataFile->Write();
    return 1;
}

int EnvelopeExtractor::Unload(void)
{
	Log("    Unload: %s", ModuleName());

    return 1;
}

