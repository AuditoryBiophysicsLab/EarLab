#include <iostream>
#include "SimpleIFR.h"
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

SimpleIFR::SimpleIFR()
{
    memset(this, 0, sizeof(SimpleIFR));
	SetModuleName("SimpleIFR");
}

SimpleIFR::~SimpleIFR()
{
	if (mMetadataFile != NULL)
		delete mMetadataFile;

	for (int i = 0; i < mNumElectrodes; i++)
		delete [] mCurrentSpreadMatrix[i];
	delete [] mCurrentSpreadMatrix;
}

int SimpleIFR::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	   return ReadParameters(ParameterFileName, "SimpleIFR");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int SimpleIFR::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);
    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, mSampleRate_Hz);
    Status = theParamFile.GetParameter(SectionName, "WeightingMatrixFilename", mWeightingMatrixFilename, sizeof(mWeightingMatrixFilename), "");
	Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), NULL);
    Status = theParamFile.GetParameter(SectionName, "BiasVectorFilename", mBiasVectorFilename, sizeof(mBiasVectorFilename), "");
    Status = theParamFile.GetParameter(SectionName, "OutputType", mOutputTypeName, sizeof(mOutputTypeName), "Rectifier");
    Status = theParamFile.GetParameter(SectionName, "OutputGain", mOutputGain, 1.0);
    return 1;
}

int SimpleIFR::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	int i, j;
	char MetadataFileName[256];
	FILE *CurrentSpreadMatrixFile, *BiasVectorFile;

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

	if (stricmp(mOutputTypeName, "Rectifier") == 0)
		mOutputType = Rectifier;
	else
	{
		if (stricmp(mOutputTypeName, "Step") == 0)
			mOutputType = Step;
		else
		{
			if (stricmp(mOutputTypeName, "Sigmoid") == 0)
				mOutputType = Sigmoid;
			else
				throw EarlabException("%s: Unknown output type %s", ModuleName(), mOutputTypeName);
		}
	}

	if (strlen(mBiasVectorFilename) != 0)
	{
		mBiasVector = new float [mNumChannels];
		BiasVectorFile = OpenInputFile(mBiasVectorFilename, "r");
		if (BiasVectorFile == NULL)
			throw EarlabException("%s: Error opening bias vector file \"%s\"", ModuleName(), mBiasVectorFilename);
		for (i = 0; i < mNumChannels; i++)
			fscanf(BiasVectorFile, "%f", &(mBiasVector[i]));
	}

	mCurrentSpreadMatrix = new float *[mNumElectrodes];
	for (i = 0; i < mNumElectrodes; i++)
		mCurrentSpreadMatrix[i] = new float [mNumChannels];

	CurrentSpreadMatrixFile = OpenInputFile(mWeightingMatrixFilename, "r");
	if (CurrentSpreadMatrixFile == NULL)
		throw EarlabException("%s: Error opening current spread matrix file \"%s\"", ModuleName(), mWeightingMatrixFilename);

	for (i = 0; i < mNumElectrodes; i++)
		for (j = 0; j < mNumChannels; j++)
			fscanf(CurrentSpreadMatrixFile, "%f", &(mCurrentSpreadMatrix[i][j]));

	sprintf(MetadataFileName, "%s.1.metadata", ModuleName());
	mMetadataFile = new XmlMetadataFile(MetadataFileName);
	mMetadataFile->SetSampleRate_Hz(mSampleRate_Hz);
	//mMetadataFile->Add(new XmlParameter("FrameSize", mFrameSize_Samples));
	//mMetadataFile->Add(new XmlParameter("SampleRate_Hz", mSampleRate_Hz));
	mMetadataFile->Add(new XmlParameter("Units", "Membrane voltage (mV)"));
	mMetadataFile->Add(new XmlDimension("Frequency (Hz)", mNumChannels, mCFFileName));
	mMetadataFile->SetSource(new XmlSource(ModuleName(), "SimpleIFR_0.1.0.0"));
	mMetadataFile->Write();
    return 1;
}

int SimpleIFR::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int counter = 0;
	FloatMatrixN *Input;
	FloatMatrixN *Output;
	int Sample, Electrode, OutputChannel;
	float CurInput, CurOutput;

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
				mOutputBuffer[OutputChannel] += CurInput * mCurrentSpreadMatrix[Electrode][OutputChannel];
			}
		} // for (Electrode)
		for (OutputChannel = 0; OutputChannel < mNumChannels; OutputChannel++)
		{
			CurOutput = mOutputBuffer[OutputChannel]; 
			if (mBiasVector != NULL)
				CurOutput -= mBiasVector[OutputChannel]; 

			switch (mOutputType)
			{
			case Rectifier:
				// Half-wave rectifier
				if (CurOutput < 0.0f)
					CurOutput = 0.0f;
				break;
			case Step:
				// Step function
				if (CurOutput < 0.0f)
					CurOutput = 0.0f;
				else
					CurOutput = 1.0f;
				break;
			case Sigmoid:
				CurOutput = 1.0f / (1 + exp(-CurOutput));
				break;
			default:
				throw EarlabException("%s: Unknown output type %s", ModuleName(), mOutputTypeName);
				break;
			}

			CurOutput *= (float)mOutputGain;
			Output->Data(Sample, OutputChannel) = CurOutput;
			mMetadataFile->UpdateMinMax(CurOutput);
		}
		mMetadataFile->AddSample();
	} // for (Sample)

	return counter + 1;
}

int SimpleIFR::Stop(void)
{
	Log("    Stop: %s", ModuleName());
	mMetadataFile->Write();
    return 1;
}

int SimpleIFR::Unload(void)
{
	Log("    Unload: %s", ModuleName());
    return 1;
}

