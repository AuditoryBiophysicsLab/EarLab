#include <iostream>
#include "Earlab.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <fcntl.h>
#include "BasilarMembraneNonlinearGammatone.h"
#include "EarlabException.h"
#include <float.h>
#ifndef NOTWINDOWS
# include <windows.h>
#endif

using namespace std;

BasilarMembraneNonlinearGammatone::BasilarMembraneNonlinearGammatone()
{
	memset(this, 0, sizeof(BasilarMembraneNonlinearGammatone));
	mLogger = new Logger();
	SetModuleName("BasilarMembraneNonlinearGammatone");
}

BasilarMembraneNonlinearGammatone::~BasilarMembraneNonlinearGammatone()
{
	if (mModuleName != NULL)
		delete [] mModuleName;
	if (mWriter != NULL)
		delete mWriter;
}

int BasilarMembraneNonlinearGammatone::ReadParameters(char *ParameterFileName)
{
	return ReadParameters(ParameterFileName, mModuleName);
}

int BasilarMembraneNonlinearGammatone::ReadParameters(char *ParameterFileName, char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	mLogger->Log("    ReadParameters: %s \"%s\"", mModuleName, ParameterFileName);

	// Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, mSampleRate_Hz);

	// Greenwood frequency placemap parameters
    Status = theParamFile.GetParameter(SectionName, "DisableNonlinearPath",
			mDisableNonlinearPath, false);
	Status = theParamFile.GetParameter(SectionName, "A", mA, 165);
    Status = theParamFile.GetParameter(SectionName, "Alpha", mAlpha, 2.1);
    Status = theParamFile.GetParameter(SectionName, "K", mK, 1.0);
    Status = theParamFile.GetParameter(SectionName, "CochleaLength_mm",
		mCochleaLength_mm, 35.0);
    Status = theParamFile.GetParameter(SectionName, "BMStartPercent", mBMStartPercent, 0.0);
    Status = theParamFile.GetParameter(SectionName, "BMEndPercent", mBMEndPercent, 100.0);

	Status = theParamFile.GetParameter(SectionName, "ProduceOutputFile", mProduceOutputFile, false);
    Status = theParamFile.GetParameter(SectionName, "BinaryOutput", mBinaryOutput, false);
    Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), NULL);
	return 1;
}

int BasilarMembraneNonlinearGammatone::Start(int NumInputs,
											 EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
											 int NumOutputs,
											 EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
											 unsigned long OutputElementCounts[EarlabMaxIOStreamCount])
{
	// Because this module produces data in a "Frequency-Time" format, the following statements apply:
	// OutputSize[i][0] is the number of samples in a frame (the term "samples" includes data for all frequency channels at one time index)
	// OutputSize[i][1] is the number of frequency channels provided
	// OutputSize[i][2] must be 0 to mark the end of the dimension count

    int i,process_counter;
	double PlacemapStepSize_mm, CurrentPlace_mm, CurrentCF_Hz;
	char OutputFileName[256];

	mLogger->Log("    Start: %s", mModuleName);
	// Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	if (NumInputs != 1)
		throw EarlabException("%s: Currently this module can only handle one input stream.  Sorry!", mModuleName);

	if (NumOutputs != 1)
		throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", mModuleName);

	if (InputTypes[0] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform input data streams.  Sorry!", mModuleName);

	if (OutputTypes[0] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform output data streams.  Sorry!", mModuleName);

	if (InputSize[0][0] != OutputSize[0][0])
		throw EarlabException("%s: Input and output frame lengths must be identical.  Sorry!", mModuleName);

	if (InputSize[0][1] != 0)
		throw EarlabException("%s: Input data must be one-dimensional array.  Sorry!", mModuleName);

	if (OutputSize[0][2] != 0)
		throw EarlabException("%s: Output signal must be two-dimensional array.  Sorry!", mModuleName);

	OutputElementCounts[0] = OutputSize[0][0] * OutputSize[0][1];

	mFrameSize_Samples = OutputSize[0][0];
	mNumChannels = OutputSize[0][1];

    process_counter = 0;
/*****************************************************************************/
/*              SET BASILIAR MEMBRANE ARRAYS AND CONSTANTS                   */
/*****************************************************************************/

    mCounter = 0;
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
			mOutputFile = fopen(mCFFileName, WR_BIN);
		}
		else
		{
#ifndef NOTWINDOWS
			_fmode = _O_TEXT;
#endif
			mOutputFile = fopen(mCFFileName, WR_TEXT);
		}

		if (mOutputFile == NULL)
			return 0;
	}
	else
		mOutputFile = NULL;

	mDRNL = new LopezPovedaMeddisDualResonanceNonlinear[mNumChannels];
	mCochleaStartPos_mm = mCochleaLength_mm * (mBMStartPercent / 100);
	mCochleaEndPos_mm =   mCochleaLength_mm * (  mBMEndPercent / 100);
	mCochleaModeledLength_mm = mCochleaEndPos_mm - mCochleaStartPos_mm;
	PlacemapStepSize_mm = mCochleaModeledLength_mm / (double)mNumChannels;
	mLowpassFilter = new FirstOrderLowpass[mNumChannels];
	for (i = 0; i < mNumChannels; i++)
    {
		mLowpassFilter[i].SetSampleRate_Hz(mSampleRate_Hz);
		mLowpassFilter[i].SetCornerFrequency_Hz(400.0);
		CurrentPlace_mm = (PlacemapStepSize_mm * i) + mCochleaStartPos_mm;
		CurrentCF_Hz = mA * (pow(10.0, mAlpha * (CurrentPlace_mm / mCochleaLength_mm)) - mK);

		// Initialize the filter for this channel
		mDRNL[i].Init(CurrentCF_Hz, mSampleRate_Hz);

		if (mOutputFile != NULL)
		{
			if (mBinaryOutput)
				fwrite(&CurrentCF_Hz, sizeof(double), 1, mOutputFile);
			else
				fprintf(mOutputFile, "%g\n", CurrentCF_Hz);
		}
    }
	if (mOutputFile != NULL)
		fclose(mOutputFile);

	mInputScaleFactor = 1.0 / 3.2e10;
	mOutputScaleFactor = 1.0e9 / (800 * PI);
	sprintf(OutputFileName, "%s.1", mModuleName);
	mWriter = new BinaryWriter(OutputFileName);
	mWriter->WriteMetadata("File_Type", "Frequency_Time");
	mWriter->WriteMetadata("Frequency_Dimension_Size", mNumChannels);
	mWriter->WriteMetadata("Frame_Length_Samples", mFrameSize_Samples);
	mWriter->WriteMetadata("Sample_Rate_Hz", mSampleRate_Hz);
	mWriter->CloseMetadataFile();
	return 1;
}

