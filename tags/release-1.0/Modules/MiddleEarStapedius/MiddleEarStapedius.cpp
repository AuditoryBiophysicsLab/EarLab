#include <iostream>
#include "MiddleEarStapedius.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "Earlab.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"
#include <windows.h>
// Following include file is for MATLAB calls
#include "engine.h"

using namespace std;

MiddleEarStapedius::MiddleEarStapedius()
{
	memset(this, 0, sizeof(MiddleEarStapedius));
	mLogger = new Logger();
	SetModuleName("MiddleEarStapedius");
}

MiddleEarStapedius::~MiddleEarStapedius()
{
	if (mModuleName != NULL)
		delete [] mModuleName;
}

int MiddleEarStapedius::ReadParameters(char *ParameterFileName)
{
	if (mModuleName == NULL)
		return ReadParameters(ParameterFileName, "MiddleEarStapedius");
	else
		return ReadParameters(ParameterFileName, mModuleName);
}

int MiddleEarStapedius::ReadParameters(char *ParameterFileName, char *SectionName)
{
	CParameterFile theParamFile(ParameterFileName);
	ParameterStatus Status;
	double aDefault[] = {1, 3140};
	double bDefault[] = {1, 0};
	int ADefLen = 2;	// Length of aDefault array
	int BDefLen = 2;	// Length of bDefault array
	int ALen = MidearMaxFilterSize;	// Length of a Array
	int BLen = MidearMaxFilterSize; // Length of b Array

	cout << "    ReadParameters: " << mModuleName << endl;

	// Number of channels and Frame Size are passed as parameters to Start, see that function for details
	Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, mSampleRate_Hz);
	Status = theParamFile.GetParameter(SectionName, "FilterSize", FilterSize, 32);
	Status = theParamFile.GetParameter(SectionName, "A", a, ALen, aDefault, ADefLen);
	Status = theParamFile.GetParameter(SectionName, "B", b, BLen, bDefault, BDefLen);
	return 1;
}

int MiddleEarStapedius::Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions])
{
	int i;

	cout << "    Start: " << mModuleName << endl;
	// Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	if (NumInputs != 1)
		throw EarlabException("%s: Currently this module can only handle one input stream.  Sorry!", mModuleName);

	if (NumOutputs != 1)
		throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", mModuleName);

	if (InputSize[0][0] != OutputSize[0][0])
		throw EarlabException("%s: Input and output frame lengths must be identical.  Sorry!", mModuleName);

	if (InputSize[0][1] != 0)
		throw EarlabException("%s: Input data must be one-dimensional array.  Sorry!", mModuleName);

	if (OutputSize[0][1] != 0)
		throw EarlabException("%s: Output signal must be one-dimensional array.  Sorry!", mModuleName);

	mFrameSize_Samples = OutputSize[0][0];

	GetBilinear((float)mSampleRate_Hz);  // perform the coefficient conversion

    z = new double[MidearMaxFilterSize + 1];

    for (i=0; i<(MidearMaxFilterSize+1); i++) z[i] = 0;

	return 1;
}

int MiddleEarStapedius::Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount])
{
    int n, m;

	cout << "    Advance: " << mModuleName << endl;
	if (Input[0]->Rank(0) != mFrameSize_Samples)
		throw EarlabException("%s: Input size mismatch with Start()", mModuleName);
	if (Output[0]->Rank(0) != mFrameSize_Samples)
		throw EarlabException("%s: Output size mismatch with Start()", mModuleName);

    for (m = 0; m < mFrameSize_Samples; m++)
    {
        Output[0]->Data(m) = (double)(num[0] * Input[0]->Data(m) + z[1]);
        for (n = 1; n < (FilterSize - 1); n++)
            z[n] = num[n] * Input[0]->Data(m) + z[n+1] - den[n] * Output[0]->Data(m);
        z[FilterSize - 1] = num[FilterSize - 1] * Input[0]->Data(m) - den[FilterSize - 1] * Output[0]->Data(m);
    }

    return m;
}

int MiddleEarStapedius::Stop(void)
{
	cout << "    Stop: " << mModuleName << endl;
	if (z != NULL)
		delete [] z;
	return 1;
}

