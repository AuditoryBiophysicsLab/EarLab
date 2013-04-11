#include "LopezPovedaMeddisDualResonanceNonlinear.h"
#include <memory.h>
#include <iostream>

using namespace std;

LopezPovedaMeddisDualResonanceNonlinear::LopezPovedaMeddisDualResonanceNonlinear()
{
	mInited = false;
}

void LopezPovedaMeddisDualResonanceNonlinear::Init(double CenterFrequency_Hz, double SampleRate_Hz)
{
	int i;

	if (mInited)
	{
		cout << "  DRNLInit: Already inited" << endl;
		return;
	}

	// cout << "  DRNLInit(CF: " << CenterFrequency_Hz << ", Fs: " << SampleRate_Hz << ")" << endl;
	// Initialize linear filter cascade
	LinearGain.Init(CenterFrequency_Hz);
	for (i = 0; i < 2; i++)
		LinearGammatone[i].LinearInit(CenterFrequency_Hz, SampleRate_Hz);
	for (i = 0; i < 4; i++)
		LinearLPF[i].LinearInit(CenterFrequency_Hz, SampleRate_Hz);

	// Initialize nonlinear filter cascade
	for (i = 0; i < 3; i++)
		NonlinearGammatone_1[i].NonlinearInit(CenterFrequency_Hz, SampleRate_Hz);
	NonlinearGain.Init(CenterFrequency_Hz);
	for (i = 0; i < 3; i++)
		NonlinearGammatone_2[i].NonlinearInit(CenterFrequency_Hz, SampleRate_Hz);
	for (i = 0; i < 4; i++)
		NonlinearLPF[i].NonlinearInit(CenterFrequency_Hz, SampleRate_Hz);
}

double LopezPovedaMeddisDualResonanceNonlinear::LinearCascade(double Sample)
{
	double LinearCascadeResult;
	int i;

	LinearCascadeResult = LinearGain.Filter(Sample);
	for (i = 0; i < 2; i++)
		LinearCascadeResult = LinearGammatone[i].Filter(LinearCascadeResult);
	for (i = 0; i < 4; i++)
		LinearCascadeResult = LinearLPF[i].Filter(LinearCascadeResult);

	return LinearCascadeResult;
}

double LopezPovedaMeddisDualResonanceNonlinear::NonlinearCascade(double Sample)
{
	double NonlinearCascadeResult;
	int i;

	NonlinearCascadeResult = NonlinearGammatone_1[0].Filter(Sample);
	for (i = 1; i < 3; i++)
		NonlinearCascadeResult = NonlinearGammatone_1[i].Filter(NonlinearCascadeResult);
	NonlinearCascadeResult = NonlinearGain.Filter(NonlinearCascadeResult);
	for (i = 0; i < 3; i++)
		NonlinearCascadeResult = NonlinearGammatone_2[i].Filter(NonlinearCascadeResult);
	for (i = 0; i < 3; i++)
		NonlinearCascadeResult = NonlinearLPF[i].Filter(NonlinearCascadeResult);

	return NonlinearCascadeResult;
}

double LopezPovedaMeddisDualResonanceNonlinear::Filter(double Sample, bool DisableNonlinearPath)
{
	double LinearCascadeResult, NonlinearCascadeResult;

	LinearCascadeResult = LinearCascade(Sample);
	NonlinearCascadeResult = NonlinearCascade(Sample);

	if (!DisableNonlinearPath)
		return LinearCascadeResult + NonlinearCascadeResult;
	else
		return LinearCascadeResult;
}

double LopezPovedaMeddisDualResonanceNonlinear::Filter(double Sample)
{
	double LinearCascadeResult, NonlinearCascadeResult;

	LinearCascadeResult = LinearCascade(Sample);
	NonlinearCascadeResult = NonlinearCascade(Sample);

	return LinearCascadeResult + NonlinearCascadeResult;
}
