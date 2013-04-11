#include <iostream>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include "Earlab.h"
#include "ANCurrentDrivenSpikes.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "EarlabException.h"
#include <math.h>
#ifdef _WIN32
# include <windows.h>
#else
# include <unistd.h>
# define LARGE_INTEGER unsigned int
#endif

using namespace std;

ANCurrentDrivenSpikes::ANCurrentDrivenSpikes()
{
    memset(this, 0, sizeof(ANCurrentDrivenSpikes));
	SetModuleName("ANCurrentDrivenSpikes");
}

ANCurrentDrivenSpikes::~ANCurrentDrivenSpikes()
{
	int OutputRep;
	int Channel;

/*	for (OutputRep = 0; OutputRep < NumOutputReps; OutputRep++)
	{
		for (Channel = 0; Channel < NumChannels; Channel++)
			delete [] SynapseList[OutputRep][Channel];
		delete [] SynapseList[OutputRep];
	}
	delete [] SynapseList;
*/
	for (Channel = 0; Channel < NumChannels; Channel++)
	{
		for (OutputRep = 0; OutputRep < NumOutputReps; OutputRep++)
		{
			delete ANFibers[Channel][OutputRep];
			//delete Synapses[Channel][OutputRep];
		}

		delete [] ANFibers[Channel];
		//delete [] Synapses[Channel];
		delete [] WasUnderThreshold[Channel];
	}
	delete [] ANFibers;
	//delete [] Synapses;
	delete [] WasUnderThreshold;
}

int ANCurrentDrivenSpikes::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	    return ReadParameters(ParameterFileName, "ANCurrentDrivenSpikes");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int ANCurrentDrivenSpikes::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0.0);
    //Status = theParamFile.GetParameter(SectionName, "FrameLength_Seconds", mFrameLength_Seconds, 0.0);
    //Status = theParamFile.GetParameter(SectionName, "SynapticStrength", mSynapticStrength, 0.0);
    //Status = theParamFile.GetParameter(SectionName, "SynapticTau_Seconds", mSynapticTau, 0.0);
    Status = theParamFile.GetParameter(SectionName, "CellTemperature_DegreesC", mCellTemperature_DegreesC, 0.0);
	//Status = theParamFile.GetParameter(SectionName, "MaxSynapseCount", mMaxSynapseCount, 0);
	Status = theParamFile.GetParameter(SectionName, "ResponseType", mResponseType, 0);
	if (mResponseType == 0)
	{
		//typedef struct
		//{
		//	double MembraneCapacitance_F;
		//	double MaxG_Na;
		//	double MaxG_HT;
		//	double MaxG_LT;
		//	double MaxG_A;
		//	double MaxG_h;
		//	double MaxG_lk;
		//} Rothman03CustomParameterValues;

		/*Rothman03ConductanceParameters Rothman03ParameterSet_II = 
		{
			1000e-9,
			150e-9,
			200e-9,
			0e-9,
			20e-9,
			2e-9,
		};*/

		Status = theParamFile.GetParameter(SectionName, "CustomMembraneCapacitance_F", mRothman03CustomParameterValues.MembraneCapacitance_F, 12e-12);
		Status = theParamFile.GetParameter(SectionName, "CustomMaxG_Na", mRothman03CustomParameterValues.MaxG_Na, 1000e-9);
		Status = theParamFile.GetParameter(SectionName, "CustomMaxG_KHT", mRothman03CustomParameterValues.MaxG_HT, 150e-9);
		Status = theParamFile.GetParameter(SectionName, "CustomMaxG_KLT", mRothman03CustomParameterValues.MaxG_LT, 200e-9);
		Status = theParamFile.GetParameter(SectionName, "CustomMaxG_KA", mRothman03CustomParameterValues.MaxG_A, 0e-9);
		Status = theParamFile.GetParameter(SectionName, "CustomMaxG_h", mRothman03CustomParameterValues.MaxG_h, 20e-9);
		Status = theParamFile.GetParameter(SectionName, "CustomMaxG_lk", mRothman03CustomParameterValues.MaxG_lk, 2e-9);
	}

    Status = theParamFile.GetParameter(SectionName, "AHPIncrement_Siemens", mAHPIncrement_Siemens, 0.0);
	Status = theParamFile.GetParameter(SectionName, "TauAHP_Seconds", mTauAHP_Seconds, 1.0/mSampleRate_Hz);
	if (mAHPIncrement_Siemens == 0.0)
		mCompartmentClassName = "Rothman03Compartment";
	else
		mCompartmentClassName = "Rothman03AHPCompartment";

	Status = theParamFile.GetParameter(SectionName, "VmemFilename", mVmemFilename, sizeof(mVmemFilename), NULL);
	//Status = theParamFile.GetParameter(SectionName, "geFilename", mgeFilename, sizeof(mgeFilename), NULL);
	//Status = theParamFile.GetParameter(SectionName, "giFilename", mgiFilename, sizeof(mgiFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "gNaFilename", mgNaFilename, sizeof(mgNaFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "gKAFilename", mgKAFilename, sizeof(mgKAFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "gKHTFilename", mgKHTFilename, sizeof(mgKHTFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "gKLTFilename", mgKLTFilename, sizeof(mgKLTFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "ghFilename", mghFilename, sizeof(mghFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "gAHPFilename", mgAHPFilename, sizeof(mgAHPFilename), NULL);

    return 1;
}

