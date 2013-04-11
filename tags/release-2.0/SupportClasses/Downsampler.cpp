#include "Downsampler.h"
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "EarlabException.h"
#ifdef WIN32
# include <io.h>
#endif
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdarg.h>

const int MinimumDownsampleBuffer = 500;

using namespace std;

Downsampler::Downsampler(int DownsampleLevel, int PointsPerSample, double SampleRate_Hz, int SamplesToBuffer,
						 char *OutputFilename)
{
	memset(this, 0, sizeof(Downsampler));
	mKeepThisLevel = true;
	mMaximum = -1e300;
	mMinimum = 1e300;

	if (SamplesToBuffer < MinimumDownsampleBuffer)
		SamplesToBuffer = MinimumDownsampleBuffer;
	mDownsampleLevel = DownsampleLevel;
	mPointsPerSample = PointsPerSample;
	mSampleRate_Hz = SampleRate_Hz;
	mSamplesToBuffer = SamplesToBuffer;
	mSampleBufferSize = mSamplesToBuffer * mPointsPerSample;

	if (strlen(OutputFilename) > 245)
		throw EarlabException("Downsampler: File name is too long (%d bytes)", strlen(OutputFilename));
	strcpy(mBaseFilename, OutputFilename);

	sprintf(mBinaryFilename, "%s.%d.binary", mBaseFilename, DownsampleLevel);
	sprintf(mMetadataFilename, "%s.%d.metadata", mBaseFilename, DownsampleLevel);

	mCurSample[0] = new float[mPointsPerSample];
	mCurSample[1] = new float[mPointsPerSample];
	mDownsampledData = new float[mPointsPerSample];
	if (mSamplesToBuffer > 0)
		mSampleBuffer = new float[mSampleBufferSize];
	mDownsampleType = DownsampleTypeAverage;
	mMinimumSampleCountToKeep = MinimumDownsampleBuffer;
	mMaxDownsampleLevel = 32;
}

Downsampler::~Downsampler()
{
	if ((mSampleCount >= mMinimumSampleCountToKeep) && mKeepThisLevel)
	{
		if (mSampleBuffer != NULL)
			Flush(NULL);
#ifdef OLD_METADATA_FORMAT
		WriteMetadata("long\tNumber_Of_Samples\t%d\n", mSampleCount);
		WriteMetadata("float\tMaximum\t%f\n", mMaximum);
		WriteMetadata("float\tMinimum\t%f\n", mMinimum);
#else
		WriteXMLMetadata();
#endif
		fclose(mBinaryFile);
		fclose(mMetadataFile);
	}

	delete [] mCurSample[0];
	delete [] mCurSample[1];
	delete [] mDownsampledData;
	if (mSampleBuffer != NULL)
		delete [] mSampleBuffer;
	if (mLevelsToKeep != NULL)
		delete [] mLevelsToKeep;

	if (mNext != NULL)
		delete mNext;
}

void Downsampler::SetType(DownsampleType TheType)
{
	mDownsampleType = TheType;
}

void Downsampler::SetMaxLevel(int MaxDownsampleLevel)
{
	mMaxDownsampleLevel = MaxDownsampleLevel;
}

void Downsampler::SetMinimumSampleCountToKeep(unsigned long MinimumSampleCountToKeep)
{
	mMinimumSampleCountToKeep = MinimumSampleCountToKeep;
}

void Downsampler::AddSample(float *Sample)
{
	int i;

	mSampleCount++;
	if (mSampleBuffer != NULL)
	{
		memcpy(&(mSampleBuffer[mSampleBufferPosition]), Sample, mPointsPerSample * sizeof(float));
		mSampleBufferPosition += mPointsPerSample;
		if (mSampleBufferPosition >= mSampleBufferSize)
			Flush(NULL);
	}
	else
		Flush(Sample);

	for (i = 0; i < mPointsPerSample; i++)
	{
		mMinimum = Sample[i] < mMinimum ? Sample[i] : mMinimum;
		mMaximum = Sample[i] > mMaximum ? Sample[i] : mMaximum;
	}
	if (mDownsampleLevel < mMaxDownsampleLevel)
	{
		memcpy(mCurSample[mCurBuf++], Sample, mPointsPerSample * sizeof(float));
		if (mCurBuf != 2)
			return;

		mCurBuf = 0;
		for (i = 0; i < mPointsPerSample; i++)
		{
			switch (mDownsampleType)
			{
			case DownsampleTypeMax:
				mDownsampledData[i] = mCurSample[0][i] > mCurSample[1][i] ? mCurSample[0][i] : mCurSample[1][i];
				break;
			case DownsampleTypeAverage:
			default:
				mDownsampledData[i] = (float)((mCurSample[0][i] + mCurSample[1][i]) / 2.0);
				break;
			}
		}

		if (mNext == NULL)
		{
			mNext = new Downsampler(mDownsampleLevel + 1, mPointsPerSample, mSampleRate_Hz / 2.0, mSamplesToBuffer / 2,
			mBaseFilename);
			mNext->SetMaxLevel(mMaxDownsampleLevel);
			mNext->SetMinimumSampleCountToKeep(mMinimumSampleCountToKeep);
			mNext->SetType(mDownsampleType);
			mNext->SetKeepArray(mLevelsToKeep, mNumLevelsToKeep);
		}

		mNext->AddSample(mDownsampledData);
	} // if (mDownsampleLevel < MaxDownsampleLevel)
}

