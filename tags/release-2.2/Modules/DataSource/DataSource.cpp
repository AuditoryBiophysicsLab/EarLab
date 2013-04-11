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
	SetModuleName("DataSource");
	SavedCellID = -1;
}

DataSource::~DataSource()
{
	if (mInputFile != NULL)
        CloseFile(mInputFile);

	if (mMetadataFile != NULL)
		delete mMetadataFile;
}

/******************************************************************/
/*                          GetParameters                         */
/******************************************************************/

int DataSource::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
		return ReadParameters(ParameterFileName, "DataSource");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int DataSource::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0);
    Status = theParamFile.GetParameter(SectionName, "FrameLength_Seconds", mFrameLength_Seconds, 0);

	Status = theParamFile.GetParameter(SectionName, "InputIsWaveFile", mInputIsWaveFile, false);
	Status = theParamFile.GetParameter(SectionName, "InputIsBinary", mInputIsBinary, false);
	Status = theParamFile.GetParameter(SectionName, "WaveFileChannelNumber", mWaveFileChannelNumber, 1);
	Status = theParamFile.GetParameter(SectionName, "WaveFileScaleFactor", mWaveFileScaleFactor, 1.0);
	Status = theParamFile.GetParameter(SectionName, "InputGain", mInputGain, 1.0);

	Status = theParamFile.GetParameter(SectionName, "InputFileName", mInputFileName, sizeof(mInputFileName), NULL);
	if (mInputFileName[0] == '\0')
		throw EarlabException("%s: No input file name was specified.  Parameter file: %s", ModuleName(), ParameterFileName);

	Status = theParamFile.GetParameter(SectionName, "OutputDelay_Samples", mOutputDelay_Samples, 0);
	return 1;
}

int DataSource::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount])
{
	// Because this module produces data in a time series format, the following statements apply:
	// OutputSize[i][0] is the number of samples in a frame
	// OutputSize[i][1] must be 0 to mark the end of the dimension count
	char MetadataFileName[256];

	Log("    Start: %s", ModuleName());
	// Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	if (NumInputs != 0)
		throw EarlabException("%s: Currently this module cannot handle input streams.  Sorry!", ModuleName());

	if (NumOutputs != 1)
		throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", ModuleName());

	if (OutputTypes[0] == WaveformData)
		mInputIsSpikeData = false;
	else
		mInputIsSpikeData = true;

	if (OutputSize[0][0] != 0)
		mNumOutputDims = 1;

	if (OutputSize[0][1] != 0)
		mNumOutputDims = 2;

	if (OutputSize[0][2] != 0)
		mNumOutputDims = 3;

	for (int i = 0; i < EarlabMaxIOStreamDimensions; i++)
		mDimension[i] = 1;

	OutputElementCounts[0] = 1;
	for (int i = 0; i < mNumOutputDims; i++)
	{
		mDimension[i] = OutputSize[0][i];
		OutputElementCounts[0] *= OutputSize[0][i];
	}
	if (mInputIsSpikeData)
	{
		OutputElementCounts[0] *= (unsigned long)(mFrameLength_Seconds * 2000);
	}
	mNumCellIDs = OutputElementCounts[0];

	if (mInputIsWaveFile)
	{
		if (mInputIsSpikeData)
			throw EarlabException("%s: Wave file input is not compatible with spike data output", ModuleName());

		if (mInputFileName != NULL)
			mInputFile = OpenInputFile(mInputFileName, "rb");

		if (mInputFile == NULL)
			throw EarlabException("%s: Error opening specified input wave file: \"%s\"", ModuleName(), mInputFileName);

		ReadWaveHeader();
	}
	else
	{
		if (mInputFileName[0] != '\0')
		{
			if (mInputIsBinary)
				mInputFile = OpenInputFile(mInputFileName, "rb");
			else
				mInputFile = OpenInputFile(mInputFileName, "r");
		}

		if (mInputFile == NULL)
			throw EarlabException("%s: Error opening specified input file: \"%s\"", ModuleName(), mInputFileName);
	}

	sprintf(MetadataFileName, "%s.1.metadata", ModuleName());
	mMetadataFile = new XmlMetadataFile(MetadataFileName);
	mMetadataFile->SetSampleRate_Hz(mSampleRate_Hz);
	switch (mNumOutputDims)
	{
	case 2:
		mMetadataFile->Add(new XmlDimension("Dim 1", OutputSize[0][1], 1, OutputSize[0][1]));
		break;
	case 3:
		mMetadataFile->Add(new XmlDimension("Dim 1", OutputSize[0][1], 1, OutputSize[0][1]));
		mMetadataFile->Add(new XmlDimension("Dim 2", OutputSize[0][2], 1, OutputSize[0][2]));
		break;
	default:
		break;
	}
	//mMetadataFile->Add(new XmlParameter("FrameSize", OutputSize[0][0]));
	//mMetadataFile->Add(new XmlParameter("SampleRate_Hz", mSampleRate_Hz));
	mMetadataFile->Add(new XmlParameter("Units", "Magnitude"));
	mMetadataFile->SetSource(new XmlSource(ModuleName(), "DataSource_0.1.0.0"));
	mMetadataFile->Write();

    return 1;
}

