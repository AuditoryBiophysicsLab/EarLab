#include <iostream>
#include "ITDAzimuth.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "Earlab.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"
#include "Logger.h"

using namespace std;

ITDAzimuth::ITDAzimuth()
{
    memset(this, 0, sizeof(ITDAzimuth));
    SetModuleName("ITDAzimuth");
}

ITDAzimuth::~ITDAzimuth()
{
	int i;

	if (mCFArray_Hz != NULL)
		delete [] mCFArray_Hz;
	if (mAutocorrelationCurves != NULL)
	{
		for (i = 0; i < mCFCount; i++)
			if (mAutocorrelationCurves[i] != NULL)
				delete [] mAutocorrelationCurves[i];
		delete [] mAutocorrelationCurves;
	}
	if (mFrameBuffers != NULL)
	{
		for (i = 0; i < mFrameBufferCount; i++)
			DeleteFrameBuffer(mFrameBuffers[i]);
		delete [] mFrameBuffers;
	}
	if (mTotalBuffer != NULL)
		DeleteFrameBuffer(mTotalBuffer);
	if (mFilteredBuffer != NULL)
		DeleteFrameBuffer(mFilteredBuffer);
	if (mAutocorrelationCurveCenters != NULL)
		delete [] mAutocorrelationCurveCenters;
	if (mMaxLags != NULL)
		delete [] mMaxLags;
	if (mTotalWeights != NULL)
		delete [] mTotalWeights;
	if (mTotalAverages != NULL)
		delete [] mTotalAverages;
}

int ITDAzimuth::ReadParameters(const char *ParameterFileName)
{
    return ReadParameters(ParameterFileName, ModuleName());
}

int ITDAzimuth::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0);
    Status = theParamFile.GetParameter(SectionName, "MaxITD_uS", mMaxITD_uS, 500.0);
    Status = theParamFile.GetParameter(SectionName, "IntegrationWindow_Frames", mFrameBufferCount, 20);
	Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), NULL);
	return 1;
}

