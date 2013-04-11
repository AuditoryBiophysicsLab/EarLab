#ifndef __ROTHMAN_03_COMPARTMENT_H_INCLUDED
#define __ROTHMAN_03_COMPARTMENT_H_INCLUDED
#include "Earlab.h"
#include "EarlabException.h"
#include "Rothman03Channels.h"
#include "HHCompartment.h"

#define	NUM_R03_CONDUCTANCES	7

/*typedef struct
{
	double MembraneCapacitance_F;
	double MaxG_Na;
	double MaxG_HT;
	double MaxG_LT;
	double MaxG_A;
	double MaxG_h;
	double MaxG_lk;
} Rothman03CustomParameterValues;
*/

class Rothman03Compartment : public HHCompartment
{
public:
	Rothman03Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC);
	Rothman03Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType);
	Rothman03Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues);
	Rothman03Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, int AdditionalConductances);
	Rothman03Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, int AdditionalConductances);
	Rothman03Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, int AdditionalConductances, Rothman03CustomParameterValues MyCustomParameterValues);
	void Initialize(void);
	void Initialize(Rothman03CustomParameterValues MyCustomParameterValues);
	virtual double GetDerivative(double IntermediateY, double InstantaneousVoltage_V);
	//virtual void GetConductances_nS(float &ge, float &gi, float &gNa, float &gKA, float &gKHT, float &gKLT, float &gh);
	virtual void CalculateConductances(void);
	void Settle(void);
protected:
//	// Input conductances
//	Rothman93Excitatory Excitatory;
//	Rothman93Inhibitory Inhibitory;
	// Input conductances
	Rothman03Excitatory Excitatory;
	Rothman03Inhibitory Inhibitory;

	// Leakage conductance
	Rothman03Leakage *Leakage;

	//int ResponseType;
	EnumedResponseType ResponseType;

	Rothman03CustomParameterValues CustomParameterValues;
};

#endif
