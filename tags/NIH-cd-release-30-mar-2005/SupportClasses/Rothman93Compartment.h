#ifndef _ROTHMAN93_COMPARTMENT_H_INCLUDED
#define _ROTHMAN93_COMPARTMENT_H_INCLUDED
#include "Earlab.h"
#include "EarlabException.h"
#include "HHChannelPopulation.h"
#include "Rothman93Channels.h"
#include "HHSynapse.h"
#include "RungeKuttaEquation.h"
#include "RungeKuttaSolver.h"
#include "HHCompartment.h"

class Rothman93Compartment : public HHCompartment
{
public:
	Rothman93Compartment() {};
	Rothman93Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC);

	virtual double GetDerivative(double Voltage_V, double InstantaneousVoltage_V);
	virtual void Settle(void);

	virtual void Initialize(void);

protected:
	// Input conductances
	Rothman93Excitatory Excitatory;
	Rothman93Inhibitory Inhibitory;

	// Leakage conductance
	Rothman93Leakage *Leakage;
};

#endif
