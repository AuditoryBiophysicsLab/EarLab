#include <iostream>
#include "Earlab.h"
#include "CompoundActionPotentialIFR.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"
#include "FirstOrderLowpass.h"

using namespace std;

CompoundActionPotentialIFR::CompoundActionPotentialIFR()
{
    memset(this, 0, sizeof(CompoundActionPotentialIFR));
	mLogger = new Logger();
	SetModuleName("CompoundActionPotentialIFR");
}

CompoundActionPotentialIFR::~CompoundActionPotentialIFR()
{
	if (mModuleName != NULL)
		delete [] mModuleName;
}

int CompoundActionPotentialIFR::ReadParameters(char *ParameterFileName)
{
	if (mModuleName == NULL)
	    return ReadParameters(ParameterFileName, "CompoundActionPotentialIFR");
	else
	    return ReadParameters(ParameterFileName, mModuleName);
}

int CompoundActionPotentialIFR::ReadParameters(char *ParameterFileName, char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	mLogger->Log("    ReadParameters: %s \"%s\"", mModuleName, ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz",mSampleRate_Hz,0);
    Status = theParamFile.GetParameter(SectionName, "LowpassCornerFrequency_Hz",
		mLowpassCornerFrequency_Hz, 5000.0);
	Status = theParamFile.GetParameter(SectionName, "LowpassFilterOrder",	
		mLowpassFilterOrder,	4);
    return 1;
}

int CompoundActionPotentialIFR::Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions])
{
	int i;

	mLogger->Log("    Start: %s", mModuleName);

    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
    if (NumInputs != 1)
        throw EarlabException("%s: Currently this module can only handle one input stream.  Sorry!", mModuleName);

    if (NumOutputs != 1)
        throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", mModuleName);

    if (InputSize[0][0] != OutputSize[0][0])
        throw EarlabException("%s: Input and output frame lengths must be identical.  Sorry!", mModuleName);

    if (OutputSize[0][1] != 0)
        throw EarlabException("%s: Output signal must be one-dimensional array.  Sorry!", mModuleName);

    mFrameSize_Samples = OutputSize[0][0];
	mDeltaT = 1.0 / (double)mSampleRate_Hz;
    mNumChannels = InputSize[0][1];

	mLowpassFilter = new FirstOrderLowpass[mLowpassFilterOrder];
	for (i = 0; i < mLowpassFilterOrder; i++)
	{
		mLowpassFilter[i].SetSampleRate_Hz(mSampleRate_Hz);
		mLowpassFilter[i].SetCornerFrequency_Hz(mLowpassCornerFrequency_Hz);
	}

	return 1;
}

int CompoundActionPotentialIFR::Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount])
{
    int counter = 0;
	double total, derivative;
	int i, samp, chan;

	mLogger->Log("    Advance: %s", mModuleName);

	if (mNumChannels > 0)
	{
		if ((Input[0]->Rank(0) != mFrameSize_Samples) || (Input[0]->Rank(1) != mNumChannels))
			throw EarlabException("%s: Input size mismatch with Start()", mModuleName);
	}

	if ((Output[0]->Rank(0) != mFrameSize_Samples))
		throw EarlabException("%s: Output size mismatch with Start()", mModuleName);

	for (samp = 0; samp < mFrameSize_Samples; samp++)
	{
		total = 0;
		if (mNumChannels == 0)
		{
			total += Input[0]->Data(samp);
			counter++;
		}
		else
		{
			for (chan = 0; chan < mNumChannels; chan++)
			{
				total += Input[0]->Data(samp, chan);
				counter++;
			}
		}

		for (i = 0; i < mLowpassFilterOrder; i++)
			total = mLowpassFilter[i].Filter(total);

		derivative = (total - mOldSample) / mDeltaT;
		mOldSample = total;

		Output[0]->Data(samp) = derivative;
		//Output[0]->Data(samp) = total;
	}

    return counter;
}

int CompoundActionPotentialIFR::Stop(void)
{
	mLogger->Log("    Stop: %s", mModuleName);

    return 1;
}

int CompoundActionPotentialIFR::Unload(void)
{
	mLogger->Log("    Unload: %s", mModuleName);

    return 1;
}

void CompoundActionPotentialIFR::SetModuleName(char *ModuleName)
{
	if (mModuleName != NULL)
		delete [] mModuleName;

	mModuleName = new char[strlen(ModuleName) + 1];
	strcpy(mModuleName, ModuleName);
}

void CompoundActionPotentialIFR::SetLogger(Logger *TheLogger)
{
	if (mLogger != NULL)
		delete mLogger;
	mLogger = TheLogger;
}
