#include "Earlab.h"
#include "SecondOrderButterworthLowpass.h"
#define USE_MATH_DEFINES
#include <math.h>
#include <memory.h>

SecondOrderButterworthLowpass::SecondOrderButterworthLowpass()
{
	memset(this, 0, sizeof(SecondOrderButterworthLowpass));
}

void SecondOrderButterworthLowpass::LinearInit(double CornerFrequency_Hz, double SampleRate_Hz)
{
	LP =  pow(10.0, (-0.06762 + (1.01679 * log10(CornerFrequency_Hz))));
	Init(SampleRate_Hz);
}

void SecondOrderButterworthLowpass::NonlinearInit(double CornerFrequency_Hz, double SampleRate_Hz)
{
	LP =  pow(10.0, (-0.05252 + (1.01650 * log10(CornerFrequency_Hz))));
	Init(SampleRate_Hz);
}

double SecondOrderButterworthLowpass::Filter(double Sample)
{
	double FilterValue;

    FilterValue = (C * Sample) + (2 * C * OldSample) + (C * OlderSample) - 
		(D * OldFilterValue) - (E * OlderFilterValue);
	OlderSample = OldSample;
	OldSample = Sample;
	OlderFilterValue = OldFilterValue;
	OldFilterValue = FilterValue;
	return FilterValue;
}

void SecondOrderButterworthLowpass::Init(double SampleRate_Hz)
{
	double theta, dt, cot_theta, cot_squared_theta;

	dt = 1.0 / SampleRate_Hz;
	theta = PI * LP * dt;
	cot_theta = 1.0 / tan(theta);
	cot_squared_theta = cot_theta * cot_theta;
	C = 1 / (1.0 + (SQRT2 * cot_theta) + cot_squared_theta);
	D = 2 * C * (1.0 - cot_squared_theta);
	E = C * (1.0 - (SQRT2 * cot_theta) + cot_squared_theta);
}
