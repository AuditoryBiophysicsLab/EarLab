#ifndef __AGC_ADV_BIONICS_H_INCLUDED
#define __AGC_ADV_BIONICS_H_INCLUDED

#include "SecondOrderButterworthLowpass.h"


class AGCAdvBionics
{
public:
	AGCAdvBionics();
	AGCAdvBionics(double EnvelopeCornerFrequency_Hz, double SampleRate_Hz, double AttackTau_sec, double ReleaseTau_sec, double Compression_dBRatio);
	void Init(double EnvelopeCornerFrequency_Hz, double SampleRate_Hz, double AttackTau_sec, double ReleaseTau_sec, double Compression_dBRatio);
	//void LinearInit(double CornerFrequency_Hz, double SampleRate_Hz);
	//void NonlinearInit(double CornerFrequency_Hz, double SampleRate_Hz);
	double Filter(double Sample);
private:

	//double LP, C, D, E;
	//double OldSample, OlderSample;
	//double OldFilterValue, OlderFilterValue;

	// from agc.m derived from Advanced Bionics AGC patent
	SecondOrderButterworthLowpass mEnvelopeLPFilter;
	double mEnvelope; // output of the envelope filter

	// Ballistic filter coefficients
	double mLamda1, mA1, mB1; // attack filter
	double mLamda2, mA2, mB2; // release filter
	// Ballistic filter state variable C and the resulting gain
	// (same state variable used for attack and release filters)
	double mC, mC_old, mGain;

	double mC_0;	// AGC threshold
	double mM;		// AGC compression ratio (ratio of input to output dynamic range)
					// the value of 10 is taken from the patent

	double mAGCOutputSample;

};
#endif