int ANCurrentDrivenSpikes::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	double FrameLength_mS;
	int CellCount;
	int Channel, OutputRep;

	cout << "    Start: " << ModuleName() << endl;

    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
    if (NumInputs != 1)
	throw EarlabException("%s: Currently this module can only handle one input stream.  Sorry!", ModuleName());

    if (NumOutputs != 1)
	throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", ModuleName());

	if (InputTypes[0] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform input data streams.  Sorry!", ModuleName());

	if (OutputTypes[0] != SpikeData)
		throw EarlabException("%s: Currently this module can only handle spike output data streams.  Sorry!", ModuleName());

    if (InputSize[0][1] != OutputSize[0][0])
        throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", ModuleName());

    if (InputSize[0][2] != OutputSize[0][1])
        throw EarlabException("%s: Number of input waveforms per channel must equal the number of output cells per channel.  Sorry!", ModuleName());

	// Input data:
	// dimension 0: number of samples per frame
	// dimension 1: frequency channel
	// dimension 2: rep (number of input current waveforms per frequency channel)
	if (InputSize[0][3] != 0)
		throw EarlabException("%s: Input data must be three-dimensional array or less.  Sorry!", ModuleName());

    if (OutputSize[0][1] == 0)
	throw EarlabException("%s: Second dimension of output signal must be non-zero cells per channel.  Sorry!", ModuleName());

    if (OutputSize[0][2] != 0)
	throw EarlabException("%s: Output data must be two-dimensional array.  Sorry!", ModuleName());

	FrameLength_Samples = InputSize[0][0];
	NumChannels = InputSize[0][1];
	NumOutputReps = OutputSize[0][1];
	FrameLength_mS = ((1.0 / mSampleRate_Hz) * FrameLength_Samples) * 1000;
	//mFrameLength_Seconds = ((1.0 / mSampleRate_Hz) * FrameLength_Samples);

	CellCount = OutputSize[0][0] * OutputSize[0][1];

	// From BushyCell2:
	//NumChannels = InputSize[0][0];
	//NumInputReps = InputSize[0][1];
	//NumOutputReps = OutputSize[0][1];

/*	if (InputSize[0][1] < mMaxSynapseCount)
	{
		//mMaxSynapseCount = InputSize[0][1];
		throw EarlabException("%s: Warning: Max synapse count %d specified in parameter file, but only %d input cells are available", mMaxSynapseCount, InputSize[0][1]);
	}
	else
	{
		int OutputRep, Channel, CurInputCell, CurSynapse, CurInputCellID;;
		LARGE_INTEGER seed;
		double RandomNumber;
		int RandomIndex;
		bool Duplicate;

#ifndef _WIN32
		seed = time(0)*getpid();
		srand(seed);
#else
		QueryPerformanceCounter(&seed);
		srand(seed.LowPart);
#endif
		SynapseList = new int **[NumOutputReps];
		for (OutputRep = 0; OutputRep < NumOutputReps; OutputRep++)
		{
			SynapseList[OutputRep] = new int *[NumChannels];
			for (Channel = 0; Channel < NumChannels; Channel++)
			{
				SynapseList[OutputRep][Channel] = new int [mMaxSynapseCount];
				for (CurSynapse = 0; CurSynapse < mMaxSynapseCount; CurSynapse++)
					SynapseList[OutputRep][Channel][CurSynapse] = -1;
				CurSynapse = 0;
				// Make sure we fill the required number with non-repeating input cell IDs
				while (CurSynapse < mMaxSynapseCount)
				{
					// Generate a random number between zero and the number of potential input cells
					RandomNumber = (((double)rand() / (double)RAND_MAX) * (double)NumInputReps);
					// Make that floating point number into an integer, which we will use as the index of the potential input cell to the current bushy cell
					RandomIndex = (int)RandomNumber;
					CurInputCellID = (Channel * NumInputReps) + RandomIndex;

					// Make sure this input cell index has not already been used for the current bushy cell
					Duplicate = false;
					for (CurInputCell = 0; CurInputCell < CurSynapse; CurInputCell++)
					{
						if (CurInputCellID == SynapseList[OutputRep][Channel][CurInputCell])
						{
							Duplicate = true;
							break;
						}
					}
					// If the index has not already been used, use it, increment the count of input cells found, and pick another one until we have enough
					if (!Duplicate)
					{
						SynapseList[OutputRep][Channel][CurSynapse] = CurInputCellID;
						CurSynapse++;
					}
				}
			}
		}
	} */

	//OutputElementCounts[0] = (int)(CellCount * (mFrameLength_Seconds * 1000));
	OutputElementCounts[0] = (int)(CellCount * FrameLength_mS);

	ANFibers = new HHCompartment **[NumChannels];
	//Synapses = new HHSynapse **[NumChannels];
	WasUnderThreshold = new bool *[NumChannels];

	for (Channel = 0; Channel < NumChannels; Channel++)
	{
		ANFibers[Channel] = new HHCompartment *[NumOutputReps];
		//Synapses[Channel] = new HHSynapse *[NumOutputReps];
		WasUnderThreshold[Channel] = new bool [NumOutputReps];
		for (OutputRep = 0; OutputRep < NumOutputReps; OutputRep++)
		{
			//Synapses[Channel][OutputRep] = new HHSynapse(mSampleRate_Hz, mSynapticTau, mSynapticStrength);
			newmResponseType = (ENUMED_RESPONSE_SET) mResponseType;
			if (newmResponseType==0)
				ANFibers[Channel][OutputRep] = HHCompartmentFactory::Create(mCompartmentClassName, mSampleRate_Hz, -64.0e-3, mCellTemperature_DegreesC, newmResponseType, TYPE_II, mTauAHP_Seconds, mAHPIncrement_Siemens, mRothman03CustomParameterValues);
			else
				ANFibers[Channel][OutputRep] = HHCompartmentFactory::Create(mCompartmentClassName, mSampleRate_Hz, -64.0e-3, mCellTemperature_DegreesC, newmResponseType, TYPE_II, mTauAHP_Seconds, mAHPIncrement_Siemens);
			WasUnderThreshold[Channel][OutputRep] = true;
		}
	}

	//SamplesPerFrame = (int)(mFrameLength_Seconds * mSampleRate_Hz);

	if (strlen(mVmemFilename) != 0)
		mVmemFile = OpenOutputFile(mVmemFilename, "w");

	//if (strlen(mgeFilename) != 0)
	//	mgeFile = OpenOutputFile(mgeFilename, "w");

	//if (strlen(mgiFilename) != 0)
	//	mgiFile = OpenOutputFile(mgiFilename, "w");

	if (strlen(mgNaFilename) != 0)
		mgNaFile = OpenOutputFile(mgNaFilename, "w");

	if (strlen(mgKAFilename) != 0)
		mgKAFile = OpenOutputFile(mgKAFilename, "w");

	if (strlen(mgKHTFilename) != 0)
		mgKHTFile = OpenOutputFile(mgKHTFilename, "w");

	if (strlen(mgKLTFilename) != 0)
		mgKLTFile = OpenOutputFile(mgKLTFilename, "w");

	if (strlen(mghFilename) != 0)
		mghFile = OpenOutputFile(mghFilename, "w");

	if (strlen(mgAHPFilename) != 0)
		mgAHPFile = OpenOutputFile(mgAHPFilename, "w");


	return 1;
}

