#ifndef _HH_CHANNEL_POPULATION_H_INCLUDED
#define _HH_CHANNEL_POPULATION_H_INCLUDED
#include "RungeKuttaEquation.h"
#include "HHCompartment.h"

class HHChannelPopulation : public RungeKuttaEquation
{
public:
	HHChannelPopulation();
	HHChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V) = 0;
	virtual double GetCurrent_Amperes(void);
	void CalculateSteadyState(void);
	void SetCompartment(HHCompartment *theCompartment) {myCompartment = theCompartment;}

	double MaxConductance_Siemens;		// Set up by the derived class constructor
	double ReversalPotential_V;			// Set up by the derived class constructor
	double alpha, beta;					// Activation and inactivation rates
	double activation;					// Current conductance activation value (0..1)

	virtual double GetDerivative(double IntermediateY, double InstantaneousVoltage_V);

	void SetTemperature_DegreesC(double Temperature_DegreesC);
	inline double TemperatureFactor_2(void)   {return T2;}
	inline double TemperatureFactor_2_5(void) {return T2_5;}
	inline double TemperatureFactor_3(void)   {return T3;}
	inline double TemperatureFactor_10(void)  {return T10;}

protected:
	double Temperature_DegreesC;
	double T2, T2_5, T3, T10;
	HHCompartment *myCompartment;
};
#endif // _HH_CHANNEL_POPULATION_H_INCLUDED
