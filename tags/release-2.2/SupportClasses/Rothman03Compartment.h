#ifndef __ROTHMAN_03_COMPARTMENT_H_INCLUDED
#define __ROTHMAN_03_COMPARTMENT_H_INCLUDED

#include "Earlab.h"
#include "EarlabException.h"
#include "HHChannelPopulation.h"
#include "Rothman03Channels.h"
#include "Rothman93Compartment.h"
#include "HHSynapse.h"
#include "RungeKuttaEquation.h"
#include "RungeKuttaSolver.h"
#include "HHCompartment.h"

class Rothman03Compartment : public Rothman93Compartment
{
public:
	Rothman03Compartment() {};
	Rothman03Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC);
	void Initialize(void);
	double GetDerivative(double IntermediateY, double InstantaneousVoltage_V);
	void Settle(void);
protected:
	// Input conductances
	Rothman93Excitatory Excitatory;
	Rothman93Inhibitory Inhibitory;

	// Leakage conductance
	Rothman03Leakage *Leakage;
};

#endif
