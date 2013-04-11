#include <iostream>
#include "Earlab.h"
#include "AuditoryNerveIFR.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"

using namespace std;

AuditoryNerveIFR::AuditoryNerveIFR()
{
    memset(this, 0, sizeof(AuditoryNerveIFR));
	mLogger = new Logger();
	SetModuleName("AuditoryNerveIFR");
}

AuditoryNerveIFR::~AuditoryNerveIFR()
{
	if (mModuleName != NULL)
		delete [] mModuleName;
	if (mWriter != NULL)
		delete mWriter;
}

int AuditoryNerveIFR::ReadParameters(char *ParameterFileName)
{
	if (mModuleName == NULL)
	    return ReadParameters(ParameterFileName, "AuditoryNerveIFR");
	else
	    return ReadParameters(ParameterFileName, mModuleName);
}

int AuditoryNerveIFR::ReadParameters(char *ParameterFileName, char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	mLogger->Log("    ReadParameters: %s \"%s\"", mModuleName, ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, mSampleRate_Hz);
    return 1;
}

int AuditoryNerveIFR::Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions])
{
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

    if (InputSize[0][1] != OutputSize[0][1])
        throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", mModuleName);

    if (InputSize[0][2] != 0)
        throw EarlabException("%s: Input data must be two-dimensional array.  Sorry!", mModuleName);

    if (OutputSize[0][2] != 0)
        throw EarlabException("%s: Output signal must be two-dimensional array.  Sorry!", mModuleName);

    mFrameSize_Samples = OutputSize[0][0];
    mNumChannels = OutputSize[0][1];

	mANB.Init(mNumChannels);
	mOldB.Init(mNumChannels);
	mNMax.Init(mNumChannels);
	mR.Init(mNumChannels);
	mOldR.Init(mNumChannels);
	mANFilter.Init(3, mNumChannels);
	mOldANFilter.Init(3, mNumChannels);

    /* Convert sample/sec  to sec/sample */ 
    mDeltaT = 1.0 / (1.0 * mSampleRate_Hz);

    v0=2.100000; 
    ks=0.800000; 
    n0=32.300000; 

    kp =3.30;
    kr =7.09;
    k3 =227;
    k4 =45.0;
    mTauCA= 1.1e-04;
    Thr_spont= 2.50;

    /*****************************************************************************/
    /*                  SET AN FILTER ARRAYS AND CONSTANTS                       */
    /*****************************************************************************/

    ka1  = 1.0/(2.0/(k4*mDeltaT) + 1);
    kb1  = 2.0/(k4*mDeltaT) - 1;

    ka2  = 1.0/(2.0*mTauCA/mDeltaT + 1);
    kb2  = 2.0*mTauCA/mDeltaT - 1;

    ka3  = 1.0/(2.0/(kp*mDeltaT) + 1);
    kb3  = 2.0/(kp*mDeltaT) - 1;

    for (i = 0; i < mNumChannels; i++)
    {
        mANB.Data(i)				= 0.028708;
        mOldB.Data(i)				= 0.028708;
        mANFilter.Data(0, i)		= 0.144793;
        mOldANFilter.Data(0, i)	= 0.144793;
        mANFilter.Data(1, i)		= 0.028708;
        mOldANFilter.Data(1, i)	= 0.028708;
        mR.Data(i)					= 5.7081;
        mOldR.Data(i)				= 5.7081;
        mANFilter.Data(2, i)		= mR.Data(i) / kp;
        mOldANFilter.Data(2, i)	= mR.Data(i) / kp;
    }

	sprintf(OutputFileName, "%s.1", mModuleName);
	mWriter = new BinaryWriter(OutputFileName);
	mWriter->WriteMetadata("File_Type", "Frequency_Time");
	mWriter->WriteMetadata("Frequency_Dimension_Size", mNumChannels);
	mWriter->WriteMetadata("Frame_Length_Samples", mFrameSize_Samples);
	mWriter->WriteMetadata("Sample_Rate_Hz", mSampleRate_Hz);
	mWriter->CloseMetadataFile();

    return 1;
}