int ANCurrentDrivenSpikes::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int counter = 0;
	FloatMatrixN *Input;  // InputCurrent_Amperes
	SpikeStream *Output;
	int Sample, Channel, Rep, CellID;
	float CurInputCurrent_Amperes;

//	int counter = 0;
//	SpikeStream *Input;
//	SpikeStream *Output;
//	int Sample, Channel, Rep;
//	int CellID;
	float Vmem_mV;
	HHConductanceInfo **Conductances;
	int ConductanceCount;
	int Pulse = 0;

	Log("    Advance: %s", ModuleName());

	Input = ((EarlabWaveformStream *)InputStream[0])->GetData();
	Output = ((EarlabSpikeStream *)OutputStream[0])->GetData();

	//Input->SetSampleRate_Hz(mSampleRate_Hz);
	Output->SetSampleRate_Hz(mSampleRate_Hz);
	//Input->NewFrame();
	Output->NewFrame();
	for (Sample = 0; Sample < FrameLength_Samples; Sample++)
	{
		for (Channel = 0; Channel < NumChannels; Channel++)
		{
			for (Rep = 0; Rep < NumOutputReps; Rep++)
			{
				CellID = (Channel * NumOutputReps) + Rep;
				//CurInput = Input->Data(Sample, Channel, Rep);

				// Set the injected current value
				CurInputCurrent_Amperes = Input->Data(Sample, Channel, Rep);
				if (CurInputCurrent_Amperes > 1e-15)
					Pulse = 1;
				else
					Pulse = 0;
				ANFibers[Channel][Rep]->SetCurrentInjection_A(CurInputCurrent_Amperes);

				//Synapses[Channel][Rep]->Fire(Input->CountSpikes(SynapseList[Rep][Channel], mMaxSynapseCount));
				//Synapses[Channel][Rep]->Step();
				if (ANFibers[Channel][Rep]->Step(NULL, 0, NULL, 0))
					Output->Fire(CellID);

				// Get and write the output waveforms to files optionally specified in the parameter file.
				// Note that data will be interleaved according to Reps, Channels.
				// The reshape function in Matlab can organize the data.
				Vmem_mV = (float) (ANFibers[Channel][Rep]->GetMembraneVoltage_V() * 1000.0);
				if (mVmemFile != NULL)
					fprintf(mVmemFile, "%f\n", Vmem_mV);

				ANFibers[Channel][Rep]->GetConductances_nS(&ConductanceCount, &Conductances);
				for (int i = 0; i < ConductanceCount; i++)
				{

					//if ((mgeFile != NULL) && (stricmp(Conductances[i]->ConductanceName, "ge") == 0))
					//	fprintf(mgeFile, "%f\n", Conductances[i]->Conductance_nS);

					//if ((mgiFile != NULL) && (stricmp(Conductances[i]->ConductanceName, "gi") == 0))
					//	fprintf(mgiFile, "%f\n", Conductances[i]->Conductance_nS);

					if ((mgNaFile != NULL) && (stricmp(Conductances[i]->ConductanceName, "gNa") == 0))
						fprintf(mgNaFile, "%f\n", Conductances[i]->Conductance_nS);

					if ((mgKAFile != NULL) && (stricmp(Conductances[i]->ConductanceName, "gKA") == 0))
						fprintf(mgKAFile, "%f\n", Conductances[i]->Conductance_nS);

					if ((mgKHTFile != NULL) && (stricmp(Conductances[i]->ConductanceName, "gKHT") == 0))
						fprintf(mgKHTFile, "%f\n", Conductances[i]->Conductance_nS);

					if ((mgKLTFile != NULL) && (stricmp(Conductances[i]->ConductanceName, "gKLT") == 0))
						fprintf(mgKLTFile, "%f\n", Conductances[i]->Conductance_nS);

					if ((mghFile != NULL) && (stricmp(Conductances[i]->ConductanceName, "gh") == 0))
						fprintf(mghFile, "%f\n", Conductances[i]->Conductance_nS);

					if ((mgAHPFile != NULL) && (stricmp(Conductances[i]->ConductanceName, "gAHP") == 0))
						fprintf(mgAHPFile, "%f\n", Conductances[i]->Conductance_nS);
				}
			}

		}
		//Input->Step();
		Output->Step();
	}

	return counter + 1;
}

int ANCurrentDrivenSpikes::Stop(void)
{
	cout << "    Stop: " << ModuleName() << endl;

	if (mVmemFile != NULL)
		CloseFile(mVmemFile);

	//if (mgeFile != NULL)
	//	CloseFile(mgeFile);

	//if (mgiFile != NULL)
	//	CloseFile(mgiFile);

	if (mgNaFile != NULL)
		CloseFile(mgNaFile);

	if (mgKAFile != NULL)
		CloseFile(mgKAFile);

	if (mgKHTFile != NULL)
		CloseFile(mgKHTFile);

	if (mgKLTFile != NULL)
		CloseFile(mgKLTFile);

	if (mghFile != NULL)
		CloseFile(mghFile);

	if (mgAHPFile != NULL)
		CloseFile(mgAHPFile);

    return 1;
}

int ANCurrentDrivenSpikes::Unload(void)
{
	cout << "    Unload: " << ModuleName() << endl;

    return 1;
}