int DataSource::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	float NewSample;
	FloatMatrixN *WaveOutput;
	SpikeStream *SpikeOutput;
	int OutputCount = 0;

	Log("    Advance: %s", ModuleName());

	if (mInputIsSpikeData)
	{
		StartOfFrame_Seconds = (float)(mFrameCount * mFrameLength_Seconds);
		EndOfFrame_Seconds = (float)((mFrameCount + 1) * mFrameLength_Seconds);
		SpikeOutput = ((EarlabSpikeStream *)OutputStream[0])->GetData();
		SpikeOutput->SetSampleRate_Hz(mSampleRate_Hz);
		SpikeOutput->NewFrame();
		OutputCount = 1;
		while (!feof(mInputFile))
		{
			if (SavedSpikeTime_Seconds >= 0)
			{
				if (SavedSpikeTime_Seconds >= EndOfFrame_Seconds)
					break;
				if (SavedCellID != -1)
				{
					DeltaSpikeTime_Seconds = (float)(SavedSpikeTime_Seconds - StartOfFrame_Seconds);
					SpikeOutput->Fire(SavedCellID, DeltaSpikeTime_Seconds);
					SavedCellID = -1;
					SavedSpikeTime_Seconds = -1;
					OutputCount++;
				}
			}
			if (mInputIsBinary)
			{
				fread(&SavedCellID, sizeof(int), 1, mInputFile);
				fread(&SavedSpikeTime_Seconds, sizeof(double), 1, mInputFile);
			}
			else
			{
				fscanf(mInputFile, "%d %f", &SavedCellID, &DeltaSpikeTime_Seconds);
				SavedSpikeTime_Seconds = DeltaSpikeTime_Seconds;
			}
		} // while (!feof(mInputFile))
		mFrameCount++;
	} // if (mInputIsSpikeData)
	else
	{
		WaveOutput = ((EarlabWaveformStream *)OutputStream[0])->GetData();

		if (WaveOutput->Rank(0) != mDimension[0])
			throw EarlabException("%s: WaveOutput size mismatch with Start()", ModuleName());

		for (int x = 0; x < mDimension[0]; x++)
		{
			for (int y = 0; y < mDimension[1]; y++)
			{
				for (int z = 0; z < mDimension[2]; z++)
				{
					if (mOutputDelay_Samples > 0)
					{
						NewSample = 0.0;
						mOutputDelay_Samples--;
					}
					else
					{
						if (mInputIsWaveFile)
						{
							NewSample = (float)ReadWaveData();
						}
						else
						{
							if ((mInputFile != NULL) && (!feof(mInputFile)))
							{
								NewSample = 0.0f;
								if (mInputIsBinary)
									fread(&NewSample, sizeof(float), 1, mInputFile);
								else
									fscanf(mInputFile, "%f", &NewSample);
								NewSample = NewSample * (float)mInputGain;
							}
							else
								NewSample = 0.0;
							OutputCount++;
						}
						mMetadataFile->UpdateMinMax(NewSample);
					}
					switch (mNumOutputDims)
					{
					case 1:
						WaveOutput->Data(x) = NewSample;
						break;
					case 2:
						WaveOutput->Data(x, y) = NewSample;
						break;
					case 3:
						WaveOutput->Data(x, y, z) = NewSample;
						break;
					default:
						throw EarlabException("%s: Number of WaveOutput dimensions must be 1, 2, or 3", ModuleName());
						break;
					}
				}
			}
			mMetadataFile->AddSample();
		}
		return mDimension[0]*mDimension[1]*mDimension[2];
	} // if (mInputIsSpikeData)
	return OutputCount;
}

int DataSource::Stop(void)
{
	Log("    Stop: %s", ModuleName());
	mMetadataFile->Write();
    if (mInputFile != NULL)
        CloseFile(mInputFile);
	mInputFile = NULL;
	return 1;
}

int DataSource::Unload(void)
{
	Log("    Unload: %s", ModuleName());
	return 1;
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
			throw EarlabException("%s: Sample rate mismatch.  Config specified %d Hz and wave header specified %d Hz.", ModuleName(), mSampleRate_Hz, WaveHeader.SampleRate_Hz);
		else
			mSampleRate_Hz = WaveHeader.SampleRate_Hz;
		if (mWaveFileChannelNumber >= WaveHeader.NumChannels)
			throw EarlabException("%s: Channel count mismatch.  Config specified channel %d but wave header says only %d channels available", ModuleName(), mWaveFileChannelNumber, WaveHeader.NumChannels);
		mWaveFileNumChannels = WaveHeader.NumChannels;
		BitsFromBytes = ((float)WaveHeader.BytesPerSample / (float)WaveHeader.NumChannels) * 8;
		if ((float)WaveHeader.BitsPerSample != BitsFromBytes)
			throw EarlabException("%s: Sample size mismatch.  Config specified %d channels and %d bytes per sample.  This mismatches header information of %d bits per sample", ModuleName(), WaveHeader.NumChannels, WaveHeader.BytesPerSample, WaveHeader.BitsPerSample);
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
			throw EarlabException("%s: Sample size not supported.  Currently, only 8 or 16 bit samples are supported, but %s contains %d bit samples", ModuleName(), mInputFileName, WaveHeader.BitsPerSample);
			break;
		}
		mSampleBuf = new double [WaveHeader.NumChannels];
	}
	else
		throw EarlabException("%s: Error reading wave file header", ModuleName());
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
			//throw EarlabException("%s: End of file reading %s", ModuleName(), mInputFileName);
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
			//throw EarlabException("%s: End of file reading %s", ModuleName(), mInputFileName);
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
			//throw EarlabException("%s: End of file reading %s", ModuleName(), mInputFileName);
		}
		LongVal  = (mS24[(mWaveFileChannelNumber * 3) + 0] <<  0);	// LSB comes first
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
