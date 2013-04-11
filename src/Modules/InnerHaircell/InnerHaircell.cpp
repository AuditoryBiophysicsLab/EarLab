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
	SetModuleName("InnerHaircell");
}

InnerHaircell::~InnerHaircell()
{
	if (mMetadataFile != NULL)
		delete mMetadataFile;

	if (IHCs != NULL)
	{
		for (int i = 0; i < mNumChannels; i++)
			delete IHCs[i];
		delete [] IHCs;
	}
}

int InnerHaircell::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	   return ReadParameters(ParameterFileName, "InnerHaircell");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int InnerHaircell::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);
    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, mSampleRate_Hz);
    Status = theParamFile.GetParameter(SectionName, "Gain", mGain, 25);
    Status = theParamFile.GetParameter(SectionName, "Tau", mTau, 5.3e-05);
 	Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), "");
    return 1;
}

int InnerHaircell::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	int i;
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

	if (InputSize[0][2] != 0)
		throw EarlabException("%s: Input data must be two-dimensional array.  Sorry!", ModuleName());

	if (OutputSize[0][2] != 0)
		throw EarlabException("%s: Output signal must be two-dimensional array.  Sorry!", ModuleName());

	OutputElementCounts[0] = OutputSize[0][0] * OutputSize[0][1];

	mFrameSize_Samples = OutputSize[0][0];
	mNumChannels = OutputSize[0][1];

	IHCs = new IHC *[mNumChannels];
	for (i = 0; i < mNumChannels; i++)
		IHCs[i] = new IHC(mSampleRate_Hz, mGain, mTau);

	sprintf(MetadataFileName, "%s.1.metadata", ModuleName());
	mMetadataFile = new XmlMetadataFile(MetadataFileName);
	mMetadataFile->SetSampleRate_Hz(mSampleRate_Hz);
	//mMetadataFile->Add(new XmlParameter("FrameSize", mFrameSize_Samples));
	//mMetadataFile->Add(new XmlParameter("SampleRate_Hz", mSampleRate_Hz));
	mMetadataFile->Add(new XmlParameter("Units", "Membrane voltage (mV)"));
	mMetadataFile->Add(new XmlDimension("Frequency (Hz)", mNumChannels, mCFFileName));
	mMetadataFile->SetSource(new XmlSource(ModuleName(), "InnerHaircell_0.1.0.0"));
	mMetadataFile->Write();
    return 1;
}

int InnerHaircell::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int i, j;
	FloatMatrixN *Input, *Output;
	float CurOutputSample;

	Log("    Advance: %s", ModuleName());

	Input = ((EarlabWaveformStream *)InputStream[0])->GetData();	// Only supporting one output at the present moment
	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();	// Only supporting one output at the present moment

	if ((Input->Rank(0) != mFrameSize_Samples) || (Input->Rank(1) != mNumChannels))
		throw EarlabException("%s: Input size mismatch with Start()", ModuleName());
	if ((Output->Rank(0) != mFrameSize_Samples) || (Output->Rank(1) != mNumChannels))
		throw EarlabException("%s: Output size mismatch with Start()", ModuleName());

	/*****************************************************************************/
	/*                   START UP THE BASILAR MEMBRANE FILTER                    */
	/*****************************************************************************/

    for (j = 0; j < mFrameSize_Samples; j++)
    {
		/***************** RESET shftin COUNTER FOR EACH NEW FRAME ******************/
        for (i = 0; i < mNumChannels; i++)
		{
			CurOutputSample = IHCs[i]->Step(Input->Data(j, i));
			mMetadataFile->UpdateMinMax(CurOutputSample);
			Output->Data(j, i) = CurOutputSample;
		}
		mMetadataFile->AddSample();
    } 
    return mFrameSize_Samples*mNumChannels;
}

int InnerHaircell::Stop(void)
{
	Log("    Stop: %s", ModuleName());
	mMetadataFile->Write();
    return 1;
}

int InnerHaircell::Unload(void)
{
	Log("    Unload: %s", ModuleName());
    return 1;
}

IHC::IHC(double SampleRate_Hz, double Gain, double Tau)
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

	/*****************************************************************************/
	/*                  SET IHC FILTER ARRAYS AND CONSTANTS                      */
	/*****************************************************************************/

    ga0  = 1 / (1 + exp(x0 / sx0) * (1 + exp(x1 / sx1)));
    ihca = 1 / ((Tau * 2.0) / DeltaT + 1);
    ihcb = ((Tau * 2.0) / DeltaT) - 1;

    mOldGA[0] = ga0;

	//state_0 = 0.6 / (1 + exp(-1 * (0 - x0) / sx0)) + (0.4) / (1 + exp(-1 * (0 - x1) / sx1));
	state_0  = mGain / (1 + exp(-(0 - x0) / sx0) * (1 + exp(-(0 - x1) / sx1)));
}

float IHC::Step(float Displacement_nM)
{
	double Displacement = Displacement_nM;

	if (IS_ALMOST_DENORMAL(Displacement))
		Displacement = 0;

	/************************* IHC RECTIFIER **************************************/ 
	mGA  = (mGain / (1 + exp(-(Displacement - x0) / sx0) * (1 + exp(-(Displacement - x1) / sx1)))) - state_0;
#if 0
	mGA = mGain * 
		((0.6) / (1 + exp(-1 * (Displacement - x0) / sx0)) + 
		 (0.4) / (1 + exp(-1 * (Displacement - x1) / sx1)) - 
		 state_0); 
#endif

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