int MiddleEarStapedius::Unload(void)
{
	cout << "    Unload: " << mModuleName << endl;
	return 1;
}

void MiddleEarStapedius::SetModuleName(char *ModuleName)
{
	if (mModuleName != NULL)
		delete [] mModuleName;

	mModuleName = new char[strlen(ModuleName) + 1];
	strcpy(mModuleName, ModuleName);
}

void MiddleEarStapedius::SetLogger(Logger *TheLogger)
{
	if (mLogger != NULL)
		delete mLogger;
	mLogger = TheLogger;
}

// GetBilinear() calls the MATLAB function bilinear() to transform
//  analog filter coefficients to the digital domain.
typedef void (*BilinearInit)(void);
typedef mxArray *(*BilinearFilter)(mxArray **, mxArray *, mxArray*, mxArray *);
typedef void (*BilinearTerm)(void);

void MiddleEarStapedius::MatBilFun(double *bd, double *ad, double *b, double *a, double sampleRate)
{
	BilinearInit BilIni;
	BilinearTerm BilTer;
	BilinearFilter BilFun;

	HMODULE hLib;

	mxArray *mxBD, *mxAD, *mxB, *mxA, *mxSampleRate;

	mxBD = mxCreateDoubleMatrix(FilterSize, 1, (mxComplexity)0);	// create a blank matlab array for BD
	mxAD = mxCreateDoubleMatrix(FilterSize, 1, (mxComplexity)0);	// create a blank matlab array for AD
	mxB = mxCreateDoubleMatrix(FilterSize, 1, (mxComplexity)0);
	memcpy((void *)mxGetPr(mxB),(void *)b,FilterSize*sizeof(double));
	mxA = mxCreateDoubleMatrix(FilterSize, 1, (mxComplexity)0);
	memcpy((void *)mxGetPr(mxA),(void *)a,FilterSize*sizeof(double));
	mxSampleRate = mxCreateDoubleScalar(sampleRate);

	hLib = LoadLibrary(".\\bilfunLibrary.dll");
	BilIni = (BilinearInit)GetProcAddress(hLib, "bilfunLibraryInitialize");
	BilFun = (BilinearFilter)GetProcAddress(hLib, "mlfBilfun");
	BilTer = (BilinearTerm)GetProcAddress(hLib, "bilfunLibraryTerminate");

	BilIni();
	mxBD = BilFun(&mxAD, mxB, mxA, mxSampleRate);
	//BilTer();

	memcpy((void *)bd,(void *)mxGetPr(mxBD),FilterSize*sizeof(double));
	memcpy((void *)ad,(void *)mxGetPr(mxAD),FilterSize*sizeof(double));

	mxDestroyArray(mxBD);
	mxDestroyArray(mxAD);
	mxDestroyArray(mxB);
	mxDestroyArray(mxA);
	mxDestroyArray(mxSampleRate);
}

int MiddleEarStapedius::GetBilinear(float SampleRate) 
{
	/* MATLAB floats must be double precision, use these temporarily */
	double A[MidearMaxFilterSize], B[MidearMaxFilterSize];
	int i;

	cout << mModuleName << ": Bilinear conversion starting" << endl;
	cout << "  A =";
	for (i = 0; i < FilterSize; i++)
		cout << " " << a[i];
	cout << endl;
	cout << "  B =";
	for (i = 0; i < FilterSize; i++)
		cout << " " << b[i];
	cout << endl;

	for (i=0; i<FilterSize; i++) A[i] = (double)a[i];  // Temporary
	for (i=0; i<FilterSize; i++) B[i] = (double)b[i];  // Temporary

	MatBilFun(num, den, B, A, SampleRate);

	cout << mModuleName << ": Bilinear conversion ending" << endl;
	cout << "  Den =";
	for (i=0; i<FilterSize; i++)
		cout << " " << den[i];
	cout << endl;
	cout << "  Num =";
	for (i=0; i<FilterSize; i++)
		cout << " " << num[i];
	cout << endl;
	return 0;
}
