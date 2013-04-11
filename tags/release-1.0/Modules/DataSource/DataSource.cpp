/******************************************************************/
/*  DataSource.cpp                                           */
/*  Generates simple test signals for EarLab modules              */
/*                                                                */
/*  4 September 2001      a.1    AWH    First Alpha draft         */
/*  8 May 2003            a.2    DJA    Turned gensig.c into      */
/*                                      EarLab module siggen.c    */
/* 29 Jul 2003            a.3    DJA    Turned siggen.c into C++  */
/*                                      module DataSource.cpp*/
/******************************************************************/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "CParameterFile.h"
#include "Earlab.h"
#include "DataSource.h"
#include "EarlabException.h"

using namespace std;

DataSource::DataSource()
{
	memset(this, 0, sizeof(DataSource));
	mLogger = new Logger();
	SetModuleName("DataSource");
}

DataSource::~DataSource()
{
	if (mModuleName != NULL)
		delete [] mModuleName;

	if (mInputFile != NULL)
        fclose(mInputFile);
}

/******************************************************************/
/*                          GetParameters                         */
/******************************************************************/

int DataSource::ReadParameters(char *ParameterFileName)
{
	if (mModuleName == NULL)
		return ReadParameters(ParameterFileName, "DataSource");
	else
	    return ReadParameters(ParameterFileName, mModuleName);
}

int DataSource::ReadParameters(char *ParameterFileName, char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	mLogger->Log("    ReadParameters: %s \"%s\"", mModuleName, ParameterFileName);
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0);

	Status = theParamFile.GetParameter(SectionName, "InputIsWaveFile", mInputIsWaveFile, false);
	Status = theParamFile.GetParameter(SectionName, "WaveFileChannelNumber", mWaveFileChannelNumber, 1);
	Status = theParamFile.GetParameter(SectionName, "WaveFileScaleFactor", mWaveFileScaleFactor, 1.0);

	Status = theParamFile.GetParameter(SectionName, "InputFileName", mInputFileName, sizeof(mInputFileName), NULL);
	if (mInputFileName[0] == '\0')
		throw EarlabException("%s: No input file name was specified.  Parameter file: %s", mModuleName, ParameterFileName);

	Status = theParamFile.GetParameter(SectionName, "OutputDelay_Samples", mOutputDelay_Samples, 0);
	return 1;
}

int DataSource::Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions])
{
	// Because this module produces data in a time series format, the following statements apply:
	// OutputSize[i][0] is the number of samples in a frame
	// OutputSize[i][1] must be 0 to mark the end of the dimension count

	mLogger->Log("    Start: %s", mModuleName);
	// Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	if (NumInputs != 0)
		throw EarlabException("%s: Currently this module cannot handle input streams.  Sorry!", mModuleName);

	if (NumOutputs != 1)
		throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", mModuleName);

	if (OutputSize[0][0] != 0)
		mNumOutputDims = 1;

	if (OutputSize[0][1] != 0)
		mNumOutputDims = 2;

	if (OutputSize[0][2] != 0)
		mNumOutputDims = 3;

	for (int i = 0; i < EarlabMaxIOStreamDimensions; i++)
		mDimension[i] = 1;

	for (int i = 0; i < mNumOutputDims; i++)
		mDimension[i] = OutputSize[0][i];

	if (mInputIsWaveFile)
	{
		if (mInputFileName != NULL)
			mInputFile = fopen(mInputFileName, "rb");

		if (mInputFile == NULL)
			throw EarlabException("%s: Error opening specified input wave file: \"%s\"", mModuleName, mInputFileName);

		ReadWaveHeader();
	}
	else
	{
		if (mInputFileName[0] != '\0')
			mInputFile = fopen(mInputFileName, "r");

		if (mInputFile == NULL)
			throw EarlabException("%s: Error opening specified input file: \"%s\"", mModuleName, mInputFileName);
	}

    return 1;
}

int DataSource::Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount])
{
	float NewSample;

	mLogger->Log("    Advance: %s", mModuleName);

	if (Output[0]->Rank(0) != mDimension[0])
		throw EarlabException("%s: Output size mismatch with Start()", mModuleName);

	for (int x = 0; x < mDimension[0]; x++)
	{
		for (int y = 0; y < mDimension[1]; y++)
		{
			for (int z = 0; z < mDimension[2]; z++)
			{
				if (x < mOutputDelay_Samples)
					NewSample = 0.0;
				else
				{
					if (mInputIsWaveFile)
					{
						NewSample = (float)ReadWaveData();
					}
					else
					{
						if ((mInputFile != NULL) && (!feof(mInputFile)))
							fscanf(mInputFile, "%f", &NewSample);
						else
							NewSample = 0.0;
					}
				}
				switch (mNumOutputDims)
				{
				case 1:
					Output[0]->Data(x) = NewSample;
					break;
				case 2:
					Output[0]->Data(x, y) = NewSample;
					break;
				case 3:
					Output[0]->Data(x, y, z) = NewSample;
					break;
				default:
					throw EarlabException("%s: Number of output dimensions must be 1, 2, or 3", mModuleName);
					break;
				}
			}
		}
	}
	mOutputDelay_Samples = 0;

	return mDimension[0]*mDimension[1]*mDimension[2];
}

