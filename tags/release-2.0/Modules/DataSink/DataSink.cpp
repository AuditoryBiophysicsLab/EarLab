/******************************************************************/
/*  DataSink.cpp                                                  */
/*  General data sink for the EarLab simulation environment       */
/*                                                                */
/*  4 September 2001      a.1    AWH    First Alpha draft         */
/*  8 May 2003            a.2    DJA    Turned gensig.c into      */
/*                                      an EarLab module          */
/* 31 Jul 2003            a.3    DJA    Created DataSink.cpp from */
/*                                      writer.c                  */
/* 14 Sep 2004			  2.0	 DJA	Added support for spike   */
/*                                      logging                   */
/******************************************************************/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <winsock2.h>
#include <stdio.h>
#include "Earlab.h"
#include "MatrixN.h"
#include "DataSink.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "EarlabException.h"

using namespace std;

DataSink::DataSink()
{
	memset(this, 0, sizeof(DataSink));
	mLogger = new Logger();
	SetModuleName("DataSink");
}

DataSink::~DataSink()
{
	if (mModuleName != NULL)
		delete [] mModuleName;
	if (mInputDims != NULL)
		delete [] mInputDims;
	if (mCFArray_Hz != NULL)
		delete [] mCFArray_Hz;
	if (mDownsampler != NULL)
		delete mDownsampler;
	if (mCurSampleBuffer != NULL)
		delete [] mCurSampleBuffer;
}

int DataSink::ReadParameters(char *ParameterFileName)
{
	if (mModuleName == NULL)
		return ReadParameters(ParameterFileName, "DataSink");
	else
	    return ReadParameters(ParameterFileName, mModuleName);
}

int DataSink::ReadParameters(char *ParameterFileName, char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	mLogger->Log("    ReadParameters: %s \"%s\"", mModuleName, ParameterFileName);

	// Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0);
    Status = theParamFile.GetParameter(SectionName, "FrameLength_Seconds", mFrameLength_Seconds, 0);
    Status = theParamFile.GetParameter(SectionName, "ProduceOutputFile", mProduceOutputFile, false);
    Status = theParamFile.GetParameter(SectionName, "UseDownsampler", mUseDownsampler, false);
    Status = theParamFile.GetParameter(SectionName, "OutputFileName", mOutputFileName, sizeof(mOutputFileName), "");
    Status = theParamFile.GetParameter(SectionName, "AcceptSocketConnections", mAcceptSocketConnections, false);
    Status = theParamFile.GetParameter(SectionName, "ListenPortNumber", mListenPort, 0);
	Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), NULL);
	Status = theParamFile.GetParameter(SectionName, "MetadataFileName", mMetadataFilename, sizeof(mMetadataFilename), NULL);

	mNumLevelsToKeep = 32;
	Status = theParamFile.GetParameter(SectionName, "DownsampleLevelsToKeep", mDownsampleLevelsToKeep, mNumLevelsToKeep, NULL, 0);
	return 1;
}

