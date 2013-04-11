#include "Earlab.h"
#include "FirstOrderLowpass.h"
#include "EarlabException.h"
#include "memory.h"
#include "math.h"

FirstOrderLowpass::FirstOrderLowpass()
{
	memset(this, 0, sizeof(FirstOrderLowpass));
}

void FirstOrderLowpass::SetSampleRate_Hz(double SampleRate_Hz)
{
	mSampleRate_Hz = SampleRate_Hz;
	mDeltaT = 1.0 / mSampleRate_Hz;
}

void FirstOrderLowpass::SetCornerFrequency_Hz(double CornerFrequency_Hz)
{
	double Omega;

	if ((mSampleRate_Hz / 2) < CornerFrequency_Hz)
		throw EarlabException("FirstOrderLowpass: Nyquist rate exceeded.  Max CF = %g", mSampleRate_Hz / 2);

	Omega = 2.0 * PI * CornerFrequency_Hz;
	mTau = 1.0 / Omega;
	A = 1.0 / (((mTau * 2.0) / mDeltaT) + 1.0);
	B = ((mTau * 2.0) / mDeltaT) - 1.0;
}

void FirstOrderLowpass::SetTimeConstant_mS(double TimeConstant_mS)
{
	if (mSampleRate_Hz == 0.0)
		throw EarlabException("FirstOrderLowpass: Must set sample rate before setting time constant");

	mTau = TimeConstant_mS / 1000;
	A = 1 / (((mTau * 2.0) / mDeltaT) + 1);
	B = ((mTau * 2.0) / mDeltaT) - 1;
}

double FirstOrderLowpass::Filter(double Xn)
{
	double Yn;

	Yn = A * (Xn + Xnm1 + B * Ynm1);

	Xnm1 = Xn;
	Ynm1 = Yn;
	return Yn;
}
