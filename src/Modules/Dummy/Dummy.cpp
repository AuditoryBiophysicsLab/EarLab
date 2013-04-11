#include <iostream>
#include "Dummy.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "Earlab.h"
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"
#include "EarlabDataStream.h"

using namespace std;

Dummy::Dummy()
{
	memset(this, 0, sizeof(Dummy));
	SetModuleName("Dummy");
}

Dummy::~Dummy()
{
}

int Dummy::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
		return ReadParameters(ParameterFileName, "Dummy");
	else
		return ReadParameters(ParameterFileName, ModuleName());
}

int Dummy::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
	CParameterFile theParamFile(ParameterFileName);

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);
	return 1;
}

int Dummy::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	int i;

	Log("    Start: %s", ModuleName());
	// Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	if (NumInputs != 1)
		throw EarlabException("%s: This module requires exactly one input stream.  Sorry!", ModuleName());

	if (NumOutputs != 1)
		throw EarlabException("%s: This module only supports one output stream.  Sorry!", ModuleName());

	for (i = 0; i < NumInputs; i++)
		if (InputTypes[i] != WaveformData)
			throw EarlabException("%s: Currently this module can only handle waveform input data streams.  Sorry!", ModuleName());

	if (OutputTypes[0] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform output data streams.  Sorry!", ModuleName());

	OutputElementCounts[0] = OutputSize[0][0];

	if ((InputSize[0][0] != OutputSize[0][0]) || (InputSize[0][1] != OutputSize[0][1]) || (InputSize[0][2] != OutputSize[0][2]))
		throw EarlabException("%s: The input stream and output stream must be the same size.", ModuleName());
	return OutputSize[0][0];
}

int Dummy::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	unsigned int BufLen;
	FloatMatrixN *Input, *Output;

	Log("    Advance: %s", ModuleName());

	Input = ((EarlabWaveformStream *)InputStream[0])->GetData();
	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();
	Output = Input;	// Use the MatrixN assignment operator to do the copy.  Coolness!
	Input->GetData(&BufLen);
    return (int)BufLen;
}

int Dummy::Stop(void)
{
	Log("    Stop: %s", ModuleName());
	return 1;
}

int Dummy::Unload(void)
{
	Log("    Unload: %s", ModuleName());
	return 1;
}
