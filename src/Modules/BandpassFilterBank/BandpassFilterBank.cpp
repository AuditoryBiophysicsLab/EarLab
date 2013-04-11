#include <iostream>
#include "Earlab.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <fcntl.h>
#include "BandpassFilterBank.h"
#include "EarlabException.h"

using namespace std;

BandpassFilterBank::BandpassFilterBank()
{
	memset(this, 0, sizeof(BandpassFilterBank));
	SetModuleName("BandpassFilterBank");
}

BandpassFilterBank::~BandpassFilterBank()
{
	if (mMetadataFile != NULL)
		delete mMetadataFile;
	if (mCFArray_Hz != NULL)
		delete [] mCFArray_Hz;
	if (NthOrderHighpassFilters != NULL)
		delete [] NthOrderHighpassFilters;
	if (NthOrderLowpassFilters != NULL)
		delete [] NthOrderLowpassFilters;
}

int BandpassFilterBank::ReadParameters(const char *ParameterFileName)
{
	return ReadParameters(ParameterFileName, ModuleName());
}

int BandpassFilterBank::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

	// Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, mSampleRate_Hz);

	// Greenwood frequency placemap parameters
	Status = theParamFile.GetParameter(SectionName, "A", mA, 165);
    Status = theParamFile.GetParameter(SectionName, "Alpha", mAlpha, 2.1);
    Status = theParamFile.GetParameter(SectionName, "K", mK, 1.0);
    Status = theParamFile.GetParameter(SectionName, "CochleaLength_mm", mCochleaLength_mm, 35.0);
    Status = theParamFile.GetParameter(SectionName, "BMStartPercent", mBMStartPercent, 0.0);
    Status = theParamFile.GetParameter(SectionName, "BMEndPercent", mBMEndPercent, 100.0);

	Status = theParamFile.GetParameter(SectionName, "FilterQ", mFilterQ, 4.0);
	Status = theParamFile.GetParameter(SectionName, "BandpassFilterOrder", mBandpassFilterOrder, 3);

	Status = theParamFile.GetParameter(SectionName, "ProduceOutputFile", mProduceOutputFile, false);
    Status = theParamFile.GetParameter(SectionName, "BinaryOutput", mBinaryOutput, false);
    Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), NULL);
	return 1;
}

