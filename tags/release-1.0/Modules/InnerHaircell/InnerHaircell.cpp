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

int InnerHaircell::Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions])
{
	double DeltaT;
	int i;
	char OutputFileName[256];

	mLogger->Log("    Start: %s", mModuleName);
	// Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	if (NumInputs != 1)
		throw EarlabException("%s: Currently this module can only handle one input stream.  Sorry!", mModuleName);

	if (NumOutputs != 1)
		throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", mModuleName);

	if (InputSize[0][0] != OutputSize[0][0])
		throw EarlabException("%s: Input and output frame lengths must be identical.  Sorry!", mModuleName);

	if (InputSize[0][2] != 0)
		throw EarlabException("%s: Input data must be two-dimensional array.  Sorry!", mModuleName);

	if (OutputSize[0][2] != 0)
		throw EarlabException("%s: Output signal must be two-dimensional array.  Sorry!", mModuleName);

	mFrameSize_Samples = OutputSize[0][0];
	mNumChannels = OutputSize[0][1];

	mGA.Init(mNumChannels);
	mOldGA.Init(4, mNumChannels);
	mIHCFilter.Init(4, mNumChannels);
	mOldIHCFilter.Init(4, mNumChannels);
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

    for (i = 0; i< mNumChannels; i++)
    {
        mOldGA.Data(0, i)  = ga0;
		mOldGA.Data(1, i) = 0;
		mOldGA.Data(2, i) = 0;
		mOldGA.Data(3, i) = 0;

		mIHCFilter.Data(0, i) = 0;
		mIHCFilter.Data(1, i) = 0;
		mIHCFilter.Data(2, i) = 0;
		mIHCFilter.Data(3, i) = 0;

		mOldIHCFilter.Data(0, i) = 0;
		mOldIHCFilter.Data(1, i) = 0;
		mOldIHCFilter.Data(2, i) = 0;
		mOldIHCFilter.Data(3, i) = 0;
    }

    state_0 = 0.6 / (1 + exp(-1 * (0 - x0) / sx0)) + (0.4) / (1 + exp(-1 * (0 - x1) / sx1));
	sprintf(OutputFileName, "%s.1", mModuleName);
	mWriter = new BinaryWriter(OutputFileName);
	mWriter->WriteMetadata("File_Type", "Frequency_Time");
	mWriter->WriteMetadata("Frequency_Dimension_Size", mNumChannels);
	mWriter->WriteMetadata("Frame_Length_Samples", mFrameSize_Samples);
	mWriter->WriteMetadata("Sample_Rate_Hz", mSampleRate_Hz);
	mWriter->CloseMetadataFile();
    return 1;
}

int InnerHaircell::Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount])
{
	int i, j;
	double CurDataPoint;
	double *Data;
	unsigned long DataLen = 0;

	mLogger->Log("    Advance: %s", mModuleName);
	if ((Input[0]->Rank(0) != mFrameSize_Samples) || (Input[0]->Rank(1) != mNumChannels))
		throw EarlabException("%s: Input size mismatch with Start()", mModuleName);
	if ((Output[0]->Rank(0) != mFrameSize_Samples) || (Output[0]->Rank(1) != mNumChannels))
		throw EarlabException("%s: Output size mismatch with Start()", mModuleName);

	/***************************************************************************/
	/* Note: The following four sections are connected so that a data point    */
	/* is passed through all the channels of each filter before the next point */
	/* is processed. The counter variable for the channels is [i], the counter */  
	/* for the time points in the frame is [j], and the counter for the number */
	/* of frames is frmcount over the number of frames(numfrm).                */
	/*                                                                         */ 
	/*                Loops: [i]  over the number of channels                  */ 
	/*                       FRMSIZ [j] outside over the NUMCH inside          */
	/***************************************************************************/


	/*****************************************************************************/
	/*                   START UP THE BASILAR MEMBRANE FILTER                    */
	/*****************************************************************************/

    for (j = 0; j < mFrameSize_Samples; j++)
    {
		/***************** RESET shftin COUNTER FOR EACH NEW FRAME ******************/

        for (i = 0; i < mNumChannels; i++)
        {

			CurDataPoint = Input[0]->Data(j, i);
			/*******************************************************************************/
			/*                   START UP THE INNER HAIR CELL FILTER                       */
			/*******************************************************************************/

			/************************* IHC RECTIFIER **************************************/ 

            mGA.Data(i) = mGain * ((0.6) / (1 + exp(-1  *(CurDataPoint - x0) / sx0)) + (0.4) / (1 + exp(-1 * (CurDataPoint - x1) / sx1)) - state_0); 


			/********************** PUT IHC OUTPUT INTO AN ARRAY FOR NEXT STAGE ***********/                      

            mIHCFilter.Data(0, i) = ihca * (mGA.Data(i)           + mOldGA.Data(0, i) + ihcb * mOldIHCFilter.Data(0, i));
            mIHCFilter.Data(1, i) = ihca * (mIHCFilter.Data(0, i) + mOldGA.Data(1, i) + ihcb * mOldIHCFilter.Data(1, i));
            mIHCFilter.Data(2, i) = ihca * (mIHCFilter.Data(1, i) + mOldGA.Data(2, i) + ihcb * mOldIHCFilter.Data(2, i));
            mIHCFilter.Data(3, i) = ihca * (mIHCFilter.Data(2, i) + mOldGA.Data(3, i) + ihcb * mOldIHCFilter.Data(3, i));

			/********************** END LOOP OVER THE NUMBER OF CHANNELS ********************/
			Output[0]->Data(j, i) = mIHCFilter.Data(3, i);

			/********************* STORE OLD VALUES FOR NEXT DATA POINT ******************/

            mOldGA.Data(0, i) = mGA.Data(i);
			mOldGA.Data(1, i) = mIHCFilter.Data(0, i);
            mOldGA.Data(2, i) = mIHCFilter.Data(1, i);
			mOldGA.Data(3, i) = mIHCFilter.Data(2, i);

            mOldIHCFilter.Data(0, i) = mIHCFilter.Data(0, i);
			mOldIHCFilter.Data(1, i) = mIHCFilter.Data(1, i); 
            mOldIHCFilter.Data(2, i) = mIHCFilter.Data(2, i);
			mOldIHCFilter.Data(3, i) = mIHCFilter.Data(3, i);

        }

/***********************************************************************************/
/*                       DOWNSAMPLE MODEL OUTPUT AND WRITE TO FILE                 */  
/***********************************************************************************/


/********************** END LOOP OVER EACH FRAME OF DATA **************************/
    } 
    //return i;
	Data = Output[0]->GetData(&DataLen);
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
