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
#include "ToddIC.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "EarlabException.h"
#include <math.h>

using namespace std;

ToddIC::ToddIC()
{
	memset(this, 0, sizeof(ToddIC));
		SetModuleName("ToddIC");
}

ToddIC::~ToddIC()
{
	for (int iChannel = 0; iChannel < mNumChannels; iChannel++)
	{
		delete [] ICCells[iChannel];
	}
	delete ICCells;
	delete InputDelayBuffer;
}

int ToddIC::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
		return ReadParameters(ParameterFileName, "ToddIC");
	else
		return ReadParameters(ParameterFileName, ModuleName());
}

int ToddIC::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
	ParameterStatus Status;
	CParameterFile theParamFile(ParameterFileName);
	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

	mSynapticStrengthLength    = EarlabMaxIOStreamCount;
	mSynapticTau_SecondsLength = EarlabMaxIOStreamCount;
	mInputDelay_SecondsLength  = EarlabMaxIOStreamCount;
	mSynapseFiberNumberLength  = EarlabMaxIOStreamCount;
	mIsExcitatoryLength        = EarlabMaxIOStreamCount;

	// Number of channels and Frame Size are passed as parameters to Start, see that function for details
		
	Status = theParamFile.GetParameter(SectionName, "ResponseType", mResponseType, 2);
	Status = theParamFile.GetParameter(SectionName, "TauAHP_Seconds", mTauAHP_Seconds, 500.0e-3);
	Status = theParamFile.GetParameter(SectionName, "AHPIncrement_Siemens", mAHPIncrement_Siemens, 0.0e-9);
	
	

	Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz,-962.8477);
	if (mSampleRate_Hz == -962.8477)
		throw EarlabException("%s: SynapticStrength Parameter must be specified.", ModuleName());
		
	Status = theParamFile.GetParameter(SectionName, "FrameLength_Seconds", mFrameLength_Seconds,-962.8477);
	if (mFrameLength_Seconds == -962.8477)
		throw EarlabException("%s: SynapticStrength Parameter must be specified.", ModuleName());
		
	Status = theParamFile.GetParameter(SectionName, "CellTemperature_DegreesC", mCellTemperature_DegreesC,-962.8477);
	if (mCellTemperature_DegreesC == -962.8477)
		throw EarlabException("%s: SynapticStrength Parameter must be specified.", ModuleName());
	
	

	Status = theParamFile.GetParameter(SectionName, "SynapticStrength", mSynapticStrength, mSynapticStrengthLength, NULL,0);
	if (Status != Success)
		throw EarlabException("%s: SynapticStrength Parameter must be specified.", ModuleName());
	
	Status = theParamFile.GetParameter(SectionName, "SynapticTau_Seconds", mSynapticTau_Seconds, mSynapticTau_SecondsLength, NULL,0);
	if (Status != Success)
		throw EarlabException("%s: SynapticTau_Seconds Parameter must be specified.", ModuleName());
	
	Status = theParamFile.GetParameter(SectionName, "InputDelay_Seconds", mInputDelay_Seconds, mInputDelay_SecondsLength, NULL, 0);
	if (Status != Success)
		throw EarlabException("%s: InputDelay_Seconds Parameter must be specified.", ModuleName());
	
	Status = theParamFile.GetParameter(SectionName, "SynapseFiberNumber", mSynapseFiberNumber, mSynapseFiberNumberLength, NULL, 0);
	if (Status != Success)
		throw EarlabException("%s: SynapseFiberNumber Parameter must be specified.", ModuleName());
	
	Status = theParamFile.GetParameter(SectionName, "IsExcitatory", mIsExcitatory, mIsExcitatoryLength, NULL, 0);
        if (Status != Success)
                throw EarlabException("%s: IsExcitatory Parameter must be specified.", ModuleName());

        return 1;

}