int AuditoryNerveIFR::Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount])
{
    int i, j, counter;
    double CurDataPoint;
	double p;
	const double *Data;
	unsigned long DataLen;

	mLogger->Log("    Advance: %s", mModuleName);
	if ((Input[0]->Rank(0) != mFrameSize_Samples) || (Input[0]->Rank(1) != mNumChannels))
		throw EarlabException("%s: Input size mismatch with Start()", mModuleName);
	if ((Output[0]->Rank(0) != mFrameSize_Samples) || (Output[0]->Rank(1) != mNumChannels))
		throw EarlabException("%s: Output size mismatch with Start()", mModuleName);

    counter = 0;
    /***************************************************************************/
    /* Note: The following four sections are connected so that a data point    */
    /* is passed through all the channels of each filter before the next point */
    /* is processed. The counter variable for the channels is [i], the counter */  
    /* for the time points in the frame is [j], and the counter for the number */
    /* of frames is frmcount over the number of frames(numfrm).                */
    /*                                                                         */ 
    /*                Loops: [i]  over the number of channels                  */ 
    /*                       FRMSIZ [j] outside over the NUMCH inside          */
    /*                       NUMFRM  outside over FRMSIZ inside                */
    /***************************************************************************/

    for (j = 0; j < mFrameSize_Samples; j++)
    {
        /***************** RESET shftin COUNTER FOR EACH NEW FRAME ******************/
        for (i = 0; i < mNumChannels; i++)
        {
			CurDataPoint = Input[0]->Data(j, i);
            /***********************************************************************************/
            /*               START UP THE AUDITORY NERVE SyNaPsE MODEL                         */
            /*                                                                                 */
            /*        ANMOD READS HYPERSIGNAL DATA FILES CONTAINING IHC MEMBRANE               */
            /*        POTENTIAL RECORD AND COMPUTES THE RESULTING NEUROTRANSMITTER             */
            /*	  RELEASE RATE.                                                                */
            /*                                                                                 */
            /*        MODEL PARAMETERS ARE FROM BILL D'ANGELO'S THESIS WITH THE                */
            /*        FOLLOWING CHANGES.  A AND B REPRESENT PROBABILITIES, NOT THE             */
            /*        NUMBER OF CHANNELS.  KR IS SCALED ACCORDINGLY SINCE BILL'S               */
            /*        MODEL ASSUMED A TOTAL OF 100 CHANNELS.  K1, K2, K3, AND K4               */
            /*        HAD INCORRECT UNITS IN BILL'S THESIS.  HIS UNITS APPEAR TO BE            */
            /*        IN MS, NOT SECONDS.                                                      */
            /*                                                                                 */
            /***********************************************************************************/

            /*********************** TO AVOID OVERFLOW ****************************************/   

            if (CurDataPoint > 18.0)
                CurDataPoint = 18.0;

            /************************* Ca ACTIVATION  ****************************************/

            p         = 1/(1+exp(2*ks*(-1*CurDataPoint+v0)));

            /************************* Ca CHANNEL { Depletion Mechanism #1 } ******************/

            mANB.Data(i)    = (1 - mANFilter.Data(0, i))*p;
            mANFilter.Data(0, i) = ka1*((k3/k4)*(mANB.Data(i)+mOldB.Data(i))+kb1*mOldANFilter.Data(0, i));

            /************************ Ca COMPARTMENT { Lowpass filter } **********************/

            mANFilter.Data(1, i) = ka2*(mANB.Data(i)+mOldB.Data(i)+kb2*mOldANFilter.Data(1, i));

            /*********************** VESICLE DEPLETION  { Depletion Mechanism #2 } ***********/
            mR.Data(i)      = mANFilter.Data(1, i)*kr*(n0 - mANFilter.Data(2, i));
            mANFilter.Data(2, i) = ka3*((1/kp)*(mR.Data(i)+mOldR.Data(i))+kb3*mOldANFilter.Data(2, i));

            /********************** PUT AN OUTPUT INTO OUTPUT ARRAY *************************/

            if (mR.Data(i) >= Thr_spont)
                mR.Data(i) = (mR.Data(i)-Thr_spont)*((tanh(mR.Data(i)-Thr_spont)+1)/2);
            else
                mR.Data(i) = 0.0;

            /********************* STORE OLD VALUES FOR NEXT DATA POINT *********************/
            if (mR.Data(i) < -1000)
                mR.Data(i)= mOldR.Data(i);
            else
                mOldR.Data(i)   = mR.Data(i);  

            mOldB.Data(i)   = mANB.Data(i);
            mOldANFilter.Data(0, i)= mANFilter.Data(0, i);
            mOldANFilter.Data(1, i)= mANFilter.Data(1, i);
            mOldANFilter.Data(2, i)= mANFilter.Data(2, i);

            /********************** END LOOP OVER THE NUMBER OF CHANNELS ********************/
            Output[0]->Data(j, i) = 10*mR.Data(i);
            counter++;
        }

        /********************** END LOOP OVER EACH FRAME OF DATA **************************/
    }
	Data = Output[0]->GetData(&DataLen);
	mWriter->CheckMinMax(Data, DataLen);
	mSampleCount += mFrameSize_Samples;
    return counter;
}

int AuditoryNerveIFR::Stop(void)
{
	mLogger->Log("    Stop: %s", mModuleName);
	mWriter->WriteMetadata("Number_Of_Samples", mSampleCount);
	mWriter->WriteMinMaxMetadata();
	mWriter->Close();
    return 1;
}

int AuditoryNerveIFR::Unload(void)
{
	mLogger->Log("    Unload: %s", mModuleName);

    return 1;
}

void AuditoryNerveIFR::SetModuleName(char *ModuleName)
{
	if (mModuleName != NULL)
		delete [] mModuleName;

	mModuleName = new char[strlen(ModuleName) + 1];
	strcpy(mModuleName, ModuleName);
}

void AuditoryNerveIFR::SetLogger(Logger *TheLogger)
{
	if (mLogger != NULL)
		delete mLogger;
	mLogger = TheLogger;
}

