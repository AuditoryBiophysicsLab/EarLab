#ifdef _WIN32
#include <windows.h>
#endif
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

using namespace std;

CaiIC::CaiIC()
{
    memset(this, 0, sizeof(CaiIC));
	SetModuleName("CaiIC");
}

CaiIC::~CaiIC()
{
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
	Status = theParamFile.GetParameter(SectionName, "ResponseType", mResponseType, 2);
	Status = theParamFile.GetParameter(SectionName, "TauAHP_Seconds", mTauAHP_Seconds, 1.0/mSampleRate_Hz);
	Status = theParamFile.GetParameter(SectionName, "AHPIncrement_Siemens", mAHPIncrement_Siemens, 0.0e-9);
	if (mAHPIncrement_Siemens == 0.0)
		mCompartmentClassName = "Brughera96Compartment";
	else
		mCompartmentClassName = "Cai98Compartment";

	Status = theParamFile.GetParameter(SectionName, "VmemFilename", mVmemFilename, sizeof(mVmemFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "geFilename", mgeFilename, sizeof(mgeFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "giFilename", mgiFilename, sizeof(mgiFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "gNaFilename", mgNaFilename, sizeof(mgNaFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "gKHTFilename", mgKHTFilename, sizeof(mgKHTFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "gKLTFilename", mgKLTFilename, sizeof(mgKLTFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "gAHPFilename", mgAHPFilename, sizeof(mgAHPFilename), NULL);
	
	return 1;
}

int CaiIC::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
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

	OutputElementCounts[0] = (int)(OutputCellCount * (mFrameLength_Seconds * 20000));
	
	ICCells = new HHCompartment **[NumChannels];
	//ICCells = new COMPARTMENT_TYPE **[NumChannels];
	ExcitatorySynapses = new HHSynapse **[NumChannels];
	InhibitorySynapses = new HHSynapse **[NumChannels];
	WasUnderThreshold = new bool *[NumChannels];
	LagBufferSpikeCount = (int)(InputCellCount * mInhInputDelay_Seconds * 20000.0);
	// Not sure why memory is reserved for InputContra here, and not for InputIpsi.
	// Maybe because with the delay spike must be held across frames.
	// TempInputContra = new SpikeStream;
	InputContra = new SpikeStream;
	// InhInputDelayBuffer: one delay for all inhibitory inputs (InputContra)
	InhInputDelayBuffer = new SpikeDelayBuffer(LagBufferSpikeCount, mInhInputDelay_Seconds, mFrameLength_Seconds, mSampleRate_Hz);

	for (Channel = 0; Channel < NumChannels; Channel++)
	{
		ICCells[Channel] = new HHCompartment *[NumLags];
		//ICCells[Channel] = new COMPARTMENT_TYPE *[NumLags];
		ExcitatorySynapses[Channel] = new HHSynapse *[NumLags];
		InhibitorySynapses[Channel] = new HHSynapse *[NumLags];
		WasUnderThreshold[Channel] = new bool [NumLags];

		for (int Lag = 0; Lag < NumLags; Lag++)
		{
			ExcitatorySynapses[Channel][Lag] = new HHSynapse(mSampleRate_Hz, mExSynapticTau, mExSynapticStrength);
			InhibitorySynapses[Channel][Lag] = new HHSynapse(mSampleRate_Hz, mInhSynapticTau, mInhSynapticStrength);
			newmResponseType = (Enumed93ResponseType) mResponseType;
			ICCells[Channel][Lag] = HHCompartmentFactory::Create(mCompartmentClassName, mSampleRate_Hz, -60.0e-3, mCellTemperature_DegreesC, TYPE2, newmResponseType, mTauAHP_Seconds, mAHPIncrement_Siemens);
			//ICCells[Channel][Lag] = new COMPARTMENT_TYPE(mSampleRate_Hz, -60.0e-3, mCellTemperature_DegreesC, newmResponseType, mTauAHP_Seconds, mAHPIncrement_Siemens);
			//ICCells[Channel][Lag] = new COMPARTMENT_TYPE(mSampleRate_Hz, -60.0e-3, mCellTemperature_DegreesC);
			WasUnderThreshold[Channel][Lag] = true;
		}
	}

	SamplesPerFrame = (int)(mFrameLength_Seconds * mSampleRate_Hz);

	if (strlen(mVmemFilename) != 0)
		mVmemFile = OpenOutputFile(mVmemFilename, "w");

	if (strlen(mgeFilename) != 0)
		mgeFile = OpenOutputFile(mgeFilename, "w");

	if (strlen(mgiFilename) != 0)
		mgiFile = OpenOutputFile(mgiFilename, "w");

	if (strlen(mgNaFilename) != 0)
		mgNaFile = OpenOutputFile(mgNaFilename, "w");

	if (strlen(mgKHTFilename) != 0)
		mgKHTFile = OpenOutputFile(mgKHTFilename, "w");

	if (strlen(mgKLTFilename) != 0)
		mgKLTFile = OpenOutputFile(mgKLTFilename, "w");

	if (strlen(mgAHPFilename) != 0)
		mgAHPFile = OpenOutputFile(mgAHPFilename, "w");

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
	float Vmem_mV;
//	float ge_nS, gi_nS, gNa_nS, gKHT_nS, gKLT_nS, gAHP_nS;
	HHConductanceInfo **Conductances;
	int ConductanceCount;

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
				ContraSpikeCount = InputContra->CountSpikes(ContraCellID);
				// Fire synapse for ipsilateral spikes received in the current sample period
				ExcitatorySynapses[Channel][Lag]->Fire(IpsiSpikeCount);
				// Fire synapse for contralateral spikes received in the current sample period
				InhibitorySynapses[Channel][Lag]->Fire(ContraSpikeCount);
				// Step the synapse alpha functions to provide input conductances for the IC cell
				ExcitatorySynapses[Channel][Lag]->Step();
				InhibitorySynapses[Channel][Lag]->Step();
				if (ICCells[Channel][Lag]->Step(&(ExcitatorySynapses[Channel][Lag]), 1, &(InhibitorySynapses[Channel][Lag]), 1))
					Output->Fire(ContraCellID);

				// Get and write the output waveforms to files optionally specified in the parameter file.
				// Note that data will be interleaved according to Lags, Channels.
				// The reshape function in Matlab can organize the data.
				Vmem_mV = (float) (ICCells[Channel][Lag]->GetMembraneVoltage_V() * 1000.0);
				if (mVmemFile != NULL)
					fprintf(mVmemFile, "%f\n", Vmem_mV);

				ICCells[Channel][Lag]->GetConductances_nS(&ConductanceCount, &Conductances);
				for (int i = 0; i < ConductanceCount; i++)
				{

					if ((mgeFile != NULL) && (stricmp(Conductances[i]->ConductanceName, "ge") == 0))
						fprintf(mgeFile, "%f\n", Conductances[i]->Conductance_nS);

					if ((mgiFile != NULL) && (stricmp(Conductances[i]->ConductanceName, "gi") == 0))
						fprintf(mgiFile, "%f\n", Conductances[i]->Conductance_nS);

					if ((mgNaFile != NULL) && (stricmp(Conductances[i]->ConductanceName, "gNa") == 0))
						fprintf(mgNaFile, "%f\n", Conductances[i]->Conductance_nS);

					if ((mgKHTFile != NULL) && (stricmp(Conductances[i]->ConductanceName, "gKfast") == 0))
						fprintf(mgKHTFile, "%f\n", Conductances[i]->Conductance_nS);

					if ((mgKLTFile != NULL) && (stricmp(Conductances[i]->ConductanceName, "gKslow") == 0))
						fprintf(mgKLTFile, "%f\n", Conductances[i]->Conductance_nS);

					if ((mgKLTFile != NULL) && (stricmp(Conductances[i]->ConductanceName, "gAHP") == 0))
						fprintf(mgKLTFile, "%f\n", Conductances[i]->Conductance_nS);
				}

//				ICCells[Channel][Lag]->GetConductances_nS(ge_nS, gi_nS, gNa_nS, gKHT_nS, gKLT_nS, gAHP_nS);
//				if (mgeFile != NULL)
//					fprintf(mgeFile, "%f\n", ge_nS);

//				if (mgiFile != NULL)
//					fprintf(mgiFile, "%f\n", gi_nS);

//				if (mgNaFile != NULL)
//					fprintf(mgNaFile, "%f\n", gNa_nS);

//				if (mgKHTFile != NULL)
//					fprintf(mgKHTFile, "%f\n", gKHT_nS);

//				if (mgKLTFile != NULL)
//					fprintf(mgKLTFile, "%f\n", gKLT_nS);

//				if (mgAHPFile != NULL)
//					fprintf(mgAHPFile, "%f\n", gAHP_nS);

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

	if (mVmemFile != NULL)
		CloseFile(mVmemFile);

	if (mgeFile != NULL)
		CloseFile(mgeFile);

	if (mgiFile != NULL)
		CloseFile(mgiFile);

	if (mgNaFile != NULL)
		CloseFile(mgNaFile);

	if (mgKHTFile != NULL)
		CloseFile(mgKHTFile);

	if (mgKLTFile != NULL)
		CloseFile(mgKLTFile);

 	if (mgAHPFile != NULL)
		CloseFile(mgAHPFile);

   return 1;
}

int CaiIC::Unload(void)
{
	cout << "    Unload: " << ModuleName() << endl;

    return 1;
}

