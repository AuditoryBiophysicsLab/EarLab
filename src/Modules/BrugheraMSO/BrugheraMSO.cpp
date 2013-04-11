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
#include "BrugheraMSO.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "EarlabException.h"
#include <math.h>

using namespace std;

BrugheraMSO::BrugheraMSO()
{
    memset(this, 0, sizeof(BrugheraMSO));
	SetModuleName("BrugheraMSO");
}

BrugheraMSO::~BrugheraMSO()
{
}

int BrugheraMSO::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	    return ReadParameters(ParameterFileName, "BrugheraMSO");
	else
	    return ReadParameters(ParameterFileName, ModuleName());

	if (mMetadataFiles != NULL)
	{
		for (int i = 0; i < mNumOptionalOutputs; i++)
			if (mMetadataFiles[i] != NULL)
				delete mMetadataFiles[i];
		delete [] mMetadataFiles;
	}
}

int BrugheraMSO::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0.0);
    Status = theParamFile.GetParameter(SectionName, "FrameLength_Seconds", mFrameLength_Seconds, 0.0);
    Status = theParamFile.GetParameter(SectionName, "ExSynapticStrength", mExcitatorySynapticStrength, 0.0);
    Status = theParamFile.GetParameter(SectionName, "ExSynapticTau_Seconds", mExcitatorySynapticTau, 0.0);
    Status = theParamFile.GetParameter(SectionName, "InhSynapticStrength", mInhibitorySynapticStrength, 0.0);
    Status = theParamFile.GetParameter(SectionName, "InhSynapticTau_Seconds", mInhibitorySynapticTau, 0.0);
    Status = theParamFile.GetParameter(SectionName, "CellTemperature_DegreesC", mCellTemperature_DegreesC, 0.0);
	Status = theParamFile.GetParameter(SectionName, "MaxLag_Seconds", mMaxLag_Seconds, 0.0);

	Status = theParamFile.GetParameter(SectionName, "VmemFilename", mVmemFilename, sizeof(mVmemFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "geFilename", mgeFilename, sizeof(mgeFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "giFilename", mgiFilename, sizeof(mgiFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "gNaFilename", mgNaFilename, sizeof(mgNaFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "gKHTFilename", mgKHTFilename, sizeof(mgKHTFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "gKLTFilename", mgKLTFilename, sizeof(mgKLTFilename), NULL);

	return 1;
}

int BrugheraMSO::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	int Channel, LagBufferSpikeCount;
	double LagSize_Seconds;
	char MetadataFileName[256];

	cout << "    Start: " << ModuleName() << endl;

    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
    if ((NumInputs != 2) && (NumInputs != 4))
        throw EarlabException("%s: This module requires two input (excitatory only) or four (with inhibition added) streams.  Sorry!", ModuleName());

	mNumOptionalOutputs = NumOutputs - 1;

    if (NumOutputs > 1)
	{
		if (OutputTypes[0] != SpikeData)
			throw EarlabException("%s: The first output stream must be a spike stream.  Sorry!", ModuleName());
		if (OutputTypes[1] != WaveformData)
			throw EarlabException("%s: The second output stream must be a waveform stream that will receieve membrane voltage data.  Sorry!", ModuleName());
		if ((NumOutputs > 2) && (OutputTypes[2] != WaveformData))
			throw EarlabException("%s: The third output stream must be a waveform stream that will receieve excitatory conductance data.  Sorry!", ModuleName());
		if ((NumOutputs > 3) && (OutputTypes[3] != WaveformData))
			throw EarlabException("%s: The fourth output stream must be a waveform stream that will receieve inhibitory conductance data.  Sorry!", ModuleName());
		if (NumOutputs > 4)
			throw EarlabException("%s: This module can only handle at most four output streams.  Sorry!", ModuleName());
	}
	else
	{
		if ((NumOutputs != 1) && (OutputTypes[0] != SpikeData))
			throw EarlabException("%s: This module required at least one output stream which must be a spike stream.  Sorry!", ModuleName());
	}

	if (InputTypes[0] != SpikeData)
		throw EarlabException("%s: This module can only handle spike input data streams.  Sorry!", ModuleName());

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

	InputCellCount = InputSize[0][0] * InputSize[0][1];		// Number of freq channels by population per channel
	OutputCellCount = OutputSize[0][0] * OutputSize[0][1];  // Number of freq channels by number of lags
	SamplesPerFrame = (int)(mFrameLength_Seconds * mSampleRate_Hz);

    if (NumOutputs > 1)
	{
		if ((OutputSize[1][0] != SamplesPerFrame) && (OutputSize[1][1] != OutputCellCount))
			throw EarlabException("%s: The second output stream must have dimensions equal to the number of samples in each frame by the number of output MSO cells, or [ %d %d ].  Sorry!", ModuleName(), SamplesPerFrame, OutputCellCount);
		if ((NumOutputs > 2) && (OutputSize[2][0] != SamplesPerFrame) && (OutputSize[2][1] != OutputCellCount))
			throw EarlabException("%s: The third output stream must have dimensions equal to the number of samples in each frame by the number of output MSO cells, or [ %d %d ].  Sorry!", ModuleName(), SamplesPerFrame, OutputCellCount);
		if ((NumOutputs > 3) && (OutputSize[3][0] != SamplesPerFrame) && (OutputSize[3][1] != OutputCellCount))
			throw EarlabException("%s: The fourth output stream must have dimensions equal to the number of samples in each frame by the number of output MSO cells, or [ %d %d ].  Sorry!", ModuleName(), SamplesPerFrame, OutputCellCount);
	}

	NumChannels = InputSize[0][0];
	NumInputReps = InputSize[0][1];
	NumLags = OutputSize[0][1];

	if (NumInputs == 4) // ***NEW*** this boolean used to switch between the two models
		Inhibitory = true;
	else
		Inhibitory = false;

	
	OutputElementCounts[0] = (int)(OutputCellCount * (mFrameLength_Seconds * 20000));
	
	MSOCells = new COMPARTMENT_TYPE **[NumChannels];
	ExcitatorySynapses = new HHSynapse **[NumChannels];
	InhibitorySynapses = new HHSynapse **[NumChannels]; //***NEW***
	WasUnderThreshold = new bool *[NumChannels];
	SynapseList = new int *[NumChannels];
	//DelayBuffers = new SpikeDelayBuffer *[NumLags]; // ***OLD***
	InhibitoryDelayBuffers= new SpikeDelayBuffer *[NumLags]; //***NEW***
	ExcitatoryDelayBuffers= new SpikeDelayBuffer *[NumLags]; //***NEW***
	LagBufferSpikeCount = (int)(InputCellCount * (mFrameLength_Seconds * 20000.0));
	LagSize_Seconds = mMaxLag_Seconds / ((double)NumLags - 1.0);
	//InputContra = new SpikeStream *[NumLags];// ***OLD***
	InputContraExcit = new SpikeStream *[NumLags];//***NEW***
	InputContraInhib = new SpikeStream *[NumLags];//***NEW***
	//DelayBuffers[0] = NULL; // ***OLD***
	InhibitoryDelayBuffers[0] = NULL;//***NEW***
	ExcitatoryDelayBuffers[0] = NULL;//***NEW***
	// delay buffers may be redundant

	for (int Lag = 1; Lag < NumLags; Lag++)
	{
		//DelayBuffers[Lag] = new SpikeDelayBuffer((int)(LagBufferSpikeCount + (InputCellCount * LagSize_Seconds * Lag * 20000)), LagSize_Seconds * Lag, mFrameLength_Seconds, mSampleRate_Hz); // ***OLD***
		ExcitatoryDelayBuffers[Lag] = new SpikeDelayBuffer((int)(LagBufferSpikeCount + (InputCellCount * LagSize_Seconds * Lag * 20000)), LagSize_Seconds * Lag, mFrameLength_Seconds, mSampleRate_Hz);//***NEW***
		InhibitoryDelayBuffers[Lag] = new SpikeDelayBuffer((int) (LagBufferSpikeCount + (InputCellCount * LagSize_Seconds * Lag * 20000)), LagSize_Seconds * Lag, mFrameLength_Seconds, mSampleRate_Hz);//***NEW***
    }

	for (Channel = 0; Channel < NumChannels; Channel++)
	{
		MSOCells[Channel] = new COMPARTMENT_TYPE *[NumLags];
		ExcitatorySynapses[Channel] = new HHSynapse *[NumLags];
		InhibitorySynapses[Channel] = new HHSynapse *[NumLags]; //***NEW***
		WasUnderThreshold[Channel] = new bool [NumLags];
		SynapseList[Channel] = new int [NumInputReps];
		for (int Rep = 0; Rep < NumInputReps; Rep++)
			SynapseList[Channel][Rep] = (Channel * NumInputReps) + Rep;

		for (int Lag = 0; Lag < NumLags; Lag++)
		{
			ExcitatorySynapses[Channel][Lag] = new HHSynapse(mSampleRate_Hz, mExcitatorySynapticTau, mExcitatorySynapticStrength);
			InhibitorySynapses[Channel][Lag] = new HHSynapse (mSampleRate_Hz, mInhibitorySynapticTau,mInhibitorySynapticStrength); // ***NEW***
			MSOCells[Channel][Lag] = new COMPARTMENT_TYPE(mSampleRate_Hz, -60.0e-3, mCellTemperature_DegreesC);
			WasUnderThreshold[Channel][Lag] = true;
		}
	}
    if (NumOutputs > 1)
	{
		mMetadataFiles = new XmlMetadataFile *[NumOutputs - 1];
		memset(mMetadataFiles, 0, sizeof(*mMetadataFiles) * (NumOutputs - 1));
		if ((OutputSize[1][0] != SamplesPerFrame) && (OutputSize[1][1] != OutputCellCount))
			throw EarlabException("%s: The second output stream must have dimensions equal to the number of samples in each frame by the number of output MSO cells, or [ %d %d ].  Sorry!", ModuleName(), SamplesPerFrame, OutputCellCount);
		if ((NumOutputs > 2) && (OutputSize[2][0] != SamplesPerFrame) && (OutputSize[2][1] != OutputCellCount))
			throw EarlabException("%s: The third output stream must have dimensions equal to the number of samples in each frame by the number of output MSO cells, or [ %d %d ].  Sorry!", ModuleName(), SamplesPerFrame, OutputCellCount);
		if ((NumOutputs > 3) && (OutputSize[3][0] != SamplesPerFrame) && (OutputSize[3][1] != OutputCellCount))
			throw EarlabException("%s: The fourth output stream must have dimensions equal to the number of samples in each frame by the number of output MSO cells, or [ %d %d ].  Sorry!", ModuleName(), SamplesPerFrame, OutputCellCount);
	}
	if (NumOutputs >= 2)
	{
		sprintf(MetadataFileName, "%s_MembraneVoltage_V.metadata", ModuleName());
		mMetadataFiles[0] = new XmlMetadataFile(MetadataFileName);
		mMetadataFiles[0]->SetSampleRate_Hz(mSampleRate_Hz);
		mMetadataFiles[0]->Add(new XmlParameter("Units", "Membrane voltage (V)"));
		mMetadataFiles[0]->Add(new XmlDimension("Cell ID", OutputCellCount, 0.0, (double)OutputCellCount));
		mMetadataFiles[0]->SetSource(new XmlSource(ModuleName(), "BrugheraMSO_0.1.0.0"));
		mMetadataFiles[0]->Write();
	}
	if (NumOutputs >= 3)
	{
		sprintf(MetadataFileName, "%s_ExcitatoryConductance_nanoSiemens.metadata", ModuleName());
		mMetadataFiles[1] = new XmlMetadataFile(MetadataFileName);
		mMetadataFiles[1]->SetSampleRate_Hz(mSampleRate_Hz);
		mMetadataFiles[1]->Add(new XmlParameter("Units", "Conductance (nanoSiemens)"));
		mMetadataFiles[1]->Add(new XmlDimension("Cell ID", OutputCellCount, 0.0, (double)OutputCellCount));
		mMetadataFiles[1]->SetSource(new XmlSource(ModuleName(), "BrugheraMSO_0.1.0.0"));
		mMetadataFiles[1]->Write();
	}
	if (NumOutputs >= 4)
	{
		sprintf(MetadataFileName, "%s_InhibitoryConductance_nanoSiemens.metadata", ModuleName());
		mMetadataFiles[2] = new XmlMetadataFile(MetadataFileName);
		mMetadataFiles[2]->SetSampleRate_Hz(mSampleRate_Hz);
		mMetadataFiles[2]->Add(new XmlParameter("Units", "Conductance (nanoSiemens)"));
		mMetadataFiles[2]->Add(new XmlDimension("Cell ID", OutputCellCount, 0.0, (double)OutputCellCount));
		mMetadataFiles[2]->SetSource(new XmlSource(ModuleName(), "BrugheraMSO_0.1.0.0"));
		mMetadataFiles[2]->Write();
	}



//	if (mVmemFilename != NULL)
//		mVmemFile = OpenOutputFile(mVmemFilename, "w");

//	if (mgeFilename != NULL)
//		mgeFile = OpenOutputFile(mgeFilename, "w");

//	if (mgiFilename != NULL)
//		mgiFile = OpenOutputFile(mgiFilename, "w");

//	if (mgNaFilename != NULL)
//		mgNaFile = OpenOutputFile(mgNaFilename, "w");

//	if (mgKHTFilename != NULL)
//		mgKHTFile = OpenOutputFile(mgKHTFilename, "w");

//	if (mgKLTFilename != NULL)
//		mgKLTFile = OpenOutputFile(mgKLTFilename, "w");


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


	return 1;
}

int BrugheraMSO::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int counter = 0;
//	SpikeStream *TempContraInput; // ***OLD*** 
//	SpikeStream *InputIpsi; // ***OLD***
	SpikeStream *TempContraExcitInput; //***NEW***
	SpikeStream *IpsiExcitInput; //***NEW***	

	SpikeStream *Output;
	FloatMatrixN *OptionalOutputs[3];
	int Sample, Channel, Lag;
//	int IpsiSpikeCount; // ***OLD***
	int IpsiSpikeCountExcit; // ***NEW***	
	int CellID;
	float CurOutput;
	float Vmem_mV;
//	float ge_nS, gi_nS, gNa_nS, gKHT_nS, gKLT_nS;
	HHConductanceInfo **Conductances;
	int ConductanceCount;

	Log("    Advance: %s", ModuleName());

//	TempContraInput = ((EarlabSpikeStream *)InputStream[1])->GetData(); // ***OLD***
	TempContraExcitInput = ((EarlabSpikeStream *)InputStream[1])->GetData(); //***NEW***
	// let [0] be ipsi-Excit, [1] be contra-Excit,[2] be ipsi-inhib, [3] be contra-inhib

//	InputContra[0] = TempContraInput; // ***OLD***
//	InputContra[0]->SetSampleRate_Hz(mSampleRate_Hz); // ***OLD***
//	InputContra[0]->NewFrame(); // ***OLD***
	
	InputContraExcit[0] = TempContraExcitInput; // ***NEW***
	InputContraExcit[0]->SetSampleRate_Hz(mSampleRate_Hz); // ***NEW***
	InputContraExcit[0]->NewFrame(); // ***NEW***
	

	SpikeStream *TempContraInhibInput; // ***NEW***	
	SpikeStream *IpsiInhibInput; // ***NEW***
	int IpsiSpikeCountInhib; // ***NEW***

	if (Inhibitory)
	{
		TempContraInhibInput = ((EarlabSpikeStream *)InputStream[3])->GetData();//***NEW*** 
		
		InputContraInhib[0] = TempContraInhibInput; // ***NEW***
		InputContraInhib[0]->SetSampleRate_Hz(mSampleRate_Hz); // ***NEW***
		InputContraInhib[0]->NewFrame(); // ***NEW***
	}

	for (Lag = 1; Lag < NumLags; Lag++)
	{
//		InputContra[Lag] = DelayBuffers[Lag]->Add(TempContraInput); // ***OLD***
//		InputContra[Lag]->SetSampleRate_Hz(mSampleRate_Hz); // ***OLD***
//		InputContra[Lag]->NewFrame(); // ***OLD***
		InputContraExcit[Lag] = ExcitatoryDelayBuffers[Lag]->Add(TempContraExcitInput); //***NEW***
		InputContraExcit[Lag]->SetSampleRate_Hz(mSampleRate_Hz); //***NEW***
		InputContraExcit[Lag]->NewFrame(); //***NEW***
	}
	
	if (Inhibitory)
		for (Lag =1; Lag < NumLags; Lag++)
		{
			InputContraInhib[Lag] = InhibitoryDelayBuffers[Lag]->Add(TempContraInhibInput); //***NEW***
			InputContraInhib[Lag]->SetSampleRate_Hz(mSampleRate_Hz); //***NEW***
			InputContraInhib[Lag]->NewFrame(); //***NEW***
		}

	//InputIpsi =  ((EarlabSpikeStream *)InputStream[0])->GetData();// ***OLD***
	IpsiExcitInput =((EarlabSpikeStream *)InputStream[0])->GetData();// ***NEW***
	Output = ((EarlabSpikeStream *)OutputStream[0])->GetData();
	if (mNumOptionalOutputs > 0)
	{
		for (int i = 0; i < mNumOptionalOutputs; i++)
			OptionalOutputs[i] = ((EarlabWaveformStream *)OutputStream[i + 1])->GetData();
	}

	//InputIpsi->SetSampleRate_Hz(mSampleRate_Hz);// ***OLD***
	IpsiExcitInput->SetSampleRate_Hz(mSampleRate_Hz);// ***NEW***
	Output->SetSampleRate_Hz(mSampleRate_Hz);
	//InputIpsi->NewFrame();// ***OLD***	
	IpsiExcitInput->NewFrame();// ***NEW***
	Output->NewFrame();
	
	if (Inhibitory)
	{
		IpsiInhibInput =((EarlabSpikeStream *)InputStream[2])->GetData();// ***NEW***
		IpsiInhibInput->SetSampleRate_Hz(mSampleRate_Hz);// ***NEW***
		IpsiInhibInput->NewFrame();// ***NEW***
	}

	for (Sample = 0; Sample < SamplesPerFrame; Sample++)
	{
		for (Channel = 0; Channel < NumChannels; Channel++)
		{
			//IpsiSpikeCount = InputIpsi->CountSpikes(SynapseList[Channel], NumInputReps); //***OLD***

			IpsiSpikeCountExcit =IpsiExcitInput->CountSpikes(SynapseList[Channel],NumInputReps); //***NEW***
			if (Inhibitory)
				IpsiSpikeCountInhib =IpsiInhibInput->CountSpikes(SynapseList[Channel],NumInputReps); ////***NEW***

			for (Lag = 0; Lag < NumLags; Lag++)
			{
				//CellID = (Channel * NumLags) + Lag;
				CellID = (Lag * NumChannels) + Channel;
				// Fire synapse for Contralateral spikes received in the current sample period				
				ExcitatorySynapses[Channel][Lag]->Fire(InputContraExcit[Lag]->CountSpikes(SynapseList[Channel], NumInputReps));//***NEW***
				// ExcitatorySynapses synapse for Ipsilateral spikes received in the current sample period
				ExcitatorySynapses[Channel][Lag]->Fire(IpsiSpikeCountExcit);//***NEW***
				// Step the synapse alpha function to provide an input conductance for the MSO cell
				ExcitatorySynapses[Channel][Lag]->Step();//***NEW***		
				//ExcitatorySynapses[Channel][Lag]->Fire(InputContra[Lag]->CountSpikes(SynapseList[Channel], NumInputReps));//***OLD***
				//ExcitatorySynapses[Channel][Lag]->Fire(IpsiSpikeCount); //***OLD***
				//ExcitatorySynapses[Channel][Lag]->Step();//***OLD***
				if (Inhibitory)
				{
					InhibitorySynapses[Channel][Lag]->Fire(InputContraInhib[Lag]->CountSpikes(SynapseList[Channel], NumInputReps));//***NEW***
					InhibitorySynapses[Channel][Lag]->Fire(IpsiSpikeCountInhib);//***NEW***
					InhibitorySynapses[Channel][Lag]->Step();//***NEw***			
				}
				if (!Inhibitory)
					if (MSOCells[Channel][Lag]->Step(&(ExcitatorySynapses[Channel][Lag]), 1, NULL, 0))
						Output->Fire(CellID);
				if (Inhibitory)
					if (MSOCells[Channel][Lag]->Step(&(ExcitatorySynapses[Channel][Lag]), 1, &(InhibitorySynapses[Channel][Lag]), 1))////***NEW***
					Output->Fire(CellID);

			
				
				if (mNumOptionalOutputs > 0)
				{
//					OptionalOutputs[0]->Data(Sample, Channel) = (float)MSOCells[Channel][Lag]->GetMembraneVoltage_V();
//					if (mNumOptionalOutputs > 1)
//						OptionalOutputs[1]->Data(Sample, Channel) = (float)MSOCells[Channel][Lag]->GetExcitatoryConductance_Siemens();
//					if (mNumOptionalOutputs > 2)
//						OptionalOutputs[2]->Data(Sample, Channel) = (float)MSOCells[Channel][Lag]->GetInhibitoryConductance_Siemens();
					CurOutput = (float)MSOCells[Channel][Lag]->GetMembraneVoltage_V();
					mMetadataFiles[0]->UpdateMinMax(CurOutput);
					OptionalOutputs[0]->Data(Sample, CellID) = CurOutput;
					if (mNumOptionalOutputs > 1)
					{
						CurOutput = (float)(MSOCells[Channel][Lag]->GetExcitatoryConductance_Siemens());
						mMetadataFiles[1]->UpdateMinMax(CurOutput);
						OptionalOutputs[1]->Data(Sample, CellID) = CurOutput;
					}
					if (mNumOptionalOutputs > 2)
					{
						CurOutput = (float)(MSOCells[Channel][Lag]->GetInhibitoryConductance_Siemens());
						mMetadataFiles[2]->UpdateMinMax(CurOutput);
						OptionalOutputs[2]->Data(Sample, CellID) = CurOutput;
					}
				}

				// Get and write the output waveforms to files optionally specified in the parameter file.
				// Note that data will be interleaved according to Lags, Channels.
				// The reshape function in Matlab can organize the data.
				Vmem_mV = (float) (MSOCells[Channel][Lag]->GetMembraneVoltage_V() * 1000.0);
				if (mVmemFile != NULL)
					fprintf(mVmemFile, "%f\n", Vmem_mV);

				MSOCells[Channel][Lag]->GetConductances_nS(&ConductanceCount, &Conductances);
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

				}

//				MSOCells[Channel][Lag]->GetConductances_nS(ge_nS, gi_nS, gNa_nS, gKHT_nS, gKLT_nS);
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


//				if (mgeFile != NULL)
//				{
//					ge_nS = (float)(MSOCells[Channel][Lag]->GetExcitatoryConductance_Siemens() * 1e9);
//					fprintf(mgeFile, "%g\n", ge_nS);
//				}

//				if (mgiFile != NULL)
//				{
//					gi_nS = (float)(MSOCells[Channel][Lag]->GetInhibitoryConductance_Siemens() * 1e9);
//					fprintf(mgiFile, "%g\n", gi_nS);
//				}

//				if (mgNaFile != NULL)
//				{
//					gNa_nS = (float)(MSOCells[Channel][Lag]->GetgNaConductance_Siemens() * 1e9);
//					fprintf(mgNaFile, "%g\n", gNa_nS);
//				}

//				if (mgKHTFile != NULL)
//				{
//					gKHT_nS = (float)(MSOCells[Channel][Lag]->GetgKHTConductance_Siemens() * 1e9);
//					fprintf(mgKHTFile, "%g\n", gKHT_nS);
//				}

//				if (mgKLTFile != NULL)
//				{
//					gKLT_nS = (float)(MSOCells[Channel][Lag]->GetgKLTConductance_Siemens() * 1e9);
//					fprintf(mgKLTFile, "%g\n", gKLT_nS);
//				}


			} // for (Lag)
		} // for (Channel)
		for (int i = 0; i < mNumOptionalOutputs; i++)
			mMetadataFiles[i]->AddSample();
//		for (Lag = 0; Lag < NumLags; Lag++) // ***OLD***
//			InputContra[Lag]->Step();
//		InputIpsi->Step();
//		Output->Step();
//	} // for (Sample)

		for (Lag = 0; Lag < NumLags; Lag++)
			InputContraExcit[Lag]->Step();//***NEW*

		IpsiExcitInput->Step(); //***NEW***		
		if (Inhibitory)
		{
			for (Lag = 0; Lag < NumLags; Lag++)
				InputContraInhib[Lag]->Step();//***NEW***
			//InputIpsi->Step();////***OLD***
			IpsiInhibInput->Step(); // ***NEW***
		}
		Output->Step();
	} // for (Sample)

	return counter + 1;
}

int BrugheraMSO::Stop(void)
{
	cout << "    Stop: " << ModuleName() << endl;
	for (int i = 0; i < mNumOptionalOutputs; i++)
		mMetadataFiles[i]->Write();

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

    return 1;
}

int BrugheraMSO::Unload(void)
{
	cout << "    Unload: " << ModuleName() << endl;

    return 1;
}

