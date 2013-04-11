#ifndef _DOWNSAMPLER_H_INCLUDED
#define _DOWNSAMPLER_H_INCLUDED
#include "stdio.h"
#include <stdarg.h>

typedef enum
{
	// Note: DownsampleTypeAverage is the default downsample type
	DownsampleTypeAverage,		// Downsampled point is the average of the two corresponding data points in the lower level data set
	DownsampleTypeMax,			// Downsampled point is the maximum of the two corresponding data points in the lower level data set
} DownsampleType;

class Downsampler
{
public:
	Downsampler(int DownsampleLevel, int PointsPerSample, double SampleRate_Hz, int SamplesToBuffer, char *OutputFilename);
	~Downsampler();
	void SetType(DownsampleType TheType);		// Note: If the user changes this in midstream, the results may look wierd
	void SetMaxLevel(int MaxDownsampleLevel);	// This must be set before the first sample is presented for downsampling otherwise it will have no effect
	void SetMinimumSampleCountToKeep(unsigned long MinimumSampleCountToKeep);	// Number of samples to keep.  If sample count is less than this, the file will be deleted when downsampling is complete
	void AddSample(double *Sample);
private:
	void Flush(double *Buffer);
	void WriteMetadata(char *fmt, ...);

	Downsampler *mNext;

	int mDownsampleLevel, mMaxDownsampleLevel;
	unsigned long mMinimumSampleCountToKeep;
	int mPointsPerSample;
	unsigned long mSampleCount;
	double mSampleRate_Hz;
	int mSamplesToBuffer;

	char mBaseFilename[246];
	char mBinaryFilename[256];
	char mMetadataFilename[256];
	char mMetadataOutputBuffer[256];
	FILE *mBinaryFile, *mMetadataFile;
	//int mBinaryFD, mMetadataFD;
	double mMaximum, mMinimum;

	double *mCurSample[2];
	int mCurBuf;
	double *mDownsampledData;
	double *mSampleBuffer;
	int mSampleBufferPosition;
	int mSampleBufferSize;

	bool mCreatedOutputFiles;
	DownsampleType mDownsampleType;
};

#endif
