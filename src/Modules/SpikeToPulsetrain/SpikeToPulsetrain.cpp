#include <iostream>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "Earlab.h"
#include "SpikeToPulsetrain.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "EarlabException.h"

using namespace std;

SpikeToPulsetrain::SpikeToPulsetrain()
{
    memset(this, 0, sizeof(SpikeToPulsetrain));
	SetModuleName("SpikeToPulsetrain");
}

SpikeToPulsetrain::~SpikeToPulsetrain()
{
	if (mMetadataFile != NULL)
		delete mMetadataFile;
}

int SpikeToPulsetrain::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	    return ReadParameters(ParameterFileName, "SpikeToPulsetrain");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int SpikeToPulsetrain::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0.0);
 	//Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), "");
    Status = theParamFile.GetParameter(SectionName, "Gain", mGain, 1.0);
    return 1;
}

int SpikeToPulsetrain::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	double FrameLength_mS;
	int CellCount;
	//char MetadataFileName[256];

	cout << "    Start: " << ModuleName() << endl;

    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
    if (NumInputs != 1)
        throw EarlabException("%s: Currently this module can only handle one input stream.  Sorry!", ModuleName());

    if (NumOutputs != 1)
        throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", ModuleName());

	if (InputTypes[0] != SpikeData)
		throw EarlabException("%s: Currently this module can only handle spike input data streams.  Sorry!", ModuleName());

	if (OutputTypes[0] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform output data streams.  Sorry!", ModuleName());

    if (InputSize[0][0] != OutputSize[0][1])
        throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", ModuleName());

    if (InputSize[0][2] != 0)
        throw EarlabException("%s: Input data must be one- or two- dimensional array of spiking nerve fibers.  Sorry!", ModuleName());

    if (OutputSize[0][2] != 0)
        throw EarlabException("%s: Output signal must be two-dimensional array.  Sorry!", ModuleName());

    if (InputSize[0][1] != 0)
	{
		CellCount = InputSize[0][0] * InputSize[0][1];
		NumReps = InputSize[0][1];
	}
	else
	{
		CellCount = InputSize[0][0];
		NumReps = 1;
	}

	FrameLength_Samples = OutputSize[0][0];
	NumChannels = OutputSize[0][1];
	FrameLength_mS = ((1.0 / mSampleRate_Hz) * FrameLength_Samples) * 1000;

	OutputElementCounts[0] = (int)(FrameLength_Samples * NumChannels);

	//sprintf(MetadataFileName, "%s.1.metadata", ModuleName());
	//mMetadataFile = new XmlMetadataFile(MetadataFileName);
	//mMetadataFile->SetSampleRate_Hz(mSampleRate_Hz);
	//mMetadataFile->Add(new XmlParameter("Units", "IFR (Spikes/S)"));
	//mMetadataFile->Add(new XmlDimension("Frequency (Hz)", NumChannels, mCFFileName));
	//mMetadataFile->SetSource(new XmlSource(ModuleName(), "SpikeToPulsetrain_0.1.0.0"));
	//mMetadataFile->Write();

	return 1;
}

int SpikeToPulsetrain::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int counter = 0;
	FloatMatrixN *Output;
	SpikeStream *Input;
	int Sample, Channel, Rep, CellID, NumSpikes;
	float CurOutputSample;

	Log("    Advance: %s", ModuleName());

	Input = ((EarlabSpikeStream *)InputStream[0])->GetData();	// Only supporting one output at the present moment
	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();	// Only supporting one output at the present moment
	Input->SetSampleRate_Hz(mSampleRate_Hz);
	Input->NewFrame();

	for (Sample = 0; Sample < FrameLength_Samples; Sample++)
	{
		for (Channel = 0; Channel < NumChannels; Channel++)
		{
			for (Rep = 0; Rep < NumReps; Rep++)
			{
				CellID = (Channel * NumReps) + Rep;
				mCellIDs[Rep] = CellID;
			}
			NumSpikes = Input->CountSpikes(mCellIDs, NumReps);
			if (NumSpikes == 0)
				CurOutputSample = 0.0f;
			else
				CurOutputSample = (float)((NumSpikes / mSampleRate_Hz) * mGain);
			//mMetadataFile->UpdateMinMax(CurOutputSample);
			Output->Data(Sample, Channel) = CurOutputSample;
		} // for (Channel)
		Input->Step();
		//mMetadataFile->AddSample();
	} // for (Sample)

	return counter + 1;
}

int SpikeToPulsetrain::Stop(void)
{
	cout << "    Stop: " << ModuleName() << endl;
	//mMetadataFile->Write();
    return 1;
}

int SpikeToPulsetrain::Unload(void)
{
	cout << "    Unload: " << ModuleName() << endl;

    return 1;
}