int ITDAzimuth::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	FILE *CFFile;
	float tmp;
	int i;
    // Because this module consumes data in a "Lag-Frequency-Time" format, the following statements apply:
    // InputSize[i][0] is the number of samples in a frame (the term "samples" includes data for all frequency channels at one time index)
    // OutputSize[i][1] is the number of frequency channels provided
    // OutputSize[i][2] must be 0 to mark the end of the dimension count

	Log("    Start: %s", ModuleName());
    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
    if (NumInputs != 1)
        throw EarlabException("%s: Currently this module can only handle one input stream.  Sorry!", ModuleName());

    if (NumOutputs != 1)
        throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", ModuleName());

	if (InputTypes[0] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform input data streams.  Sorry!", ModuleName());

	if (OutputTypes[0] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform output data streams.  Sorry!", ModuleName());

	if ((InputSize[0][0] == 0) || (InputSize[0][1] == 0) || (InputSize[0][2] != 0))
        throw EarlabException("%s: Input signal must be two-dimensional array.  Sorry!", ModuleName());

	if (mCFFileName == NULL)
		throw EarlabException("%s: Center Frequency Array is not optional.  You must define the CFFileName parameter", ModuleName());

	OutputElementCounts[0] = OutputSize[0][0] * OutputSize[0][1];

	mMaxLag_Samples = (InputSize[0][1] * 2) - 1;	// Minus one because both left and right have zero-lag data, so one will be discarded
	mZeroLag_Index = InputSize[0][1] + 1;			// This is the index of the "zero lag" sample
	mCFCount = 0;
	CFFile = OpenOutputFile(mCFFileName, "r");
	if (CFFile == NULL)
		throw EarlabException("%s: Error opening Center Frequency Array file \"%s\"");
	while (!feof(CFFile))
	{
		fscanf(CFFile, "%f", &tmp);
		mCFCount++;
	}
	mCFCount--;

	if (InputSize[0][0] != mCFCount)
		throw EarlabException("%s: Center Frequency array from file %s must contain the same number of channels as the inputs (%d).  Sorry!",
			ModuleName(), mCFFileName, InputSize[0][0]);

	mCFArray_Hz = new double[mCFCount];
	fseek(CFFile, 0, SEEK_SET);
	for (i = 0; i < mCFCount; i++)
	{
		fscanf(CFFile, "%f", &tmp);
		mCFArray_Hz[i] = tmp;
	}
	CloseFile(CFFile);

	mAutocorrelationCurves = new double *[mCFCount];
	mAutocorrelationCurveCenters = new int [mCFCount];
	mMaxLags = new double [mCFCount];
	mTotalWeights = new double [mCFCount];
	mTotalAverages = new double [mCFCount];
	memset(mAutocorrelationCurves, 0, sizeof(double *) * mCFCount);
	for (i = 0; i < mCFCount; i++)
		GenerateAutocorrelationCurve(i, mSampleRate_Hz, mCFArray_Hz[i], mMaxITD_uS);

	mTotalBuffer = NewFrameBuffer();	// Allocate a frame buffer to hold the totals of all the buffered frames
	mAdjustedTotalBuffer = NewFrameBuffer();
	mFilteredBuffer = NewFrameBuffer();	// Allocate a buffer to hold the filtered output of the total buffer
	mFrameBuffers = new double **[mFrameBufferCount];
	memset(mFrameBuffers, 0, sizeof(double **) * mFrameBufferCount);
    return 1;
}

// Note: We assume here that Input 0 is Left and Input 1 is Right
int ITDAzimuth::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	double AverageLag_Seconds;
	FloatMatrixN *InputL, *InputR, *Output;

    cout << "    Advance: " << ModuleName() << endl;

	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();
	InputL = ((EarlabWaveformStream *)InputStream[0])->GetData();	
	InputR = ((EarlabWaveformStream *)InputStream[1])->GetData();

	// Average zero lag data points and string the rest out like this: left < 0 < right
	// Average estimates over the last 100-200 ms
	// Inputs are single time point by freq chan by delay chan
	// Save last N frames of data to add the new one
	DeleteOldFrameFromBuffer();
	AddNewFrameToBuffer(InputL);
	AverageLag_Seconds = GetAverageLag_Seconds();
	if (AverageLag_Seconds < 0)
		Log("      Average lag this frame: left %g uS", fabs(AverageLag_Seconds * 1e6));
	else
	{
		if (AverageLag_Seconds > 0)
			Log("      Average lag this frame: right %g uS", AverageLag_Seconds * 1e6);
		else
			Log("      Average lag this frame: zero uS (straight ahead or straight behind)");
	}
	return 1;
}

int ITDAzimuth::Stop(void)
{
	Log("    Stop: %s", ModuleName());
    return 1;
}

int ITDAzimuth::Unload(void)
{
	Log("    Unload: %s", ModuleName());
    return 1;
}

//#define WRITE_OUTPUT

