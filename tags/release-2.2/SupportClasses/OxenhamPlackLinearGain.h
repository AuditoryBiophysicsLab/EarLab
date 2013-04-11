#ifndef __OXENHAM_PLACK_LINEAR_GAIN_INCLUDED
#define __OXENHAM_PLACK_LINEAR_GAIN_INCLUDED

class OxenhamPlackLinearGain
{
public:
	OxenhamPlackLinearGain();
	void Init(double BestFrequency_Hz);
	double Filter(double InputSample);
private:
	double g;
	double BF_Hz;
};
#endif
