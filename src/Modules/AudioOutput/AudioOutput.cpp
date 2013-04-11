/******************************************************************/
/*  AudioOutput.cpp                                          */
/*  Writes wave data to the Linux audio device                    */
/*  part of the EarLab simulation environment                     */
/*                                                                */
/*  4 September 2001      a.1    AWH    First Alpha draft         */
/*  8 May 2003            a.2    DJA    Turned gensig.c into      */
/*                                      an EarLab module          */
/* 31 Jul 2003            a.3    DJA    Created DataSink.cpp from */
/*                                      writer.c                  */
/* 14 Sep 2004			  2.0	 DJA	Added support for spike   */
/*                                      logging                   */
/* 20 Mar 2006			  2.1	 DJA	Turned DataSink module    */
/*                                      into AudioOutput     */
/*                                      module
/******************************************************************/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#endif
#include <stdio.h>
#include "Earlab.h"
#include "MatrixN.h"
#include "AudioOutput.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "EarlabException.h"

using namespace std;

AudioOutput::AudioOutput()
{
	memset(this, 0, sizeof(AudioOutput));
	SetModuleName("AudioOutput");
}

AudioOutput::~AudioOutput()
{
}

int AudioOutput::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
		return ReadParameters(ParameterFileName, "AudioOutput");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int AudioOutput::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;
	int i;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

	// Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0);
	Status = theParamFile.GetParameter(SectionName, "AudioDeviceName", mAudioDeviceName, sizeof(mAudioDeviceName), NULL);
	Status = theParamFile.GetParameter(SectionName, "StereoOutput", mStereoOutput, false);
	return 1;
}

int AudioOutput::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
#ifdef _WIN32
	struct sockaddr_in local;
	WORD VersionRequested;
	WSADATA WSAData;
#endif
	int status, LastError;
	unsigned int enable;
	FILE *CFFile;
	float tmp;
	int ElementCount;

	Log("    Start: %s", ModuleName());
	// Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	if (NumInputs != 1)
	{
		printf("%s: Currently this module can only handle one input stream.  Sorry!\n", ModuleName());
		throw EarlabException("%s: Currently this module can only handle one input stream.  Sorry!", ModuleName());
	}

	if (NumOutputs != 0)
	{
		printf("%s: Currently this module cannot produce any output streams.  Sorry!\n", ModuleName());
		throw EarlabException("%s: Currently this module cannot produce any output streams.  Sorry!", ModuleName());
	}

	if (InputTypes[0] != WaveformData)
	{
		printf("%s: Currently this module can only handle one waveform input data stream.  Sorry!\n", ModuleName());
		throw EarlabException("%s: Currently this module can only handle one waveform input data stream.  Sorry!\n", ModuleName());
	}

	if (InputSize[0][1] != 0)
		throw EarlabException("%s: Input signal must be one-dimensional array.  Sorry!", ModuleName());

    return 1;
}

// Check in about using vectors of MatrixN's for input and output buffers
// Fallback position is float*[] for i/o vectors
// Limit the number of MatrixN dimensions to a #define number

int AudioOutput::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	FloatMatrixN *WaveInput;
	SpikeStream *SpikeInput;
	int result;

	Log("    Advance: %s", ModuleName());

	return 1;
}

int AudioOutput::Stop(void)
{
	Log("    Stop: %s", ModuleName());
	return 1;
}

int AudioOutput::Unload(void)
{
	Log("    Unload: %s", ModuleName());
	return 1;
}

