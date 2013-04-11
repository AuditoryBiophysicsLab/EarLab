#include <iostream>
#include "Splitter.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "Earlab.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"
#include "FPException.h"
#include <float.h>

using namespace std;

/*	EarLab input splitter module, V0.1, 15 July 2005 by Dave Anderson (da@bu.edu)
 *
 *  This is a utility module for EarLab - it splits its single input signal into multiple output signals
 *
 *  Only one input stream is permitted, and it must be two-dimensional
 *
 *  Multiple output streams are permitted, and they may be one- or two-dimensional only
 *
 *  The number of samples (dimension 0) of the input and output streams must be identical
 *
 *  The number of channels (dimension 1) in the input stream must be greater than or equal to the 
 *  combined number of channels across all output streams.
 *
 *  The bool parameter 'AllowUnusedInputData' must be set to true if the number of input channels is greater 
 *  than the combined number of channels across all output streams, or an exception will be thrown
 *
 *  The input stream will be split into the various output streams according to the number of channels in each output stream.
 *  For example, if output 0 has one channel (one dimensional), it will get channel 0 of the input stream.  If output 1 has two channels,
 *  it will get channels 1 and 2 of the input stream.  If output 2 has three channels, it will get channels 3, 4 and 5 of the input stream, and
 *  so on.
 *
 *  Currently, this module works on waveform streams only.  Eventually it should support a similar type of operation on spike streams, splitting
 *  the input by CellID into the various output streams (and of course recalculating the cellID appropriately)
 */

Splitter::Splitter()
{
    memset(this, 0, sizeof(Splitter));
	SetModuleName("Splitter");
}

Splitter::~Splitter()
{
	delete [] mNumOutputChannels;
}

int Splitter::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	   return ReadParameters(ParameterFileName, "Splitter");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int Splitter::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);
	Status = theParamFile.GetParameter(SectionName, "AllowUnusedInputData", mAllowUnusedInputData, false);
    return 1;
}

int Splitter::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount])
{
	int i;

	Log("    Start: %s", ModuleName());
	// Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	if (NumInputs != 1)
		throw EarlabException("%s: Currently this module can handle only one input stream.  Sorry!", ModuleName());

	if (NumOutputs == 0)
		throw EarlabException("%s: Currently this module requires at least one output stream.  Sorry!", ModuleName());

	mFrameSize_Samples = InputSize[0][0];
	mNumInputChannels = InputSize[0][1];
	mNumOutputStreams = NumOutputs;
	mNumOutputChannels = new int [mNumOutputStreams];

	if (InputTypes[0] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform type input streams.  Stream 1 is not the correct type.", ModuleName());

	if (InputSize[0][2] != 0)
		throw EarlabException("%s: Input stream must be two-dimensional.", ModuleName());

	for (i = 0; i < mNumOutputStreams; i++)
	{
		if (OutputTypes[i] != WaveformData)
			throw EarlabException("%s: Currently this module can only handle waveform type output streams.  Stream %d is not the correct type.", ModuleName(), i + 1);

		if (OutputSize[i][0] != mFrameSize_Samples)
			throw EarlabException("%s: Output frame lengths must all be the same size as all the input streams.  Output stream %d is a different size.", ModuleName(), i + 1);

		if (OutputSize[i][1] != 0)
		{
			mNumOutputChannels[i] =  OutputSize[i][1];
			mTotalOutputChannelCount += OutputSize[i][1];
		}
		else
		{
			mNumOutputChannels[i] =  1;
			mTotalOutputChannelCount++;
		}

		if (OutputSize[i][2] != 0)
			throw EarlabException("%s: Output stream must be two-dimensional.  Stream %d appears to have more than two dimensions.", ModuleName(), i + 1);

		OutputElementCounts[i] = OutputSize[i][0] * mNumOutputChannels[i];
	}

	if (mNumInputChannels > mTotalOutputChannelCount)
		throw EarlabException("%s: Total number of output channels may not exceed the number of input channels.  Exiting", ModuleName());
	if (mNumInputChannels < mTotalOutputChannelCount)
	{
		if (!mAllowUnusedInputData)
			throw EarlabException("%s: Total number of output channels (%d) must equal the number of input channels (%d) unless the bool parameter 'AllowUnusedInputData' is set to true.  Exiting.", ModuleName(), mTotalOutputChannelCount, mNumInputChannels);
	}

	return 1;
}

int Splitter::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int counter = 0;
	int Sample, OutputChannel, Output, InputChannel;

	Log("    Advance: %s", ModuleName());

	for (Sample = 0; Sample < mFrameSize_Samples; Sample++)
	{
		InputChannel = 0;
		for (Output = 0; Output < mNumOutputStreams; Output++)
		{
			for (OutputChannel = 0; OutputChannel < mNumOutputChannels[Output]; OutputChannel++)
			{
				((EarlabWaveformStream *)OutputStream[Output])->GetData()->Data(Sample, OutputChannel) = ((EarlabWaveformStream *)InputStream[0])->GetData()->Data(Sample, InputChannel);
				InputChannel++;
			}
		} // for (Output)
	} // for (Sample)

	return counter + 1;
}

int Splitter::Stop(void)
{
	Log("    Stop: %s", ModuleName());
    return 1;
}

int Splitter::Unload(void)
{
	Log("    Unload: %s", ModuleName());
    return 1;
}