int DataSink::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount])
{
	struct sockaddr_in local;
	WORD VersionRequested;
	WSADATA WSAData;
	int status, LastError;
	unsigned long enable;
	FILE *CFFile;
	float tmp;

	mLogger->Log("    Start: %s", mModuleName);
	// Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	if (NumInputs != 1)
		throw EarlabException("%s: Currently this module can only handle one input stream.  Sorry!", mModuleName);

	if (NumOutputs != 0)
		throw EarlabException("%s: Currently this module cannot produce any output streams.  Sorry!", mModuleName);

	if ((InputTypes[0] != WaveformData) && (InputTypes[0] != SpikeData))
		throw EarlabException("%s: Currently this module can only handle either one waveform or one spike input data stream.  Sorry!", mModuleName);

	if (InputTypes[0] = WaveformData)
		ProcessingWaveforms = true;
	else
		ProcessingWaveforms = false;

	if (InputSize[0][0] != 0)
		mNumInputDims = 1;

	if (InputSize[0][1] != 0)
		mNumInputDims = 2;

	if (InputSize[0][2] != 0)
		mNumInputDims = 3;

	mInputDims = new int [mNumInputDims + 1];
	mInputDims[0] = mNumInputDims;
	for (int i = 0; i < EarlabMaxIOStreamDimensions; i++)
		mDimension[i] = 1;
	for (int i = 0; i < mNumInputDims; i++)
	{
		mInputDims[i + 1] = InputSize[0][i];
		mDimension[i] = InputSize[0][i];
	}

	mCFCount = 0;
	if ((mCFFileName != NULL) && (strlen(mCFFileName) != 0))
	{
		CFFile = fopen(mCFFileName, "r");
		if (CFFile != NULL)
		{
			while (!feof(CFFile))
			{
				fscanf(CFFile, "%f", &tmp);
				mCFCount++;
			}
			mCFCount--;

			mCFArray_Hz = new float[mCFCount];
			fseek(CFFile, 0, SEEK_SET);
			for (int i = 0; i < mCFCount; i++)
			{
				fscanf(CFFile, "%f", &tmp);
				mCFArray_Hz[i] = tmp;
			}
			fclose(CFFile);
		}
	}

	if (mProduceOutputFile)
	{
		mCurSampleBuffer = new float[mCFCount];
		if (mUseDownsampler)
		{
			if (mCFCount > 0)
			{
				mDownsampler = new Downsampler(0, mCFCount, mSampleRate_Hz, 8192, mOutputFileName);
				mDownsampler->SetKeepArray(mDownsampleLevelsToKeep, mNumLevelsToKeep);
			}
			else
				throw EarlabException("%s: Error: Downsampling requires a CF array to work properly", mModuleName);
		}
		else
		{
			if (mOutputFileName[0] != '\0')
			{
				mOutputFile = fopen(mOutputFileName, "wb");
				if (mOutputFile == NULL)
					throw EarlabException("%s: Error opening output file \"%s\"", mModuleName, mOutputFileName);
			}
			else
				throw EarlabException("%s: Error: ProduceOutputFile is TRUE and OutputFileName is not defined");
		}
	}

	if (mAcceptSocketConnections)
	{
		if (mMetadataFilename[0] == '\0')
			throw EarlabException("%s: When AcceptSocketConnections is true, MetadataFileName must also be provided", mModuleName);
		VersionRequested = MAKEWORD(2, 2);
		status = WSAStartup(VersionRequested, &WSAData);
		if (status != 0)
		{
			mAcceptSocketConnections = false;
			throw EarlabException("%s: WSAStartup() failed", mModuleName);
		}
		else
		{
			mListenSocket = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
			if (mListenSocket == INVALID_SOCKET)
			{
				mAcceptSocketConnections = false;
				LastError = WSAGetLastError();
				WSACleanup();
				throw EarlabException("%s: socket() failed with error %d", mModuleName, LastError);
			}
			else
			{
				local.sin_family = AF_INET;
				local.sin_addr.s_addr = INADDR_ANY; 
				local.sin_port = htons(mListenPort);
				if (bind(mListenSocket,(struct sockaddr*)&local,sizeof(local)) == SOCKET_ERROR) 
				{
					mAcceptSocketConnections = false;
					closesocket(mListenSocket);
					LastError = WSAGetLastError();
					WSACleanup();
					throw EarlabException("%s: bind() failed with error %d", mModuleName, LastError);
				}
				else
				{
					if (listen(mListenSocket,5) == SOCKET_ERROR)
					{
						mAcceptSocketConnections = false;
						closesocket(mListenSocket);
						LastError = WSAGetLastError();
						WSACleanup();
						throw EarlabException("%s: listen() failed with error %d", mModuleName, LastError);
					}
					else
					{
						enable = 1;
						if (ioctlsocket(mListenSocket, FIONBIO, &enable) != 0)
						{
							mAcceptSocketConnections = false;
							LastError = WSAGetLastError();
							closesocket(mListenSocket);
							WSACleanup();
							throw EarlabException("%s: ioctlsocket() failed with error %d", mModuleName, LastError);
						}
						else
							mLogger->Log("      %s: Listening for inbound connections on port %d", mModuleName, mListenPort);
					}
				}
			}
		}
	}
	mSampleCount = 0;

    return 1;
}

