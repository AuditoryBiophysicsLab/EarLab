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
    SetModuleName("MSOCombiner");
}

MSOCombiner::~MSOCombiner()
{
	if (mCFArray_Hz != NULL)
		delete [] mCFArray_Hz;
	if (mChannelBuffer != NULL)
		delete [] mChannelBuffer;
	if (mMetadataFile != NULL)
		delete mMetadataFile;
}

int MSOCombiner::ReadParameters(const char *ParameterFileName)
{
    return ReadParameters(ParameterFileName, ModuleName());
}

int MSOCombiner::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0);
    Status = theParamFile.GetParameter(SectionName, "MaxITD_uS", mMaxITD_uS, 500.0);
 	Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), "");
	return 1;
}

int MSOCombiner::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	char MetadataFileName[256];

	// Because this module produces data in a "Frequency-Lag" format, the following statements apply:
    // OutputSize[0][0] is the number of lags desired.  This must be sized according to the MaxITD_uS and SampleRate_Hz parameters
    // OutputSize[0][1] is the number of frequency channels provided
	// OutputSize[0][2] must be zero to mark the end of the provided dimensions
	// This module outputs one lag curve per channel per frame.  In other words, it downsamples the time dimension of the inputs by one frame size
	// Each output frame consists of OutputSize[0][0] frequency channels by OutputSize[0][1] lags

	Log("    Start: %s", ModuleName());
    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
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

    if (InputSize[0][1] != InputSize[1][1])
        throw EarlabException("%s: Input channel counts must be identical.  Sorry!", ModuleName());
	mNumChannels = InputSize[0][1];

    if (InputSize[0][0] != InputSize[1][0])
        throw EarlabException("%s: Input lag counts must be identical.  Sorry!", ModuleName());
	mInputLagCount = InputSize[0][0];

	if ((OutputSize[0][0] == 0) || (OutputSize[0][1] == 0) || (OutputSize[0][2] != 0))
        throw EarlabException("%s: Output signal must be two-dimensional array.  Sorry!", ModuleName());

	// OutputElementCounts[0] = OutputSize[0][0] * OutputSize[0][1] * OutputSize[0][2];
	OutputElementCounts[0] = OutputSize[0][0] * OutputSize[0][1];

	mMaxLag_Samples = (mInputLagCount * 2) - 1;	// Minus one because both left and right have zero-lag data, so one will be discarded
	mZeroLag_Index = mInputLagCount + 1;			// This is the index of the "zero lag" sample

	if (OutputSize[0][1] != InputSize[0][1])
        throw EarlabException("%s: Output channel count must be identical to input channel count.  Sorry!", ModuleName());

	if (OutputSize[0][0] != mMaxLag_Samples)
        throw EarlabException("%s: Output lag count must be identical to (input lag count * 2) - 1.  Sorry!", ModuleName());
	
	mChannelBuffer = new double [mMaxLag_Samples];

	mLagResolution_uS = mMaxITD_uS / ((mMaxLag_Samples - 1) / 2);
	sprintf(MetadataFileName, "%s.0.metadata", ModuleName());
	mMetadataFile = new XmlMetadataFile(MetadataFileName);
	mMetadataFile->SetSampleRate_Hz(mSampleRate_Hz);
	mMetadataFile->Add(new XmlParameter("Units", "IFR (Spikes/S)"));
	//mMetadataFile->Add(new XmlParameter("LagMin_uS", -1.0 * mMaxITD_uS));
	//mMetadataFile->Add(new XmlParameter("LagMax_uS", mMaxITD_uS));
	//mMetadataFile->Add(new XmlParameter("LagStep_uS", mLagResolution_uS));
	//mMetadataFile->Add(new XmlParameter("SampleRate_Hz", mSampleRate_Hz));
	mMetadataFile->Add(new XmlDimension("Frequency (Hz)", mNumChannels, mCFFileName));
	mMetadataFile->Add(new XmlDimension("Interaural Time Difference (uS)", mMaxLag_Samples, -1.0 * mMaxITD_uS, mMaxITD_uS));
	mMetadataFile->SetSource(new XmlSource(ModuleName(), "MSOCombiner_0.1.0.0"));
	mMetadataFile->Write();

   return 1;
}

// Note: We assume here that Input 0 is Left and Input 1 is Right
int MSOCombiner::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int Chan, Lag;
	FloatMatrixN *InputL, *InputR, *Output;

    cout << "    Advance: " << ModuleName() << endl;

	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();
	InputL = ((EarlabWaveformStream *)InputStream[0])->GetData();	
	InputR = ((EarlabWaveformStream *)InputStream[1])->GetData();

	if ((InputL->Rank(0) != InputR->Rank(0)) || (InputL->Rank(1) != InputR->Rank(1)))
		throw EarlabException("%s: Input sizes inconsistent: Left: %d %d Right: %d %d", ModuleName(), 
			InputL->Rank(0), InputL->Rank(1), InputR->Rank(0), InputR->Rank(1));

	// Rearrange the input streams as follows:
	//  Left input is copied to the output stream in reverse order, because zero lag is first in MSOXCorr output data streams
	//    and the output of MSOCombiner wants to have max left leading as first data point, proceeding to zero lag, and then on up to max right leading
	//  Right input is copied to the output stream starting in the middle, because again zero lag is first in the right input stream.
	for (Chan = 0; Chan < mNumChannels; Chan++)
	{
		// Copy left data into array, backwards since zero lag is first and wants to be in the middle in our output stream
		for (Lag = 0; Lag < mInputLagCount; Lag++)
			mChannelBuffer[Lag] = InputL->Data(mInputLagCount - Lag - 1, Chan);
		
		// Copy right data into array, starting at the middle (zero lag)
		for (Lag = 0; Lag < mInputLagCount; Lag++)
			mChannelBuffer[mInputLagCount + Lag - 1] = InputR->Data(Lag, Chan);

		for (Lag = 0; Lag < mMaxLag_Samples; Lag++)
		{
			mMetadataFile->UpdateMinMax(mChannelBuffer[Lag]);
			Output->Data(Lag, Chan) = (float)mChannelBuffer[Lag];
		}
	}
	mMetadataFile->AddSample();
	return mMaxLag_Samples * mNumChannels;
}

int MSOCombiner::Stop(void)
{
	Log("    Stop: %s", ModuleName());
	mMetadataFile->Write();
    return 1;
}

int MSOCombiner::Unload(void)
{
	Log("    Unload: %s", ModuleName());
    return 1;
}
