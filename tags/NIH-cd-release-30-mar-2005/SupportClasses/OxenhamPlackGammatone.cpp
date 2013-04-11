#include "OxenhamPlackGammatone.h"
#include "Earlab.h"
#include <math.h>
#include <memory.h>

OxenhamPlackGammatone::OxenhamPlackGammatone()
{
	memset(this, 0, sizeof(OxenhamPlackGammatone));
}

void OxenhamPlackGammatone::LinearInit(double BestFrequency_Hz, double SampleRate_Hz)
{
	//                                       CFlin_p0    CFlin_m   
	mCenterFrequency_Hz = pow(10.0, (double)(-0.06762 + (1.01679 * log10(BestFrequency_Hz))));
	//                                 BWlin_p0   BWlin_m   
	mBandwidth_Hz = pow(10.0, (double)(0.03728 + (0.78563 * log10(BestFrequency_Hz))));
	GenericInit(SampleRate_Hz);
}

void OxenhamPlackGammatone::NonlinearInit(double BestFrequency_Hz, double SampleRate_Hz)
{
	//                                       CFnl_p0     CFnl_m   
	mCenterFrequency_Hz = pow(10.0, (double)(-0.05252 + (1.01650 * log10(BestFrequency_Hz))));
	//                                  BWnl_p0    BWnl_m   
	mBandwidth_Hz = pow(10.0, (double)(-0.03193 + (0.77426 * log10(BestFrequency_Hz))));
	GenericInit(SampleRate_Hz);
}

void OxenhamPlackGammatone::GenericInit(double SampleRate_Hz)
{
	DoubleComplex t1, t2, t3, t4, t5;
	mDeltaT = 1.0 / SampleRate_Hz;
    theta = 2 * PI * mCenterFrequency_Hz * mDeltaT;
    phi = 2 * PI * mBandwidth_Hz * mDeltaT;
    alpha = -1.0 * exp(-phi) * cos(theta);
    b1 = 2 * alpha;
    b2 = exp(-2.0 * phi);

	t1.Real = 1 + b1 * cos(theta);
	t1.Imag = -1.0 * b1 * sin(theta);
	t2.Real = b2 * cos(2 * theta);
	t2.Imag = -1.0 * b2 * sin(2 * theta);
	t3.Real = 1 + alpha * cos(theta);
	t3.Imag = -1.0 * alpha * sin(theta);
	t4 = t1 + t2;
	t5 = t4 / t3;
	a0 = t5.Magnitude();

    a1 = alpha * a0;
}

double OxenhamPlackGammatone::Filter(double Sample)
{
	double result;
	result = a0 * Sample + a1 * OldSamp - b1 * OldResult - b2 * OlderResult; 
	OldSamp = Sample;
	OlderResult = OldResult;
	OldResult = result;
	return result;
}
