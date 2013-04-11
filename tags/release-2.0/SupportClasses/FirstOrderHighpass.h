#ifndef __FIRST_ORDER_HIGHPASS_H_INCLUDED
#define __FIRST_ORDER_HIGHPASS_H_INCLUDED

#include "FirstOrderLowpass.h"

class FirstOrderHighpass
{
public:
	FirstOrderHighpass();
	void SetSampleRate_Hz(double SampleRate_Hz);
	void SetCornerFrequency_Hz(double CornerFrequency_Hz);
	void SetTimeConstant_mS(double TimeConstant_mS);
	double Filter(double Sample);
private:
	FirstOrderLowpass mLowpass;
};

#endif
