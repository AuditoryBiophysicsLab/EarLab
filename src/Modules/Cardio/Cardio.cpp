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
#include "Cardio.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "EarlabException.h"
#include <math.h>
#ifdef _DEBUG
#include "Crtdbg.h"
#endif

using namespace std;

Cardio::Cardio()
{
	SetModuleName("Cardio");
	mModel = NULL;
	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_ALLOC_MEM_DF);
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
			"Cardiovascular compartment",
			"",
			"",
			"Cardiovascular compartment model, more detail needed",
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
			&mInitialVoltage
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
		FILE *fp = fopen("Cardio.xml", "w");
		WriteXMLDescription(fp);
		fclose(fp);
	}
#endif
}

Cardio::~Cardio()
{
	if (mModel != NULL)
		delete mModel;
}

int Cardio::ReadParameters(const char *ParameterFileName)
{
	if (ModuleName() == NULL)
	    return ReadParameters(ParameterFileName, "Cardio");
	else
	    return ReadParameters(ParameterFileName, ModuleName());

}

int Cardio::ReadParameters(const char *ParameterFileName, const char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	Log("    ReadParameters: %s \"%s\"", ModuleName(), ParameterFileName);

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, 0.0);
    if (Success != theParamFile.GetParameter(SectionName, "ModelDescriptionFile", mModelDescriptionFile, sizeof(mModelDescriptionFile), ""))
		throw EarlabException("%s: The ModelDescriptionFile MUST be specified", ModuleName());
	Status = theParamFile.GetParameter(SectionName, "VentricularFunctionInputIndex", mVentricularFunctionInputIndex, -1);
	Status = theParamFile.GetParameter(SectionName, "BiasPressureInputIndex", mBiasPressureInputIndex, -1);

    return 1;
}

int Cardio::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	int Stream;
	int RequiredInputCount = 0;
	double C, R1, R2, Vinitial;
	bool isVentricle, hasVbias;
	char CompartmentName[32];
	_ASSERTE( _CrtCheckMemory( ) );
	CParameterFile theParamFile(mModelDescriptionFile);
	_ASSERTE( _CrtCheckMemory( ) );

	cout << "    Start: " << ModuleName() << endl;

    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...

	// SO this means that ONLY Pressure modules ARE ALLOWED to have ganged inputs
	if (mVentricularFunctionInputIndex != -1)
	{
		mVentricularFunctionInputIndex--;
		if ((mVentricularFunctionInputIndex >= NumInputs) || (mVentricularFunctionInputIndex == mBiasPressureInputIndex))
	        throw EarlabException("%s: VentricularFunctionInputIndex must be the index of a valid input that is not the same as the bias pressure input, if given.  Sorry", ModuleName());
		RequiredInputCount++;
	}
	if (mBiasPressureInputIndex != -1)
	{
		mBiasPressureInputIndex--;
		if ((mBiasPressureInputIndex >= NumInputs) || (mBiasPressureInputIndex < NumOutputs) || (mVentricularFunctionInputIndex == mBiasPressureInputIndex))
	        throw EarlabException("%s: BiasPressureInputIndex must be the index of a valid input that is not the same as the ventricular function input, if given.  Sorry", ModuleName());
		RequiredInputCount++;
	}
	if (NumInputs != RequiredInputCount)
		throw EarlabException("%s: Taking into account this module's Ventricular Function input and Bias Pressure input requirements, the required number of inputs for this module is %d.  Sorry!", ModuleName(), RequiredInputCount);

	if (NumInputs > 0)
	{
		for (Stream = 0; Stream < NumInputs; Stream++)
		{
			if (InputTypes[Stream] != WaveformData)
				throw EarlabException("%s: This module requires all of its input streams to be of Waveform type.  Sorry!", ModuleName());
			if (InputSize[Stream][0] != InputSize[0][0])
				throw EarlabException("%s: This module requires all of its input streams to be the same size.  Sorry!", ModuleName());
			if (InputSize[Stream][1] != 0)
				throw EarlabException("%s: This module requires all of its input streams to one dimensional.  Sorry!", ModuleName());
		}
	}

	if ((NumOutputs != 1) || (OutputTypes[0] != WaveformData))
		throw EarlabException("%s: This module supports only one output stream, whose type must be Waveform.  Sorry!", ModuleName());
	if (OutputSize[Stream][1] != 0)
		throw EarlabException("%s: This module requires all of its output streams to one dimensional.  Sorry!", ModuleName());
	if (NumInputs > 0)
	{
		if (OutputSize[0][0] != InputSize[0][0])
			throw EarlabException("%s: This module requires dimension 1 of its input and output streams to be the same size.  Sorry!", ModuleName());
	}

	if (Success != theParamFile.GetParameter("Model", "NumCompartments", mNumCompartments, 0))
		throw EarlabException("%s: Error: An int parameter with name NumCompartments must appear in the [Model] section of the ModelDescriptionFile", ModuleName());

	_ASSERTE( _CrtCheckMemory( ) );
	if ((OutputSize[0][1] != 4 + (mNumCompartments * 3)) || (OutputSize[0][2] != 0))
		throw EarlabException("%s: This module requires its output streams to be two dimensional and the second dimension must be 4 + (NumCompartments * 3). See %s for the definition of NumCompartments", ModuleName(), mModelDescriptionFile);

	_ASSERTE( _CrtCheckMemory( ) );
	mModel = new CardioModel(mNumCompartments, mSampleRate_Hz);
	_ASSERTE( _CrtCheckMemory( ) );
	for (int CurCompartment = 0; CurCompartment < mNumCompartments; CurCompartment++)
	{
		_ASSERTE( _CrtCheckMemory( ) );
		sprintf(CompartmentName, "Compartment%d", CurCompartment + 1);
		_ASSERTE( _CrtCheckMemory( ) );
		if (Success != theParamFile.GetParameter(CompartmentName, "Capacitance", C, 0))
			throw EarlabException("%s: Error: A float parameter with name Capacitance must appear in the [%s] section of the ModelDescriptionFile", ModuleName(), CompartmentName);
		_ASSERTE( _CrtCheckMemory( ) );
		if (Success != theParamFile.GetParameter(CompartmentName, "R1", R1, 0))
			throw EarlabException("%s: Error: A float parameter with name R1 must appear in the [%s] section of the ModelDescriptionFile", ModuleName(), CompartmentName);
		_ASSERTE( _CrtCheckMemory( ) );
		if (Success != theParamFile.GetParameter(CompartmentName, "R2", R2, 0))
			throw EarlabException("%s: Error: A float parameter with name R2 must appear in the [%s] section of the ModelDescriptionFile", ModuleName(), CompartmentName);
		_ASSERTE( _CrtCheckMemory( ) );
		if (Success != theParamFile.GetParameter(CompartmentName, "Vinitial", Vinitial, 0))
			throw EarlabException("%s: Error: A float parameter with name Vinitial must appear in the [%s] section of the ModelDescriptionFile", ModuleName(), CompartmentName);
		_ASSERTE( _CrtCheckMemory( ) );
		theParamFile.GetParameter(CompartmentName, "IsVentricle", isVentricle, false);
		_ASSERTE( _CrtCheckMemory( ) );
		theParamFile.GetParameter(CompartmentName, "HasVbias", hasVbias, false);
		_ASSERTE( _CrtCheckMemory( ) );
		if ((R1 <= 0) || (R2 <= 0))
			throw EarlabException("%s: Error in ModelDescriptionFile section [%s]: Resistance values R1 and R2 must all be greater than zero for the compartment to function properly.", ModuleName(), CompartmentName);
		if (C <= 0)
			throw EarlabException("%s: Error in ModelDescriptionFile section [%s]: Capacitance value greater than zero for the compartment to function properly.", ModuleName(), CompartmentName);
		if (isVentricle && (mVentricularFunctionInputIndex == -1))
			throw EarlabException("%s: Error in ModelDescriptionFile section [%s]: IsVentricle is true but no ventricular function input was specified in the parameter file integer parameter VentricularFunctionInputIndex.", ModuleName(), CompartmentName);
		if (hasVbias && (mBiasPressureInputIndex == -1))
			throw EarlabException("%s: Error in ModelDescriptionFile section [%s]: HasVbias is true but no bias function input was specified in the parameter file integer parameter BiasPressureInputIndex.", ModuleName(), CompartmentName);
		_ASSERTE( _CrtCheckMemory( ) );
		mModel->AddCompartment(new CardioCompartment(mSampleRate_Hz, Vinitial, C, R1, R2, isVentricle, hasVbias));
		_ASSERTE( _CrtCheckMemory( ) );
	}
	mModel->Initialize();

	SamplesPerFrame = InputSize[0][0];

	mNumInputs = NumInputs;
	mNumOutputs = NumOutputs;

	return 1;
}

