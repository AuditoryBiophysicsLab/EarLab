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
#include "CardioVentricularDriver.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "EarlabException.h"
#include <math.h>

using namespace std;

CardioVentricularDriver::CardioVentricularDriver()
{
	mMetadataFile = NULL;
	mCardioDriver = NULL;

	SetModuleName("CardioVentricularDriver");

#ifdef DO_THIS_LATER	
	RegisterModuleInfo(
		new EarlabModuleInfo(
			ModuleName(),
			"0.5.0.0",
			"2.5",
			"Unreleased",
			__DATE__,
			__TIME__,
			"Dave Anderson",
			"da@bu.edu",
			"CardioVentricularDrivervascular compartment",
			"",
			"",
			"CardioVentricularDrivervascular compartment model, more detail needed",
			"Written for senior project",
			"Some paper I have as a PDF"
		));
	RegisterParameterInfo(
		EarlabParameterInfoFactory::DoubleParam(
			"SampleRate_Hz",
			0.0,
			"Samples Per Second (Hertz)",
			"Sample rate for the input and output streams",
			&mSampleRate_Hz
		));
	RegisterParameterInfo(
		EarlabParameterInfoFactory::DoubleVectorParam(
			"Resistances",
			(double *)NULL, 0,
			"Flow resistance (internally specified in Ohms)",
			"Flow resistance for the vessels connecting to this compartment, indices in this parameter must correspond with the indices of input and output streams of this module",
			&mResistances,
			&mNumBranches
		));
	RegisterParameterInfo(
		EarlabParameterInfoFactory::DoubleParam(
			"InitialVoltageOrCurrent",
			0.0,
			"Value used to initialize the compartment (Volts or Amperes)",
			"If required, this value is used to 'prime the pump', or give a pre-existing pressure or current value to the compartment modeled by this module",
			&mInitialVoltageOrCurrent
		));
	RegisterParameterInfo(
		EarlabParameterInfoFactory::BooleanParam(
			"PressureIsInput",
			0.0,
			NULL,
			"True if the module should treat its INPUT streams as pressures, false if the module should treat its INPUT streams as flows",
			&mPressureIsInput
		));
	RegisterParameterInfo(
		EarlabParameterInfoFactory::BooleanParam(
			"IsVentricleModel",
			0.0,
			NULL,
			"True if the module takes its last input stream as the value of the compartment's compliance, false otherwise",
			&mIsVentricleModel
		));
	RegisterInputInfo(new EarlabIOStreamInfo(
		1,
		"Input 1",
		WaveformData,
		"Generic input stream",
		"Contraints go here"
		));
	RegisterOutputInfo(new EarlabIOStreamInfo(
		1,
		"Output 1",
		WaveformData,
		"Generic output stream",
		"Contraints go here"
		));
	{
		FILE *fp = fopen("CardioVentricularDriver.xml", "w");
		WriteXMLDescription(fp);
		fclose(fp);
	}
#endif
}

CardioVentricularDriver::~CardioVentricularDriver()
{
	if (mMetadataFile != NULL)
		delete mMetadataFile;
	if (mCardioDriver == NULL)
		delete mCardioDriver;
}

int CardioVentricularDriver::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	    return ReadParameters(ParameterFileName, "CardioVentricularDriver");
	else
	    return ReadParameters(ParameterFileName, ModuleName());

}

int CardioVentricularDriver::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0.0);
    Status = theParamFile.GetParameter(SectionName, "InitalHeartrate_BeatsPerMinute", mInitialHeartrate_BeatsPerMinute, 60.0);
    Status = theParamFile.GetParameter(SectionName, "SystolicElastance", mSystolicElastance, 20.0);
    Status = theParamFile.GetParameter(SectionName, "DiastolicElastance", mDiastolicElastance, 1.2);
    Status = theParamFile.GetParameter(SectionName, "OutputGain", mOutputGain, 1.0);

    return 1;
}

int CardioVentricularDriver::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	cout << "    Start: " << ModuleName() << endl;

    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	if (NumInputs != 0)
        throw EarlabException("%s: Currently this module allows NO inputs.  Sorry!", ModuleName());

	if (NumOutputs != 1)
        throw EarlabException("%s: Currently this module allows only ONE output.  Sorry!", ModuleName());

	if (OutputTypes[0] != WaveformData)
        throw EarlabException("%s: The output format must be Waveform.  Sorry!", ModuleName());

	if ((OutputSize[0][0] == 0) || (OutputSize[0][1] != 0))
        throw EarlabException("%s: The output must be one-dimensional.  Sorry!", ModuleName());

	if (mSampleRate_Hz == 0)
        throw EarlabException("%s: Sample rate (in Hz) must be specified and be greater than zero!", ModuleName());

	mSamplesPerFrame = OutputSize[0][0];

	mCardioDriver = new CardioDriver(mSampleRate_Hz, mSystolicElastance, mDiastolicElastance, mInitialHeartrate_BeatsPerMinute / 60.0);
	return 1;
}

int CardioVentricularDriver::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	FloatMatrixN *Output;
		
	Log("    Advance: %s", ModuleName());

	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();

	for (int Sample = 0; Sample < mSamplesPerFrame; Sample++)
		Output->Data(Sample) = (float)mCardioDriver->Step(mInitialHeartrate_BeatsPerMinute / 60.0);

	return mSamplesPerFrame;
}

int CardioVentricularDriver::Stop(void)
{
	cout << "    Stop: " << ModuleName() << endl;
	//mMetadataFile->Write();

    return 1;
}

int CardioVentricularDriver::Unload(void)
{
	cout << "    Unload: " << ModuleName() << endl;

    return 1;
}

CardioDriver::CardioDriver(double SampleRate_Hz, double SystolicElastance, double DiastolicElastance, double InitialPeriod_Seconds)
{
	mSamplePeriod_Seconds = 1.0 / SampleRate_Hz;
	mSystolicElastance = SystolicElastance;
	mDiastolicElastance = DiastolicElastance;
	mLastPeriod_Seconds = mCurPeriod_Seconds = InitialPeriod_Seconds;
	mCurTime_Seconds = 0;
	NewBeat();
}

double CardioDriver::Step(double NewPeriod_Seconds)
{
	double retVal;

	if (mCurTime_Seconds < mSystolicPhase1EndTime_Seconds)
		retVal = mDiastolicElastance + (mSystolicElasticityConstant * (1.0 - cos(PI * (mCurTime_Seconds / mPeriodConstant))));
	else
		if (mCurTime_Seconds < mSystolicPhase2EndTime_Seconds)
			retVal = mDiastolicElastance + (mSystolicElasticityConstant * (1.0 + cos(TWOPI * ((mCurTime_Seconds  - mPeriodConstant)/ mPeriodConstant))));
		else
			retVal = mDiastolicElastance;

	mCurTime_Seconds += mSamplePeriod_Seconds;
	if (mCurTime_Seconds >= mCurPeriod_Seconds)
	{
		mCurTime_Seconds = 0;
		mLastPeriod_Seconds = mCurPeriod_Seconds;
		mCurPeriod_Seconds = NewPeriod_Seconds;
		NewBeat();
	}
	return retVal;
}

void CardioDriver::NewBeat(void)
{
	mSystolicElasticityConstant = (mSystolicElastance - mDiastolicElastance) / 2.0;
	mPeriodConstant = 0.3 * sqrt(mLastPeriod_Seconds);
	mSystolicPhase1EndTime_Seconds = mPeriodConstant;
	mSystolicPhase2EndTime_Seconds = (1.5 * mPeriodConstant);
}
