#include <iostream>
#include "StatisticalAnalysis.h"
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

/*	EarLab statistical analysis module, V0.1, 15 July 2005 by Dave Anderson (da@bu.edu)
 *
 *  This is a unique module for EarLab - it is our first module that takes a variable number of inputs and outputs
 *
 *	All input streams and output streams must be two dimensions, and all of them must be the same size in both of those two dimensions.
 *	Here's what this module does:
 *
 *  All the output streams are the respective central moments of all of the input streams combined, so output stream 0 is the 1st moment (mean)
 *  of the combined input streams, output stream 1 is the 2nd moment (Std Deviation) of the combined input streams, output stream 2 is the 3rd moment
 *  (skewness) of the combined input streams, output stream 3 is the 4th moment (kurtosis) of the combined input streams, and so on.
 *
 *  The input streams are combined at matching sample and channel numbers, and the resulting data is used to form each output stream
 *  in other words sample i and channel j of each input stream are averaged to produce sample i and channel j of output stream 0.  The
 *  square of sample i and channel j of each input stream are averaged to produce sample i and channel j of output stream 1.  The cube of the same
 *  data produce output stream 2, and so on for as many output streams as are specified in the diagram file
 *
 *  Got that?  There will be a quiz later...
 */

StatisticalAnalysis::StatisticalAnalysis()
{
    memset(this, 0, sizeof(StatisticalAnalysis));
	SetModuleName("StatisticalAnalysis");
}

StatisticalAnalysis::~StatisticalAnalysis()
{
	delete [] mOutputAccumulationBuffer;
}

int StatisticalAnalysis::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	   return ReadParameters(ParameterFileName, "StatisticalAnalysis");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int StatisticalAnalysis::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    //ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);
    return 1;
}

int StatisticalAnalysis::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount])
{
	int i;

	Log("    Start: %s", ModuleName());
	// Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	if (NumInputs <= 2)
		throw EarlabException("%s: Currently this module requires at least two input streams, preferably more.  Sorry!", ModuleName());

	if (NumOutputs == 0)
		throw EarlabException("%s: Currently this module requires at least one output stream.  Sorry!", ModuleName());

	mFrameSize_Samples = InputSize[0][0];
	mNumChannels = InputSize[0][1];
	mNumInputStreams = NumInputs;
	mNumOutputStreams = NumOutputs;

	for (i = 0; i < mNumInputStreams; i++)
	{
		if (InputTypes[i] != WaveformData)
			throw EarlabException("%s: Currently this module can only handle waveform type input streams.  Stream %d is not the correct type.", ModuleName(), i + 1);

		if ((InputSize[i][0] != mFrameSize_Samples) && (InputSize[i][1] != mNumChannels))
			throw EarlabException("%s: Input streams must all be the same size.  Streams %d and %d are different sizes.", ModuleName(), 1, i + 1);

		if (InputSize[i][2] != 0)
			throw EarlabException("%s: Input stream must be two-dimensional.  Stream %d appears to have more than two dimensions.", ModuleName(), i + 1);
	}

	for (i = 0; i < mNumOutputStreams; i++)
	{
		if (OutputTypes[i] != WaveformData)
			throw EarlabException("%s: Currently this module can only handle waveform type output streams.  Stream %d is not the correct type.", ModuleName(), i + 1);

		if ((OutputSize[i][0] != mFrameSize_Samples) && (OutputSize[i][1] != mNumChannels))
			throw EarlabException("%s: Output streams must all be the same size as all the input streams.  Output stream %d is a different size.", ModuleName(), i + 1);

		if (OutputSize[i][2] != 0)
			throw EarlabException("%s: Output stream must be two-dimensional.  Stream %d appears to have more than two dimensions.", ModuleName(), i + 1);

		OutputElementCounts[i] = mFrameSize_Samples * mNumChannels;
	}

	mOutputAccumulationBuffer = new float [mNumOutputStreams];
	return 1;
}

int StatisticalAnalysis::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int counter = 0;
	int Sample, Channel, Input, Output;
	float CurInput, CurOutput;

	Log("    Advance: %s", ModuleName());

	for (Sample = 0; Sample < mFrameSize_Samples; Sample++)
	{
		for (Channel = 0; Channel < mNumChannels; Channel++)
		{
			memset(mOutputAccumulationBuffer, 0, sizeof(float) * mNumOutputStreams);
			for (Input = 0; Input < mNumInputStreams; Input++)
			{
				CurInput = ((EarlabWaveformStream *)InputStream[Input])->GetData()->Data(Sample, Channel);
				CurOutput = CurInput;
				for (Output = 0; Output < mNumOutputStreams; Output++)
				{
					mOutputAccumulationBuffer[Output] += CurOutput;
					CurOutput *= CurInput;
				} // for (Output)
			} // for (Input)
			for (Output = 0; Output < mNumOutputStreams; Output++)
				((EarlabWaveformStream *)OutputStream[Output])->GetData()->Data(Sample, Channel) /= mNumInputStreams;
		} // for (Channel)
	} // for (Sample)

	return counter + 1;
}

int StatisticalAnalysis::Stop(void)
{
	Log("    Stop: %s", ModuleName());
    return 1;
}

int StatisticalAnalysis::Unload(void)
{
	Log("    Unload: %s", ModuleName());
    return 1;
}
