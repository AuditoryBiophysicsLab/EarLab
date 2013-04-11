#include <iostream>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "Earlab.h"
#include "ZhouLSO.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "EarlabException.h"
#include "FPException.h"

using namespace std;

EarlabDataStreamType InputDataType;


ZhouLSO::ZhouLSO()
{
    memset(this, 0, sizeof(ZhouLSO));
	SetModuleName("ZhouLSO");
}

ZhouLSO::~ZhouLSO()
{
//	if (Neurons != NULL)
//		delete [] Neurons;

	int Channel;

	for (Channel = 0; Channel < NumChannels; Channel++)
	{
		if (Neurons != NULL)
			delete Neurons[Channel];
		if (ExcitatorySynapses != NULL)
			delete ExcitatorySynapses[Channel];
		if (InhibitorySynapses != NULL)
			delete InhibitorySynapses[Channel];
	}
	if (Neurons != NULL)
		delete [] Neurons;
	if (ExcitatorySynapses != NULL)
		delete [] ExcitatorySynapses;
	if (InhibitorySynapses != NULL)
		delete [] InhibitorySynapses;
}

int ZhouLSO::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	    return ReadParameters(ParameterFileName, "ZhouLSO");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int ZhouLSO::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

	// In case of spike data, set the FrameLength in the parameter file
    Status = theParamFile.GetParameter(SectionName, "FrameLength_Seconds", mFrameLength_Seconds, 0.0);
    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0.0);

	// parameters names from Zhou's manuscript (units added)
	// membrane parameters
    Status = theParamFile.GetParameter(SectionName, "C_m_Farads", mC_m_Farads, 31.4e-12); // membrane capacitance
    Status = theParamFile.GetParameter(SectionName, "G_L_Siemens", mG_L_Siemens, 0.0314e-6); // leakage conductance
    Status = theParamFile.GetParameter(SectionName, "E_L_Volts", mE_L_Volts, -0.065); // leakage reversal potential
    Status = theParamFile.GetParameter(SectionName, "G_abs_increment_Siemens", mG_abs_increment_Siemens, 10.0e-6); // absolute refractory conductance
    Status = theParamFile.GetParameter(SectionName, "E_abs_Volts", mE_abs_Volts, -0.065); // absolute refractory reversal potenital
    Status = theParamFile.GetParameter(SectionName, "t_abs_seconds", mt_abs_seconds, 0.002); // absolute refractory duration
    Status = theParamFile.GetParameter(SectionName, "G_AHP_increment_Siemens", mG_AHP_increment_Siemens, 0.02e-6); // relative refractory (afterhyperpolarization) conductance    
    Status = theParamFile.GetParameter(SectionName, "E_AHP_Volts", mE_AHP_Volts, -0.065); // relative refractory (afterhyperpolarization) reversal potential    
    Status = theParamFile.GetParameter(SectionName, "Tau_AHP_Seconds", mTau_AHP_Seconds, 0.020); // relative refractory (afterhyperpolarization) time constant    
	Status = theParamFile.GetParameter(SectionName, "V_th_Volts", mV_th_Volts, -0.050); // threshold voltage (activate G_abs and FiredThisStep)  
	Status = theParamFile.GetParameter(SectionName, "V_rest_Volts", mV_rest_Volts, -0.065); // resting potential (is this used??)  
    // input parameters (either current or input spikes, not both)
	// current input is provided using a waveform input stream
	// synaptic input parameters (the default values can be used during current inputs)
	Status = theParamFile.GetParameter(SectionName, "G_e_Siemens", mG_e_Siemens, 0.0e-9); // peak excitatory conductance increment per input spike
	Status = theParamFile.GetParameter(SectionName, "E_e_Volts", mE_e_Volts, 0.0); // reversal potential of excitatory conductance
	Status = theParamFile.GetParameter(SectionName, "Tau_e_rise_Seconds", mTau_e_rise_Seconds, 0.0001); // rise time constant of excitatory conductance
	Status = theParamFile.GetParameter(SectionName, "Tau_e_fall_Seconds", mTau_e_fall_Seconds, 0.001); // decay time constant of excitatory conductance
	Status = theParamFile.GetParameter(SectionName, "G_i_Siemens", mG_i_Siemens, 0.0e-9); // peak inhibitory conductance increment per input spike
	Status = theParamFile.GetParameter(SectionName, "E_i_Volts", mE_i_Volts, -0.070); // reversal potential of inhibitory conductance
	Status = theParamFile.GetParameter(SectionName, "Tau_i_rise_Seconds", mTau_i_rise_Seconds, 0.0001); // rise time constant of inhibitory conductance
	Status = theParamFile.GetParameter(SectionName, "Tau_i_fall_Seconds", mTau_i_fall_Seconds, 0.001); // decay time constant of inhibitory conductance

	// Optional outputs
	Status = theParamFile.GetParameter(SectionName, "VmemFilename", mVmemFilename, sizeof(mVmemFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "GeFilename", mGeFilename, sizeof(mGeFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "GiFilename", mGiFilename, sizeof(mGiFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "GabsFilename", mGabsFilename, sizeof(mGabsFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "GAHPFilename", mGAHPFilename, sizeof(mGAHPFilename), NULL);

	return 1;
}

int ZhouLSO::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	double FrameLength_mS;
	int CellCount;
	int Channel;

	cout << "    Start: " << ModuleName() << endl;

    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
    if ((NumInputs != 1) & (NumInputs != 2))
        throw EarlabException("%s: Currently this module can only handle one or two input streams.  Sorry!", ModuleName());

    if (NumOutputs != 1)
        throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", ModuleName());


	if (InputTypes[0] != WaveformData)
		if (InputTypes[0] != SpikeData)
			throw EarlabException("%s: Currently this module can handle only waveform or spike input data streams.  Sorry!", ModuleName());

//	if (InputTypes[1] != SpikeData)
//			throw EarlabException("%s: Currently this module can handle only waveform or spike input data streams.  Sorry!", ModuleName());

	InputDataType = InputTypes[0];

	if (OutputTypes[0] != SpikeData)
		throw EarlabException("%s: Currently this module can only handle spike output data streams.  Sorry!", ModuleName());

	if (InputTypes[0] == WaveformData)
	{
		if (InputSize[0][1] != OutputSize[0][0])
			throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", ModuleName());
	}

	if (InputTypes[0] == SpikeData)
	{
		if (InputSize[0][0] != OutputSize[0][0])
			throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", ModuleName());
	}

    if (InputSize[0][2] != 0)
        throw EarlabException("%s: Input data must be two-dimensional array.  Sorry!", ModuleName());

    if (OutputSize[0][1] != 0)
        throw EarlabException("%s: Output signal must be one-dimensional array.  Sorry!", ModuleName());

	CellCount = OutputSize[0][0];

	if (InputTypes[0] == WaveformData)
	{
		FrameLength_Samples = InputSize[0][0];
		NumChannels = InputSize[0][1];
		FrameLength_mS = ((1.0 / mSampleRate_Hz) * FrameLength_Samples) * 1000;
	}
	else
	{
		FrameLength_mS = 1000 * mFrameLength_Seconds;
		FrameLength_Samples = (int)(mFrameLength_Seconds * mSampleRate_Hz);
		NumChannels = InputSize[0][0];
		NumInputReps = InputSize[0][1];
	}

	OutputElementCounts[0] = (int)(10 * CellCount * FrameLength_mS);

	Neurons = new ZhouLSOCompartment *[NumChannels];

	if (InputDataType == SpikeData)
	{
		ExcitatorySynapses = new ZLSynapse1stOrderDiffc *[NumChannels];
		InhibitorySynapses = new ZLSynapse1stOrderDiffc *[NumChannels];
		SynapseList = new int *[NumChannels];
	}

	for (Channel = 0; Channel < NumChannels; Channel++)
	{
		Neurons[Channel] = new ZhouLSOCompartment(mSampleRate_Hz,  mC_m_Farads, 
		                            mG_L_Siemens, mE_L_Volts, 
									mG_abs_increment_Siemens, mE_abs_Volts, mt_abs_seconds, 
									mG_AHP_increment_Siemens, mE_AHP_Volts, mTau_AHP_Seconds, 
									mV_th_Volts, mV_rest_Volts,
									mE_e_Volts, mE_i_Volts, mG_e_Siemens, mG_i_Siemens, 
									mTau_e_rise_Seconds, mTau_e_fall_Seconds, mTau_i_rise_Seconds, mTau_i_fall_Seconds);

		if (InputDataType == SpikeData)
		{
			//initialize synapses!!!!
			ExcitatorySynapses[Channel] = new ZLSynapse1stOrderDiffc(mSampleRate_Hz, mTau_e_rise_Seconds, mTau_e_fall_Seconds, mG_e_Siemens, Neurons[Channel]);
			InhibitorySynapses[Channel] = new ZLSynapse1stOrderDiffc(mSampleRate_Hz, mTau_i_rise_Seconds, mTau_i_fall_Seconds, mG_i_Siemens, Neurons[Channel]);
			SynapseList[Channel] = new int [NumInputReps];
			for (int Rep = 0; Rep < NumInputReps; Rep++)
				SynapseList[Channel][Rep] = (Channel * NumInputReps) + Rep;
		}
	}
 


//	for (Channel = 0; Channel < NumChannels; Channel++)
//	{
//		MSOCells[Channel] = new COMPARTMENT_TYPE *[NumLags];
//		ExcitatorySynapses[Channel] = new HHSynapse *[NumLags];
//		InhibitorySynapses[Channel] = new HHSynapse *[NumLags]; //***NEW***
//		WasUnderThreshold[Channel] = new bool [NumLags];
//		SynapseList[Channel] = new int [NumInputReps];
//		for (int Rep = 0; Rep < NumInputReps; Rep++)
//			SynapseList[Channel][Rep] = (Channel * NumInputReps) + Rep;
//
//		for (int Lag = 0; Lag < NumLags; Lag++)
//		{
//			ExcitatorySynapses[Channel][Lag] = new HHSynapse(mSampleRate_Hz, mExcitatorySynapticTau, mExcitatorySynapticStrength);
//			InhibitorySynapses[Channel][Lag] = new HHSynapse (mSampleRate_Hz, mInhibitorySynapticTau,mInhibitorySynapticStrength); // ***NEW***
//			MSOCells[Channel][Lag] = new COMPARTMENT_TYPE(mSampleRate_Hz, -60.0e-3, mCellTemperature_DegreesC);
//			WasUnderThreshold[Channel][Lag] = true;
//		}
//	}



	if (strlen(mVmemFilename) != 0)
		mVmemFile = OpenOutputFile(mVmemFilename, "w");

	if (strlen(mGeFilename) != 0)
		mGeFile = OpenOutputFile(mGeFilename, "w");

	if (strlen(mGiFilename) != 0)
		mGiFile = OpenOutputFile(mGiFilename, "w");

	if (strlen(mGabsFilename) != 0)
		mGabsFile = OpenOutputFile(mGabsFilename, "w");

	if (strlen(mGAHPFilename) != 0)
		mGAHPFile = OpenOutputFile(mGAHPFilename, "w");

	return 1;
}

int ZhouLSO::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int counter = 0;
	//FloatMatrixN *Input;
	FloatMatrixN *InputWaveform;
	SpikeStream *ExcitatoryInputSpikeStream, *InhibitoryInputSpikeStream;
	SpikeStream *Output;
	int Sample, Channel, CellID, s;
	float Vmem_mV;
	float Ge_nS, Gi_nS, Gabs_nS, GAHP_nS;

	Log("    Advance: %s", ModuleName());

	Output = ((EarlabSpikeStream *)OutputStream[0])->GetData();	// Only supporting one output at the present moment

	if (InputDataType == WaveformData)
	{
		InputWaveform = ((EarlabWaveformStream *)InputStream[0])->GetData();	// Only supporting one input at the present moment
	}
	else // SpikeData
	{
		ExcitatoryInputSpikeStream =  ((EarlabSpikeStream *)InputStream[0])->GetData();
		ExcitatoryInputSpikeStream->SetSampleRate_Hz(mSampleRate_Hz);
		ExcitatoryInputSpikeStream->NewFrame();
		InhibitoryInputSpikeStream =  ((EarlabSpikeStream *)InputStream[1])->GetData();
		InhibitoryInputSpikeStream->SetSampleRate_Hz(mSampleRate_Hz);
		InhibitoryInputSpikeStream->NewFrame();
	}

	Output->SetSampleRate_Hz(mSampleRate_Hz);
	Output->NewFrame();

//	Sample = 0;
	for (Sample = 0; Sample < FrameLength_Samples; Sample++)
	{
		if (Sample == 333)
			s = 500;

		for (Channel = 0; Channel < NumChannels; Channel++)
		{
			if (InputDataType == WaveformData)
			{
				if (Neurons[Channel]->Step(InputWaveform->Data(Sample, Channel)))
				{
					Output->Fire(Channel);
					counter++;
				}
			}
			if (InputDataType == SpikeData)
			{
				CellID = Channel;
				//ExcitatorySynapses[Channel]->Fire(ExcitatoryInputSpikeStream->CountSpikes(CellID));
				//InhibitorySynapses[Channel]->Fire(InhibitoryInputSpikeStream->CountSpikes(CellID));
				ExcitatorySynapses[Channel]->Fire(ExcitatoryInputSpikeStream->CountSpikes(SynapseList[Channel],NumInputReps));
				InhibitorySynapses[Channel]->Fire(InhibitoryInputSpikeStream->CountSpikes(SynapseList[Channel],NumInputReps));
				ExcitatorySynapses[Channel]->Step();
				InhibitorySynapses[Channel]->Step();
				if (Neurons[Channel]->Step(&(ExcitatorySynapses[Channel]), 1, &(InhibitorySynapses[Channel]), 1))
					Output->Fire(CellID);
			}

				// Get and write the output waveforms to files optionally specified in the parameter file.
				// Note that data will be interleaved according to Channels.
				// The reshape function in Matlab can organize the data.
				Vmem_mV = (float) (Neurons[Channel]->GetMembraneVoltage_V() * 1000.0);
				Neurons[Channel]->GetConductances_nS(Ge_nS, Gi_nS, Gabs_nS, GAHP_nS);

				if (mVmemFile != NULL)
					fprintf(mVmemFile, "%f\n", Vmem_mV);

				if (mGeFile != NULL)
					fprintf(mGeFile, "%f\n", Ge_nS);

				if (mGiFile != NULL)
					fprintf(mGiFile, "%f\n", Gi_nS);

				if (mGabsFile != NULL)
					fprintf(mGabsFile, "%f\n", Gabs_nS);

				if (mGAHPFile != NULL)
					fprintf(mGAHPFile, "%f\n", GAHP_nS);

		} // for (Channel)
		Output->Step();
		if (InputDataType == SpikeData)
		{
			ExcitatoryInputSpikeStream->Step();
			InhibitoryInputSpikeStream->Step();
		}
	} // for (Sample)

	return counter + 1;
}

int ZhouLSO::Stop(void)
{
	cout << "    Stop: " << ModuleName() << endl;

    return 1;
}

int ZhouLSO::Unload(void)
{
	cout << "    Unload: " << ModuleName() << endl;

    return 1;
}

