#include <iostream>
#include "InnerHaircell.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "Earlab.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"
#include "FPException.h"
#include <float.h>

using namespace std;

InnerHaircell::InnerHaircell()
{
    memset(this, 0, sizeof(InnerHaircell));
	mLogger = new Logger();
	SetModuleName("InnerHaircell");
}

InnerHaircell::~InnerHaircell()
{
	if (mModuleName != NULL)
		delete [] mModuleName;
	if (mWriter != NULL)
		delete mWriter;
}

int InnerHaircell::ReadParameters(char *ParameterFileName)
{
	if (mModuleName == NULL)
	   return ReadParameters(ParameterFileName, "InnerHaircell");
	else
	    return ReadParameters(ParameterFileName, mModuleName);
}

int InnerHaircell::ReadParameters(char *ParameterFileName, char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	mLogger->Log("    ReadParameters: %s \"%s\"", mModuleName, ParameterFileName);
    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, mSampleRate_Hz);
    Status = theParamFile.GetParameter(SectionName, "Gain", mGain, 100);
    return 1;
}

int InnerHaircell::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount])
{
	int i;
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

	if (InputSize[0][2] != 0)
		throw EarlabException("%s: Input data must be two-dimensional array.  Sorry!", mModuleName);

	if (OutputSize[0][2] != 0)
		throw EarlabException("%s: Output signal must be two-dimensional array.  Sorry!", mModuleName);

	OutputElementCounts[0] = OutputSize[0][0] * OutputSize[0][1];

	mFrameSize_Samples = OutputSize[0][0];
	mNumChannels = OutputSize[0][1];

	IHCs = new IHC *[mNumChannels];
	for (i = 0; i < mNumChannels; i++)
		IHCs[i] = new IHC(mSampleRate_Hz, mGain);

	sprintf(OutputFileName, "%s.1", mModuleName);
	mWriter = new BinaryWriter(OutputFileName);
	mWriter->WriteMetadata("File_Type", "Frequency_Time");
	mWriter->WriteMetadata("Frequency_Dimension_Size", mNumChannels);
	mWriter->WriteMetadata("Frame_Length_Samples", mFrameSize_Samples);
	mWriter->WriteMetadata("Sample_Rate_Hz", mSampleRate_Hz);
	mWriter->CloseMetadataFile();
    return 1;
}

int InnerHaircell::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int i, j;
	float *Data;
	unsigned long DataLen = 0;
	FloatMatrixN *Input, *Output;

	mLogger->Log("    Advance: %s", mModuleName);

	Input = ((EarlabWaveformStream *)InputStream[0])->GetData();	// Only supporting one output at the present moment
	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();	// Only supporting one output at the present moment

	if ((Input->Rank(0) != mFrameSize_Samples) || (Input->Rank(1) != mNumChannels))
		throw EarlabException("%s: Input size mismatch with Start()", mModuleName);
	if ((Output->Rank(0) != mFrameSize_Samples) || (Output->Rank(1) != mNumChannels))
		throw EarlabException("%s: Output size mismatch with Start()", mModuleName);

	/*****************************************************************************/
	/*                   START UP THE BASILAR MEMBRANE FILTER                    */
	/*****************************************************************************/

    for (j = 0; j < mFrameSize_Samples; j++)
    {
		/***************** RESET shftin COUNTER FOR EACH NEW FRAME ******************/
        for (i = 0; i < mNumChannels; i++)
			Output->Data(j, i) = IHCs[i]->Step(Input->Data(j, i));
    } 
	Data = Output->GetData(&DataLen);
	mWriter->CheckMinMax(Data, DataLen);
	mSampleCount += mFrameSize_Samples;
    return mFrameSize_Samples*mNumChannels;
}

int InnerHaircell::Stop(void)
{
	mLogger->Log("    Stop: %s", mModuleName);
	mWriter->WriteMetadata("Number_Of_Samples", mSampleCount);
	mWriter->WriteMinMaxMetadata();
    return 1;
}