// An array of level numbers to keep
void Downsampler::SetKeepArray(int *LevelsToKeep, int NumLevels)
{
	int i;

	if (mLevelsToKeep != NULL)
		delete [] mLevelsToKeep;

	mLevelsToKeep = NULL;
	mNumLevelsToKeep = 0;

	if ((LevelsToKeep == NULL) || (NumLevels == 0))
		mKeepThisLevel = true;
	{
		mNumLevelsToKeep = NumLevels;
		mLevelsToKeep = new int [mNumLevelsToKeep];

		mKeepThisLevel = false;
		for (i = 0; i < mNumLevelsToKeep; i++)
		{
			mLevelsToKeep[i] = LevelsToKeep[i];
			if (mLevelsToKeep[i] == mDownsampleLevel)
				mKeepThisLevel = true;
		}
		// If we've been told not to keep this level after we've already created an output file
		if (!mKeepThisLevel && mCreatedOutputFiles)
		{
			mCreatedOutputFiles = false;
			fclose(mBinaryFile);
			fclose(mMetadataFile);
			remove(mBinaryFilename);
			remove(mMetadataFilename);
		}
	}

	if (mNext != NULL)
		mNext->SetKeepArray(mLevelsToKeep, mNumLevelsToKeep);
}

void Downsampler::Flush(float *Buffer)
{
	if (!mKeepThisLevel)
	{
		mSampleBufferPosition = 0;
		return;
	}

	if (mSampleCount >= mMinimumSampleCountToKeep)
	{
		//cout << "Downsampler: Flushing buffer to disk" << endl
		if (!mCreatedOutputFiles)
		{
			//cout << "Downsampler: Creating output files" << endl;
			mBinaryFile = fopen(mBinaryFilename, "wb");
			mMetadataFile = fopen(mMetadataFilename, "w")   ;

#ifdef OLD_METADATA_FORMAT
			WriteMetadata("string\tFile_Type\tFrequency_Time\n");
			WriteMetadata("long\tFrequency_Dimension_Size\t%d\n", mPointsPerSample);
			WriteMetadata("float\tSample_Rate_Hz\t%f\n", mSampleRate_Hz);
#endif
			mCreatedOutputFiles = true;
		}
		if (Buffer == NULL)
			fwrite(mSampleBuffer, sizeof(float), mSampleBufferPosition, mBinaryFile);
		else
			fwrite(Buffer, sizeof(float), mPointsPerSample, mBinaryFile);
		mSampleBufferPosition = 0;
	}
}

void Downsampler::WriteMetadata(char *fmt, ...)
{
	va_list vl;

	if (mMetadataFile == NULL)
		throw EarlabException("Downsampler: Unable to write metadata file: File is not open");
	va_start(vl, fmt);
	vsprintf(mMetadataOutputBuffer, fmt, vl);
	fwrite(mMetadataOutputBuffer, sizeof(char), strlen(mMetadataOutputBuffer), mMetadataFile);
}

void Downsampler::WriteXMLMetadata(void)
{
	fprintf(mMetadataFile, "<?xml version=\"1.0\" standalone=\"yes\"?>\n<EarLabMetadata xmlns=\"http://earlab.bu.edu/software/specifications/schemas/EarLabMetadata.xsd\">\n");
	fprintf(mMetadataFile, "  <Parameters>\n");
    fprintf(mMetadataFile, "    <FileType>FrequencyTime</FileType>\n");
    fprintf(mMetadataFile, "    <MinValue>%f</MinValue>\n", mMinimum);
    fprintf(mMetadataFile, "    <MaxValue>%f</MaxValue>\n", mMaximum);
    fprintf(mMetadataFile, "    <SampleRate_Hz>%f</SampleRate_Hz>\n", mSampleRate_Hz);
	fprintf(mMetadataFile, "  </Parameters>\n");
	fprintf(mMetadataFile, "  <Dimensions>\n");
	fprintf(mMetadataFile, "    <Dimension>\n");
	fprintf(mMetadataFile, "     <Name>Frequency</Name>\n");
	fprintf(mMetadataFile, "     <Value>%d</Value>\n", mPointsPerSample);
	fprintf(mMetadataFile, "    </Dimension>\n");
	fprintf(mMetadataFile, "    <Dimension>\n");
	fprintf(mMetadataFile, "     <Name>Time</Name>\n");
	fprintf(mMetadataFile, "     <Value>%d</Value>\n", mSampleCount);
	fprintf(mMetadataFile, "    </Dimension>\n");
	fprintf(mMetadataFile, "  </Dimensions>\n");
	fprintf(mMetadataFile, "</EarLabMetadata>\n");
}
