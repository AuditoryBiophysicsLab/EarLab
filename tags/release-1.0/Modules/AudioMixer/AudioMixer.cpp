#include <iostream>
#include "AudioMixer.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "Earlab.h"
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"

using namespace std;

AudioMixer::AudioMixer()
{
	memset(this, 0, sizeof(AudioMixer));
	mLogger = new Logger();
	SetModuleName("AudioMixer");
}

AudioMixer::~AudioMixer()
{
	if (mModuleName != NULL)
		delete [] mModuleName;
}

int AudioMixer::ReadParameters(char *ParameterFileName)
{
	if (mModuleName == NULL)
		return ReadParameters(ParameterFileName, "AudioMixer");
	else
		return ReadParameters(ParameterFileName, mModuleName);
}

int AudioMixer::ReadParameters(char *ParameterFileName, char *SectionName)
{
	CParameterFile theParamFile(ParameterFileName);
	ParameterStatus Status;
	int i;

	mLogger->Log("    ReadParameters: %s \"%s\"", mModuleName, ParameterFileName);
	Status = theParamFile.GetParameter(SectionName, "InputScaleFactorArray", InputScaleFactorArray, mNumInputs, NULL, 0);
	if (Status == Success)
		mScaleFactorsSpecified = true;
	else
	{
		mScaleFactorsSpecified = false;
		for (i = 0; i < EarlabMaxIOStreamCount; i++)
			InputScaleFactorArray[i] = 1.0;
	}

	return 1;
}

int AudioMixer::Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions])
{
	int i;

	mLogger->Log("    Start: %s", mModuleName);
	// Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	if (NumInputs == 0)
		throw EarlabException("%s: This module requires at least one input stream.  Sorry!", mModuleName);

	if (NumOutputs != 1)
		throw EarlabException("%s: This module only supports one output stream.  Sorry!", mModuleName);

	if (mScaleFactorsSpecified)
	{
		if (NumInputs != mNumInputs)
			throw EarlabException("%s: When InputScaleFactorArray is specifed, the number of elements in the array must be the same as the number of inputs to this module.  Exiting.", mModuleName);
	}
	mNumInputs = NumInputs;

	for (i = 0; i < NumInputs; i++)
	{
		if ((InputSize[i][0] != 0) && (InputSize[i][1] != 0))
			throw EarlabException("%s: All input streams to this module must be one-dimensional.  Input %d has more than one.", mModuleName, i);
		if (InputSize[i][0] != OutputSize[0][0])
			throw EarlabException("%s: All input streams to this module must have the same length as the output stream.  Input %d's length is different.", mModuleName, i);
	}
	mNumSamples = OutputSize[0][0];
	return 1;
}

int AudioMixer::Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount])
{
    int i, j;
	unsigned long BufLen;
	double CurSample;

	mLogger->Log("    Advance: %s", mModuleName);

	for (i = 0; i < mNumSamples; i++)
	{
		CurSample = 0;
		for (j = 0; j < mNumInputs; j++)
		{
			CurSample += Input[j]->Data(i) * InputScaleFactorArray[j];
		}
		Output[i]->Data(i) = CurSample;
	}
	Output[0]->GetData(&BufLen);
    return (int)BufLen;
}

int AudioMixer::Stop(void)
{
	mLogger->Log("    Stop: %s", mModuleName);
	return 1;
}

int AudioMixer::Unload(void)
{
	mLogger->Log("    Unload: %s", mModuleName);
	return 1;
}

void AudioMixer::SetModuleName(char *ModuleName)
{
	if (mModuleName != NULL)
		delete [] mModuleName;

	mModuleName = new char[strlen(ModuleName) + 1];
	strcpy(mModuleName, ModuleName);
}

void AudioMixer::SetLogger(Logger *TheLogger)
{
	if (mLogger != NULL)
		delete mLogger;
	mLogger = TheLogger;
}