int InnerHaircell::Unload(void)
{
	mLogger->Log("    Unload: %s", mModuleName);
    return 1;
}

void InnerHaircell::SetModuleName(char *ModuleName)
{
	if (mModuleName != NULL)
		delete [] mModuleName;

	mModuleName = new char[strlen(ModuleName) + 1];
	strcpy(mModuleName, ModuleName);
}

void InnerHaircell::SetLogger(Logger *TheLogger)
{
	if (mLogger != NULL)
		delete mLogger;
	mLogger = TheLogger;
}

IHC::IHC(double SampleRate_Hz, double Gain)
{
	double DeltaT;
	
	memset(this, 0, sizeof(IHC));

	mSampleRate_Hz = SampleRate_Hz;
	mGain = Gain;

	/* Convert sample/sec  to sec/sample */ 
    DeltaT = 1.0 / (1.0 * mSampleRate_Hz);

    x0 = 27;
    x1 = 27;
    sx0= 85;
    sx1= 11;

    tau = 5.3e-05;

	/*****************************************************************************/
	/*                  SET IHC FILTER ARRAYS AND CONSTANTS                      */
	/*****************************************************************************/

    ga0  = 1 / (1 + exp(x0 / sx0) * (1 + exp(x1 / sx1)));
    ihca = 1 / ((tau * 2.0) / DeltaT + 1);
    ihcb = ((tau * 2.0) / DeltaT) - 1;

    mOldGA[0] = ga0;

	state_0 = 0.6 / (1 + exp(-1 * (0 - x0) / sx0)) + (0.4) / (1 + exp(-1 * (0 - x1) / sx1));
}

float IHC::Step(float Displacement_nM)
{
	double Displacement = Displacement_nM;

	if (IS_ALMOST_DENORMAL(Displacement))
		Displacement = 0;

	/************************* IHC RECTIFIER **************************************/ 
    mGA = mGain * ((0.6) / (1 + exp(-1  *(Displacement - x0) / sx0)) + (0.4) / (1 + exp(-1 * (Displacement - x1) / sx1)) - state_0); 

	// First check for denormal filter coefficients, which cause enormous slowdowns on Pentium 4
	if (IS_ALMOST_DENORMAL(mOldIHCFilter[0]))
		mOldIHCFilter[0] = 0;
	if (IS_ALMOST_DENORMAL(mOldIHCFilter[1]))
		mOldIHCFilter[1] = 0;
	if (IS_ALMOST_DENORMAL(mOldIHCFilter[2]))
		mOldIHCFilter[2] = 0;
	if (IS_ALMOST_DENORMAL(mOldIHCFilter[3]))
		mOldIHCFilter[3] = 0;

	/********************** PUT IHC OUTPUT INTO AN ARRAY FOR NEXT STAGE ***********/                      
    mIHCFilter[0] = ihca * (mGA           + mOldGA[0] + ihcb * mOldIHCFilter[0]);
    mIHCFilter[1] = ihca * (mIHCFilter[0] + mOldGA[1] + ihcb * mOldIHCFilter[1]);
    mIHCFilter[2] = ihca * (mIHCFilter[1] + mOldGA[2] + ihcb * mOldIHCFilter[2]);
    mIHCFilter[3] = ihca * (mIHCFilter[2] + mOldGA[3] + ihcb * mOldIHCFilter[3]);

	/********************* STORE OLD VALUES FOR NEXT DATA POINT ******************/
    mOldGA[0] = mGA;
	mOldGA[1] = mIHCFilter[0];
    mOldGA[2] = mIHCFilter[1];
	mOldGA[3] = mIHCFilter[2];

    mOldIHCFilter[0] = mIHCFilter[0];
	mOldIHCFilter[1] = mIHCFilter[1]; 
    mOldIHCFilter[2] = mIHCFilter[2];
	mOldIHCFilter[3] = mIHCFilter[3];

	return (float)mIHCFilter[3];
}
