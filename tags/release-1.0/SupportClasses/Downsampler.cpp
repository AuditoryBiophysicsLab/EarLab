#include "Downsampler.h"
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "EarlabException.h"
#include <io.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdarg.h>

const int MinimumDownsampleBuffer = 400;

using namespace std;

Downsampler::Downsampler(int DownsampleLevel, int PointsPerSample, double SampleRate_Hz, int SamplesToBuffer, 
						 char *OutputFilename)
{
	memset(this, 0, sizeof(Downsampler));
	mMaximum = -1e300;
	mMinimum = 1e300;

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

	mCurSample[0] = new double[mPointsPerSample];
	mCurSample[1] = new double[mPointsPerSample];
	mDownsampledData = new double[mPointsPerSample];
	if (mSamplesToBuffer > 0)
		mSampleBuffer = new double[mSampleBufferSize];
	mDownsampleType = DownsampleTypeAverage;
	mMinimumSampleCountToKeep = MinimumDownsampleBuffer;
	mMaxDownsampleLevel = 32;
}

Downsampler::~Downsampler()
{
	if (mSampleCount < mMinimumSampleCountToKeep) 
	{
		if (mCreatedOutputFiles)
		{
			fclose(mBinaryFile);
			fclose(mMetadataFile);
			remove(mBinaryFilename);
			remove(mMetadataFilename);
		}
	}
	else
	{
		if (mSampleBuffer != NULL)
			Flush(NULL);
		WriteMetadata("long\tNumber_Of_Samples\t%d\n", mSampleCount);
		WriteMetadata("double\tMaximum\t%f\n", mMaximum);
		WriteMetadata("double\tMinimum\t%f\n", mMinimum);
		fclose(mBinaryFile);
		fclose(mMetadataFile);
	}

	delete [] mCurSample[0];
	delete [] mCurSample[1];
	delete [] mDownsampledData;
	if (mSampleBuffer != NULL)
		delete [] mSampleBuffer;

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

void Downsampler::AddSample(double *Sample)
{
	int i;

	mSampleCount++;
	if (mSampleBuffer != NULL)
	{
		memcpy(&(mSampleBuffer[mSampleBufferPosition]), Sample, mPointsPerSample * sizeof(double));
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
		memcpy(mCurSample[mCurBuf++], Sample, mPointsPerSample * sizeof(double));
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
				mDownsampledData[i] = (mCurSample[0][i] + mCurSample[1][i]) / 2.0;
				break;
			}
		}

		if (mNext == NULL)
		{
			mNext = new Downsampler(mDownsampleLevel + 1, mPointsPerSample, mSampleRate_Hz / 2.0, mSamplesToBuffer / 2,
			mBaseFilename);
			mNext->SetMaxLevel(mMaxDownsampleLevel);
			mNext->SetMinimumSampleCountToKeep(mMinimumSampleCountToKeep / 2);
			mNext->SetType(mDownsampleType);
		}

		mNext->AddSample(mDownsampledData);
	} // if (mDownsampleLevel < MaxDownsampleLevel)
}

void Downsampler::Flush(double *Buffer)
{
	cout << "Downsampler: Flushing buffer to disk" << endl;
	if (!mCreatedOutputFiles)
	{
		cout << "Downsampler: Creating output files" << endl;
		mBinaryFile = fopen(mBinaryFilename, "wb");
		mMetadataFile = fopen(mMetadataFilename, "w")	;

		WriteMetadata("string\tFile_Type\tSample_Rate\n");
		WriteMetadata("long\tFixed_Dimension\t%d\n", mPointsPerSample);
		WriteMetadata("float\tSample_Rate_Hz\t%f\n", mSampleRate_Hz);
		mCreatedOutputFiles = true;
	}
	if (Buffer == NULL)
		fwrite(mSampleBuffer, sizeof(double), mSampleBufferPosition, mBinaryFile);
	else
		fwrite(Buffer, sizeof(double), mPointsPerSample, mBinaryFile);
	mSampleBufferPosition = 0;
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
