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
    SetModuleName("MSOXcorr");
}

MSOXcorr::~MSOXcorr()
{
    if (mIpsilateral != NULL)
        delete [] mIpsilateral;
    if (mContralateral != NULL)
        delete [] mContralateral;
    if (mLag != NULL)
        delete [] mLag;
	if (mMetadataFile != NULL)
		delete mMetadataFile;
}

int MSOXcorr::ReadParameters(const char *ParameterFileName)
{
    return ReadParameters(ParameterFileName, ModuleName());
}

int MSOXcorr::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0);
    Status = theParamFile.GetParameter(SectionName, "MaxITD_uS", mMaxITD_uS, 500.0);
	Status = theParamFile.GetParameter(SectionName, "LagFileName", mLagFileName, sizeof(mLagFileName), "");
 	Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), "");

	return 1;
}

int MSOXcorr::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount])
{
	char MetadataFileName[256];
	int i;
	double uSPerSample;

	// Because this module produces data in a "Frequency-Lag" format, the following statements apply:
    // OutputSize[0][0] is the number of lags desired.  This must be sized according to the MaxITD_uS and SampleRate_Hz parameters
    // OutputSize[0][1] is the number of frequency channels provided
	// OutputSize[0][2] must be zero to mark the end of the provided dimensions
	// This module outputs one lag curve per channel per frame.  In other words, it downsamples the time dimension of the inputs by one frame size
	// Each output frame consists of OutputSize[0][0] frequency channels by OutputSize[0][1] lags

	Log("    Start: %s", ModuleName());
    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	mMaxLag_Samples = (int)((mMaxITD_uS / (1e6 / mSampleRate_Hz)) + 0.5) + 1;	// Round off the sample count to the nearest integer

	if (NumInputs != 2)
        throw EarlabException("%s: Currently this module can only handle two input streams.  Sorry!", ModuleName());

    if (NumOutputs != 1)
        throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", ModuleName());

	if (InputTypes[0] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform input data streams.  Sorry!", ModuleName());

	if (InputTypes[1] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform input data streams.  Sorry!", ModuleName());

	if (OutputTypes[0] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform output data streams.  Sorry!", ModuleName());

	if ((InputSize[0][0] == 0) || (InputSize[0][1] == 0) || (InputSize[0][2] != 0))
        throw EarlabException("%s: Input signal 1 must be two-dimensional array.  Sorry!", ModuleName());

	if ((InputSize[1][0] == 0) || (InputSize[1][1] == 0) || (InputSize[1][2] != 0))
        throw EarlabException("%s: Input signal 2 must be two-dimensional array.  Sorry!", ModuleName());

	if ((OutputSize[0][0] == 0) || (OutputSize[0][1] == 0) || (OutputSize[0][2] != 0))
        throw EarlabException("%s: Output signal must be two-dimensional array.  Sorry!", ModuleName());

    if (InputSize[0][0] != InputSize[1][0])
        throw EarlabException("%s: Input frame lengths must be identical.  Sorry!", ModuleName());

    if (InputSize[0][1] != InputSize[1][1])
        throw EarlabException("%s: Input channel counts must be identical.  Sorry!", ModuleName());

    if (InputSize[0][1] != OutputSize[0][1])
        throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", ModuleName());

    if (OutputSize[0][0] != mMaxLag_Samples)
        throw EarlabException("%s: Output lag dimension (%d) must match head size/sample rate (%d).  Sorry!", ModuleName(), 
			OutputSize[0][0], mMaxLag_Samples);

	OutputElementCounts[0] = OutputSize[0][0] * OutputSize[0][1];

	uSPerSample = 1e6 / mSampleRate_Hz;
	sprintf(MetadataFileName, "%s.1.metadata", ModuleName());
	mMetadataFile = new XmlMetadataFile(MetadataFileName);
	mMetadataFile->SetSampleRate_Hz(mSampleRate_Hz);
	mMetadataFile->Add(new XmlParameter("Units", "IFR (Spikes/S)"));
	//mMetadataFile->Add(new XmlParameter("LagMin_uS", -1.0 * mMaxITD_uS));
	//mMetadataFile->Add(new XmlParameter("LagMax_uS", mMaxITD_uS));
	//mMetadataFile->Add(new XmlParameter("LagStep_uS", mMaxITD_uS / mMaxLag_Samples));
	//mMetadataFile->Add(new XmlParameter("SampleRate_Hz", mSampleRate_Hz));
	mMetadataFile->Add(new XmlDimension("Frequency (Hz)", mNumChannels, mCFFileName));
	mMetadataFile->Add(new XmlDimension("Interaural Time Difference (uS)", mMaxLag_Samples, 0, mMaxITD_uS));	// Linear lags from zero to mMaxITD_uS
	mMetadataFile->SetSource(new XmlSource(ModuleName(), "MSOXcorr_0.1.0.0"));
	mMetadataFile->Write();

	if (mLagFileName[0] != '\0')
	{
		FILE *fp;

		fp = OpenOutputFile(mLagFileName, "w");
		if (fp == NULL)
			throw EarlabException("%s: Error opening Lag file (%s) for writing", ModuleName(), mLagFileName);
		for (i = 0; i < mMaxLag_Samples; i++)
			fprintf(fp, "%f\n", uSPerSample * i);
		CloseFile(fp);
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
int MSOXcorr::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int i, k, NumLags, CurIndex;
	double TestThreshold = 37;
	FloatMatrixN *InputIpsi, *InputContra, *Output;

	Log("    Advance: %s", ModuleName());

	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();
	InputIpsi = ((EarlabWaveformStream *)InputStream[0])->GetData();	
	InputContra = ((EarlabWaveformStream *)InputStream[1])->GetData();

	if ((InputIpsi->Rank(0) != mFrameSize_Samples) || (InputIpsi->Rank(1) != mNumChannels) ||
		(InputContra->Rank(0) != mFrameSize_Samples) || (InputContra->Rank(1) != mNumChannels))
		throw EarlabException("%s: Input size mismatch with Start(): %d %d %d %d %d %d", ModuleName(), 
			InputIpsi->Rank(0), InputIpsi->Rank(1), 
			InputContra->Rank(0), InputContra->Rank(1), 
			mFrameSize_Samples, mNumChannels);
	if ((Output->Rank(0) != mMaxLag_Samples) || (Output->Rank(1) != mNumChannels))
		throw EarlabException("%s: Output size mismatch with Start()", ModuleName());

	for (i = 0; i < mNumChannels; i++)
	{
		//Log("Processing channel %d of %d", i + 1, mNumChannels);
		for (k = 0; k < mFrameSize_Samples; k++)
		{
			CurIndex = k + 1;
			mIpsilateral[k] = InputIpsi->Data(k, i);
			mContralateral[k] = InputContra->Data(k, i);
		}
		NumLags = HalfCorrelate(mContralateral, mIpsilateral, mFrameSize_Samples, mLag, mMaxLag_Samples);
		for (k = 0; k < NumLags; k++)
		{
			mLag[k] /= (37.5 * 37.5); // the 37.5*37.5 term is to normalize by the square of the spont rate
			mMetadataFile->UpdateMinMax(mLag[k]);
			Output->Data(k, i) = (float)mLag[k];
		}
	}
	mMetadataFile->AddSample();
    return mNumChannels * mMaxLag_Samples;
}

int MSOXcorr::Stop(void)
{
	Log("    Stop: %s", ModuleName());
	mMetadataFile->Write();
    return 1;
}

int MSOXcorr::Unload(void)
{
	Log("    Unload: %s", ModuleName());
    return 1;
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
