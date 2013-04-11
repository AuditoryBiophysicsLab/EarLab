#ifndef __FIRST_ORDER_LOWPASS_H_INCLUDED
#define __FIRST_ORDER_LOWPASS_H_INCLUDED

class FirstOrderLowpass
{
public:
	FirstOrderLowpass();
	void SetSampleRate_Hz(double SampleRate_Hz);
	void SetCornerFrequency_Hz(double CornerFrequency_Hz);
	void SetTimeConstant_mS(double TimeConstant_mS);
	double Filter(double Sample);
	void ResetFilterTo(double FilterValue);
	void ResetFilterYTo(double FilterYValue);
	void ResetFilterXTo(double FilterXValue);
private:
	double mSampleRate_Hz;
	double A, B, Ynm1, Xnm1;
	double mDeltaT, mTau;
};

#endif
