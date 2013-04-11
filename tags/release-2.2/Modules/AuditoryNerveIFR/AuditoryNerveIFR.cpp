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
	SetModuleName("AuditoryNerveIFR");
}

AuditoryNerveIFR::~AuditoryNerveIFR()
{
	if (mMetadataFile != NULL)
		delete mMetadataFile;
	if (ANFibers != NULL)
		delete [] ANFibers;
}

int AuditoryNerveIFR::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	    return ReadParameters(ParameterFileName, "AuditoryNerveIFR");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int AuditoryNerveIFR::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, mSampleRate_Hz);
    Status = theParamFile.GetParameter(SectionName, "Gain", mGain, 10);
 	Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), "");
    return 1;
}

int AuditoryNerveIFR::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount])
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

    if (InputSize[0][1] != OutputSize[0][1])
	throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", ModuleName());

    if (InputSize[0][2] != 0)
	throw EarlabException("%s: Input data must be two-dimensional array.  Sorry!", ModuleName());

    if (OutputSize[0][2] != 0)
	throw EarlabException("%s: Output signal must be two-dimensional array.  Sorry!", ModuleName());

	OutputElementCounts[0] = OutputSize[0][0] * OutputSize[0][1];

    mFrameSize_Samples = OutputSize[0][0];
    mNumChannels = OutputSize[0][1];

	sprintf(MetadataFileName, "%s.1.metadata", ModuleName());
	mMetadataFile = new XmlMetadataFile(MetadataFileName);
	mMetadataFile->SetSampleRate_Hz(mSampleRate_Hz);
	mMetadataFile->Add(new XmlParameter("Units", "IFR (Spikes/S)"));
	//mMetadataFile->Add(new XmlParameter("FrameSize", mFrameSize_Samples));
	//mMetadataFile->Add(new XmlParameter("SampleRate_Hz", mSampleRate_Hz));
	mMetadataFile->Add(new XmlDimension("Frequency (Hz)", mNumChannels, mCFFileName));
	mMetadataFile->SetSource(new XmlSource(ModuleName(), "AuditoryNerveIFR_0.1.0.0"));
	mMetadataFile->Write();

	ANFibers = new AuditoryNerve *[mNumChannels];
	for (i = 0; i < mNumChannels; i++)
		ANFibers[i] = new AuditoryNerve(mSampleRate_Hz);

    return 1;
}

int AuditoryNerveIFR::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
    int i, j, counter;
	float CurOutputSample;
	FloatMatrixN *Input, *Output;

	Log("    Advance: %s", ModuleName());

	Input = ((EarlabWaveformStream *)InputStream[0])->GetData();    // Only supporting one output at the present moment
	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();  // Only supporting one output at the present moment

	if ((Input->Rank(0) != mFrameSize_Samples) || (Input->Rank(1) != mNumChannels))
		throw EarlabException("%s: Input size mismatch with Start()", ModuleName());
	if ((Output->Rank(0) != mFrameSize_Samples) || (Output->Rank(1) != mNumChannels))
		throw EarlabException("%s: Output size mismatch with Start()", ModuleName());

    counter = 0;

	for (j = 0; j < mFrameSize_Samples; j++)
    {
		for (i = 0; i < mNumChannels; i++)
		{
			CurOutputSample = (float)(mGain * ANFibers[i]->Step(Input->Data(j, i) * 0.2f));
			mMetadataFile->UpdateMinMax(CurOutputSample);
			Output->Data(j, i) = CurOutputSample;
			counter++;
		}
		mMetadataFile->AddSample();
    }
    return counter;
}

int AuditoryNerveIFR::Stop(void)
{
	Log("    Stop: %s", ModuleName());
	mMetadataFile->Write();
    return 1;
}

int AuditoryNerveIFR::Unload(void)
{
	Log("    Unload: %s", ModuleName());

    return 1;
}

AuditoryNerve::AuditoryNerve(double SampleRate_Hz)
{
	mSampleRate_Hz = SampleRate_Hz;

    /* Convert sample/sec  to sec/sample */
    mDeltaT = 1.0 / (1.0 * mSampleRate_Hz);

    v0 = 2.100000;
    ks = 0.800000;
    n0 = 32.300000;

    kp = 3.30;
    kr = 7.09;
    k3 = 227;
    k4 = 45.0;
    mTauCA = 1.1e-04;
    Thr_spont = 2.50;

	ka1  = 1.0/(2.0/(k4*mDeltaT) + 1);
    kb1  = 2.0/(k4*mDeltaT) - 1;

    ka2  = 1.0/(2.0*mTauCA/mDeltaT + 1);
    kb2  = 2.0*mTauCA/mDeltaT - 1;

    ka3  = 1.0/(2.0/(kp*mDeltaT) + 1);
    kb3  = 2.0/(kp*mDeltaT) - 1;


	mANB                            = 0.028708;
    mOldB                               = 0.028708;
    mANFilter[0]                = 0.144793;
    mOldANFilter[0]             = 0.144793;
    mANFilter[1]                = 0.028708;
    mOldANFilter[1]             = 0.028708;
    mR                                  = 5.7081;
    mOldR                               = 5.7081;
    mANFilter[2]                = mR / kp;
    mOldANFilter[2]             = mR / kp;
}

float AuditoryNerve::Step(float InputValue)
{
	double p;
    /***********************************************************************************/
    /*               START UP THE AUDITORY NERVE SyNaPsE MODEL                         */
    /*                                                                                 */
    /*        ANMOD READS HYPERSIGNAL DATA FILES CONTAINING IHC MEMBRANE               */
    /*        POTENTIAL RECORD AND COMPUTES THE RESULTING NEUROTRANSMITTER             */
    /*    RELEASE RATE.                                                                */
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

    if (InputValue > 18.0)
	InputValue = 18.0;

    /************************* Ca ACTIVATION  ****************************************/

    p         = 1/(1+exp(2*ks*(-1*InputValue+v0)));

    /************************* Ca CHANNEL { Depletion Mechanism #1 } ******************/

    mANB    = (1 - mANFilter[0])*p;
    mANFilter[0] = ka1*((k3/k4)*(mANB+mOldB)+kb1*mOldANFilter[0]);

    /************************ Ca COMPARTMENT { Lowpass filter } **********************/

    mANFilter[1] = ka2*(mANB+mOldB+kb2*mOldANFilter[1]);

    /*********************** VESICLE DEPLETION  { Depletion Mechanism #2 } ***********/
    mR     = mANFilter[1]*kr*(n0 - mANFilter[2]);
    mANFilter[2] = ka3*((1/kp)*(mR+mOldR)+kb3*mOldANFilter[2]);

    /********************** PUT AN OUTPUT INTO OUTPUT ARRAY *************************/

    if (mR >= Thr_spont)
	mR = (mR-Thr_spont)*((tanh(mR-Thr_spont)+1)/2);
    else
	mR = 0.0;

    /********************* STORE OLD VALUES FOR NEXT DATA POINT *********************/
    if (mR < -1000)
	mR = mOldR;
    else
	mOldR   = mR;

    mOldB   = mANB;
    mOldANFilter[0] = mANFilter[0];
    mOldANFilter[1] = mANFilter[1];
    mOldANFilter[2] = mANFilter[2];

	return (float)mR;
}
