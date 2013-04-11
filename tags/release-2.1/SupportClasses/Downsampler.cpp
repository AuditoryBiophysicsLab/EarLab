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
#include "ModuleServices.h"

const int MinimumDownsampleBuffer = 500;

using namespace std;

Downsampler::Downsampler(int DownsampleLevel, int PointsPerSample, double SampleRate_Hz, int SamplesToBuffer,
						 const char *OutputFilename, const char *CFFileName, const char *ModuleInstanceName)
{
	memset(this, 0, sizeof(Downsampler));
	mKeepThisLevel = true;

	if (SamplesToBuffer < MinimumDownsampleBuffer)
		SamplesToBuffer = MinimumDownsampleBuffer;
	mDownsampleLevel = DownsampleLevel;
	mPointsPerSample = PointsPerSample;
	mSampleRate_Hz = SampleRate_Hz;
	mSamplesToBuffer = SamplesToBuffer;
	mSampleBufferSize = mSamplesToBuffer * mPointsPerSample;

	if (CFFileName != NULL)
	{
		mCFFileName = new char [strlen(CFFileName) + 1];
		strcpy(mCFFileName, CFFileName);
	}

	if (strlen(OutputFilename) > 245)
		throw EarlabException("Downsampler: File name is too long (%d bytes)", strlen(OutputFilename));
	strcpy(mBaseFilename, OutputFilename);

	sprintf(mBinaryFilename, "%s.%d.binary", mBaseFilename, DownsampleLevel);
	sprintf(mMetadataFilename, "%s.%d.metadata", mBaseFilename, DownsampleLevel);
	strncpy(mInstanceName, ModuleInstanceName, sizeof(mInstanceName) - 1);

	mCurSample[0] = new float[mPointsPerSample];
	mCurSample[1] = new float[mPointsPerSample];
	mDownsampledData = new float[mPointsPerSample];
	if (mSamplesToBuffer > 0)
		mSampleBuffer = new float[mSampleBufferSize];
	mDownsampleType = DownsampleTypeAverage;
	mMinimumSampleCountToKeep = MinimumDownsampleBuffer;
	mMaxDownsampleLevel = 32;
	mMetadataFile = new XmlMetadataFile(mMetadataFilename);
	mMetadataFile->SetSampleRate_Hz(mSampleRate_Hz);
	mMetadataFile->Add(new XmlParameter("Units", "Magnitude"));
	//mMetadataFile->Add(new XmlParameter("SampleRate_Hz", mSampleRate_Hz));
	if (mCFFileName != NULL)
		mMetadataFile->Add(new XmlDimension("Best Frequency (Hz)", mPointsPerSample, CFFileName));
	else
		mMetadataFile->Add(new XmlDimension("Frequency Channel", mPointsPerSample, 1, mPointsPerSample));
	mMetadataFile->SetSource(new XmlSource(mInstanceName, "DataSink_0.1.0.0"));
	//mMetadataFile->Write();
}

Downsampler::~Downsampler()
{
	if ((mMetadataFile->GetSampleCount() >= mMinimumSampleCountToKeep) && mKeepThisLevel)
	{
		if (mSampleBuffer != NULL)
			Flush(NULL);
		//WriteMetadata();
		fclose(mBinaryFile);
	}
	if (mCFFileName != NULL)
		delete [] mCFFileName;

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

	if (mSampleBuffer != NULL)
	{
		for (i = 0; i < mPointsPerSample; i++)
		{
			mMetadataFile->UpdateMinMax(Sample[i]);
			mSampleBuffer[mSampleBufferPosition + i] = Sample[i];
		}
		//memcpy(&(mSampleBuffer[mSampleBufferPosition]), Sample, mPointsPerSample * sizeof(float));
		mMetadataFile->AddSample();
		mSampleBufferPosition += mPointsPerSample;
		if (mSampleBufferPosition >= mSampleBufferSize)
			Flush(NULL);
	}
	else
		Flush(Sample);

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
			mNext = new Downsampler(mDownsampleLevel + 1, mPointsPerSample, mSampleRate_Hz / 2.0, mSamplesToBuffer / 2, mBaseFilename, mCFFileName, mInstanceName);
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

	if (mMetadataFile->GetSampleCount() >= mMinimumSampleCountToKeep)
	{
		//cout << "Downsampler: Flushing buffer to disk" << endl
		if (!mCreatedOutputFiles)
		{
			//cout << "Downsampler: Creating output files" << endl;
			mBinaryFile = OpenOutputFile(mBinaryFilename, "wb");
			mCreatedOutputFiles = true;
		}
		if (Buffer == NULL)
			fwrite(mSampleBuffer, sizeof(float), mSampleBufferPosition, mBinaryFile);
		else
			fwrite(Buffer, sizeof(float), mPointsPerSample, mBinaryFile);
		mMetadataFile->Write();
		mSampleBufferPosition = 0;
	}
}

void Downsampler::WriteMetadata(void)
{
	mMetadataFile->Write();
}
