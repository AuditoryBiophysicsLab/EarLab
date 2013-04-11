#ifndef __LOPEZ_POVEDA_MEDDIS_DUAL_RESONANCE_NONLINEAR_H_INCLUDED
#define __LOPEZ_POVEDA_MEDDIS_DUAL_RESONANCE_NONLINEAR_H_INCLUDED

#include "OxenhamPlackLinearGain.h"
#include "OxenhamPlackNonlinearGain.h"
#include "OxenhamPlackGammatone.h"
#include "SecondOrderButterworthLowpass.h"

class LopezPovedaMeddisDualResonanceNonlinear
{
public:
	LopezPovedaMeddisDualResonanceNonlinear();
	void Init(double CenterFrequency_Hz, double SampleRate_Hz);
	double LinearCascade(double Sample);
	double NonlinearCascade(double Sample);
	double Filter(double Sample);
	double Filter(double Sample, bool DisableNonlinearPath);
private:
	bool mInited;

	// Linear filter cascade
	OxenhamPlackLinearGain LinearGain;
	OxenhamPlackGammatone LinearGammatone[2];
	SecondOrderButterworthLowpass LinearLPF[4];

	// Nonlinear filter cascade
	OxenhamPlackGammatone NonlinearGammatone_1[3];
	OxenhamPlackNonlinearGain NonlinearGain;
	OxenhamPlackGammatone NonlinearGammatone_2[3];
	SecondOrderButterworthLowpass NonlinearLPF[4];
};

#endif
