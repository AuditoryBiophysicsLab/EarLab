#include <iostream>
#include "Earlab.h"
#include "AuditoryNerveSpike.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"

using namespace std;

AuditoryNerveSpike::AuditoryNerveSpike()
{
    memset(this, 0, sizeof(AuditoryNerveSpike));
	SetModuleName("AuditoryNerveSpike");
}

AuditoryNerveSpike::~AuditoryNerveSpike()
{
	if (mWriter != NULL)
		delete mWriter;
}

int AuditoryNerveSpike::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	    return ReadParameters(ParameterFileName, "AuditoryNerveSpike");
	else
	    return ReadParameters(ParameterFileName, ModuleName());
}

int AuditoryNerveSpike::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, mSampleRate_Hz);
    Status = theParamFile.GetParameter(SectionName, "c0", c0, 0.0);
    Status = theParamFile.GetParameter(SectionName, "c1", c1, 0.0);
    Status = theParamFile.GetParameter(SectionName, "s0", s0, 0.0);
    Status = theParamFile.GetParameter(SectionName, "s1", s1, 0.0);
    Status = theParamFile.GetParameter(SectionName, "Ra", Ra, 0.0);
    Status = theParamFile.GetParameter(SectionName, "Scale", Scale, 0.0);
    Status = theParamFile.GetParameter(SectionName, "Offset", Offset, 0.0);
    Status = theParamFile.GetParameter(SectionName, "Gain", mGain, 10.0);
    return 1;
}

int AuditoryNerveSpike::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount])
{
	int i;
	char OutputFileName[256];
	int Channel, Rep;

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
	NumChannels = InputSize[0][1];
	NumReps = OutputSize[0][1];

	sprintf(OutputFileName, "%s.1", ModuleName());
	mWriter = new BinaryWriter(OutputFileName);
	mWriter->WriteMetadata("File_Type", "Frequency_Time");
	mWriter->WriteMetadata("Frequency_Dimension_Size", NumChannels);
	mWriter->WriteMetadata("Frame_Length_Samples", mFrameSize_Samples);
	mWriter->WriteMetadata("Sample_Rate_Hz", mSampleRate_Hz);
	mWriter->CloseMetadataFile();

	ANFibers = new AuditoryNerve *[NumChannels];
	for (i = 0; i < NumChannels; i++)
		ANFibers[i] = new AuditoryNerve(mSampleRate_Hz);

	Cells = new CarneySpikeGenerator *[NumChannels * NumReps];
	for (Channel = 0; Channel < NumChannels; Channel++)
		for (Rep = 0; Rep < NumReps; Rep++)
		{
			Cells[(Channel * NumReps) + Rep] = new CarneySpikeGenerator(mSampleRate_Hz, c0, c1, s0, s1, Ra, Scale, Offset);
		}


    return 1;
}

int AuditoryNerveSpike::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
    int i, j, counter;
	float *Data;
	unsigned long DataLen;
	FloatMatrixN *Input, *Output;

	Log("    Advance: %s", ModuleName());

	Input = ((EarlabWaveformStream *)InputStream[0])->GetData();	// Only supporting one output at the present moment
	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();	// Only supporting one output at the present moment

	if ((Input->Rank(0) != mFrameSize_Samples) || (Input->Rank(1) != NumChannels))
		throw EarlabException("%s: Input size mismatch with Start()", ModuleName());
	if ((Output->Rank(0) != mFrameSize_Samples) || (Output->Rank(1) != NumChannels))
		throw EarlabException("%s: Output size mismatch with Start()", ModuleName());

    counter = 0;

	for (j = 0; j < mFrameSize_Samples; j++)
    {
        for (i = 0; i < NumChannels; i++)
        {
			Output->Data(j, i) = (float)(mGain * ANFibers[i]->Step(Input->Data(j, i)));
            counter++;
        }
    }
	Data = Output->GetData(&DataLen);
	mWriter->CheckMinMax(Data, DataLen);
	mSampleCount += mFrameSize_Samples;
    return counter;
}

int AuditoryNerveSpike::Stop(void)
{
	Log("    Stop: %s", ModuleName());
	mWriter->WriteMetadata("Number_Of_Samples", mSampleCount);
	mWriter->WriteMinMaxMetadata();
	mWriter->Close();
    return 1;
}

int AuditoryNerveSpike::Unload(void)
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


	mANB				= 0.028708;
    mOldB				= 0.028708;
    mANFilter[0]		= 0.144793;
    mOldANFilter[0]		= 0.144793;
    mANFilter[1]		= 0.028708;
    mOldANFilter[1]		= 0.028708;
    mR					= 5.7081;
    mOldR				= 5.7081;
    mANFilter[2]		= mR / kp;
    mOldANFilter[2]		= mR / kp;
}

float AuditoryNerve::Step(float InputValue)
{
	double p;
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
