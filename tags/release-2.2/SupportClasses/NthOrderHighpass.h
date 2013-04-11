#ifndef __NTH_ORDER_HIGHPASS_H_INCLUDED
#define __NTH_ORDER_HIGHPASS_H_INCLUDED

#include "FirstOrderHighpass.h"

class NthOrderHighpass
{
public:
	NthOrderHighpass();
	void SetSampleRate_Hz(double SampleRate_Hz);
	void SetCornerFrequency_Hz(double CornerFrequency_Hz);
	void SetTimeConstant_mS(double TimeConstant_mS);
	void SetFilterOrder(int FilterOrder);
	double Filter(double Sample);
	void ResetFilterTo(double FilterValue);
private:
	int mFilterOrder;
	FirstOrderHighpass *mFilters;
};

#endif
