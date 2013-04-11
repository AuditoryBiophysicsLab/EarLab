#include <iostream>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include "Earlab.h"
#include "VoigtDCNCell.h"
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

VoigtDCNCell::VoigtDCNCell()
{
    memset(this, 0, sizeof(VoigtDCNCell));
	SetModuleName("VoigtDCNCell");
}

VoigtDCNCell::~VoigtDCNCell()
{
	int CellNumInThisChan;
	int Channel;

	//for (CellNumInThisChan = 0; CellNumInThisChan < NumOutputReps; CellNumInThisChan++)
	//{
	//	for (Channel = 0; Channel < NumChannels; Channel++)
	//	{
	//		//delete [] SynapseList[OutputRep][Channel];
	//	}
	//	//delete [] SynapseList[OutputRep];
	//}
	//delete [] SynapseList;

	delete SpikeMapExcitatory;
	delete SpikeMapInhibitory1;
	delete SpikeMapInhibitory2;



	for (Channel = 0; Channel < NumOutputFreqChans; Channel++)
	{
		for (CellNumInThisChan = 0; CellNumInThisChan < NumOutputCellsPerFreqChan; CellNumInThisChan++)
		{
			delete MGCells[Channel][CellNumInThisChan];
			delete ExcitatorySynapses[Channel][CellNumInThisChan];
			delete InhibitorySynapses1[Channel][CellNumInThisChan];
			delete InhibitorySynapses2[Channel][CellNumInThisChan];
		}

		delete [] MGCells[Channel];
		delete [] ExcitatorySynapses[Channel];
		delete [] InhibitorySynapses1[Channel];
		delete [] InhibitorySynapses2[Channel];
		delete [] WasUnderThreshold[Channel];
	}
	delete [] MGCells;
	delete [] ExcitatorySynapses;
	delete [] InhibitorySynapses1;
	delete [] InhibitorySynapses2;
	delete [] WasUnderThreshold;
}

