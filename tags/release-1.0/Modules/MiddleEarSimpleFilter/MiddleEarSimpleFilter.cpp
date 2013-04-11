#include <iostream>
#include "MiddleEarSimpleFilter.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "Earlab.h"
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"

using namespace std;

MiddleEarSimpleFilter::MiddleEarSimpleFilter()
{
	memset(this, 0, sizeof(MiddleEarSimpleFilter));
	mLogger = new Logger();
	SetModuleName("MiddleEarSimpleFilter");
}

MiddleEarSimpleFilter::~MiddleEarSimpleFilter()
{
	if (mModuleName != NULL)
		delete [] mModuleName;
}

int MiddleEarSimpleFilter::ReadParameters(char *ParameterFileName)
{
	if (mModuleName == NULL)
		return ReadParameters(ParameterFileName, "MiddleEarSimpleFilter");
	else
		return ReadParameters(ParameterFileName, mModuleName);
}

int MiddleEarSimpleFilter::ReadParameters(char *ParameterFileName, char *SectionName)
{
	CParameterFile theParamFile(ParameterFileName);
	ParameterStatus Status;

	mLogger->Log("    ReadParameters: %s \"%s\"", mModuleName, ParameterFileName);

	// Number of channels and Frame Size are passed as parameters to Start, see that function for details
	Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0);
	Status = theParamFile.GetParameter(SectionName, "HighpassCornerFreq_Hz", mHighpassCornerFreq_Hz, 500.0);
	Status = theParamFile.GetParameter(SectionName, "HighpassFilterOrder", mHighpassFilterOrder, 2);
	Status = theParamFile.GetParameter(SectionName, "Gain", mGain, 30.0);
	return 1;
}

int MiddleEarSimpleFilter::Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions])
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

	if (InputSize[0][1] != 0)
		throw EarlabException("%s: Input data must be one-dimensional array.  Sorry!", mModuleName);

	if (OutputSize[0][1] != 0)
		throw EarlabException("%s: Output signal must be one-dimensional array.  Sorry!", mModuleName);

	mFrameSize_Samples = OutputSize[0][0];
	mHighpassFilter = new FirstOrderHighpass[mHighpassFilterOrder];
	for (i = 0; i < mHighpassFilterOrder; i++)
	{
		mHighpassFilter[i].SetSampleRate_Hz(mSampleRate_Hz);
		mHighpassFilter[i].SetCornerFrequency_Hz(mHighpassCornerFreq_Hz);
	}
	return 1;
}

int MiddleEarSimpleFilter::Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount])
{
    int i, j;
	double CurSample;

	mLogger->Log("    Advance: %s", mModuleName);
	if (Input[0]->Rank(0) != mFrameSize_Samples)
		throw EarlabException("%s: Input size mismatch with Start()", mModuleName);
	if (Output[0]->Rank(0) != mFrameSize_Samples)
		throw EarlabException("%s: Output size mismatch with Start()", mModuleName);

    for (j = 0; j < mFrameSize_Samples; j++)
    {
		CurSample = Input[0]->Data(j);
		for (i = 0; i < mHighpassFilterOrder; i++)
			CurSample = mHighpassFilter[i].Filter(CurSample);
        Output[0]->Data(j) = CurSample * mGain;
    }

    return j;
}

int MiddleEarSimpleFilter::Stop(void)
{
	mLogger->Log("    Stop: %s", mModuleName);
	return 1;
}

int MiddleEarSimpleFilter::Unload(void)
{
	mLogger->Log("    Unload: %s", mModuleName);
	return 1;
}

void MiddleEarSimpleFilter::SetModuleName(char *ModuleName)
{
	if (mModuleName != NULL)
		delete [] mModuleName;

	mModuleName = new char[strlen(ModuleName) + 1];
	strcpy(mModuleName, ModuleName);
}

void MiddleEarSimpleFilter::SetLogger(Logger *TheLogger)
{
	if (mLogger != NULL)
		delete mLogger;
	mLogger = TheLogger;
}