int BasilarMembraneNonlinearGammatone::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
    int i,j;
	double CurInputSample;
	const float *Data;
	unsigned long DataLen;
	FloatMatrixN *Input, *Output;

	mLogger->Log("    Advance: %s", mModuleName);

	Input = ((EarlabWaveformStream *)InputStream[0])->GetData();    // Only supporting one output at the present moment
	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();  // Only supporting one output at the present moment

	if (Input->Rank(0) != mFrameSize_Samples)
		throw EarlabException("%s: Input size mismatch with Start()", mModuleName);
	if ((Output->Rank(0) != mFrameSize_Samples) || (Output->Rank(1) != mNumChannels))
		throw EarlabException("%s: Output size mismatch with Start()", mModuleName);

    for (j = 0; j < mFrameSize_Samples; j++)
    {
		CurInputSample = Input->Data(j) * mInputScaleFactor;
	for (i = 0; i < mNumChannels; i++)
	{
			Output->Data(j, i) = (float)(mLowpassFilter[i].Filter(mDRNL[i].Filter(CurInputSample, mDisableNonlinearPath)) * mOutputScaleFactor);
		}
    }
	Data = Output->GetData(&DataLen);
	mWriter->CheckMinMax(Data, DataLen);
	mSampleCount += mFrameSize_Samples;
    return (mFrameSize_Samples * mNumChannels);
}

int BasilarMembraneNonlinearGammatone::Stop(void)
{
	mLogger->Log("    Stop: %s", mModuleName);
	mWriter->WriteMetadata("Number_Of_Samples", mSampleCount);
	mWriter->WriteMinMaxMetadata();
	mWriter->Close();
	return 1;
}

int BasilarMembraneNonlinearGammatone::Unload(void)
{
	mLogger->Log("    Unload: %s", mModuleName);
	return 1;
}

void BasilarMembraneNonlinearGammatone::SetModuleName(char *ModuleName)
{
	if (mModuleName != NULL)
		delete [] mModuleName;

	mModuleName = new char[strlen(ModuleName) + 1];
	strcpy(mModuleName, ModuleName);
}

void BasilarMembraneNonlinearGammatone::SetLogger(Logger *TheLogger)
{
	if (mLogger != NULL)
		delete mLogger;
	mLogger = TheLogger;
}
