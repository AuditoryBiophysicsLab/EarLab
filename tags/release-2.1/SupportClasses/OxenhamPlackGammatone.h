#ifndef __OXENHAM_PLACK_GAMMATONE_H_INCLUDED
#define __OXENHAM_PLACK_GAMMATONE_H_INCLUDED

#include "Complex.h"

class OxenhamPlackGammatone
{
public:
	OxenhamPlackGammatone();
	void LinearInit(double BestFrequency_Hz, double SampleRate_Hz);
	void NonlinearInit(double BestFrequency_Hz, double SampleRate_Hz);
	double Filter(double Sample);
private:
	void GenericInit(double SampleRate_Hz);
	double mDeltaT;
	double mCenterFrequency_Hz;
	double mBestFrequency_Hz;
	double mBandwidth_Hz;
	double theta, phi, alpha, b1, b2, a0, a1;
	double OldSamp, OldResult, OlderResult;
};
#endif
