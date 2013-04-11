#include "Earlab.h"
#include "FirstOrderLowpass.h"
#include "EarlabException.h"
#include "memory.h"
#include "math.h"
#include <float.h>
#include "FPException.h"
#include <stdio.h>

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

	Xn += 0.0000000000000000000000001;
	Yn = A * (Xn + Xnm1 + B * Ynm1);
	//if (IS_ALMOST_DENORMAL(Yn))
	//      Yn = 0;

	Xnm1 = Xn;
	Ynm1 = Yn;
	return Yn;
}

void FirstOrderLowpass::ResetFilterTo(double FilterValue)
{
	Ynm1 = Xnm1 = FilterValue;
}	

void FirstOrderLowpass::ResetFilterYTo(double FilterYValue)
{
	Ynm1 = FilterYValue;
}

void FirstOrderLowpass::ResetFilterXTo(double FilterXValue)
{
	Xnm1 = FilterXValue;
}

#ifdef MSWIN
void DisplayFPStatusBits(char *tag)
{
	unsigned int status;

	status = _statusfp();
	if ((status != 0x00000000) && (status != 0x00000001))
	{
		if (tag != NULL)
			printf("%s ", tag);
		printf("FPU Status: %08.8X ( ", status);
		if (status & _SW_DENORMAL)
			printf("DENORMAL ");
		if (status & _SW_INVALID)
			printf("INVALID ");
		if (status & _SW_ZERODIVIDE)
			printf("DIV0 ");
		if (status & _SW_OVERFLOW)
			printf("OVERFLOW ");
		if (status & _SW_UNDERFLOW)
			printf("UNDERFLOW ");
		if (status & _SW_INEXACT)
			printf("INEXACT ");
		printf(")\n");
	}
}
#else
void DisplayFPStatusBits(char *tag)
{
	if (tag != NULL) printf("%s ", tag);
	printf("FPU Status: UNKNOWN\n");
}
#endif
