#ifndef _ROTHMAN93_COMPARTMENT_H_INCLUDED
#define _ROTHMAN93_COMPARTMENT_H_INCLUDED
#include "Earlab.h"
#include "EarlabException.h"
#include "CardioRungeKuttaEquation.h"
#include "CardioRungeKuttaSolver.h"

class CardioCompartment
{
public:
	CardioCompartment(double SampleRate_Hz, double Vinitial, double C, double R1, double R2, bool IsVentricle, bool HasVbias);

	double GetDerivative(double V1, double V2, double Vc);

	inline void SetElastance(double NewElastance) 
	{ 
		if (NewElastance > 0)
			SetCompliance(1.0 / NewElastance);
	}
	inline void SetCompliance(double NewCompliance) 
	{
		if (NewCompliance > 0)
		{
			dCdt = (NewCompliance - mC) / mDeltaT; 
			mC = NewCompliance; 
		}
		else
			dCdt = 0;
	}
	inline double	GetCompliance	(void)			{ return mC; }
	inline void		SetVbias		(double Vbias)	{ mVbias = Vbias; }
	inline double	GetVbias		(void)			{ return mVbias; }
	inline double	GetR1			(void)			{return mR1;}
	inline double	GetR2			(void)			{return mR2;}
	inline double	GetVc			(void)			{return mVc;}
	inline double	GetBiasedVc		(void)			{return mVc + mVbias;}
	inline void		SetVc			(double NewVc)	{mVc = NewVc;}
	inline double	GetI1			(void)			{return mI1;}
	inline double	GetI2			(void)			{return mI2;}
	inline bool		IsVentricle		(void)			{return mIsVentricle;}
	inline bool		HasVbias		(void)			{return mHasVbias;}
	inline bool		Valve1IsOpen	(void)			{return !mDiode1ReverseBiased;}
	inline bool		Valve2IsOpen	(void)			{return !mDiode2ReverseBiased;}

protected:
	double mC;			// Capacitance, in farads
	double mR1, mR2;	// Resistance, in ohms
	double mDeltaT;		// Length of one sample in seconds
	double mVbias;		// Bias voltage
	double dCdt;		// Change in capacitance over time
	double mVc;			// Voltage at the capacitor
	double mI1, mI2;	// Currents through R1 and R2, respectively
	bool mIsVentricle;	// true if the current compartment is a ventricle (has a variable capacitance, in other words)
	bool mHasVbias;		// true if the current compartment is subject to a variable bias voltage due to thoracic pressure
	bool mDiode1ReverseBiased;	// true if the diode is reverse-biased and has opened the circuit
	bool mDiode2ReverseBiased;	// true if the diode is reverse-biased and has opened the circuit
};
#endif