int ToddIC::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	int LagBufferSpikeCount;
	int iInput, iExcit, iInhib, iChannel;
	int InputCellCount, OutputCellCount;
	mNumInputs=NumInputs;

	cout << "    Start: " << ModuleName() << endl;

	// Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	if (NumOutputs != 1)
		throw EarlabException("%s: This module can only handle one output stream.", ModuleName());

	if (InputTypes[0] != SpikeData)
		throw EarlabException("%s: This module can only handle spike input data streams.", ModuleName());

	if (OutputTypes[0] != SpikeData)
		throw EarlabException("%s: This module can only handle spike output data streams.", ModuleName());

	if (InputSize[0][0] != OutputSize[0][0])
		throw EarlabException("%s: Input and output channel counts must be identical.", ModuleName());

	if (OutputSize[0][1] != 1)
		throw EarlabException("%s: Second dimension of output signal must have a size of one.", ModuleName());

	if (OutputSize[0][2] != 0)
		throw EarlabException("%s: Output data must be two-dimensional array.", ModuleName());
	
	if (mSynapticStrengthLength != mNumInputs)
		throw EarlabException("%s: The length of SynapticStrength must be the same as the number of inputs.", ModuleName());

	if (mSynapticTau_SecondsLength != mNumInputs)
		throw EarlabException("%s: The length of SynapticTau_Seconds must be the same as the number of inputs.", ModuleName());

	if (mInputDelay_SecondsLength != mNumInputs)
		throw EarlabException("%s: The length of InputDelay_Seconds must be the same as the number of inputs.", ModuleName());
	
	if (mSynapseFiberNumberLength != mNumInputs)
		throw EarlabException("%s: The length of SynapseFiberNumber must be the same as the number of inputs.", ModuleName());
	
	if (mIsExcitatoryLength != mNumInputs)
		throw EarlabException("%s: The length of IsExcitatory must be the same as the number of inputs.", ModuleName());
	
	
	
	if (mSampleRate_Hz <= 0)
		throw EarlabException("%s: SampleRate_Hz must be positive.", ModuleName());

	if (mFrameLength_Seconds <= 0)
		throw EarlabException("%s: FrameLength_Seconds must be positive.", ModuleName());

	if (mCellTemperature_DegreesC <= -273.15)
		throw EarlabException("%s: CellTemperature_DegreesC must be above absolute zero.", ModuleName());

	if (mTauAHP_Seconds <= 0)
		throw EarlabException("%s: TauAHP_Seconds must be positive.", ModuleName());

	for (iInput=0; iInput < mNumInputs; iInput++)
	{
		if (InputSize[iInput][2] != 0)
			throw EarlabException("%s, Input %s: Inputs must be two-dimensional.", ModuleName(),iInput);

		if (mSynapseFiberNumber[iInput] < 0)
			throw EarlabException("%s, Input %s: SynapseFiberNumber cannot be negative.", ModuleName(),iInput);

		if (mSynapseFiberNumber[iInput] > InputSize[iInput][1])
			throw EarlabException("%s, Input %s: SynapseFiberNumber cannot be larger than synapse fiber count.", ModuleName(),iInput);

		if (mSynapticStrength[iInput] < 0)
			throw EarlabException("%s, Input %s: SynapticStrength cannot be negative.", ModuleName(),iInput);

		if (mSynapticTau_Seconds[iInput] <= 0)
			throw EarlabException("%s, Input %s: SynapticTau_Seconds must be positive.", ModuleName(),iInput);

		if (mInputDelay_Seconds[iInput] < 0)
			throw EarlabException("%s, Input %s: InputDelay_Seconds cannot be negative.", ModuleName(),iInput);

		if ((mIsExcitatory[iInput] != 0) && (mIsExcitatory[iInput] != 1))
			throw EarlabException("%s, Input %s: mIsExcitatory must be zero or one.", ModuleName(),iInput);

		for (int iInput2=iInput+1; iInput2 < mNumInputs; iInput2++)
			if (InputSize[iInput][0] != InputSize[iInput2][0])
				throw EarlabException("%s, Inputs %s and %s: Input channel counts must be identical.", ModuleName(),iInput,iInput2);
	}

	OutputCellCount = 1;

        mNumChannels = InputSize[0][0];

        OutputElementCounts[0] = (int)(OutputCellCount * (mFrameLength_Seconds * 1000));
        
        ICCells = new COMPARTMENT_TYPE *[mNumChannels];
        WasUnderThreshold = new bool [mNumChannels];

	ExcitatorySynapses = new HHSynapse **[mNumChannels];
	InhibitorySynapses = new HHSynapse **[mNumChannels];
	InputDelayBuffer = new SpikeDelayBuffer *[mNumInputs];

	mExcitCount = 0;

        for (iInput=0; iInput < mNumInputs; iInput++)
        {	
		if (InputSize[iInput][1] == 0)
		{
		    InputCellCount = InputSize[0][0];
		} else {
		    InputCellCount = InputSize[0][0] * InputSize[iInput][1];
		}

                if (mInputDelay_Seconds[iInput] != 0)
                {
			LagBufferSpikeCount = (int)(InputCellCount * (mFrameLength_Seconds * 2000000.0));
			InputDelayBuffer[iInput] = new SpikeDelayBuffer((int)(LagBufferSpikeCount + (InputCellCount * mInputDelay_Seconds[iInput] * 20000)), mInputDelay_Seconds[iInput] , mFrameLength_Seconds, mSampleRate_Hz);
//                         LagBufferSpikeCount = (int)(InputCellCount * 10 * mInputDelay_Seconds[iInput] * 100000.0);
//                         InputDelayBuffer[iInput] = new SpikeDelayBuffer(LagBufferSpikeCount, mInputDelay_Seconds[iInput], mFrameLength_Seconds, mSampleRate_Hz);
// 			Log("laginfo: Input sizes %d %d InputCellCount %d delay %g lagbuffercount %d", InputSize[0][0], InputSize[iInput][1], InputCellCount,  mInputDelay_Seconds[iInput], LagBufferSpikeCount);
                }

		if (mIsExcitatory[iInput] == 1)
			mExcitCount++;
	}
		
	newmResponseType = (EnumedResponseType) mResponseType;
		
	for (iChannel= 0; iChannel < mNumChannels; iChannel++)
	{
		ICCells[iChannel] = new COMPARTMENT_TYPE(mSampleRate_Hz, -64.0e-3, mCellTemperature_DegreesC, newmResponseType);
		//ICCells[iChannel] = new COMPARTMENT_TYPE(mSampleRate_Hz, -64.0e-3, mCellTemperature_DegreesC, newmResponseType, mTauAHP_Seconds, mAHPIncrement_Siemens);
		WasUnderThreshold[iChannel] = true;

		ExcitatorySynapses[iChannel] = new HHSynapse *[mExcitCount];
		InhibitorySynapses[iChannel] = new HHSynapse *[mNumInputs-mExcitCount];

		iExcit = 0;
		iInhib = 0;


		for (iInput=0; iInput < mNumInputs; iInput++)
		{
			if (mIsExcitatory[iInput]==1)
			{
				ExcitatorySynapses[iChannel][iExcit] = new HHSynapse(mSampleRate_Hz, mSynapticTau_Seconds[iInput], mSynapticStrength[iInput]);
				iExcit++;
			}
			else
			{
				InhibitorySynapses[iChannel][iInhib] = new HHSynapse(mSampleRate_Hz, mSynapticTau_Seconds[iInput], mSynapticStrength[iInput]);
				iInhib++;
			}
		}
// 		Log("synapse info: %d Excitatory, %d Inhibitory", iExcit, iInhib);

	}

        SamplesPerFrame = (int)(mFrameLength_Seconds * mSampleRate_Hz);



        return 1;
}

