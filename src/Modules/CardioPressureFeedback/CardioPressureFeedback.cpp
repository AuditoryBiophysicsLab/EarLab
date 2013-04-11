#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include "Earlab.h"
#include "CardioPressureFeedback.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "EarlabException.h"
#include <math.h>

using namespace std;

CardioPressureFeedback::CardioPressureFeedback()
{
    memset(this, 0, sizeof(CardioPressureFeedback));
	SetModuleName("CardioPressureFeedback");
}

CardioPressureFeedback::~CardioPressureFeedback()
{
	if (mCompartment != NULL)
		delete mCompartment;

	if (mMetadataFile != NULL)
		delete mMetadataFile;
}

int CardioPressureFeedback::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	    return ReadParameters(ParameterFileName, "CardioPressureFeedback");
	else
	    return ReadParameters(ParameterFileName, ModuleName());

}

int CardioPressureFeedback::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0.0);
    if (Success != theParamFile.GetParameter(SectionName, "As", mAs, mNumAs, NULL, 0))
		mNumAs = 0;
    if (Success != theParamFile.GetParameter(SectionName, "Bs", mBs, mNumBs, NULL, 0))
		mNumBs = 0;
    return 1;
}

int CardioPressureFeedback::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	cout << "    Start: " << ModuleName() << endl;

    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	if ((NumInputs != 1) || (InputTypes[0] != WaveformData) || (InputSize[0][1] != 0))
		throw EarlabException("%s: This module requires one waveform data input stream of just one dimension.  Sorry!", ModuleName());

	if ((NumOutputs != 1) || (OutputTypes[0] != WaveformData) || (OutputSize[0][1] != 0))
		throw EarlabException("%s: This module requires one waveform data output stream of just one dimension.  Sorry!", ModuleName());

	if ((InputSize[0][0] != OutputSize[0][0]) || (InputSize[0][1] != OutputSize[0][1]))
		throw EarlabException("%s: This module requires the input and output data streams to be the same size.  Sorry!", ModuleName());

	if (InputSize[0][0] <= 0)
		throw EarlabException("%s: This module requires the input and output data streams to be of non-zero size.  Sorry!", ModuleName());

	SamplesPerFrame = InputSize[0][0];

	mCompartment = new GenericIIRFilter(mNumBs, mBs, mNumAs, mAs);
	
	return 1;
}

int CardioPressureFeedback::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int Sample;

	Log("    Advance: %s", ModuleName());

	mInput = ((EarlabWaveformStream *)InputStream[0])->GetData();
	mOutput = ((EarlabWaveformStream *)OutputStream[0])->GetData();

	for (Sample = 0; Sample < SamplesPerFrame; Sample++)
	{
		mOutput->Data(Sample) = (float)mCompartment->Filter(mInput->Data(Sample));
	} // for (Sample)

	return SamplesPerFrame;
}

int CardioPressureFeedback::Stop(void)
{
	cout << "    Stop: " << ModuleName() << endl;
	//mMetadataFile->Write();

    return 1;
}

int CardioPressureFeedback::Unload(void)
{
	cout << "    Unload: " << ModuleName() << endl;

    return 1;
}