int VoigtDCNCell::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	    return ReadParameters(ParameterFileName, "VoigtDCNCell");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int VoigtDCNCell::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0.0);
    Status = theParamFile.GetParameter(SectionName, "FrameLength_Seconds", mFrameLength_Seconds, 0.0);

    Status = theParamFile.GetParameter(SectionName, "Tau_m_Seconds", Tau_m_Seconds, 0.005);
    Status = theParamFile.GetParameter(SectionName, "b_K", b_K, 2.0);
    Status = theParamFile.GetParameter(SectionName, "V_th_Volts", V_th_Volts, 0.004);
    Status = theParamFile.GetParameter(SectionName, "Tau_K_Seconds", Tau_K_Seconds, 0.001);
    Status = theParamFile.GetParameter(SectionName, "E_K_Volts", E_K_Volts, -0.01);
    Status = theParamFile.GetParameter(SectionName, "E_ex_Volts", E_ex_Volts, 0.07);
    Status = theParamFile.GetParameter(SectionName, "E_in_Volts", E_in_Volts, -0.01);

    //Status = theParamFile.GetParameter(SectionName, "EPSP_Strength", EPSP_strength, 0.0);
    Status = theParamFile.GetParameter(SectionName, "EPSP_Strength", mEPSP_strength, 0.0);
    Status = theParamFile.GetParameter(SectionName, "EPSP_StrengthNSA", mEPSP_strengthNSA, 0.0);
    Status = theParamFile.GetParameter(SectionName, "IPSP_Strength1", mIPSP_strength1, 0.0);
    Status = theParamFile.GetParameter(SectionName, "IPSP_Strength2", mIPSP_strength2, 0.0);
    //Status = theParamFile.GetParameter(SectionName, "SynapticTau_Seconds", SynapticTau_Seconds, 0.0);
    Status = theParamFile.GetParameter(SectionName, "ExcitatorySynapticTau_Seconds", mExcitatorySynapticTau_Seconds, 0.0);
    Status = theParamFile.GetParameter(SectionName, "ExcitatorySynapticTau_SecondsNSA", mExcitatorySynapticTau_SecondsNSA, 0.0);
    Status = theParamFile.GetParameter(SectionName, "InhibitorySynapticTau_Seconds1", mInhibitorySynapticTau_Seconds1, 0.0);
    Status = theParamFile.GetParameter(SectionName, "InhibitorySynapticTau_Seconds2", mInhibitorySynapticTau_Seconds2, 0.0);
	//Status = theParamFile.GetParameter(SectionName, "MaxSynapseCount", mMaxSynapseCount, 0);
	//Status = theParamFile.GetParameter(SectionName, "SynapseCountExcitatory", mSynapseCountExcitatory, 0);
	//Status = theParamFile.GetParameter(SectionName, "SynapseCountInhibitory1", mSynapseCountInhibitory1, 0);
	//Status = theParamFile.GetParameter(SectionName, "SynapseCountInhibitory2", mSynapseCountInhibitory2, 0);

	//Status = theParamFile.GetParameter(SectionName, "InputFileName", mInputFileName, sizeof(mInputFileName), NULL);
	//if (mInputFileName[0] == '\0')
	//	throw EarlabException("%s: No input file name was specified.  Parameter file: %s", ModuleName(), ParameterFileName);

	Status = theParamFile.GetParameter(SectionName, "SpikeMapFileNameExcitatory", mSpikeMapFileNameExcitatory, sizeof(mSpikeMapFileNameExcitatory), NULL);
	if (mSpikeMapFileNameExcitatory[0] == '\0')
		throw EarlabException("%s: No SpikeMapFileNameExcitatory was specified.  Parameter file: %s", ModuleName(), ParameterFileName);

	Status = theParamFile.GetParameter(SectionName, "SpikeMapFileNameInhibitory1", mSpikeMapFileNameInhibitory1, sizeof(mSpikeMapFileNameInhibitory1), NULL);
	if (mSpikeMapFileNameInhibitory1[0] == '\0')
		throw EarlabException("%s: No SpikeMapFileNameInhibitory1 was specified.  Parameter file: %s", ModuleName(), ParameterFileName);

	Status = theParamFile.GetParameter(SectionName, "SpikeMapFileNameInhibitory2", mSpikeMapFileNameInhibitory2, sizeof(mSpikeMapFileNameInhibitory2), NULL);
	if (mSpikeMapFileNameInhibitory2[0] == '\0')
		throw EarlabException("%s: No SpikeMapFileNameInhibitory2 was specified.  Parameter file: %s", ModuleName(), ParameterFileName);


	//Status = theParamFile.GetParameter(SectionName, "SpikeMapFileExcitatory", mSpikeMapFileExcitatory, 19, "DefaultSpikeMap.txt");
	//Status = theParamFile.GetParameter(SectionName, "SpikeMapFileInhibitory1", mSpikeMapFileInhibitory1, 19, "DefaultSpikeMap.txt");
	//Status = theParamFile.GetParameter(SectionName, "SpikeMapFileInhibitory2", mSpikeMapFileInhibitory2, 19, "DefaultSpikeMap.txt");
    return 1;
}

int VoigtDCNCell::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	int CellCount, CellNumInThisChan, Channel;
	// Set the CarneySpikeGenerator parameters for no refractoriness
	double c0 = 0.0;
	double c1 = 0.0;
	double s0 = 1.0;
	double s1 = 1.0;
	double Ra = 0.0;
	double Scale = 1.0;
	double Offset = 0.0;

	cout << "    Start: " << ModuleName() << endl;

    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
    if (NumInputs != 3)
	throw EarlabException("%s: Currently this module can only handle exactly three input streams.  Sorry!", ModuleName());

    if (NumOutputs != 1)
	throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", ModuleName());

	if (InputTypes[0] != SpikeData)
		throw EarlabException("%s: Currently this module can only handle spike input data streams.  Sorry!", ModuleName());

	if (OutputTypes[0] != SpikeData)
		throw EarlabException("%s: Currently this module can only handle spike output data streams.  Sorry!", ModuleName());

    //if (InputSize[0][0] != OutputSize[0][0])
	//throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", ModuleName());
	// *** Check these sizes independently against the SpikeMaps (below)

    if (InputSize[0][2] != 0)
	throw EarlabException("%s: Input data must be two-dimensional array.  Sorry!", ModuleName());

    if (OutputSize[0][1] == 0)
	throw EarlabException("%s: Second dimension of output signal must be non-zero cells per channel.  Sorry!", ModuleName());

    if (OutputSize[0][2] != 0)
	throw EarlabException("%s: Output data must be two-dimensional array.  Sorry!", ModuleName());

	CellCount = OutputSize[0][0] * OutputSize[0][1];

	//NumChannels = InputSize[0][0];
	//NumInputReps = InputSize[0][1];
	//NumOutputReps = OutputSize[0][1];

	NumInputFreqChansExcitatory = InputSize[0][0]; // each element is the input spike stream from one cell
	NumInputFreqChansInhibitory1 = InputSize[1][0];
	NumInputFreqChansInhibitory2 = InputSize[2][0];
	NumOutputFreqChans = OutputSize[0][0];

	// The number of reps per channel is likely 1 (for inputs and outputs),
	// but higher numbers are supported.
	NumInputCellsPerFreqChanExcitatory = InputSize[0][1];
	NumInputCellsPerFreqChanInhibitory1 = InputSize[1][1];
	NumInputCellsPerFreqChanInhibitory2 = InputSize[2][1];
	NumOutputCellsPerFreqChan = OutputSize[0][1];

	//And each Input Stream requires a SpikeStreamMap
	//to select the inputs that synapse onto each OutputCell

	SpikeMapExcitatory = new SpikeStreamMapper(NumInputFreqChansExcitatory, NumOutputFreqChans, mSpikeMapFileNameExcitatory);
	SpikeMapInhibitory1 = new SpikeStreamMapper(NumInputFreqChansInhibitory1, NumOutputFreqChans, mSpikeMapFileNameInhibitory1);
	SpikeMapInhibitory2 = new SpikeStreamMapper(NumInputFreqChansInhibitory2, NumOutputFreqChans, mSpikeMapFileNameInhibitory2);






