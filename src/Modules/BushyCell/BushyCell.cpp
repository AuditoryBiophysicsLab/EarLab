#include <iostream>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include "Earlab.h"
#include "BushyCell.h"
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

BushyCell::BushyCell()
{
    memset(this, 0, sizeof(BushyCell));
	SetModuleName("BushyCell");
}

BushyCell::~BushyCell()
{
	int OutputRep;
	int Channel;

	for (OutputRep = 0; OutputRep < NumOutputReps; OutputRep++)
	{
		for (Channel = 0; Channel < NumChannels; Channel++)
			delete [] SynapseList[OutputRep][Channel];
		delete [] SynapseList[OutputRep];
	}
	delete [] SynapseList;

	for (Channel = 0; Channel < NumChannels; Channel++)
	{
		for (OutputRep = 0; OutputRep < NumOutputReps; OutputRep++)
		{
			delete BushyCells[Channel][OutputRep];
			delete Synapses[Channel][OutputRep];
		}

		delete [] BushyCells[Channel];
		delete [] Synapses[Channel];
		delete [] WasUnderThreshold[Channel];
	}
	delete [] BushyCells;
	delete [] Synapses;
	delete [] WasUnderThreshold;
}

int BushyCell::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	    return ReadParameters(ParameterFileName, "BushyCell");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int BushyCell::ReadParameters(const char *ParameterFileName, const char *SectionName)
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
	Status = theParamFile.GetParameter(SectionName, "MaxSynapseCount", mMaxSynapseCount, 0);
	Status = theParamFile.GetParameter(SectionName, "ResponseType", mResponseType, 2);
	Status = theParamFile.GetParameter(SectionName, "CompartmentClassName", mCompartmentClassName, sizeof(mCompartmentClassName), "Brughera96Compartment");
	if (!(stricmp(mCompartmentClassName, "Brughera96Compartment") == 0) && !(stricmp(mCompartmentClassName, "Rothman93Compartment") == 0))
			throw EarlabException("%s: Brughera96Compartment or Rothman93Compartment are the only valid compartments for BushyCell.  Sorry!", ModuleName());

	Status = theParamFile.GetParameter(SectionName, "VmemFilename", mVmemFilename, sizeof(mVmemFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "geFilename", mgeFilename, sizeof(mgeFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "giFilename", mgiFilename, sizeof(mgiFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "gNaFilename", mgNaFilename, sizeof(mgNaFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "gKHTFilename", mgKHTFilename, sizeof(mgKHTFilename), NULL);
	Status = theParamFile.GetParameter(SectionName, "gKLTFilename", mgKLTFilename, sizeof(mgKLTFilename), NULL);

	return 1;
}

int BushyCell::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	int CellCount;
	int Channel;

	cout << "    Start: " << ModuleName() << endl;

    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
    if (NumInputs != 1)
	throw EarlabException("%s: Currently this module can only handle one input stream.  Sorry!", ModuleName());

    if (NumOutputs != 1)
	throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", ModuleName());

	if (InputTypes[0] != SpikeData)
		throw EarlabException("%s: Currently this module can only handle spike input data streams.  Sorry!", ModuleName());

	if (OutputTypes[0] != SpikeData)
		throw EarlabException("%s: Currently this module can only handle spike output data streams.  Sorry!", ModuleName());

    if (InputSize[0][0] != OutputSize[0][0])
	throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", ModuleName());

    if (InputSize[0][2] != 0)
	throw EarlabException("%s: Input data must be two-dimensional array.  Sorry!", ModuleName());

    if (OutputSize[0][1] == 0)
	throw EarlabException("%s: Second dimension of output signal must be non-zero cells per channel.  Sorry!", ModuleName());

    if (OutputSize[0][2] != 0)
	throw EarlabException("%s: Output data must be two-dimensional array.  Sorry!", ModuleName());

	CellCount = OutputSize[0][0] * OutputSize[0][1];

	NumChannels = InputSize[0][0];
	NumInputReps = InputSize[0][1];
	NumOutputReps = OutputSize[0][1];

	if (InputSize[0][1] < mMaxSynapseCount)
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
	}

	OutputElementCounts[0] = (int)(CellCount * (mFrameLength_Seconds * 1000));

	BushyCells = new HHCompartment **[NumChannels];
	//BushyCells = new BUSHY_CELL_TYPE **[NumChannels];
	Synapses = new HHSynapse **[NumChannels];
	WasUnderThreshold = new bool *[NumChannels];

	for (Channel = 0; Channel < NumChannels; Channel++)
	{
		BushyCells[Channel] = new HHCompartment *[NumOutputReps];
		//BushyCells[Channel] = new BUSHY_CELL_TYPE *[NumOutputReps];
		Synapses[Channel] = new HHSynapse *[NumOutputReps];
		WasUnderThreshold[Channel] = new bool [NumOutputReps];
		for (int OutputRep = 0; OutputRep < NumOutputReps; OutputRep++)
		{
			Synapses[Channel][OutputRep] = new HHSynapse(mSampleRate_Hz, mSynapticTau, mSynapticStrength);
			newmResponseType = (ENUMED_RESPONSE_SET) mResponseType;
			BushyCells[Channel][OutputRep] = HHCompartmentFactory::Create(mCompartmentClassName, mSampleRate_Hz, -60.0e-3, mCellTemperature_DegreesC, TYPE2, newmResponseType, 1.0/mSampleRate_Hz, 0.0);
			//BushyCells[Channel][OutputRep] = new BUSHY_CELL_TYPE(mSampleRate_Hz, -60.0e-3, mCellTemperature_DegreesC, newmResponseType);
			WasUnderThreshold[Channel][OutputRep] = true;
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

	return 1;
}

int BushyCell::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int counter = 0;
	SpikeStream *Input;
	SpikeStream *Output;
	int Sample, Channel, Rep;
	int CellID;
	float Vmem_mV;
	//float ge_nS, gi_nS, gNa_nS, gKHT_nS, gKLT_nS;
	HHConductanceInfo **Conductances;
	int ConductanceCount;

	Log("    Advance: %s", ModuleName());

	Input =  ((EarlabSpikeStream *)InputStream[0])->GetData();
	Output = ((EarlabSpikeStream *)OutputStream[0])->GetData();

	Input->SetSampleRate_Hz(mSampleRate_Hz);
	Output->SetSampleRate_Hz(mSampleRate_Hz);
	Input->NewFrame();
	Output->NewFrame();
	for (Sample = 0; Sample < SamplesPerFrame; Sample++)
	{
		for (Channel = 0; Channel < NumChannels; Channel++)
		{
			for (Rep = 0; Rep < NumOutputReps; Rep++)
			{
				CellID = (Channel * NumOutputReps) + Rep;
				Synapses[Channel][Rep]->Fire(Input->CountSpikes(SynapseList[Rep][Channel], mMaxSynapseCount));
				Synapses[Channel][Rep]->Step();
				if (BushyCells[Channel][Rep]->Step(&(Synapses[Channel][Rep]), 1, NULL, 0))
					Output->Fire(CellID);

				// Get and write the output waveforms to files optionally specified in the parameter file.
				// Note that data will be interleaved according to Lags, Channels.
				// The reshape function in Matlab can organize the data.
				Vmem_mV = (float) (BushyCells[Channel][Rep]->GetMembraneVoltage_V() * 1000.0);
				if (mVmemFile != NULL)
					fprintf(mVmemFile, "%f\n", Vmem_mV);

				//BushyCells[Channel][Rep]->GetConductances_nS(ge_nS, gi_nS, gNa_nS, gKHT_nS, gKLT_nS);
				BushyCells[Channel][Rep]->GetConductances_nS(&ConductanceCount, &Conductances);
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


			} // for (Rep)
		} // for (Channel)
		Input->Step();
		Output->Step();
	} // for (Samplw)

	return counter + 1;
}

int BushyCell::Stop(void)
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

    return 1;
}

int BushyCell::Unload(void)
{
	cout << "    Unload: " << ModuleName() << endl;

    return 1;
}
