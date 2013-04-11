#include "MatrixN.h"
#include "Earlab.h"
#include <stdio.h>
#include "Logger.h"

class ITDAzimuth
{
public:
    ITDAzimuth();
    ~ITDAzimuth();

    int ReadParameters(char *ParameterFileName);
    int ReadParameters(char *ParameterFileName, char *SectionName);

    int Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions]);
    int Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

	void SetModuleName(char *ModuleName);
	void SetLogger(Logger *TheLogger);

private:
	void GenerateAutocorrelationCurve(int AutocorrelationCurveIndex, double SampleRate_Hz, double CenterFrequency_Hz, double MaxITD_uS);
	int CrossCorrelate(double sig1[], double sig2[], long n, double lag[], long shiftmax);
	double ComputeLag(double sig1[], double sig2[], long n);
	double **NewFrameBuffer(void);
	void DeleteFrameBuffer(double **FrameBuffer);
	void AddNewFrameToBuffer(DoubleMatrixN *Frame);
	void DeleteOldFrameFromBuffer(void);
	double GetAverageLag_Seconds(void);

	Logger *mLogger;
	char *mModuleName;
	char mCFFileName[256];
	double mSampleRate_Hz;
	int mNumChannels, mMaxLag_Samples, mZeroLag_Index;
	double mMaxITD_uS;
	int mCFCount, mFrameBufferCount;
	double *mCFArray_Hz;
	double *mMaxLags;				// Stores the lag (positive = right, negative = left, zero = zero) for each peak
	double **mAutocorrelationCurves;
	int *mAutocorrelationCurveCenters;
	double ***mFrameBuffers;
	double **mTotalBuffer;
	double **mAdjustedTotalBuffer;
	double *mTotalWeights;
	double *mTotalAverages;
	double **mFilteredBuffer;
};
