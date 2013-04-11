#ifndef _ROTHMAN93_COMPARTMENT_H_INCLUDED
#define _ROTHMAN93_COMPARTMENT_H_INCLUDED
#include "Earlab.h"
#include "EarlabException.h"
#include "Rothman93Channels.h"
#include "HHCompartment.h"

#define	NUM_R93_CONDUCTANCES	5

class Rothman93Compartment : public HHCompartment
{
public:
	Rothman93Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC);
	Rothman93Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType);
	Rothman93Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, int AdditionalConductances);
	Rothman93Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType, int AdditionalConductances);

	virtual double GetDerivative(double Voltage_V, double InstantaneousVoltage_V);
	//virtual void GetConductances_nS(float &ge, float &gi, float &gNa, float &gKfast, float &gKslow);
	virtual void Settle(void);
	virtual void CalculateConductances(void);

	virtual void Initialize(void);

protected:
	// Input conductances
	Rothman93Excitatory Excitatory;
	Rothman93Inhibitory Inhibitory;

	// Leakage conductance
	Rothman93Leakage *Leakage;

	Enumed93ResponseType ResponseType;
};

#endif
