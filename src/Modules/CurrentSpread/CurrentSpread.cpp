#include <iostream>
#include "CurrentSpread.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "Earlab.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"
#include "FPException.h"
#include <float.h>

using namespace std;

CurrentSpread::CurrentSpread()
{
    memset(this, 0, sizeof(CurrentSpread));
	SetModuleName("CurrentSpread");
}

CurrentSpread::~CurrentSpread()
{
	if (mMetadataFile != NULL)
		delete mMetadataFile;

	for (int i = 0; i < mNumElectrodes; i++)
		delete [] CurrentSpreadMatrix[i];
	delete [] CurrentSpreadMatrix;
}

int CurrentSpread::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	   return ReadParameters(ParameterFileName, "CurrentSpread");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int CurrentSpread::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);
    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, mSampleRate_Hz);
    Status = theParamFile.GetParameter(SectionName, "CurrentSpreadMatrixFilename", mCurrentSpreadMatrixFilename, sizeof(mCurrentSpreadMatrixFilename), "");
	Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), NULL);
    return 1;
}

int CurrentSpread::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	int i, j;
	char MetadataFileName[256];
	FILE *CurrentSpreadMatrixFile;

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

	if (InputSize[0][1] == 0)
		throw EarlabException("%s: Input data must be two-dimensional array.  Sorry!", ModuleName());

	if (InputSize[0][2] != 0)
		throw EarlabException("%s: Input data must be two-dimensional array.  Sorry!", ModuleName());

	if (OutputSize[0][1] == 0)
		throw EarlabException("%s: Input data must be two-dimensional array.  Sorry!", ModuleName());

	if (OutputSize[0][2] != 0)
		throw EarlabException("%s: Output signal must be two-dimensional array.  Sorry!", ModuleName());

	OutputElementCounts[0] = OutputSize[0][0] * OutputSize[0][1];

	mFrameSize_Samples = OutputSize[0][0];
	mNumElectrodes = InputSize[0][1];
	mNumChannels = OutputSize[0][1];
	mOutputBuffer = new float[mNumChannels];

	CurrentSpreadMatrix = new float *[mNumElectrodes];
	for (i = 0; i < mNumElectrodes; i++)
		CurrentSpreadMatrix[i] = new float [mNumChannels];

	Log("      About to open Matrix file: %s", mCurrentSpreadMatrixFilename);
	CurrentSpreadMatrixFile = OpenInputFile(mCurrentSpreadMatrixFilename, "r");
	if (CurrentSpreadMatrixFile == NULL)
		throw EarlabException("%s: Error opening current spread matrix file \"%s\"", ModuleName(), mCurrentSpreadMatrixFilename);

	Log("      About to read Matrix file: %s", mCurrentSpreadMatrixFilename);
	for (i = 0; i < mNumElectrodes; i++)
		for (j = 0; j < mNumChannels; j++)
			fscanf(CurrentSpreadMatrixFile, "%f", &(CurrentSpreadMatrix[i][j]));

	sprintf(MetadataFileName, "%s.1.metadata", ModuleName());
	Log("      About to open Metadata file: %s", MetadataFileName);
	mMetadataFile = new XmlMetadataFile(MetadataFileName);
	Log("        Setting sample rate: %f", mSampleRate_Hz);
	mMetadataFile->SetSampleRate_Hz(mSampleRate_Hz);
	//mMetadataFile->Add(new XmlParameter("FrameSize", mFrameSize_Samples));
	//mMetadataFile->Add(new XmlParameter("SampleRate_Hz", mSampleRate_Hz));
	mMetadataFile->Add(new XmlParameter("Units", "Membrane voltage (mV)"));
	Log("        Setting units: Membrane voltage (mV)");
	mMetadataFile->Add(new XmlDimension("Frequency (Hz)", mNumChannels, mCFFileName));
	Log("        Setting frequency dimension: %d channels from file %s", mNumChannels, mCFFileName);
	mMetadataFile->SetSource(new XmlSource(ModuleName(), "CurrentSpread_0.1.0.0"));
	Log("        Writing metadata file");
	mMetadataFile->Write();
	Log("        Finished writing metadata file");
    return 1;
}

int CurrentSpread::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int counter = 0;
	FloatMatrixN *Input;
	FloatMatrixN *Output;
	int Sample, Electrode, OutputChannel;
	float CurInput;

	Log("    Advance: %s", ModuleName());

	Input = ((EarlabWaveformStream *)InputStream[0])->GetData();	// Only supporting one input at the present moment
	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();		// Only supporting one output at the present moment

	for (Sample = 0; Sample < mFrameSize_Samples; Sample++)
	{
		memset(mOutputBuffer, 0, sizeof(float) * mNumChannels);
		for (Electrode = 0; Electrode < mNumElectrodes; Electrode++)
		{
			CurInput = Input->Data(Sample, Electrode);
			for (OutputChannel = 0; OutputChannel < mNumChannels; OutputChannel++)
			{
				mOutputBuffer[OutputChannel] += CurInput * CurrentSpreadMatrix[Electrode][OutputChannel];
			}
		} // for (Electrode)
		for (OutputChannel = 0; OutputChannel < mNumChannels; OutputChannel++)
		{
			Output->Data(Sample, OutputChannel) = mOutputBuffer[OutputChannel]; 
			mMetadataFile->UpdateMinMax(mOutputBuffer[OutputChannel]);
		}
		mMetadataFile->AddSample();
	} // for (Sample)

	return counter + 1;
}

int CurrentSpread::Stop(void)
{
	Log("    Stop: %s", ModuleName());
	mMetadataFile->Write();
    return 1;
}

int CurrentSpread::Unload(void)
{
	Log("    Unload: %s", ModuleName());
    return 1;
}

