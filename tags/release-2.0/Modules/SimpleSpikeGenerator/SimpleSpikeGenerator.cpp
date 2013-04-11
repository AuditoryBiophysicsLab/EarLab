#include <iostream>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "Earlab.h"
#include "SimpleSpikeGenerator.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "EarlabException.h"

using namespace std;

SimpleSpikeGenerator::SimpleSpikeGenerator()
{
    memset(this, 0, sizeof(SimpleSpikeGenerator));
	mLogger = new Logger();
	SetModuleName("SimpleSpikeGenerator");
}

SimpleSpikeGenerator::~SimpleSpikeGenerator()
{
	if (mModuleName != NULL)
		delete [] mModuleName;
}

int SimpleSpikeGenerator::ReadParameters(char *ParameterFileName)
{
	if (mModuleName == NULL)
	    return ReadParameters(ParameterFileName, "SimpleSpikeGenerator");
	else
	    return ReadParameters(ParameterFileName, mModuleName);
}

int SimpleSpikeGenerator::ReadParameters(char *ParameterFileName, char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	mLogger->Log("    ReadParameters: %s \"%s\"", mModuleName, ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0.0);
    Status = theParamFile.GetParameter(SectionName, "c0", c0, 0.0);
    Status = theParamFile.GetParameter(SectionName, "c1", c1, 0.0);
    Status = theParamFile.GetParameter(SectionName, "s0", s0, 0.0);
    Status = theParamFile.GetParameter(SectionName, "s1", s1, 0.0);
    Status = theParamFile.GetParameter(SectionName, "Ra", Ra, 0.0);
    Status = theParamFile.GetParameter(SectionName, "Scale", Scale, 0.0);
    Status = theParamFile.GetParameter(SectionName, "Offset", Offset, 0.0);
    return 1;
}

int SimpleSpikeGenerator::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount])
{
	double FrameLength_mS;
	int CellCount;
	int Channel, Rep;

	cout << "    Start: " << mModuleName << endl;

    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
    if (NumInputs != 1)
        throw EarlabException("%s: Currently this module can only handle one input stream.  Sorry!", mModuleName);

    if (NumOutputs != 1)
        throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", mModuleName);

	if (InputTypes[0] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform input data streams.  Sorry!", mModuleName);

	if (OutputTypes[0] != SpikeData)
		throw EarlabException("%s: Currently this module can only handle spike output data streams.  Sorry!", mModuleName);

    if (InputSize[0][1] != OutputSize[0][0])
        throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", mModuleName);

    if (InputSize[0][2] != 0)
        throw EarlabException("%s: Input data must be two-dimensional array.  Sorry!", mModuleName);

    if (OutputSize[0][1] == 0)
        throw EarlabException("%s: Second dimension of output signal must be non-zero cells per channel.  Sorry!", mModuleName);

    if (OutputSize[0][2] != 0)
        throw EarlabException("%s: Output signal must be two-dimensional array.  Sorry!", mModuleName);

	CellCount = OutputSize[0][0] * OutputSize[0][1];

	FrameLength_Samples = InputSize[0][0];
	NumChannels = InputSize[0][1];
	NumReps = OutputSize[0][1];
	FrameLength_mS = ((1.0 / mSampleRate_Hz) * FrameLength_Samples) * 1000;

	OutputElementCounts[0] = (int)(CellCount * FrameLength_mS);

	Cells = new CarneySpikeGenerator *[NumChannels * NumReps];
	for (Channel = 0; Channel < NumChannels; Channel++)
		for (Rep = 0; Rep < NumReps; Rep++)
		{
			Cells[(Channel * NumReps) + Rep] = new CarneySpikeGenerator(mSampleRate_Hz, c0, c1, s0, s1, Ra, Scale, Offset);
		}

	return 1;
}

int SimpleSpikeGenerator::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int counter = 0;
	FloatMatrixN *Input;
	SpikeStream *Output;
	int Sample, Channel, Rep, CellID;
	float CurInput;

	mLogger->Log("    Advance: %s", mModuleName);

	Input = ((EarlabWaveformStream *)InputStream[0])->GetData();	// Only supporting one output at the present moment
	Output = ((EarlabSpikeStream *)OutputStream[0])->GetData();	// Only supporting one output at the present moment
	Output->SetSampleRate_Hz(mSampleRate_Hz);
	Output->NewFrame();

	for (Sample = 0; Sample < FrameLength_Samples; Sample++)
	{
		for (Channel = 0; Channel < NumChannels; Channel++)
		{
			CurInput = Input->Data(Sample, Channel);
			for (Rep = 0; Rep < NumReps; Rep++)
			{
				CellID = (Channel * NumReps) + Rep;
				if (Cells[CellID]->Step(CurInput))
				{
					Output->Fire(CellID);
					counter++;
				}
			} // for (Rep)
		} // for (Channel)
		Output->Step();
	} // for (Sample)

	return counter + 1;
}

int SimpleSpikeGenerator::Stop(void)
{
	cout << "    Stop: " << mModuleName << endl;

    return 1;
}

int SimpleSpikeGenerator::Unload(void)
{
	cout << "    Unload: " << mModuleName << endl;

    return 1;
}

void SimpleSpikeGenerator::SetModuleName(char *ModuleName)
{
	if (mModuleName != NULL)
		delete [] mModuleName;

	mModuleName = new char[strlen(ModuleName) + 1];
	strcpy(mModuleName, ModuleName);
}

void SimpleSpikeGenerator::SetLogger(Logger *TheLogger)
{
	if (mLogger != NULL)
		delete mLogger;
	mLogger = TheLogger;
}
