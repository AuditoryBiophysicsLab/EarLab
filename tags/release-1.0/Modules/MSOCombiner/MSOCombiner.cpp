#include <iostream>
#include "MSOCombiner.h"
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

MSOCombiner::MSOCombiner()
{
    memset(this, 0, sizeof(MSOCombiner));
	mLogger = new Logger();
    SetModuleName("MSOCombiner");
}

MSOCombiner::~MSOCombiner()
{
    if (mModuleName != NULL)
        delete [] mModuleName;
	if (mCFArray_Hz != NULL)
		delete [] mCFArray_Hz;
	if (mChannelBuffer != NULL)
		delete [] mChannelBuffer;
	if (mWriter != NULL)
		delete mWriter;
}

int MSOCombiner::ReadParameters(char *ParameterFileName)
{
    return ReadParameters(ParameterFileName, mModuleName);
}

int MSOCombiner::ReadParameters(char *ParameterFileName, char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	mLogger->Log("    ReadParameters: %s \"%s\"", mModuleName, ParameterFileName);

    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0);
    Status = theParamFile.GetParameter(SectionName, "MaxITD_uS", mMaxITD_uS, 500.0);
	return 1;
}

int MSOCombiner::Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions])
{
	char OutputFileName[256];
    // Because this module produces data in a "Frequency-Time" format, the following statements apply:
    // OutputSize[i][0] is the number of samples in a frame (the term "samples" includes data for all frequency channels at one time index)
    // OutputSize[i][1] is the number of frequency channels provided
    // OutputSize[i][2] must be 0 to mark the end of the dimension count

	mLogger->Log("    Start: %s", mModuleName);
    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
    if (NumInputs != 2)
        throw EarlabException("%s: Currently this module can only handle two input streams.  Sorry!", mModuleName);

    if (NumOutputs != 1)
        throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", mModuleName);

	if ((InputSize[0][0] == 0) || (InputSize[0][1] == 0) || (InputSize[0][2] != 0))
        throw EarlabException("%s: Input signal must be two-dimensional array.  Sorry!", mModuleName);

    if (InputSize[0][0] != InputSize[1][0])
        throw EarlabException("%s: Input channel counts must be identical.  Sorry!", mModuleName);
	mNumChannels = InputSize[0][0];

    if (InputSize[0][1] != InputSize[1][1])
        throw EarlabException("%s: Input lag counts must be identical.  Sorry!", mModuleName);
	mInputLagCount = InputSize[0][1];

	mMaxLag_Samples = (InputSize[0][1] * 2) - 1;	// Minus one because both left and right have zero-lag data, so one will be discarded
	mZeroLag_Index = InputSize[0][1] + 1;			// This is the index of the "zero lag" sample

	if (OutputSize[0][0] != InputSize[0][0])
        throw EarlabException("%s: Output channel count must be identical to input channel count.  Sorry!", mModuleName);

	if (OutputSize[0][1] != mMaxLag_Samples)
        throw EarlabException("%s: Output lag count must be identical to (input lag count * 2) - 1.  Sorry!", mModuleName);
	
	mChannelBuffer = new double [mMaxLag_Samples];

	mLagResolution_uS = mMaxITD_uS / ((mMaxLag_Samples - 1) / 2);
	sprintf(OutputFileName, "%s.1", mModuleName);
	mWriter = new BinaryWriter(OutputFileName);
	mWriter->WriteMetadata("File_Type", "Lag_Frequency_Time");
	mWriter->WriteMetadata("Lag_Dimension_Size", mMaxLag_Samples);
	mWriter->WriteMetadata("Frequency_Dimension_Size", mNumChannels);
	mWriter->WriteMetadata("Min_Lag_uS", -1.0 * mMaxITD_uS);
	mWriter->WriteMetadata("Max_Lag_uS", mMaxITD_uS);
	mWriter->WriteMetadata("Lag_Time_Step_uS", mLagResolution_uS);
	mWriter->WriteMetadata("Sample_Rate_Hz", mSampleRate_Hz);
	mWriter->CloseMetadataFile();

   return 1;
}

// Note: We assume here that Input 0 is Left and Input 1 is Right
int MSOCombiner::Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount])
{
	int Chan, Lag;
	double *Data;
	unsigned long DataLen;

	mLogger->Log("    Advance: %s", mModuleName);

	if ((Input[0]->Rank(0) != Input[1]->Rank(0)) || (Input[0]->Rank(1) != Input[1]->Rank(1)))
		throw EarlabException("%s: Input sizes inconsistent: Left: %d %d Right: %d %d", mModuleName, 
			Input[0]->Rank(0), Input[0]->Rank(1), Input[1]->Rank(0), Input[1]->Rank(1));

	// Rearrange the input streams as follows:
	//  Left input is copied to the output stream in reverse order, because zero lag is first in MSOXCorr output data streams
	//    and the output of MSOCombiner wants to have max left leading as first data point, proceeding to zero lag, and then on up to max right leading
	//  Right input is copied to the output stream starting in the middle, because again zero lag is first in the right input stream.
	for (Chan = 0; Chan < mNumChannels; Chan++)
	{
		// Copy left data into array, backwards since zero lag is first and wants to be in the middle in our output stream
		for (Lag = 0; Lag < mInputLagCount; Lag++)
			mChannelBuffer[Lag] = Input[0]->Data(Chan, mInputLagCount - Lag - 1);
		
		// Copy right data into array, starting at the middle (zero lag)
		for (Lag = 0; Lag < mInputLagCount; Lag++)
			mChannelBuffer[mInputLagCount + Lag - 1] = Input[1]->Data(Chan, Lag);

		//mWriter->Write(mChannelBuffer, mMaxLag_Samples);

		for (Lag = 0; Lag < mMaxLag_Samples; Lag++)
			Output[0]->Data(Chan, Lag) = mChannelBuffer[Lag];
	}
	//mWriter->CloseBinaryFile();
	Data = Output[0]->GetData(&DataLen);
	mWriter->CheckMinMax(Data, DataLen);
	mFrameCount++;
	return mMaxLag_Samples * mNumChannels;
}

int MSOCombiner::Stop(void)
{
	mLogger->Log("    Stop: %s", mModuleName);
	mWriter->WriteMetadata("Number_Of_Samples", mFrameCount);
	mWriter->WriteMinMaxMetadata();
	mWriter->Close();
    return 1;
}

int MSOCombiner::Unload(void)
{
	mLogger->Log("    Unload: %s", mModuleName);
    return 1;
}

void MSOCombiner::SetModuleName(char *ModuleName)
{
    if (mModuleName != NULL)
        delete [] mModuleName;

    mModuleName = new char[strlen(ModuleName) + 1];
    strcpy(mModuleName, ModuleName);
}

void MSOCombiner::SetLogger(Logger *TheLogger)
{
	if (mLogger != NULL)
		delete mLogger;
	mLogger = TheLogger;
}