// Check in about using vectors of MatrixN's for input and output buffers
// Fallback position is float*[] for i/o vectors
// Limit the number of MatrixN dimensions to a #define number

int DataSink::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	FloatMatrixN *WaveInput;
	SpikeStream *SpikeInput;
	int result;

	mLogger->Log("    Advance: %s", mModuleName);

	switch (InputStream[0]->GetStreamType())
	{
	case WaveformData:
		WaveInput = ((EarlabWaveformStream *)InputStream[0])->GetData();
		result = ProcessWaveforms(WaveInput);
		break;
	case SpikeData:
		SpikeInput = ((EarlabSpikeStream *)InputStream[0])->GetData();
		result = ProcessSpikes(SpikeInput);
		CurFrameStart_Seconds += mFrameLength_Seconds;
		break;
	default:
		throw EarlabException("%s: Error, unknown stream type %d on input", mModuleName, InputStream[0]->GetStreamType());
		return 0;
		break;
	}
	return result;
}

int DataSink::ProcessSpikes(SpikeStream *SpikeInput)
{
	int i;
	int SpikeCount = SpikeInput->SpikeCount();
	Spike *CurSpike;

	for (i = 0; i < SpikeCount; i++)
	{
		CurSpike = SpikeInput->GetSpike(i);
		if (CurSpike != NULL)
		{
			if ((mProduceOutputFile) && (mOutputFile != NULL))
			{
				fprintf(mOutputFile, "%d %g\n", CurSpike->CellID, CurSpike->SpikeTime_Seconds + CurFrameStart_Seconds);
			}
		}
		else
			break;
	}
	fflush(mOutputFile);
	return SpikeCount + 1;
}

