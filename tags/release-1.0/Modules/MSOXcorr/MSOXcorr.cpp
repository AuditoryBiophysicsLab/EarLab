#include <iostream>
#include "MSOXcorr.h"
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

MSOXcorr::MSOXcorr()
{
    memset(this, 0, sizeof(MSOXcorr));
	mLogger = new Logger();
    SetModuleName("MSOXcorr");
}

MSOXcorr::~MSOXcorr()
{
    if (mModuleName != NULL)
        delete [] mModuleName;
    if (mIpsilateral != NULL)
        delete [] mIpsilateral;
    if (mContralateral != NULL)
        delete [] mContralateral;
    if (mLag != NULL)
        delete [] mLag;
	if (mWriter != NULL)
		delete mWriter;
}

int MSOXcorr::ReadParameters(char *ParameterFileName)
{
    return ReadParameters(ParameterFileName, mModuleName);
}

int MSOXcorr::ReadParameters(char *ParameterFileName, char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	mLogger->Log("    ReadParameters: %s \"%s\"", mModuleName, ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0);
    Status = theParamFile.GetParameter(SectionName, "MaxITD_uS", mMaxITD_uS, 500.0);
	Status = theParamFile.GetParameter(SectionName, "LagFileName", mLagFileName, sizeof(mLagFileName), "");
 	Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), "");

	return 1;
}

int MSOXcorr::Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, 
					int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions])
{
	char OutputFileName[256];
	int i;
	double uSPerSample;

	// Because this module produces data in a "Lag-Frequency" format, the following statements apply:
    // OutputSize[0][0] is the number of frequency channels provided
    // OutputSize[0][1] is the number of lags desired.  This must be sized according to the MaxITD_uS and SampleRate_Hz parameters
	// OutputSize[0][2] must be zero to mark the end of the provided dimensions
	// This module outputs one lag curve per channel per frame.  In other words, it downsamples the time dimension of the inputs by one frame size
	// Each output frame consists of OutputSize[0][0] frequency channels by OutputSize[0][1] lags

	mLogger->Log("    Start: %s", mModuleName);
    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	mMaxLag_Samples = (int)((mMaxITD_uS / (1e6 / mSampleRate_Hz)) + 0.5) + 1;	// Round off the sample count to the nearest integer

	if (NumInputs != 2)
        throw EarlabException("%s: Currently this module can only handle two input streams.  Sorry!", mModuleName);

    if (NumOutputs != 1)
        throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", mModuleName);

	if ((InputSize[0][0] == 0) || (InputSize[0][1] == 0) || (InputSize[0][2] != 0))
        throw EarlabException("%s: Input signal must be two-dimensional array.  Sorry!", mModuleName);

	if ((OutputSize[0][0] == 0) || (OutputSize[0][1] == 0) || (OutputSize[0][2] != 0))
        throw EarlabException("%s: Output signal must be two-dimensional array.  Sorry!", mModuleName);

    if (InputSize[0][0] != InputSize[1][0])
        throw EarlabException("%s: Input frame lengths must be identical.  Sorry!", mModuleName);

    if (InputSize[0][1] != InputSize[1][1])
        throw EarlabException("%s: Input channel counts must be identical.  Sorry!", mModuleName);

    if (InputSize[0][1] != OutputSize[0][0])
        throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", mModuleName);

    if (OutputSize[0][1] != mMaxLag_Samples)
        throw EarlabException("%s: Output lag dimension (%d) must match head size/sample rate (%d).  Sorry!", mModuleName, 
			OutputSize[0][1], mMaxLag_Samples);

	uSPerSample = 1e6 / mSampleRate_Hz;
	sprintf(OutputFileName, "%s.1", mModuleName);
	mWriter = new BinaryWriter(OutputFileName);
	mWriter->WriteMetadata("File_Type", "Lag_Frequency_Time");
	mWriter->WriteMetadata("Lag_Dimension_Size", mMaxLag_Samples);
	mWriter->WriteMetadata("Frequency_Dimension_Size", mNumChannels);
	mWriter->WriteMetadata("Min_Lag_uS", 0.0);
	mWriter->WriteMetadata("Max_Lag_uS", mMaxITD_uS);
	mWriter->WriteMetadata("Lag_Time_Step_uS", uSPerSample);
	mWriter->WriteMetadata("Sample_Rate_Hz", mSampleRate_Hz / mFrameSize_Samples);
	mWriter->CloseMetadataFile();

	if (mLagFileName[0] != '\0')
	{
		FILE *fp;

		fp = fopen(mLagFileName, "w");
		if (fp == NULL)
			throw EarlabException("%s: Error opening Lag file (%s) for writing", mModuleName, mLagFileName);
		for (i = 0; i < mMaxLag_Samples; i++)
			fprintf(fp, "%f\n", uSPerSample * i);
		fclose(fp);
	}
	mFrameSize_Samples = InputSize[0][0];
    mNumChannels = InputSize[0][1];
    mIpsilateral = new double[mFrameSize_Samples + 1];
    mContralateral = new double[mFrameSize_Samples + 1];
    mLag = new double[mMaxLag_Samples + 1];
	mFrameBuf[0].Init(InputSize[0][0], InputSize[0][1]);
	mFrameBuf[1].Init(InputSize[1][0], InputSize[1][1]);
	for (int i = 0; i < InputSize[0][0]; i++)
		for (int j = 0; j < InputSize[0][1]; j++)
		{
			mFrameBuf[0].Data(i, j) = 0.0;
			mFrameBuf[1].Data(i, j) = 0.0;
		}

    if (mMaxLag_Samples > mMaxLag_Samples)
        mMaxLag_Samples = mMaxLag_Samples;

    return 1;
}

