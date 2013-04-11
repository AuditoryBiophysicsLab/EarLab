// AGCAdvBionics.cpp
// The purpose of this program is to duplicate the AGC circuit
// described in US patent 7,136,706 from Advanced Bionics, Inc.
// Andrew Brughera (brughera@bu.edu) 8 May 2007
//
// cpp program based on matlab script
// Title:    agc.m
// Author:   D.C. Mountain <dcm@bu.edu>
// Date:     25-Jan-2007

#include "Earlab.h"
#include "AGCAdvBionics.h"
#define USE_MATH_DEFINES
#include <math.h>
#include <memory.h>

AGCAdvBionics::AGCAdvBionics()
{
	memset(this, 0, sizeof(AGCAdvBionics));
}

//AGCAdvBionics(double EnvelopeCornerFrequency_Hz, double SampleRate_Hz, double AttackTau_sec, double ReleaseTau_sec, double Compression_dBRatio)
//{
//	Init(EnvelopeCornerFrequency_Hz, SampleRate_Hz, AttackTau_sec, ReleaseTau_sec, Compression_dBRatio);
//}

//void SecondOrderButterworthLowpass::LinearInit(double CornerFrequency_Hz, double SampleRate_Hz)
//{
//	//				 LP_lincutoff_P0  LP_lincutoff_m
//	LP =  pow(10.0, (-0.06762 + (1.01679 * log10(CornerFrequency_Hz))));
//	Init(SampleRate_Hz);
//}

//void SecondOrderButterworthLowpass::NonlinearInit(double CornerFrequency_Hz, double SampleRate_Hz)
//{
//	//				 LP_nlcutoff_P0  LP_nlcutoff_m
//	LP =  pow(10.0, (-0.05252 + (1.01650 * log10(CornerFrequency_Hz))));
//	Init(SampleRate_Hz);
//}

double AGCAdvBionics::Filter(double Sample)
{
	//double FilterValue;

//    FilterValue = (C * Sample) + (2 * C * OldSample) + (C * OlderSample) - 
//		(D * OldFilterValue) - (E * OlderFilterValue);
//	OlderSample = OldSample;
//	OldSample = Sample;
//	OlderFilterValue = OldFilterValue;
//	OldFilterValue = FilterValue;
//	return FilterValue;

	double AbsValueSample;

	if (Sample < 0)
		AbsValueSample = -Sample;
	else
		AbsValueSample = Sample;

	mEnvelope = mEnvelopeLPFilter.Filter(AbsValueSample);

	// compute the "ballistic" filter output and the AGC gain

	if (mEnvelope >= mC_old) // use the attack filter
        mC = mA1*mC_old + mB1*mEnvelope;
    else //if mEnvelope < mC_old use the decay filter
		mC = mA2*mC_old + mB2*mEnvelope;

	if (mC > mC_0) // nonlinear gain only applied if input is above the AGC threshold
        mGain = pow(mC/mC_0, 1/mM-1); // (mC/mC_0)^(1/mM -1)
    else
        mGain = 1;
	
	mAGCOutputSample = mGain * Sample;
	mC_old = mC;

	return mAGCOutputSample;

// compute the "ballistic" filter output and the AGC gain
//for k = 1:length(input)
//    if envelope(k) >= Cold
//        C(k)    = A1*Cold + B1*envelope(k);
//    elseif envelope(k) < Cold
//        C(k)    = A2*Cold + B2*envelope(k);
//    end
//    if C(k) > Co    % nonlinear gain only applied if input is above the AGC threshold
//        Gain(k) = (C(k)/Co)^(1/M -1);
//    else
//        Gain(k) = 1;
//    end
//    Cold    = C(k);
//end

}

void AGCAdvBionics::Init(double EnvelopeCornerFrequency_Hz, double SampleRate_Hz, double AttackTau_sec, double ReleaseTau_sec, double Compression_dBRatio)
{
	// initialize the Envelope filter
	mEnvelopeLPFilter.LinearInit(EnvelopeCornerFrequency_Hz, SampleRate_Hz);	
	//mEnvelopeLPFilter.NonlinearInit(EnvelopeCornerFrequency_Hz, SampleRate_Hz);	
	
	//double theta, dt, cot_theta, cot_squared_theta;

	mC_0 = 1.0;	// AGC threshold
	mM = Compression_dBRatio;	// AGC compression ratio (ratio of input to output dynamic range)


	mLamda1 = 1/(AttackTau_sec*SampleRate_Hz);
	mA1 = (1 - mLamda1);
	mB1 = mLamda1;
	mLamda2 = 1/(ReleaseTau_sec*SampleRate_Hz);
	mA2 = (1 - mLamda2);
	mB2 = mLamda2;

	// initiate state variable for "ballistic" filter. Note that the same state
	// variable is used for both the "attack" and "release" filters
	mC_old = 0;

	// Start the envelope at zero as well
	mEnvelope = 0;

	//dt = 1.0 / SampleRate_Hz;
	//theta = PI * LP * dt;
	//cot_theta = 1.0 / tan(theta);
	//cot_squared_theta = cot_theta * cot_theta;
	//C = 1 / (1.0 + (SQRT2 * cot_theta) + cot_squared_theta);
	//D = 2 * C * (1.0 - cot_squared_theta);
	//E = C * (1.0 - (SQRT2 * cot_theta) + cot_squared_theta);
}