//	if (InputSize[0][1] < mMaxSynapseCount)
//	{
//		//mMaxSynapseCount = InputSize[0][1];
//		throw EarlabException("%s: Warning: Max synapse count %d specified in parameter file, but only %d input cells are available", mMaxSynapseCount, InputSize[0][1]);
//	}
//	else
//	{
//		int OutputRep, Channel, CurInputCell, CurSynapse, CurInputCellID;;
//		LARGE_INTEGER seed;
//		double RandomNumber;
//		int RandomIndex;
//		bool Duplicate;
//
//#ifndef _WIN32
//		seed = time(0)*getpid();
//		srand(seed);
//#else
//		QueryPerformanceCounter(&seed);
//		srand(seed.LowPart);
//#endif
//		SynapseList = new int **[NumOutputReps];
//		for (OutputRep = 0; OutputRep < NumOutputReps; OutputRep++)
//		{
//			SynapseList[OutputRep] = new int *[NumChannels];
//			for (Channel = 0; Channel < NumChannels; Channel++)
//			{
//				SynapseList[OutputRep][Channel] = new int [mMaxSynapseCount];
//				for (CurSynapse = 0; CurSynapse < mMaxSynapseCount; CurSynapse++)
//					SynapseList[OutputRep][Channel][CurSynapse] = -1;
//				CurSynapse = 0;
//				// Make sure we fill the required number with non-repeating input cell IDs
//				while (CurSynapse < mMaxSynapseCount)
//				{
//					// Generate a random number between zero and the number of potential input cells
//					RandomNumber = (((double)rand() / (double)RAND_MAX) * (double)NumInputReps);
//					// Make that floating point number into an integer, which we will use as the index of the potential input cell to the current bushy cell
//					RandomIndex = (int)RandomNumber;
//					CurInputCellID = (Channel * NumInputReps) + RandomIndex;
//
//					// Make sure this input cell index has not already been used for the current bushy cell
//					Duplicate = false;
//					for (CurInputCell = 0; CurInputCell < CurSynapse; CurInputCell++)
//					{
//						if (CurInputCellID == SynapseList[OutputRep][Channel][CurInputCell])
//						{
//							Duplicate = true;
//							break;
//						}
//					}
//					// If the index has not already been used, use it, increment the count of input cells found, and pick another one until we have enough
//					if (!Duplicate)
//					{
//						SynapseList[OutputRep][Channel][CurSynapse] = CurInputCellID;
//						CurSynapse++;
//					}
//				}
//			}
//		}
//	}

	OutputElementCounts[0] = (int)(CellCount * (mFrameLength_Seconds * 1000));

	MGCells = new MGCompartment **[NumOutputFreqChans];
	//Synapses = new MGSynapseFirstOrder **[NumChannels];
	ExcitatorySynapses = new MGSynapseFirstOrder **[NumOutputFreqChans];
	ExcitatorySynapsesNSA = new MGSynapseFirstOrder **[NumOutputFreqChans];
	InhibitorySynapses1 = new MGSynapseFirstOrder **[NumOutputFreqChans];
	InhibitorySynapses2 = new MGSynapseFirstOrder **[NumOutputFreqChans];
	WasUnderThreshold = new bool *[NumOutputFreqChans];
	if (mEPSP_strengthNSA > 0.0)
	{
		NSAinputs = new CarneySpikeGenerator **[NumOutputFreqChans];
		//ExcitatorySynapsesNSA = new MGSynapseFirstOrder **[NumOutputFreqChans];
	}

	for (Channel = 0; Channel < NumOutputFreqChans; Channel++)
	{
		MGCells[Channel] = new MGCompartment *[NumOutputCellsPerFreqChan];
		//Synapses[Channel] = new MGSynapseFirstOrder *[NumOutputReps];
		ExcitatorySynapses[Channel] = new MGSynapseFirstOrder *[NumOutputCellsPerFreqChan];
		ExcitatorySynapsesNSA[Channel] = new MGSynapseFirstOrder *[NumOutputCellsPerFreqChan];
		InhibitorySynapses1[Channel] = new MGSynapseFirstOrder *[NumOutputCellsPerFreqChan];
		InhibitorySynapses2[Channel] = new MGSynapseFirstOrder *[NumOutputCellsPerFreqChan];
		WasUnderThreshold[Channel] = new bool [NumOutputCellsPerFreqChan];
		if (mEPSP_strengthNSA > 0.0)
		{
			NSAinputs[Channel] = new CarneySpikeGenerator *[NumOutputCellsPerFreqChan];
		}

		for (CellNumInThisChan = 0; CellNumInThisChan < NumOutputCellsPerFreqChan; CellNumInThisChan++)
		{
			MGCells[Channel][CellNumInThisChan] = new MGCompartment(mSampleRate_Hz, Tau_m_Seconds, b_K, V_th_Volts, Tau_K_Seconds, E_K_Volts, E_ex_Volts, E_in_Volts);
			//Synapses[Channel][OutputRep] = new MGSynapseFirstOrder(mSampleRate_Hz, SynapticTau_Seconds, EPSP_strength, MGCells[Channel][OutputRep]);
			ExcitatorySynapses[Channel][CellNumInThisChan] = new MGSynapseFirstOrder(mSampleRate_Hz, mExcitatorySynapticTau_Seconds, mEPSP_strength, MGCells[Channel][CellNumInThisChan]);
			ExcitatorySynapsesNSA[Channel][CellNumInThisChan] = new MGSynapseFirstOrder(mSampleRate_Hz, mExcitatorySynapticTau_SecondsNSA, mEPSP_strengthNSA, MGCells[Channel][CellNumInThisChan]);
			InhibitorySynapses1[Channel][CellNumInThisChan] = new MGSynapseFirstOrder(mSampleRate_Hz, mInhibitorySynapticTau_Seconds1, mIPSP_strength1, MGCells[Channel][CellNumInThisChan]);
			InhibitorySynapses2[Channel][CellNumInThisChan] = new MGSynapseFirstOrder(mSampleRate_Hz, mInhibitorySynapticTau_Seconds2, mIPSP_strength2, MGCells[Channel][CellNumInThisChan]);
			WasUnderThreshold[Channel][CellNumInThisChan] = true;
			if (mEPSP_strengthNSA > 0.0)
			{
				NSAinputs[Channel][CellNumInThisChan] = new CarneySpikeGenerator(mSampleRate_Hz, c0, c1, s0, s1, Ra, Scale, Offset);
			}
		}
	}

	SamplesPerFrame = (int)(mFrameLength_Seconds * mSampleRate_Hz);

	return 1;
}