int DataSink::ProcessWaveforms(FloatMatrixN *WaveInput)
{
    int i, j;
	struct sockaddr_in from;
	int fromlen, retval;
	SOCKET inbound;
	int err;
	int CurSample = 0;
	FILE *MetadataFile;
	float *Data;
	unsigned long DataLen = 0;

	Data = WaveInput->GetData(&DataLen);
	if (mProduceOutputFile)
	{
		if ((mDownsampler != NULL) && (mNumInputDims == 2))
		{
			for (i = 0; i < mDimension[0]; i++)
			{
				for (j = 0; j < mDimension[1]; j++)
				{
					mCurSampleBuffer[j] = WaveInput->Data(i, j);
				}
				if ((mDownsampler != NULL) && ((mNumInputDims == 2)))
					mDownsampler->AddSample(mCurSampleBuffer);
			}
		}
		else
		{
			if (mOutputFile != NULL)
			{
				// Data = Input->GetData(&DataLen);
				for (i = 0; i < mDimension[0]; i++)
				{
					for (j = 0; j < mDimension[1]; j++)
						fwrite(&(WaveInput->Data(i, j)), sizeof(float), (size_t)1, mOutputFile);
				}
			}
		}
	}

	if (mAcceptSocketConnections)
	{
		char eof = 0x04;
		inbound = 0;
		while ((inbound != INVALID_SOCKET) && (mNumClients < MAX_INBOUND_SOCKETS))
		{
			fromlen = sizeof(from);
			inbound = accept(mListenSocket,(struct sockaddr*)&from, &fromlen);
			if (inbound != INVALID_SOCKET)
			{
				mLogger->Log("      %s: Accepted connection from %s, port %d",
					mModuleName, inet_ntoa(from.sin_addr), htons(from.sin_port));
				mNumClients++;
				for (i = 0; i < MAX_INBOUND_SOCKETS; i++)
				{
					if (mClientSocket[i] == 0)
					{
						mClientSocket[i] = inbound;
						break;
					}
				}
				if (i == MAX_INBOUND_SOCKETS)
					closesocket(inbound);
				else
				{
					u_long disable = 0;
					int LastError;
					if (ioctlsocket(inbound, FIONBIO, &disable) != 0)
					{
						mAcceptSocketConnections = false;
						LastError = WSAGetLastError();
						closesocket(mListenSocket);
						closesocket(inbound);
						WSACleanup();
						throw EarlabException("%s: ioctlsocket() failed with error %d", mModuleName, LastError);
					}
					MetadataFile = fopen(mMetadataFilename, "r");
					if (MetadataFile == NULL)
						throw EarlabException("%s: Error opening metadata file \"%s\"", mModuleName, mMetadataFilename);
					while (!feof(MetadataFile))
					{
						size_t len;
						char FileBuf[512];
						len = fread(FileBuf, sizeof(char), sizeof(FileBuf), MetadataFile);
						send(inbound, FileBuf, (int)len, 0);
					}
					send(inbound, &eof, sizeof(char), 0);
					fclose(MetadataFile);
					mLogger->Log("      %s: Sent metadata file \"%s\" to client %d", mModuleName, mMetadataFilename, i);
#ifdef OLD
					// Tell our client the number of dimensions and their respective sizes
					send(inbound, (const char *)&mSampleRate_Hz, sizeof(float), 0);
					send(inbound, (const char *)mInputDims, (mNumInputDims + 1) * sizeof(int), 0); 
					send(inbound, (const char *)&mCFCount, sizeof(int), 0);
					if (mCFCount > 0)
						send(inbound, (const char *)mCFArray_Hz, (mCFCount) * sizeof(float), 0);
#endif
				}
			}
		}

		Data = WaveInput->GetData(&DataLen);

		for (i = 0; i < MAX_INBOUND_SOCKETS; i++)
		{
			if (mClientSocket[i] != 0)
			{
				mLogger->Log("      %s: Sending %d bytes to client %d...", mModuleName,
					DataLen * sizeof(float), i);
				retval = send(mClientSocket[i], (char *)Data, DataLen * sizeof(float), 0);
				if (retval == SOCKET_ERROR)
				{
					err = WSAGetLastError();
					switch (err)
					{
					case WSAECONNRESET:
						mLogger->Log("      %s: Client %d closed its connection", mModuleName, i);
						break;
					default:
						mLogger->Log("      %s: Client %d send() failed with error", 
							mModuleName, i, WSAGetLastError());
						break;
					}
					closesocket(mClientSocket[i]);
					mClientSocket[i] = 0;
				}
				mLogger->Log("      %s: Sent %d bytes to client %d", 
					mModuleName, retval, i);
			}
		}
	}

    return 1;
}

int DataSink::Stop(void)
{
	mLogger->Log("    Stop: %s", mModuleName);
	if (mAcceptSocketConnections)
	{
		closesocket(mListenSocket);
		for (int i = 0; i < MAX_INBOUND_SOCKETS; i++)
			if (mClientSocket[i] != 0)
				closesocket(mClientSocket[i]);
	}
	if (mOutputFile != NULL)
		fclose(mOutputFile);
	if (mDownsampler != NULL)
		delete mDownsampler;
	if (mCurSampleBuffer != NULL)
		delete [] mCurSampleBuffer;
	mDownsampler = NULL;
	mCurSampleBuffer = NULL;
	return 1;
}

int DataSink::Unload(void)
{
	mLogger->Log("    Unload: %s", mModuleName);
	return 1;
}

void DataSink::SetModuleName(char *ModuleName)
{
	if (mModuleName != NULL)
		delete [] mModuleName;

	mModuleName = new char[strlen(ModuleName) + 1];
	strcpy(mModuleName, ModuleName);
}

void DataSink::SetLogger(Logger *TheLogger)
{
	if (mLogger != NULL)
		delete mLogger;
	mLogger = TheLogger;
}