int DataSource::Stop(void)
{
	mLogger->Log("    Stop: %s", mModuleName);
    if (mInputFile != NULL)
        fclose(mInputFile);
	mInputFile = NULL;
	return 1;
}

int DataSource::Unload(void)
{
	mLogger->Log("    Unload: %s", mModuleName);
	return 1;
}

void DataSource::SetModuleName(char *ModuleName)
{
	if (mModuleName != NULL)
		delete [] mModuleName;

	mModuleName = new char[strlen(ModuleName) + 1];
	strcpy(mModuleName, ModuleName);
}

void DataSource::SetLogger(Logger *TheLogger)
{
	if (mLogger != NULL)
		delete mLogger;
	mLogger = TheLogger;
}

void DataSource::ReadWaveHeader(void)
{
	struct
	{
		char RIFF[4];
		int ChunkSize_1;
		char WAVE[4];
		char Subchunk1_ID[4];
		int Subchunk1Size;
		short AudioFormat;
		short NumChannels;
		int SampleRate_Hz;
		int BytesPerSec;
		short BytesPerSample;
		short BitsPerSample;
		char Subchunk2_ID[4];
		int Subchunk2Size;
	} WaveHeader;

	if (fread(&WaveHeader, sizeof(WaveHeader), 1, mInputFile) == 1)
	{
		float BitsFromBytes;
		if ((mSampleRate_Hz != WaveHeader.SampleRate_Hz) && (mSampleRate_Hz != 0))
			throw EarlabException("%s: Sample rate mismatch.  Config specified %d Hz and wave header specified %d Hz.", mModuleName, mSampleRate_Hz, WaveHeader.SampleRate_Hz);
		else
			mSampleRate_Hz = WaveHeader.SampleRate_Hz;
		if (mWaveFileChannelNumber >= WaveHeader.NumChannels)
			throw EarlabException("%s: Channel count mismatch.  Config specified channel %d but wave header says only %d channels available", mModuleName, mWaveFileChannelNumber, WaveHeader.NumChannels);
		mWaveFileNumChannels = WaveHeader.NumChannels;
		BitsFromBytes = ((float)WaveHeader.BytesPerSample / (float)WaveHeader.NumChannels) * 8;
		if ((float)WaveHeader.BitsPerSample != BitsFromBytes)
			throw EarlabException("%s: Sample size mismatch.  Config specified %d channels and %d bytes per sample.  This mismatches header information of %d bits per sample", mModuleName, WaveHeader.NumChannels, WaveHeader.BytesPerSample, WaveHeader.BitsPerSample);
		mBitsPerSample = WaveHeader.BitsPerSample;
		switch (mBitsPerSample)
		{
		case 8:
			mU8 = new unsigned char [mWaveFileNumChannels];
			break;
		case 16:
			mS16 = new short [mWaveFileNumChannels];
			break;
		case 24:
			mS24 = new unsigned char [mWaveFileNumChannels * 3];
			break;
		default:
			throw EarlabException("%s: Sample size not supported.  Currently, only 8 or 16 bit samples are supported, but %s contains %d bit samples", mModuleName, mInputFileName, WaveHeader.BitsPerSample);
			break;
		}
		mSampleBuf = new float [WaveHeader.NumChannels];
	}
	else
		throw EarlabException("%s: Error reading wave file header", mModuleName);
}

double DataSource::ReadWaveData(void)
{
	size_t result;
	double value;
	long LongVal;
	switch (mBitsPerSample)
	{
	case 8:
		result = fread(mU8, sizeof(char), mWaveFileNumChannels, mInputFile);
		if (feof(mInputFile))
		{
			return 0.0;
			//throw EarlabException("%s: End of file reading %s", mModuleName, mInputFileName);
		}
		value = (double)(mU8[mWaveFileChannelNumber] - 128);
		value = value / 128.0;
		value = value * mWaveFileScaleFactor;
		break;
	case 16:
		result = fread(mS16, sizeof(short), mWaveFileNumChannels, mInputFile);
		if (feof(mInputFile))
		{
			return 0.0;
			//throw EarlabException("%s: End of file reading %s", mModuleName, mInputFileName);
		}
		value = (double)mS16[mWaveFileChannelNumber];
		value = value / 32768.0;
		value = value * mWaveFileScaleFactor;
		break;
	case 24:
		result = fread(mS24, 3, mWaveFileNumChannels, mInputFile);
		if (feof(mInputFile))
		{
			return 0.0;
			//throw EarlabException("%s: End of file reading %s", mModuleName, mInputFileName);
		}
		LongVal |= (mS24[(mWaveFileChannelNumber * 3) + 0] <<  0);	// LSB comes first
		LongVal |= (mS24[(mWaveFileChannelNumber * 3) + 1] <<  8);
		LongVal |= (mS24[(mWaveFileChannelNumber * 3) + 2] << 16);	// MSB comes last
		if (LongVal & 0x00800000)
		{
			LongVal &= 0xFF7FFFFF;	// Turn off sign bit of 24-bit quantity
			LongVal *= -1;			// And make the long value negative
		}
		value = (double)LongVal;
		value = value / 8388608.0;
		value = value * mWaveFileScaleFactor;
		break;
	default:
		return 0.0;
		break;
	}
	return value;
}
