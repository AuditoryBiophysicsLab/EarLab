#include <iostream>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include "Earlab.h"
#include "BrugheraMSO.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "EarlabException.h"
#include <math.h>
#include <windows.h>

using namespace std;

BrugheraMSO::BrugheraMSO()
{
    memset(this, 0, sizeof(BrugheraMSO));
	SetModuleName("BrugheraMSO");
}

BrugheraMSO::~BrugheraMSO()
{
	if (ModuleName() != NULL)
		delete [] ModuleName();
}

int BrugheraMSO::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	    return ReadParameters(ParameterFileName, "BrugheraMSO");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int BrugheraMSO::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0.0);
    Status = theParamFile.GetParameter(SectionName, "FrameLength_Seconds", mFrameLength_Seconds, 0.0);
    Status = theParamFile.GetParameter(SectionName, "SynapticStrength", mSynapticStrength, 0.0);
    Status = theParamFile.GetParameter(SectionName, "SynapticTau_Seconds", mSynapticTau, 0.0);
    Status = theParamFile.GetParameter(SectionName, "CellTemperature_DegreesC", mCellTemperature_DegreesC, 0.0);
	Status = theParamFile.GetParameter(SectionName, "MaxLag_Seconds", mMaxLag_Seconds, 0.0);
    return 1;
}

int BrugheraMSO::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount])
{
	int Channel, LagBufferSpikeCount;
	double LagSize_Seconds;

	cout << "    Start: " << ModuleName() << endl;

    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
    if (NumInputs != 2)
        throw EarlabException("%s: This module requires two input streams.  Sorry!", ModuleName());

    if (NumOutputs != 1)
        throw EarlabException("%s: This module can only handle one output stream.  Sorry!", ModuleName());

	if (InputTypes[0] != SpikeData)
		throw EarlabException("%s: This module can only handle spike input data streams.  Sorry!", ModuleName());

	if (OutputTypes[0] != SpikeData)
		throw EarlabException("%s: This module can only handle spike output data streams.  Sorry!", ModuleName());

    if (InputSize[0][0] != InputSize[1][0])
        throw EarlabException("%s: Input channel counts must be identical.  Sorry!", ModuleName());

    if (InputSize[0][1] != InputSize[1][1])
        throw EarlabException("%s: Input repetition counts must be identical.  Sorry!", ModuleName());

    if ((InputSize[0][2] != 0) || (InputSize[1][2] != 0))
        throw EarlabException("%s: Inputs must be two-dimensional.  Sorry!", ModuleName());

	if (InputSize[0][0] != OutputSize[0][0])
        throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", ModuleName());

    if (OutputSize[0][1] == 0)
        throw EarlabException("%s: Second dimension of output signal must be non-zero number of lags.  Sorry!", ModuleName());

    if (OutputSize[0][2] != 0)
        throw EarlabException("%s: Output data must be two-dimensional array.  Sorry!", ModuleName());

	InputCellCount = InputSize[0][0] * InputSize[0][1];
	OutputCellCount = OutputSize[0][0] * OutputSize[0][1];

	NumChannels = InputSize[0][0];
	NumInputReps = InputSize[0][1];
	NumLags = OutputSize[0][1];

	OutputElementCounts[0] = (int)(OutputCellCount * (mFrameLength_Seconds * 1000));
	
	MSOCells = new Rothman93Compartment **[NumChannels];
	Synapses = new HHSynapse **[NumChannels];
	WasUnderThreshold = new bool *[NumChannels];
	SynapseList = new int *[NumChannels];
	DelayBuffers = new SpikeDelayBuffer *[NumLags];
	LagBufferSpikeCount = (int)(InputCellCount * (mFrameLength_Seconds * 1000.0)) + InputCellCount;
	LagSize_Seconds = mMaxLag_Seconds / (double)NumLags;
	InputIpsi = new SpikeStream *[NumLags];
	DelayBuffers[0] = NULL;

	for (int Lag = 1; Lag < NumLags; Lag++)
		DelayBuffers[Lag] = new SpikeDelayBuffer(LagBufferSpikeCount, LagSize_Seconds * Lag, mFrameLength_Seconds, mSampleRate_Hz);

	for (Channel = 0; Channel < NumChannels; Channel++)
	{
		MSOCells[Channel] = new Rothman93Compartment *[NumLags];
		Synapses[Channel] = new HHSynapse *[NumLags];
		WasUnderThreshold[Channel] = new bool [NumLags];
		SynapseList[Channel] = new int [NumInputReps];
		for (int Rep = 0; Rep < NumInputReps; Rep++)
			SynapseList[Channel][Rep] = (Channel * NumInputReps) + Rep;

		for (int Lag = 0; Lag < NumLags; Lag++)
		{
			Synapses[Channel][Lag] = new HHSynapse(mSampleRate_Hz, mSynapticTau, mSynapticStrength);
			MSOCells[Channel][Lag] = new Rothman93Compartment(mSampleRate_Hz, -60.0e-3, mCellTemperature_DegreesC);
			WasUnderThreshold[Channel][Lag] = true;
		}
	}

	SamplesPerFrame = (int)(mFrameLength_Seconds * mSampleRate_Hz);

	return 1;
}