int VoigtDCNCell::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int counter = 0;
	SpikeStream *InputExcitatory, *InputInhibitory1, *InputInhibitory2;
	SpikeStream *Output;
	int Sample, Channel, CellNumInThisChan;
	int CellID, NumCountedSpikesExcitatory, NumCountedSpikesInhibitory1, NumCountedSpikesInhibitory2;
	//int NumCountedSpikesExcitatoryNSA;
	float NominalInputSpikeRateNSA = 120.0;
	bool SpikeOccurred = false;

	Log("    Advance: %s", ModuleName());

	InputExcitatory =  ((EarlabSpikeStream *)InputStream[0])->GetData();
	InputInhibitory1 =  ((EarlabSpikeStream *)InputStream[1])->GetData();
	InputInhibitory2 =  ((EarlabSpikeStream *)InputStream[2])->GetData();
	Output = ((EarlabSpikeStream *)OutputStream[0])->GetData();

	InputExcitatory->SetSampleRate_Hz(mSampleRate_Hz);
	InputInhibitory1->SetSampleRate_Hz(mSampleRate_Hz);
	InputInhibitory2->SetSampleRate_Hz(mSampleRate_Hz);
	Output->SetSampleRate_Hz(mSampleRate_Hz);
	
	InputExcitatory->NewFrame();
	InputInhibitory1->NewFrame();
	InputInhibitory2->NewFrame();
	Output->NewFrame();

	for (Sample = 0; Sample < SamplesPerFrame; Sample++)
	{
		for (Channel = 0; Channel < NumOutputFreqChans; Channel++)
		{
			for (CellNumInThisChan = 0; CellNumInThisChan < NumOutputCellsPerFreqChan; CellNumInThisChan++)
			{
				CellID = (Channel * NumOutputCellsPerFreqChan) + CellNumInThisChan;
				//Synapses[Channel][CellNumInThisChan]->Fire(Input->CountSpikes(SynapseList[CellNumInThisChan][Channel], mMaxSynapseCount));
				//Synapses[Channel][CellNumInThisChan]->Step();
				NumCountedSpikesExcitatory = SpikeMapExcitatory->CountMappedSpikes(InputExcitatory, CellID);
				//if (NumCountedSpikesExcitatory > 0)
				//	SpikeOccurred = true;
				ExcitatorySynapses[Channel][CellNumInThisChan]->Fire(NumCountedSpikesExcitatory);
				ExcitatorySynapses[Channel][CellNumInThisChan]->Step();

				if (mEPSP_strengthNSA > 0.0)
				{
					//NumCountedSpikesExcitatoryNSA = 0;
					// Use the CarneySpikeGenerator to check if an input spike from a non-specific afferent (NSA) occurs
					if (NSAinputs[Channel][CellNumInThisChan]->Step(NominalInputSpikeRateNSA))  
					{
						//NumCountedSpikesExcitatoryNSA = 1; 
						//ExcitatorySynapsesNSA[Channel][CellNumInThisChan]->Fire(NumCountedSpikesExcitatoryNSA);
						// Only one spike per sample can occur.  Seems fine at 120 spikes/s and 10^5 samples/s.
						ExcitatorySynapsesNSA[Channel][CellNumInThisChan]->Fire();
						ExcitatorySynapsesNSA[Channel][CellNumInThisChan]->Step();
					}
				}

				NumCountedSpikesInhibitory1 = SpikeMapInhibitory1->CountMappedSpikes(InputInhibitory1, CellID);
				//if (NumCountedSpikesInhibitory1 > 0)
				//	SpikeOccurred = true;
				InhibitorySynapses1[Channel][CellNumInThisChan]->Fire(NumCountedSpikesInhibitory1);
				InhibitorySynapses1[Channel][CellNumInThisChan]->Step();
				
				NumCountedSpikesInhibitory2 = SpikeMapInhibitory2->CountMappedSpikes(InputInhibitory2, CellID);
				// if (NumCountedSpikesInhibitory2 > 0)
				//	SpikeOccurred = true;
				InhibitorySynapses2[Channel][CellNumInThisChan]->Fire(NumCountedSpikesInhibitory2);
				InhibitorySynapses2[Channel][CellNumInThisChan]->Step();
				if (MGCells[Channel][CellNumInThisChan]->Step(&(ExcitatorySynapses[Channel][CellNumInThisChan]), 1, &(ExcitatorySynapsesNSA[Channel][CellNumInThisChan]), 1, &(InhibitorySynapses1[Channel][CellNumInThisChan]), 1, &(InhibitorySynapses2[Channel][CellNumInThisChan]), 1))
					Output->Fire(CellID);
			} // for (Channel)
		}
		InputExcitatory->Step();
		InputInhibitory1->Step();
		InputInhibitory2->Step();
		Output->Step();
	}

	return counter + 1;
}

int VoigtDCNCell::Stop(void)
{
	cout << "    Stop: " << ModuleName() << endl;

    return 1;
}

int VoigtDCNCell::Unload(void)
{
	cout << "    Unload: " << ModuleName() << endl;

    return 1;
}
