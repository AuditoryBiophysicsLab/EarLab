#include "MatrixN.h"
#include "Earlab.h"
#include <stdio.h>
#include "Logger.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"

class ITDAzimuth : public EarlabModule
{
public:
    ITDAzimuth();
    ~ITDAzimuth();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	void GenerateAutocorrelationCurve(int AutocorrelationCurveIndex, double SampleRate_Hz, double CenterFrequency_Hz, double MaxITD_uS);
	int CrossCorrelate(double sig1[], double sig2[], int n, double lag[], int shiftmax);
	double ComputeLag(double sig1[], double sig2[], int n);
	double **NewFrameBuffer(void);
	void DeleteFrameBuffer(double **FrameBuffer);
	void AddNewFrameToBuffer(FloatMatrixN *Frame);
	void DeleteOldFrameFromBuffer(void);
	double GetAverageLag_Seconds(void);

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
