#include <iostream>
#include "MiddleEarSimpleFilter.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "Earlab.h"
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"

using namespace std;

MiddleEarSimpleFilter::MiddleEarSimpleFilter()
{
	memset(this, 0, sizeof(MiddleEarSimpleFilter));
	SetModuleName("MiddleEarSimpleFilter");
}

MiddleEarSimpleFilter::~MiddleEarSimpleFilter()
{
	if (mMetadataFile != NULL)
		delete mMetadataFile;
	if (mHighpassFilter != NULL)
		delete [] mHighpassFilter;
}

int MiddleEarSimpleFilter::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
		return ReadParameters(ParameterFileName, "MiddleEarSimpleFilter");
	else
		return ReadParameters(ParameterFileName, ModuleName());
}

int MiddleEarSimpleFilter::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
	CParameterFile theParamFile(ParameterFileName);
	ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

	// Number of channels and Frame Size are passed as parameters to Start, see that function for details
	Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0);
	Status = theParamFile.GetParameter(SectionName, "HighpassCornerFreq_Hz", mHighpassCornerFreq_Hz, 500.0);
	Status = theParamFile.GetParameter(SectionName, "HighpassFilterOrder", mHighpassFilterOrder, 2);
	Status = theParamFile.GetParameter(SectionName, "Gain", mGain, 30.0);
	return 1;
}

int MiddleEarSimpleFilter::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
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

	if (InputSize[0][1] != 0)
		throw EarlabException("%s: Input data must be one-dimensional array.  Sorry!", ModuleName());

	if (OutputSize[0][1] != 0)
		throw EarlabException("%s: Output signal must be one-dimensional array.  Sorry!", ModuleName());

	OutputElementCounts[0] = OutputSize[0][0];

	mFrameSize_Samples = OutputSize[0][0];
	mHighpassFilter = new FirstOrderHighpass[mHighpassFilterOrder];
	for (i = 0; i < mHighpassFilterOrder; i++)
	{
		mHighpassFilter[i].SetSampleRate_Hz(mSampleRate_Hz);
		mHighpassFilter[i].SetCornerFrequency_Hz(mHighpassCornerFreq_Hz);
	}
	sprintf(MetadataFileName, "%s.1.metadata", ModuleName());
	mMetadataFile = new XmlMetadataFile(MetadataFileName);
	mMetadataFile->SetSampleRate_Hz(mSampleRate_Hz);
	mMetadataFile->Add(new XmlParameter("Units", "Pressure (uPa)"));
	//mMetadataFile->Add(new XmlParameter("FrameSize", mFrameSize_Samples));
	//mMetadataFile->Add(new XmlParameter("SampleRate_Hz", mSampleRate_Hz));
	mMetadataFile->SetSource(new XmlSource(ModuleName(), "MiddleEarSimpleFilter_0.1.0.0"));
	mMetadataFile->Write();
	return 1;
}

int MiddleEarSimpleFilter::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
    int i, j;
	double CurSample;
	FloatMatrixN *Input, *Output;

	Log("    Advance: %s", ModuleName());

	Input = ((EarlabWaveformStream *)InputStream[0])->GetData();	// Only supporting one output at the present moment
	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();	// Only supporting one output at the present moment

	if (Input->Rank(0) != mFrameSize_Samples)
		throw EarlabException("%s: Input size mismatch with Start()", ModuleName());
	if (Output->Rank(0) != mFrameSize_Samples)
		throw EarlabException("%s: Output size mismatch with Start()", ModuleName());

    for (j = 0; j < mFrameSize_Samples; j++)
    {
		CurSample = Input->Data(j);
		for (i = 0; i < mHighpassFilterOrder; i++)
			CurSample = mHighpassFilter[i].Filter(CurSample);
		mMetadataFile->UpdateMinMax(CurSample);
		mMetadataFile->AddSample();
        Output->Data(j) = (float)(CurSample * mGain);
    }

    return j;
}

int MiddleEarSimpleFilter::Stop(void)
{
	Log("    Stop: %s", ModuleName());
	mMetadataFile->Write();
	return 1;
}

int MiddleEarSimpleFilter::Unload(void)
{
	Log("    Unload: %s", ModuleName());
	return 1;
}