int Cardio::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
	int Sample;
	int Stream;
	double *OutputArray;

	Log("    Advance: %s", ModuleName());

	if (stricmp(ModuleName(), "LeftVentricle") == 0)
		Sample = 0;

	mOutput = ((EarlabWaveformStream *)OutputStream[0])->GetData();

	if (mVentricularFunctionInputIndex != -1)
		mElastance = ((EarlabWaveformStream *)InputStream[mVentricularFunctionInputIndex])->GetData();
	if (mBiasPressureInputIndex != -1)
		mBiasPressure = ((EarlabWaveformStream *)InputStream[mBiasPressureInputIndex])->GetData();

	for (Sample = 0; Sample < SamplesPerFrame; Sample++)
	{
		if (mBiasPressureInputIndex != -1)
			mModel->SetVbias(mBiasPressure->Data(Sample));
		if (mVentricularFunctionInputIndex != -1)
			mModel->SetVentricularElastance(mElastance->Data(Sample));

		_ASSERTE( _CrtCheckMemory( ) );
		mModel->Step();
		_ASSERTE( _CrtCheckMemory( ) );

		OutputArray = mModel->GetOutputs();
		mOutput->Data(Sample, 0) = (float)mElastance->Data(Sample);
		mOutput->Data(Sample, 1) = (float)0;

		for (Stream = 0; Stream < 2 + (mNumCompartments * 3); Stream++)
			mOutput->Data(Sample, Stream + 2) = (float)OutputArray[Stream];
	} // for (Sample)

	return SamplesPerFrame * mNumOutputs * 3;
}

int Cardio::Stop(void)
{
	cout << "    Stop: " << ModuleName() << endl;

    return 1;
}


int Cardio::Unload(void)
{
	cout << "    Unload: " << ModuleName() << endl;

    return 1;
}