int BandpassFilterBank::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	// Because this module produces data in a "Frequency-Time" format, the following statements apply:
	// OutputSize[i][0] is the number of samples in a frame (the term "samples" includes data for all frequency channels at one time index)
	// OutputSize[i][1] is the number of frequency channels provided
	// OutputSize[i][2] must be 0 to mark the end of the dimension count

    int i,process_counter;
	double PlacemapStepSize_mm, CurrentPlace_mm, CurrentCF_Hz, CurrentFilterBandwidth_Hz;
	char MetadataFileName[256];

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

	if (InputSize[0][0] != OutputSize[0][0])
		throw EarlabException("%s: Input and output frame lengths must be identical.  Sorry!", ModuleName());

	if (InputSize[0][1] != 0)
		throw EarlabException("%s: Input data must be one-dimensional array.  Sorry!", ModuleName());

	if (OutputSize[0][2] != 0)
		throw EarlabException("%s: Output signal must be two-dimensional array.  Sorry!", ModuleName());

	OutputElementCounts[0] = OutputSize[0][0] * OutputSize[0][1];

	mFrameSize_Samples = OutputSize[0][0];
	mNumChannels = OutputSize[0][1];

	/* Convert sample/sec  to sec/sample */ 
    mDeltaT = (double)(1.0 / mSampleRate_Hz);

    process_counter = 0;
	
	if (mProduceOutputFile)
	{
#ifdef NOTWINDOWS
# define WR_BIN  "w"
# define WR_TEXT "w"
#else
# define WR_BIN  "wb"
# define WR_TEXT "wt"
#endif
		if (mBinaryOutput)
		{
#ifndef NOTWINDOWS
			_fmode = _O_BINARY;
#endif
			mOutputFile = OpenOutputFile(mCFFileName, WR_BIN);
		}
		else
		{
#ifndef NOTWINDOWS
			_fmode = _O_TEXT;
#endif
			mOutputFile = OpenOutputFile(mCFFileName, WR_TEXT);
		}

		if (mOutputFile == NULL)
			return 0;
	}
	else
		mOutputFile = NULL;

	mCochleaStartPos_mm = mCochleaLength_mm * (mBMStartPercent / 100);
	mCochleaEndPos_mm =   mCochleaLength_mm * (  mBMEndPercent / 100);
	mCochleaModeledLength_mm = mCochleaEndPos_mm - mCochleaStartPos_mm;
	PlacemapStepSize_mm = mCochleaModeledLength_mm / (double)mNumChannels;
	mCFArray_Hz = new double [mNumChannels];
	NthOrderHighpassFilters = new NthOrderHighpass [mNumChannels];
	NthOrderLowpassFilters = new NthOrderLowpass [mNumChannels];

	for (i = 0; i < mNumChannels; i++)
    {
		CurrentPlace_mm = (PlacemapStepSize_mm * i) + mCochleaStartPos_mm;
		CurrentCF_Hz = mA * (pow((double)10.0, (double)(mAlpha * (CurrentPlace_mm / mCochleaLength_mm))) - mK);
		mCFArray_Hz[i] = CurrentCF_Hz;

		if (mOutputFile != NULL)
		{
			if (mBinaryOutput)
				fwrite(&CurrentCF_Hz, sizeof(double), 1, mOutputFile);
			else
				fprintf(mOutputFile, "%g\n", CurrentCF_Hz);
		}
		
		CurrentFilterBandwidth_Hz = CurrentCF_Hz / mFilterQ;

		NthOrderHighpassFilters[i].SetSampleRate_Hz(mSampleRate_Hz);
		NthOrderHighpassFilters[i].SetCornerFrequency_Hz(CurrentCF_Hz + (CurrentFilterBandwidth_Hz / 2));
		NthOrderHighpassFilters[i].SetFilterOrder(mBandpassFilterOrder);

		NthOrderLowpassFilters[i].SetSampleRate_Hz(mSampleRate_Hz);
		NthOrderLowpassFilters[i].SetCornerFrequency_Hz(CurrentCF_Hz - (CurrentFilterBandwidth_Hz / 2));
		NthOrderLowpassFilters[i].SetFilterOrder(mBandpassFilterOrder);
    } 
	if (mOutputFile != NULL)
		CloseFile(mOutputFile);
	sprintf(MetadataFileName, "%s.1.metadata", ModuleName());
	mMetadataFile = new XmlMetadataFile(MetadataFileName);
	mMetadataFile->SetSampleRate_Hz(mSampleRate_Hz);
	mMetadataFile->Add(new XmlParameter("Units", "Membrane displacement (nm)"));
	//mMetadataFile->Add(new XmlParameter("FrameSize", mFrameSize_Samples));
	//mMetadataFile->Add(new XmlParameter("SampleRate_Hz", mSampleRate_Hz));
	mMetadataFile->Add(new XmlDimension("Frequency (Hz)", mNumChannels, mCFArray_Hz));
	mMetadataFile->SetSource(new XmlSource(ModuleName(), "BandpassFilterBank_0.1.0.0"));
	mMetadataFile->Write();
	return 1;
}

int BandpassFilterBank::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
    int i, j;
	FloatMatrixN *Input, *Output;
	float CurOutputSample;

	Log("    Advance: %s", ModuleName());

	Input = ((EarlabWaveformStream *)InputStream[0])->GetData();	// Only supporting one output at the present moment
	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();	// Only supporting one output at the present moment

	if (Input->Rank(0) != mFrameSize_Samples)
		throw EarlabException("%s: Input size mismatch with Start()", ModuleName());
	if ((Output->Rank(0) != mFrameSize_Samples) || (Output->Rank(1) != mNumChannels))
		throw EarlabException("%s: Output size mismatch with Start()", ModuleName());

	for (j = 0; j < mFrameSize_Samples; j++)
	{
        for (i = 0; i< mNumChannels; i++)
        {
			CurOutputSample = (float)NthOrderLowpassFilters[i].Filter(NthOrderHighpassFilters[i].Filter(Input->Data(j, i)));
			mMetadataFile->UpdateMinMax(CurOutputSample);

            Output->Data(j, i) = CurOutputSample;
        }
		mMetadataFile->AddSample();
    }
    return mFrameSize_Samples * mNumChannels;
}

int BandpassFilterBank::Stop(void)
{
	Log("    Stop: %s", ModuleName());
	mMetadataFile->Write();
	return 1;
}

int BandpassFilterBank::Unload(void)
{
	Log("    Unload: %s", ModuleName());
	return 1;
}
