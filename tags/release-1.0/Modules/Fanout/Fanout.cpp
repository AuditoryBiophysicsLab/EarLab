#include <iostream>
#include "Fanout.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "Earlab.h"
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"

using namespace std;

Fanout::Fanout()
{
	memset(this, 0, sizeof(Fanout));
	mLogger = new Logger();
	SetModuleName("Fanout");
}

Fanout::~Fanout()
{
	if (mModuleName != NULL)
		delete [] mModuleName;
}

int Fanout::ReadParameters(char *ParameterFileName)
{
	if (mModuleName == NULL)
		return ReadParameters(ParameterFileName, "Fanout");
	else
		return ReadParameters(ParameterFileName, mModuleName);
}

int Fanout::ReadParameters(char *ParameterFileName, char *SectionName)
{
	CParameterFile theParamFile(ParameterFileName);
	//ParameterStatus Status;

	mLogger->Log("    ReadParameters: %s \"%s\"", mModuleName, ParameterFileName);

	return 1;
}

int Fanout::Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions])
{
	int i, j;

	mLogger->Log("    Start: %s", mModuleName);
	// Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	if (NumInputs != 1)
		throw EarlabException("%s: Currently this module can only handle one input stream.  Sorry!", mModuleName);

	if (NumOutputs == 0)
		throw EarlabException("%s: This module must have at least one output stream.  Sorry!", mModuleName);

	mNumOutputs = NumOutputs;

	for (i = 0; i < NumOutputs; i++)
	{
		for (j = 0; j < EarlabMaxIOStreamDimensions; j++)
		{
			if (InputSize[0][j] != OutputSize[i][j])
				throw EarlabException("%s: Input size mismatch.  Dimension %d of input does not match output stream %d", mModuleName, j, i + 1);
			if (InputSize[0][j] == 0)
			{
				mNumberOfDimensions = j;
				break;
			}
		}
	}
	return 1;
}

int Fanout::Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount])
{
    int i;
	unsigned long BufLen;

	mLogger->Log("    Advance: %s", mModuleName);

	for (i = 0; i < mNumOutputs; i++)
	{
		*(Output[i]) = *(Input[0]);
	}
	Output[0]->GetData(&BufLen);
    return (int)BufLen;
}

int Fanout::Stop(void)
{
	mLogger->Log("    Stop: %s", mModuleName);
	return 1;
}

int Fanout::Unload(void)
{
	mLogger->Log("    Unload: %s", mModuleName);
	return 1;
}

void Fanout::SetModuleName(char *ModuleName)
{
	if (mModuleName != NULL)
		delete [] mModuleName;

	mModuleName = new char[strlen(ModuleName) + 1];
	strcpy(mModuleName, ModuleName);
}

void Fanout::SetLogger(Logger *TheLogger)
{
	if (mLogger != NULL)
		delete mLogger;
	mLogger = TheLogger;
}