int ToddIC::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int counter = 0;
	// Add SpikeStream TempInput to hold undelayed spikes
	SpikeStream **InputCell;
	SpikeStream *TempInput;
	SpikeStream *Output;
        int iSample, iInput, iChannel, iExcit, iInhib;
        int InputCellID;
        int SpikeCount;

        Log("    Advance: %s", ModuleName());

	InputCell = new SpikeStream *[mNumInputs];

        for (iInput=0; iInput < mNumInputs; iInput++)
        {

                if (mInputDelay_Seconds[iInput] == 0)
                        InputCell[iInput] = ((EarlabSpikeStream *)InputStream[iInput])->GetData();
                else
                {
                        TempInput = ((EarlabSpikeStream *)InputStream[iInput])->GetData();
                        InputCell[iInput] = InputDelayBuffer[iInput]->Add(TempInput);
                }
 
                InputCell[iInput]->SetSampleRate_Hz(mSampleRate_Hz);
                InputCell[iInput]->NewFrame();
	}

	Output = ((EarlabSpikeStream *)OutputStream[0])->GetData();
	Output->SetSampleRate_Hz(mSampleRate_Hz);
	Output->NewFrame();


	for (iSample = 0; iSample < SamplesPerFrame; iSample++)
	{
		for (iChannel = 0; iChannel < mNumChannels; iChannel++)
		{
			
			iExcit = 0;
			iInhib = 0;

			for (iInput=0; iInput < mNumInputs; iInput++)
			{
				InputCellID = (mSynapseFiberNumber[iInput] * mNumChannels) + iChannel;
				SpikeCount = InputCell[iInput]->CountSpikes(InputCellID);
//				if (SpikeCount>0) {
//				    Log("spike count for sample %d channel %d input %d ID %d: %d", iSample, iChannel, iInput, InputCellID, SpikeCount);
//				}
				// Fire synapse for spikes received in the current sample period
				// Step the synapse alpha functions to provide input conductances for the IC cell
				if (mIsExcitatory[iInput]==1)
				{
					ExcitatorySynapses[iChannel][iExcit]->Fire(SpikeCount);
					ExcitatorySynapses[iChannel][iExcit]->Step();
					iExcit++;
				}
				else
				{
					InhibitorySynapses[iChannel][iInhib]->Fire(SpikeCount);
					InhibitorySynapses[iChannel][iInhib]->Step();
					iInhib++;
				}
                        } // for (iInput)
// 			Log("synapse info for sample %d channel %d: %d Excitatory, %d Inhibitory", iSample, iChannel, iExcit, iInhib);

                        if (ICCells[iChannel]->Step(ExcitatorySynapses[iChannel], mExcitCount,InhibitorySynapses[iChannel], mNumInputs-mExcitCount))
                                Output->Fire(0);
                } // for (iChannel)
                for (iInput = 0; iInput < mNumInputs; iInput++)
                        InputCell[iInput]->Step();
		Output->Step();
	} // for (iSample)
	return counter + 1;

}

int ToddIC::Stop(void)
{
        cout << "    Stop: " << ModuleName() << endl;

        return 1;
}

int ToddIC::Unload(void)
{
	cout << "    Unload: " << ModuleName() << endl;

	return 1;
}
