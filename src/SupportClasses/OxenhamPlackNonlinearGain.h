#ifndef __OXENHAM_PLACK_NONLINEAR_GAIN_H_INCLUDED
#define __OXENHAM_PLACK_NONLINEAR_GAIN_H_INCLUDED

class OxenhamPlackNonlinearGain
{
public:
	OxenhamPlackNonlinearGain();
	void Init(double BestFrequency_Hz);
	double Filter(double Sample);
private:
	double sign(double value);
	double a, b, c;
};

#endif