double ITDAzimuth::GetAverageLag_Seconds(void)
{
	int i, j, CurCenter, MaxIndex;
	double CurMax, CurPoint, AverageLag_Seconds;
	int Chan, Lag, count;
	double sum, average, total;
#ifdef WRITE_OUTPUT
	FILE *fp;
	char OutputFileName[100];
#endif

	total = 0;
	for (Chan = 0; Chan < mCFCount; Chan++)
	{
		count = 0;
		sum = 0.0;
		for (Lag = 0; Lag < mMaxLag_Samples; Lag++)
		{
			// Copy the total data into the adjusted total buffer
			mAdjustedTotalBuffer[Chan][Lag] = mTotalBuffer[Chan][Lag];
			// And calculate the total of all the samples in the current channel
			sum += mAdjustedTotalBuffer[Chan][Lag];
			count++;
		}
		// Compute the average value for the current channel
		average = sum / count;
		// Store the average in the array
		mTotalAverages[Chan] = average;
		// Add the current average to the total for the entire total buffer
		total += average;
		for (Lag = 0; Lag < mMaxLag_Samples; Lag++)
			mAdjustedTotalBuffer[Chan][Lag] -= average;	// Adjust all samples in the current frame by the average value
	}
	// Calculate the weighting factor for all channels.  Weighting is the percentage of the total average value in each channel
	for (Chan = 0; Chan < mCFCount; Chan++)
		mTotalWeights[Chan] = mTotalAverages[Chan] / total;	

	for (i = 0; i < mCFCount; i++)
	{
		CurCenter = mAutocorrelationCurveCenters[i];
		MaxIndex = -1;
		CurMax = -1e300;
		for (j = 0; j < mMaxLag_Samples; j++)
		{
			mFilteredBuffer[i][j] = CurPoint = ComputeLag(mAdjustedTotalBuffer[i], &(mAutocorrelationCurves[i][CurCenter - j]), 
															mMaxLag_Samples);
			// TODO: Insert code to pick the peak closest to the center, not just the highest overall if two are equal or nearly equal
			if (CurMax < CurPoint)
			{
				CurMax = CurPoint;
				MaxIndex = j;
			}
		}
#ifdef WRITE_OUTPUT
		sprintf(OutputFileName, "Input_%05.2fHz.txt", mCFArray_Hz[i]);
		fp = OpenOutputFile(OutputFileName, "w");
		if (fp != NULL)
		{
			for (j = 0; j < mMaxLag_Samples; j++)
				fprintf(fp, "%g\n", mAdjustedTotalBuffer[i][j]);
			CloseFile(fp);
		}
		sprintf(OutputFileName, "Output_%05.2fHz.txt", mCFArray_Hz[i]);
		fp = OpenOutputFile(OutputFileName, "w");
		if (fp != NULL)
		{
			for (j = 0; j < mMaxLag_Samples; j++)
				fprintf(fp, "%g\n", mFilteredBuffer[i][j]);
			CloseFile(fp);
		}
#endif
		mMaxLags[i] = ((double)MaxIndex - (double)mZeroLag_Index) * (1.0 / mSampleRate_Hz);
	}

	// Perform a weighted average of the max lags with respect to the total peak value
	AverageLag_Seconds = 0;
	for (i = 0; i < mCFCount; i++)
		AverageLag_Seconds += (mMaxLags[i] * mTotalWeights[i]);	
	return AverageLag_Seconds;
}
#undef WRITE_OUTPUT

double **ITDAzimuth::NewFrameBuffer(void)
{
	int i;
	double **buffer;

	buffer = new double *[mCFCount];
	memset(buffer, 0, sizeof(double *) * mCFCount);
	for (i = 0; i < mCFCount; i++)
	{
		buffer[i] = new double [mMaxLag_Samples];
		memset(buffer[i], 0, sizeof(double) * mMaxLag_Samples);
	}
	return buffer;
}

void ITDAzimuth::DeleteFrameBuffer(double **FrameBuffer)
{
	int i;
	
	if (FrameBuffer == NULL)
		return;
	for (i = 0; i < mCFCount; i++)
		if (FrameBuffer[i] != NULL)
			delete [] FrameBuffer[i];
	delete [] FrameBuffer;
}

void ITDAzimuth::AddNewFrameToBuffer(FloatMatrixN *Frame)
{
	double **NewFrame;
	int Chan, Lag;

	NewFrame = NewFrameBuffer();
	for (Chan = 0; Chan < mCFCount; Chan++)
		for (Lag = 0; Lag < mMaxLag_Samples; Lag++)
		{
			NewFrame[Chan][Lag] = Frame->Data(Chan, Lag);
			mTotalBuffer[Chan][Lag] += NewFrame[Chan][Lag];
		}
	mFrameBuffers[0] = NewFrame;
}

void ITDAzimuth::DeleteOldFrameFromBuffer(void)
{
	int Chan, Lag, Frame;
	double **OldFrame = mFrameBuffers[mFrameBufferCount - 1];

	if (OldFrame != NULL)
	{
		for (Chan = 0; Chan < mCFCount; Chan++)
			for (Lag = 0; Lag < mMaxLag_Samples; Lag++)
				mTotalBuffer[Chan][Lag] -= OldFrame[Chan][Lag];
		DeleteFrameBuffer(OldFrame);
	}
	for (Frame = mFrameBufferCount - 1; Frame > 0; Frame--)
		mFrameBuffers[Frame] = mFrameBuffers[Frame - 1];	// Move all frames down 1 in the buffer list
}

