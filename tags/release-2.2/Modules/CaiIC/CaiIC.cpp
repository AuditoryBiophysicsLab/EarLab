#include <iostream>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include "Earlab.h"
#include "CaiIC.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "EarlabException.h"
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

CaiIC::CaiIC()
{
    memset(this, 0, sizeof(CaiIC));
	SetModuleName("CaiIC");
}

CaiIC::~CaiIC()
{
	if (ModuleName() != NULL)
		delete [] ModuleName();
	for (int Channel = 0; Channel < NumChannels; Channel++)
	{
		for (int Lag = 0; Lag < NumLags; Lag++)
		{
			delete ICCells[Channel][Lag];
		}
		delete [] ICCells[Channel];
	}
	delete ICCells;
	delete InhInputDelayBuffer;
}

int CaiIC::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	    return ReadParameters(ParameterFileName, "CaiIC");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int CaiIC::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0.0);
    Status = theParamFile.GetParameter(SectionName, "FrameLength_Seconds", mFrameLength_Seconds, 0.0);
    Status = theParamFile.GetParameter(SectionName, "ExSynapticStrength", mExSynapticStrength, 0.0);
    Status = theParamFile.GetParameter(SectionName, "ExSynapticTau_Seconds", mExSynapticTau, 0.0);
    Status = theParamFile.GetParameter(SectionName, "InhSynapticStrength", mInhSynapticStrength, 0.0);
    Status = theParamFile.GetParameter(SectionName, "InhSynapticTau_Seconds", mInhSynapticTau, 0.0);
    Status = theParamFile.GetParameter(SectionName, "CellTemperature_DegreesC", mCellTemperature_DegreesC, 0.0);
	Status = theParamFile.GetParameter(SectionName, "InhInputDelay_Seconds", mInhInputDelay_Seconds, 0.0);
	Status = theParamFile.GetParameter(SectionName, "ExcitatoryMSOLagNumber", mExcitatoryMSOLagNumber, 0);
    return 1;
}

int CaiIC::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount])
{
	int Channel, LagBufferSpikeCount;

	cout << "    Start: " << ModuleName() << endl;

    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
    if ((NumInputs != 2))
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
	
	ICCells = new Cai98Compartment **[NumChannels];
	ExcitatorySynapses = new HHSynapse **[NumChannels];
	InhibitorySynapses = new HHSynapse **[NumChannels];
	WasUnderThreshold = new bool *[NumChannels];
	LagBufferSpikeCount = (int)(InputCellCount * 10 * mInhInputDelay_Seconds * 1000.0);
	// Not sure why memory is reserved for InputContra here, and not for InputIpsi.
	// Maybe because with the delay spike must be held across frames.
	// TempInputContra = new SpikeStream;
	InputContra = new SpikeStream;
	// InhInputDelayBuffer: one delay for all inhibitory inputs (InputContra)
	InhInputDelayBuffer = new SpikeDelayBuffer(LagBufferSpikeCount, mInhInputDelay_Seconds, mFrameLength_Seconds, mSampleRate_Hz);

	for (Channel = 0; Channel < NumChannels; Channel++)
	{
		ICCells[Channel] = new Cai98Compartment *[NumLags];
		ExcitatorySynapses[Channel] = new HHSynapse *[NumLags];
		InhibitorySynapses[Channel] = new HHSynapse *[NumLags];
		WasUnderThreshold[Channel] = new bool [NumLags];

		for (int Lag = 0; Lag < NumLags; Lag++)
		{
			ExcitatorySynapses[Channel][Lag] = new HHSynapse(mSampleRate_Hz, mExSynapticTau, mExSynapticStrength);
			InhibitorySynapses[Channel][Lag] = new HHSynapse(mSampleRate_Hz, mInhSynapticTau, mInhSynapticStrength);
			ICCells[Channel][Lag] = new Cai98Compartment(mSampleRate_Hz, -60.0e-3, mCellTemperature_DegreesC);
			WasUnderThreshold[Channel][Lag] = true;
		}
	}

	SamplesPerFrame = (int)(mFrameLength_Seconds * mSampleRate_Hz);

	return 1;
}

int CaiIC::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int counter = 0;
	SpikeStream *InputIpsi;
	// Add SpikeStream *TempInputContra to hold undelayed spikes
	SpikeStream *TempInputContra;
	SpikeStream *InputContra;
	SpikeStream *Output;
	int Sample, Channel, Lag;
	int IpsiCellID, ContraCellID;
	int IpsiSpikeCount, ContraSpikeCount;

	Log("    Advance: %s", ModuleName());

	InputIpsi = ((EarlabSpikeStream *)InputStream[0])->GetData();
	// InputContra =  ((EarlabSpikeStream *)InputStream[1])->GetData();
	TempInputContra = ((EarlabSpikeStream *)InputStream[1])->GetData();
	InputContra = InhInputDelayBuffer->Add(TempInputContra); 
	Output = ((EarlabSpikeStream *)OutputStream[0])->GetData();

	InputIpsi->SetSampleRate_Hz(mSampleRate_Hz);
	InputContra->SetSampleRate_Hz(mSampleRate_Hz);
	Output->SetSampleRate_Hz(mSampleRate_Hz);
	InputContra->NewFrame();
	InputIpsi->NewFrame();
	Output->NewFrame();
	for (Sample = 0; Sample < SamplesPerFrame; Sample++)
	{
		for (Channel = 0; Channel < NumChannels; Channel++)
		{
			IpsiCellID = (mExcitatoryMSOLagNumber * NumChannels) + Channel;
			IpsiSpikeCount = InputIpsi->CountSpikes(IpsiCellID);
			for (Lag = 0; Lag < NumLags; Lag++)
			{
				ContraCellID = (Lag * NumChannels) + Channel;
				ContraSpikeCount = InputContra->CountSpikes(IpsiCellID);
				// Fire synapse for ipsilateral spikes received in the current sample period
				ExcitatorySynapses[Channel][Lag]->Fire(IpsiSpikeCount);
				// Fire synapse for contralateral spikes received in the current sample period
				InhibitorySynapses[Channel][Lag]->Fire(ContraSpikeCount);
				// Step the synapse alpha function to provide an input conductance for the MSO cell
				ExcitatorySynapses[Channel][Lag]->Step();
				InhibitorySynapses[Channel][Lag]->Step();
				if (ICCells[Channel][Lag]->Step(&(ExcitatorySynapses[Channel][Lag]), 1, &(InhibitorySynapses[Channel][Lag]), 1))
					Output->Fire(ContraCellID);
			} // for (Lag)
		} // for (Channel)
		InputIpsi->Step();
		InputContra->Step();
		Output->Step();
	} // for (Sample)

	return counter + 1;
}

int CaiIC::Stop(void)
{
	cout << "    Stop: " << ModuleName() << endl;

    return 1;
}

int CaiIC::Unload(void)
{
	cout << "    Unload: " << ModuleName() << endl;

    return 1;
}
