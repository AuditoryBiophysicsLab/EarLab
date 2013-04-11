#include <iostream>
#include "Vocoder.h"
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

Vocoder::Vocoder()
{
    memset(this, 0, sizeof(Vocoder));
	SetModuleName("Vocoder");
}

Vocoder::~Vocoder()
{
	if (mMetadataFile != NULL)
		delete mMetadataFile;
	
	if (mCFArray_Hz != NULL)
		delete [] mCFArray_Hz;

	if (Vocoders != NULL)
	{
		for (int i = 0; i < mNumChannels; i++)
			delete Vocoders[i];
		delete [] Vocoders;
	}
}

int Vocoder::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	   return ReadParameters(ParameterFileName, "Vocoder");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int Vocoder::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);
    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, mSampleRate_Hz);
    Status = theParamFile.GetParameter(SectionName, "Gain", mGain, 1);
 	Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), "");
    return 1;
}

int Vocoder::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	int i;
	char MetadataFileName[256];
	FILE *CFFile;
	float tmp;

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

	OutputElementCounts[0] = OutputSize[0][0];

	mFrameSize_Samples = OutputSize[0][0];
	mNumChannels = InputSize[0][1];

	mCFCount = 0;
	if ((mCFFileName != NULL) && (strlen(mCFFileName) != 0))
	{
		CFFile = OpenOutputFile(mCFFileName, "r");
		if (CFFile != NULL)
		{
			while (!feof(CFFile))
			{
				fscanf(CFFile, "%f", &tmp);
				mCFCount++;
			}
			mCFCount--;
			
			mCFArray_Hz = new float[mCFCount];
			fseek(CFFile, 0, SEEK_SET);
			for (int i = 0; i < mCFCount; i++)
			{
				fscanf(CFFile, "%f", &tmp);
				mCFArray_Hz[i] = tmp;
			}
			CloseFile(CFFile);
		}
	}


	Vocoders = new VocoderChannel *[mNumChannels];
	for (i = 0; i < mNumChannels; i++)
		Vocoders[i] = new VocoderChannel(mSampleRate_Hz, mCFArray_Hz[i], mGain);

	sprintf(MetadataFileName, "%s.1.metadata", ModuleName());
	mMetadataFile = new XmlMetadataFile(MetadataFileName);
	mMetadataFile->SetSampleRate_Hz(mSampleRate_Hz);
	mMetadataFile->Add(new XmlParameter("FrameSize", mFrameSize_Samples));
	mMetadataFile->Add(new XmlParameter("SampleRate_Hz", mSampleRate_Hz));
	mMetadataFile->Add(new XmlParameter("Units", "Membrane voltage (mV)"));
	mMetadataFile->Add(new XmlDimension("Frequency (Hz)", mNumChannels, mCFFileName));
	mMetadataFile->SetSource(new XmlSource(ModuleName(), "Vocoder_0.1.0.0"));
	mMetadataFile->Write();
    return 1;
}

int Vocoder::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int i, j;
	FloatMatrixN *Input, *Output;
	float CurOutputSample;

	Log("    Advance: %s", ModuleName());

	Input = ((EarlabWaveformStream *)InputStream[0])->GetData();	// Only supporting one output at the present moment
	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();	// Only supporting one output at the present moment

	if ((Input->Rank(0) != mFrameSize_Samples) || (Input->Rank(1) != mNumChannels))
		throw EarlabException("%s: Input size mismatch with Start()", ModuleName());
	if ((Output->Rank(0) != mFrameSize_Samples) || (Output->Rank(1) != mNumChannels))
		throw EarlabException("%s: Output size mismatch with Start()", ModuleName());

    for (j = 0; j < mFrameSize_Samples; j++)
    {
		CurOutputSample = 0.0f;
        for (i = 0; i < mNumChannels; i++)
		{
			CurOutputSample = Vocoders[i]->Step(Input->Data(j, i));
			Output->Data(j, i) = CurOutputSample;
			mMetadataFile->UpdateMinMax(CurOutputSample);
		}
		mMetadataFile->AddSample();
    } 
    return mFrameSize_Samples;
}

int Vocoder::Stop(void)
{
	Log("    Stop: %s", ModuleName());
	mMetadataFile->Write();
    return 1;
}

int Vocoder::Unload(void)
{
	Log("    Unload: %s", ModuleName());
    return 1;
}

VocoderChannel::VocoderChannel(double SampleRate_Hz, double Frequency_Hz, double Gain)
{
	memset(this, 0, sizeof(VocoderChannel));

	mSampleRate_Hz = SampleRate_Hz;
	mFrequency_Hz = Frequency_Hz;
	mGain = Gain;
	/* Convert sample/sec  to sec/sample */ 
    mDeltaT_Sec = 1.0 / mSampleRate_Hz;
	mStepCount = 0;
}

float VocoderChannel::Step(float CurSample)
{
	double result;

	result = float(sin(2 * PI * mFrequency_Hz * (mStepCount * mDeltaT_Sec)) * mGain);
	mStepCount++;
	return (float)(result * CurSample);
}
