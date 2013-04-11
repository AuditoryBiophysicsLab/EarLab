#include <iostream>
#include "Earlab.h"
#include "SimpleSpikeGenerator.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"

using namespace std;

SimpleSpikeGenerator::SimpleSpikeGenerator()
{
    memset(this, 0, sizeof(SimpleSpikeGenerator));
	mLogger = new Logger();
	SetModuleName("SimpleSpikeGenerator");
}

SimpleSpikeGenerator::~SimpleSpikeGenerator()
{
	if (mModuleName != NULL)
		delete [] mModuleName;
}

int SimpleSpikeGenerator::ReadParameters(char *ParameterFileName)
{
	if (mModuleName == NULL)
	    return ReadParameters(ParameterFileName, "SimpleSpikeGenerator");
	else
	    return ReadParameters(ParameterFileName, mModuleName);
}

int SimpleSpikeGenerator::ReadParameters(char *ParameterFileName, char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	cout << "    ReadParameters: " << mModuleName << endl;

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, mSampleRate_Hz);
    return 1;
}

int SimpleSpikeGenerator::Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions])
{
	cout << "    Start: " << mModuleName << endl;

    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
    if (NumInputs != 1)
        throw EarlabException("%s: Currently this module can only handle one input stream.  Sorry!", mModuleName);

    if (NumOutputs != 1)
        throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", mModuleName);

    if (InputSize[0][0] != OutputSize[0][0])
        throw EarlabException("%s: Input and output frame lengths must be identical.  Sorry!", mModuleName);

    if (InputSize[0][1] != OutputSize[0][1])
        throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", mModuleName);

    if (InputSize[0][2] != 0)
        throw EarlabException("%s: Input data must be two-dimensional array.  Sorry!", mModuleName);

    if (OutputSize[0][2] != 0)
        throw EarlabException("%s: Output signal must be two-dimensional array.  Sorry!", mModuleName);

    //mFrameSize_Samples = OutputSize[0][0];
    //mNumCells = OutputSize[0][1];

    return 1;
}

int SimpleSpikeGenerator::Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount])
{
	int counter;
	cout << "    Advance: " << mModuleName << endl;
	//if ((Input[0]->Rank(0) != mFrameSize_Samples) || (Input[0]->Rank(1) != mNumChannels))
	//	throw EarlabException("%s: Input size mismatch with Start()", mModuleName);
	//if ((Output[0]->Rank(0) != mFrameSize_Samples) || (Output[0]->Rank(1) != mNumChannels))
	//	throw EarlabException("%s: Output size mismatch with Start()", mModuleName);

    counter = 1;

	return counter;
}

int SimpleSpikeGenerator::Stop(void)
{
	cout << "    Stop: " << mModuleName << endl;

    return 1;
}

int SimpleSpikeGenerator::Unload(void)
{
	cout << "    Unload: " << mModuleName << endl;

    return 1;
}

void SimpleSpikeGenerator::SetModuleName(char *ModuleName)
{
	if (mModuleName != NULL)
		delete [] mModuleName;

	mModuleName = new char[strlen(ModuleName) + 1];
	strcpy(mModuleName, ModuleName);
}

void SimpleSpikeGenerator::SetLogger(Logger *TheLogger)
{
	if (mLogger != NULL)
		delete mLogger;
	mLogger = TheLogger;
}