int BrugheraMSO::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int counter = 0;
	SpikeStream *TempIpsiInput;
	SpikeStream *InputContra;
	SpikeStream *Output;
	int Sample, Channel, Lag;
	int ContraSpikeCount;
	int CellID;

	Log("    Advance: %s", ModuleName());

	TempIpsiInput = ((EarlabSpikeStream *)InputStream[0])->GetData();
	InputIpsi[0] = TempIpsiInput;
	InputIpsi[0]->SetSampleRate_Hz(mSampleRate_Hz);
	InputIpsi[0]->NewFrame();
	for (Lag = 1; Lag < NumLags; Lag++)
	{
		InputIpsi[Lag] = DelayBuffers[Lag]->Add(TempIpsiInput);
		InputIpsi[Lag]->SetSampleRate_Hz(mSampleRate_Hz);
		InputIpsi[Lag]->NewFrame();
	}
	InputContra =  ((EarlabSpikeStream *)InputStream[1])->GetData();
	Output = ((EarlabSpikeStream *)OutputStream[0])->GetData();

	InputContra->SetSampleRate_Hz(mSampleRate_Hz);
	Output->SetSampleRate_Hz(mSampleRate_Hz);
	InputContra->NewFrame();
	Output->NewFrame();
	for (Sample = 0; Sample < SamplesPerFrame; Sample++)
	{
		for (Channel = 0; Channel < NumChannels; Channel++)
		{
			ContraSpikeCount = InputContra->CountSpikes(SynapseList[Channel], NumInputReps);
			for (Lag = 0; Lag < NumLags; Lag++)
			{
				//CellID = (Channel * NumLags) + Lag;
				CellID = (Lag * NumChannels) + Channel;
				// Fire synapse for ipsilateral spikes received in the current sample period
				Synapses[Channel][Lag]->Fire(InputIpsi[Lag]->CountSpikes(SynapseList[Channel], NumInputReps));
				// Fire synapse for contralateral spikes received in the current sample period
				Synapses[Channel][Lag]->Fire(ContraSpikeCount);
				// Step the synapse alpha function to provide an input conductance for the MSO cell
				Synapses[Channel][Lag]->Step();
				if (MSOCells[Channel][Lag]->Step(&(Synapses[Channel][Lag]), 1, NULL, 0))
					Output->Fire(CellID);
			} // for (Lag)
		} // for (Channel)
		for (Lag = 0; Lag < NumLags; Lag++)
			InputIpsi[Lag]->Step();
		InputContra->Step();
		Output->Step();
	} // for (Sample)

	return counter + 1;
}

int BrugheraMSO::Stop(void)
{
	cout << "    Stop: " << ModuleName() << endl;

    return 1;
}

int BrugheraMSO::Unload(void)
{
	cout << "    Unload: " << ModuleName() << endl;

    return 1;
}
