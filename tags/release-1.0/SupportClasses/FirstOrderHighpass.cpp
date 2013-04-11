#include "Earlab.h"
#include "FirstOrderHighpass.h"
#include "EarlabException.h"
#include "memory.h"
#include "math.h"
#include <iostream>

using namespace std;

FirstOrderHighpass::FirstOrderHighpass()
{
}

void FirstOrderHighpass::SetSampleRate_Hz(double SampleRate_Hz)
{
	mLowpass.SetSampleRate_Hz(SampleRate_Hz);
}

void FirstOrderHighpass::SetCornerFrequency_Hz(double CornerFrequency_Hz)
{
	mLowpass.SetCornerFrequency_Hz(CornerFrequency_Hz);
}

void FirstOrderHighpass::SetTimeConstant_mS(double TimeConstant_mS)
{
	mLowpass.SetTimeConstant_mS(TimeConstant_mS);
}

double FirstOrderHighpass::Filter(double Xn)
{
	return Xn - mLowpass.Filter(Xn);
}
