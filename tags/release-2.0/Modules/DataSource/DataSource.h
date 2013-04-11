/******************************************************************/
/*  DataSource.h                                                  */
/*  Header file for Signal Generator module                       */
/*                                                                */
/*  8 May 2003      1.0    DJA    Initial C version               */
/* 29 Jul 2003      1.1    DJA    C++ version                     */
/******************************************************************/
#include "Earlab.h"
#include "MatrixN.h"
#include <stdio.h>
#include "Logger.h"
#include "EarlabDataStream.h"

class DataSource
{
public:
    DataSource();
    ~DataSource();

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
	void ReadWaveHeader(void);
	double ReadWaveData(void);
	char *mModuleName;

	char mInputFileName[256];
	FILE *mInputFile;

	bool mInputIsWaveFile;
	int mWaveFileChannelNumber;
	int mWaveFileNumChannels;
	int mWaveFileBytesPerSample;
	double mWaveFileScaleFactor;
	int mNumOutputDims;
	int mBitsPerSample;
	unsigned char *mU8;
	short *mS16;
	unsigned char *mS24;
	int mDimension[EarlabMaxIOStreamDimensions];
	int mOutputDelay_Samples;
	double mSampleRate_Hz;
	double *mSampleBuf;
};
