/******************************************************************/
/*  DataSink.h                                                    */
/*  Header file for generic Data Sink module                      */
/*                                                                */
/*  8 May 2003      1.0    DJA    Initial version                 */
/* 31 Jul 2003      1.1    DJA    Converted code from writer.h    */
/*                                into C++ and made it DataSink.h */
/******************************************************************/

#include "MatrixN.h"
#include "stdio.h"
#include "Earlab.h"
#include "Downsampler.h"
#include "Logger.h"
#include "EarlabDataStream.h"

#define MAX_INBOUND_SOCKETS 10
class DataSink
{
public:
    DataSink();
    ~DataSink();

    int ReadParameters(char *ParameterFileName);
    int ReadParameters(char *ParameterFileName, char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

	void SetModuleName(char *ModuleName);
	void SetLogger(Logger *TheLogger);

private:
	Logger *mLogger;
	void Initialize(void);  // Called by the constructors to perform common initialization tasks
	int ProcessWaveforms(FloatMatrixN *WaveInput);
	int ProcessSpikes(SpikeStream *SpikeInput);
	int mDownsampleLevelsToKeep[32];
	int mNumLevelsToKeep;

	char *mModuleName;

	char mCFFileName[256];
	char mMetadataFilename[256];
	float *mCFArray_Hz;
	int mCFCount;
	double mSampleRate_Hz;
	double mFrameLength_Seconds;
	double CurFrameStart_Seconds;
	unsigned long mSampleCount;
	int mDimension[EarlabMaxIOStreamDimensions];
	char mOutputFileName[256];
	bool mProduceOutputFile;
	bool mAcceptSocketConnections;
	bool mUseDownsampler;
	int mListenPort, mNumClients;
	SOCKET mListenSocket, mClientSocket[MAX_INBOUND_SOCKETS];
	int mNumInputDims;
	int *mInputDims;
	FILE *mOutputFile;
	bool ProcessingWaveforms;

	Downsampler *mDownsampler;
	float *mCurSampleBuffer;
};
