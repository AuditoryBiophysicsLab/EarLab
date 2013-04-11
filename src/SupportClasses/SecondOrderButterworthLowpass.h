#ifndef __SECOND_ORDER_BUTTERWORTH_LOWPASS_H_INCLUDED
#define __SECOND_ORDER_BUTTERWORTH_LOWPASS_H_INCLUDED

class SecondOrderButterworthLowpass
{
public:
	SecondOrderButterworthLowpass();
	void LinearInit(double CornerFrequency_Hz, double SampleRate_Hz);
	void NonlinearInit(double CornerFrequency_Hz, double SampleRate_Hz);
	double Filter(double Sample);
private:
	void Init(double SampleRate_Hz);

	double LP, C, D, E;
	double OldSample, OlderSample;
	double OldFilterValue, OlderFilterValue;
};
#endif
