#ifndef _HH_CONDUCTANCE_H_INCLUDED
#define _HH_CONDUCTANCE_H_INCLUDED
#include "RungeKuttaEquation.h"

class HHChannelPopulation : public RungeKuttaEquation
{
public:
	HHChannelPopulation();
	HHChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V) = 0;
	void CalculateSteadyState(void);

	double MaxConductance_Siemens;		// Set up by the derived class constructor
	double ReversalPotential_V;		// Set up by the derived class constructor
	double alpha, beta;					// Activation and inactivation rates
	double activation;					// Current conductance activation value (0..1)

	virtual double GetDerivative(double IntermediateY, double InstantaneousVoltage_V);

	void SetTemperature_DegreesC(double Temperature_DegreesC);
	inline double TemperatureFactor_2(void)   {return T2;}
	inline double TemperatureFactor_2_5(void) {return T2_5;}
	inline double TemperatureFactor_3(void)   {return T3;}
	inline double TemperatureFactor_10(void)  {return T10;}

private:
	double Temperature_DegreesC;
	double T2, T2_5, T3, T10;
};

class HH_Kslow : public HHChannelPopulation
{
public:
	HH_Kslow(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

class HH_Kfast : public HHChannelPopulation
{
public:
	HH_Kfast(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

class HH_NaActivation : public HHChannelPopulation
{
public:
	HH_NaActivation(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

class HH_NaInactivation : public HHChannelPopulation
{
public:
	HH_NaInactivation(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

class HH_Excitatory : public HHChannelPopulation
{
public:
	HH_Excitatory();
	virtual void CalculateRates(double Voltage_V);
};

class HH_Inhibitory : public HHChannelPopulation
{
public:
	HH_Inhibitory();
	virtual void CalculateRates(double Voltage_V);
};

class HH_Leakage : public HHChannelPopulation
{
public:
	HH_Leakage(double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

#endif
