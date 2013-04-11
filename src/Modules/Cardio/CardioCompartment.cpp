#include "CardioCompartment.h"
#include "math.h"
#include <stdio.h>
#include <stdarg.h>

CardioCompartment::CardioCompartment(double SampleRate_Hz, double Vinitial, double C, double R1, double R2, bool IsVentricle, bool HasVbias)
{
	mDeltaT = 1.0 / SampleRate_Hz;
	mR1 = R1;
	mR2 = R2;

	mC = C;
	dCdt = 0;

	SetVc(Vinitial);

	mVbias = 0.0;
	mIsVentricle = IsVentricle;
	mHasVbias = HasVbias;
	mDiode1ReverseBiased = mDiode2ReverseBiased = false;
}

double CardioCompartment::GetDerivative(double V1, double V2, double Vc)
{
	double dVdt, BiasedVc;

	// All voltages are in Volts
	//     Resistances are in Ohms
	//     Currents are in Amperes
	// dCdt is the change in capacitance between the last time step and the current one.
	// dCdt is defined as follows: dCdt = NewCapacitance - OldCapacitance / (1 / SampleRate_Hz)
	mVc = Vc;
	BiasedVc = GetBiasedVc();
	mI1 = (V1 - BiasedVc) / mR1;		// Current through R1
	mI2 = (V2 - BiasedVc) / mR2;		// Current through R2
	if (mIsVentricle)
	{
		if (mI1 < 0)
		{
			mI1 = 0;
			mDiode1ReverseBiased = true;
		}
		else
			mDiode1ReverseBiased = false;

		if (mI2 > 0)
		{
			mI2 = 0;
			mDiode2ReverseBiased = true;
		}
		else
			mDiode2ReverseBiased = false;
	}

	dVdt = ((mI1 + mI2) / mC) - ((mVc / mC) * dCdt);

	return dVdt;
}