// Note: We assume here that Input 0 is Ipsilateral and Input 1 is Contralateral
int MSOXcorr::Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount])
{
	int i, k, NumLags, CurIndex;
	double TestThreshold = 37;
	double *Data;
	unsigned long DataLen;

	mLogger->Log("    Advance: %s", mModuleName);

	if ((Input[0]->Rank(0) != mFrameSize_Samples) || (Input[0]->Rank(1) != mNumChannels) ||
		(Input[1]->Rank(0) != mFrameSize_Samples) || (Input[1]->Rank(1) != mNumChannels))
		throw EarlabException("%s: Input size mismatch with Start(): %d %d %d %d %d %d", mModuleName, 
			Input[0]->Rank(0), Input[0]->Rank(1), 
			Input[1]->Rank(0), Input[1]->Rank(1), 
			mFrameSize_Samples, mNumChannels);
	if ((Output[0]->Rank(0) != mNumChannels) || (Output[0]->Rank(1) != mMaxLag_Samples))
		throw EarlabException("%s: Output size mismatch with Start()", mModuleName);

	for (i = 0; i < mNumChannels; i++)
	{
		//mLogger->Log("Processing channel %d of %d", i + 1, mNumChannels);
		for (k = 0; k < mFrameSize_Samples; k++)
		{
			CurIndex = k + 1;
#ifdef OLD			
			if (CurIndex < mFrameSize_Samples)
			{
				// When we're checking lags from previous frame's data
				mIpsilateral[k] = mFrameBuf[0].Data(CurIndex, i);
				mContralateral[k] = mFrameBuf[1].Data(CurIndex, i);
			}
			else
			{
				// When we're checking lags from current frame's data
				CurIndex -= mFrameSize_Samples;
				mIpsilateral[k] = Input[0]->Data(CurIndex, i);
				mContralateral[k] = Input[1]->Data(CurIndex, i);
			}
#else
			mIpsilateral[k] = Input[0]->Data(k, i);
			mContralateral[k] = Input[1]->Data(k, i);
#endif
		}
		NumLags = HalfCorrelate(mContralateral, mIpsilateral, mFrameSize_Samples, mLag, mMaxLag_Samples);
		for (k = 0; k < NumLags; k++)
		{
			mLag[k] /= (37.5 * 37.5); // the 37.5*37.5 term is to normalize by the square of the spont rate
			Output[0]->Data(i, k) = mLag[k];
		}
		//mWriter->Write(mLag, NumLags);
	}
#ifdef OLD
	for (i = 0; i < mFrameSize_Samples; i++)
	{
		for (k = 0; k < mNumChannels; k++)
		{
			mFrameBuf[0].Data(i, k) = Input[0]->Data(i, k);
			mFrameBuf[1].Data(i, k) = Input[1]->Data(i, k);
		}
	}
#endif
	//mWriter->CloseBinaryFile();
	Data = Output[0]->GetData(&DataLen);
	mWriter->CheckMinMax(Data, DataLen);

	mFrameCount++;
    return mNumChannels * mMaxLag_Samples;
}

int MSOXcorr::Stop(void)
{
	mLogger->Log("    Stop: %s", mModuleName);
	mWriter->WriteMetadata("Number_Of_Samples", mFrameCount);
	mWriter->WriteMinMaxMetadata();
	mWriter->Close();
    return 1;
}

int MSOXcorr::Unload(void)
{
	mLogger->Log("    Unload: %s", mModuleName);
    return 1;
}

void MSOXcorr::SetModuleName(char *ModuleName)
{
    if (mModuleName != NULL)
        delete [] mModuleName;

    mModuleName = new char[strlen(ModuleName) + 1];
    strcpy(mModuleName, ModuleName);
}

void MSOXcorr::SetLogger(Logger *TheLogger)
{
	if (mLogger != NULL)
		delete mLogger;
	mLogger = TheLogger;
}

int MSOXcorr::HalfCorrelate(double ipsi[], double contra[], long n, double lag[], long shiftmax)
{
    int lagpos = 0;
    int cur_shift;
	double tmp;

	tmp = ComputeLag(ipsi, contra, n);										// Compute zero lag
    lag[lagpos++] = tmp;

    for (cur_shift = 1; cur_shift < shiftmax; cur_shift++)                  // Compute ipsilateral leading lags
	{
		tmp = ComputeLag(&ipsi[cur_shift], contra, n - cur_shift);
        lag[lagpos++] = tmp;
	}

	return lagpos;
}

double MSOXcorr::ComputeLag(double ipsi[], double contra[], long n)
{
    long i;
    double total = 0.0;

    for (i = 0; i < n; i++)
        total += ipsi[i] * contra[i];

    total /= n;

    return total;
}
