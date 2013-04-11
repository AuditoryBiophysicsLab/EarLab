#include <iostream>
#include "Combiner.h"
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

/*	EarLab input Combiner module, V0.1, 15 July 2005 by Dave Anderson (da@bu.edu)
 *
 *  This is a utility module for EarLab - it combines its multiple input signals into a single output signal
 *
 *  Multiple input streams are permitted, and they may be one- or two-dimensional only
 *
 *  Only one output stream is permitted, and it must be two-dimensional
 *
 *  The number of samples (dimension 0) of the input and output streams must be identical
 *
 *  If the bool parameter "AddInputStreamsTogether" is true, then all the input streams must be
 *  the same size.  In this case, the output stream size must be the same as any one of the input streams.
 *  In this mode of operation, the output stream is formed by adding together the input streams.
 *  If the streams are waveforms, a simple sample-by-sample addition is performed.  If the streams are
 *  spikes, then all spikes in matching CellIDs are combined into a single output CellID of the same value
 *
 *  If the bool parameter "AddInputStreamsTogether" is false, then all the input streams are concatenated, starting
 *  with the lowest numbered channel or CellID of the lowest numbered input and moving up to the highest-numbered channel 
 *  or CellID of the highest-numbered input.  For waveform data the output must be a single, multichannel waveform with 
 *  the dimension 1 equal to the number of channels in all the provided input streams, and dimension 0 equal to
 *  dimension 0 of any one input stream.  For spike data, the output must be an array of N CellIDs, where N is the sum of
 *  the number of CellIDs in all input streams.  CellID zero of the lowest numbered input becomes CellID zero of the output stream
 *  After all the cells in the lowest numbered input are copied to the output stream, CellID zero of the second-lowest-numbered 
 *	input stream becomes the next higher CellID in the output stream, continuing until all CellIDs of all input streams are accounted 
 *  for.
 */

Combiner::Combiner()
{
    memset(this, 0, sizeof(Combiner));
	SetModuleName("Combiner");
}

Combiner::~Combiner()
{
	delete [] mNumInputChannels;
}

int Combiner::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	   return ReadParameters(ParameterFileName, "Combiner");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int Combiner::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);
	Status = theParamFile.GetParameter(SectionName, "AddInputStreamsTogether", mAddInputStreamsTogether, false);
    return 1;
}

int Combiner::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	int i;

	Log("    Start: %s", ModuleName());
	// Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	if (NumInputs == 0)
		throw EarlabException("%s: This module requires at least one input stream.", ModuleName());

	if (NumOutputs != 1)
		throw EarlabException("%s: This module can handle only one output stream.", ModuleName());

	mFrameSize_Samples = InputSize[0][0];
	mNumOutputChannels = OutputSize[0][1];
	mNumInputStreams = NumInputs;
	mNumInputChannels = new int [mNumInputStreams];
	mDataIsWaveform = (InputTypes[0] == WaveformData);

	if (InputTypes[0] != OutputTypes[0])
		throw EarlabException("%s: The input and output data types of this module are mismatched.", ModuleName());

	if (OutputSize[0][2] != 0)
		throw EarlabException("%s: Output stream must be two-dimensional.", ModuleName());

	for (i = 0; i < mNumInputStreams; i++)
	{
		if (InputTypes[i] != InputTypes[0])
			throw EarlabException("%s: All input streams to this module must be the same type as the output stream.  Input stream %d is not the correct type.", ModuleName(), i + 1);

		if (InputSize[i][0] != mFrameSize_Samples)
			throw EarlabException("%s: All input streams to this module must have the first dimension of the same length.  Input stream %d is not the correct size.", ModuleName(), i + 1);

		if (InputSize[i][1] != 0)
		{
			mNumInputChannels[i] =  InputSize[i][1];
			mTotalInputChannelCount += InputSize[i][1];
		}
		else
		{
			mNumInputChannels[i] =  1;
			mTotalInputChannelCount++;
		}

		if (InputSize[i][2] != 0)
			throw EarlabException("%s: Input stream must be two-dimensional.  Stream %d appears to have more than two dimensions.", ModuleName(), i + 1);

		OutputElementCounts[i] = mNumOutputChannels * mFrameSize_Samples;
	}

	if (mNumOutputChannels != mTotalInputChannelCount)
		throw EarlabException("%s: Total number of input channels must equal the number of output channels.", ModuleName());

	return 1;
}

int Combiner::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int counter = 0;
	int Sample, OutputChannel, Input, InputChannel;

	Log("    Advance: %s", ModuleName());

	mAddInputStreamsTogether;

	for (Sample = 0; Sample < mFrameSize_Samples; Sample++)
	{
		if (mAddInputStreamsTogether)
		{
			if (mDataIsWaveform)
			{
				for (OutputChannel = 0; OutputChannel < mNumOutputChannels; OutputChannel++)
				{
				}
			}
			else
				((EarlabSpikeStream *)OutputStream[0])->GetData()->NewFrame();
		}
		else
		{
			OutputChannel = 0;
		}
		for (Input = 0; Input < mNumInputStreams; Input++)
		{
			if (mAddInputStreamsTogether)
			{
				if (mDataIsWaveform)
				{
					for (InputChannel = 0; InputChannel < mNumInputChannels[Input]; InputChannel++)
					{
						((EarlabWaveformStream *)OutputStream[0])->GetData()->Data(Sample, InputChannel) += ((EarlabWaveformStream *)InputStream[Input])->GetData()->Data(Sample, InputChannel);
					}
				}
				else
				{
				}

			} // if (mAddInputStreamsTogether)
			else
			{
				if (mDataIsWaveform)
				{
					for (InputChannel = 0; InputChannel < mNumInputChannels[Input]; InputChannel++)
					{
						((EarlabWaveformStream *)OutputStream[0])->GetData()->Data(Sample, OutputChannel) = ((EarlabWaveformStream *)InputStream[Input])->GetData()->Data(Sample, InputChannel);
						OutputChannel++;
					}
				}
				else
				{
				}
			} // if (mAddInputStreamsTogether) else
		} // for (Input)
	} // for (Sample)

	return counter + 1;
}

int Combiner::Stop(void)
{
	Log("    Stop: %s", ModuleName());
    return 1;
}

int Combiner::Unload(void)
{
	Log("    Unload: %s", ModuleName());
    return 1;
}