//#define WRITE_OUTPUT
void ITDAzimuth::GenerateAutocorrelationCurve(int AutocorrelationCurveIndex, double SampleRate_Hz, double CenterFrequency_Hz, double MaxITD_uS)
{
	int HeadSize_Samples, AutocorrelationBufferLength, i;
	double *AutocorrelationInputBuffer, *AutocorrelationOutputBuffer;
	double CurTime, TimeStep;
	int InputBufferLength_Samples;
#define TRY_LATER
#ifdef TRY_LATER
	double ScaleFactor;
#endif
#ifdef WRITE_OUTPUT
	FILE *fp;
	char OutputFileName[100];
#endif

	// Convert sample rate in hertz to microseconds per sample, then divide head size by that
	HeadSize_Samples = (int)(MaxITD_uS / (1e6 / SampleRate_Hz));
	CurTime = 0.0;
	TimeStep = 1.0 / SampleRate_Hz;
#ifdef TRY_LATER
	InputBufferLength_Samples = (int)ceil((1.0 / CenterFrequency_Hz) / TimeStep);
	ScaleFactor = (double)InputBufferLength_Samples / ((double)(HeadSize_Samples) * 2.0);
	if (ScaleFactor < 1.0)
	{
		ScaleFactor = ceil(1.0 / ScaleFactor);
		InputBufferLength_Samples *= (int)ScaleFactor;
	}
	mAutocorrelationCurveCenters[AutocorrelationCurveIndex] = InputBufferLength_Samples;
#else
	InputBufferLength_Samples = (HeadSize_Samples * 2) + 1;
	mAutocorrelationCurveCenters[AutocorrelationCurveIndex] = HeadSize_Samples * 2;
#endif
	AutocorrelationBufferLength = (InputBufferLength_Samples * 2) + 1;
	AutocorrelationInputBuffer = new double[InputBufferLength_Samples];
	mAutocorrelationCurves[AutocorrelationCurveIndex] = new double[AutocorrelationBufferLength];
	AutocorrelationOutputBuffer = mAutocorrelationCurves[AutocorrelationCurveIndex];
	for (i = 0; i < InputBufferLength_Samples; i++)
	{
		AutocorrelationInputBuffer[i] = sin(TWOPI * CenterFrequency_Hz * CurTime);
		CurTime += TimeStep;
	}
	CrossCorrelate(AutocorrelationInputBuffer, AutocorrelationInputBuffer, InputBufferLength_Samples, 
		AutocorrelationOutputBuffer, InputBufferLength_Samples);
#ifdef WRITE_OUTPUT
	sprintf(OutputFileName, "Signal_%05.2fHz.txt", CenterFrequency_Hz);
	fp = OpenOutputFile(OutputFileName, "w");
	if (fp != NULL)
	{
		for (i = 0; i < InputBufferLength_Samples; i++)
			fprintf(fp, "%g\n", AutocorrelationInputBuffer[i]);
		CloseFile(fp);
	}
	sprintf(OutputFileName, "Autocorr_%05.2fHz.txt", CenterFrequency_Hz);
	fp = OpenOutputFile(OutputFileName, "w");
	if (fp != NULL)
	{
		for (i = 0; i < AutocorrelationBufferLength; i++)
			fprintf(fp, "%g\n", AutocorrelationOutputBuffer[i]);
		CloseFile(fp);
	}
#endif
	delete [] AutocorrelationInputBuffer;
}
#undef WRITE_OUTPUT

int ITDAzimuth::CrossCorrelate(double sig1[], double sig2[], int n, double lag[], int shiftmax)
{
    int lagpos = 0;
    int cur_shift;
	double tmp;

    for (cur_shift = shiftmax - 1; cur_shift > 0; cur_shift--)              // Compute sig1 shift lags
	{
		tmp = ComputeLag(&sig1[cur_shift], sig2, n - cur_shift);
        lag[lagpos++] = tmp;
	}

	tmp = ComputeLag(sig1, sig2, n);									// Compute zero lag
    lag[lagpos++] = tmp;

    for (cur_shift = 1; cur_shift <= (shiftmax - 1); cur_shift++)             // Compute sig2 shift lag   
	{
        tmp = ComputeLag(sig1, &sig2[cur_shift], n - cur_shift);
        lag[lagpos++] = tmp;
	}

	return lagpos;
}

double ITDAzimuth::ComputeLag(double sig1[], double sig2[], int n)
{
    int i;
    double total = 0.0;

    for (i = 0; i < n; i++)
        total += sig1[i] * sig2[i];

    total /= n;

    return total;
}
