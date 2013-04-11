#include "HHChannels.h"
#include <math.h>

HHChannelPopulation::HHChannelPopulation()
{
	SetTemperature_DegreesC(38.0);	// Default value
	alpha = 0.0;
	beta = 0.0;
}

HHChannelPopulation::HHChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC)
{
	SetTemperature_DegreesC(Temperature_DegreesC);
}

void HHChannelPopulation::SetTemperature_DegreesC(double Temperature_DegreesC)
{
	double tempTemperature;

	this->Temperature_DegreesC = Temperature_DegreesC;

	tempTemperature = (Temperature_DegreesC - 22.0) / 10.0;
	T2   = pow(2.0,   tempTemperature);
	T2_5 = pow(2.5,   tempTemperature);
	T3   = pow(3.0,   tempTemperature);
	T10  = pow(10.0,  tempTemperature);
}

void HHChannelPopulation::CalculateSteadyState(void)
{
	// Calculate steady-state rate
	if ((alpha + beta) == 0)
		InitialY = 1;
	else
		InitialY = alpha / (alpha + beta);	
}	

double HHChannelPopulation::GetDerivative(double IntermediateY, double InstantaneousVoltage_V)
{
	double dydt;

	this->IntermediateY = IntermediateY;

	CalculateRates(InstantaneousVoltage_V);
	
	dydt = ((1.0 - IntermediateY) * alpha) - (IntermediateY * beta);
	
	return dydt;
}

HH_Kslow::HH_Kslow(double InitialVoltage_V, double Temperature_DegreesC) : 
					 HHChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	/* SLOW POTASSIUM ACTIVATION VALUES (w) */
	// Was: 20e-9 when we started fiddling
	MaxConductance_Siemens = 20e-9 * TemperatureFactor_2_5();	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2581]
	ReversalPotential_V = -77.0e-3;	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2564]
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

#define SLOW_K_RATE_CONSTANT 1000
#define FAST_K_RATE_CONSTANT 1000

void HH_Kslow::CalculateRates(double Voltage_V)
{
	static double w1 = .107;  
	static double w2 = 33.0;  
	static double w3 = 13.1;  
	static double w4 = .01881;
	static double w5 = 30.0;  
	static double w6 = 30.3;     

	alpha = SLOW_K_RATE_CONSTANT * ((w1 * TemperatureFactor_3()) / (1. + exp(-((Voltage_V * 1000) + w2) / w3)));	// Activation rate
	beta =  SLOW_K_RATE_CONSTANT * (w4 * TemperatureFactor_3() * exp(-((Voltage_V * 1000) + w5) / w6));				// Deactivation rate
}

HH_Kfast::HH_Kfast(double InitialVoltage_V, double Temperature_DegreesC) : 
				     HHChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	/* FAST POTASSIUM ACTIVATION VALUES (w) */
	MaxConductance_Siemens = 40e-9 * TemperatureFactor_2_5();	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2581]
	ReversalPotential_V = -77.0e-3;	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2564]
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void HH_Kfast::CalculateRates(double Voltage_V)
{
	/* Fast potassium activation channel */
	static double n1 = .0282; 
	static double n2 = 9.;    
	static double n3 = 12.;   
	static double n4 = 6.;
	static double n5 = 144;   
	static double n6 = 30.;   
	static double n8 = 6.;    
	static double n9 = 62.;

	if ((Voltage_V > (-n2 - 1e-7)) && (Voltage_V < (-n2 + 1e-7)) ||
		(Voltage_V > (-n5 - 1e-7)) && (Voltage_V < (-n5 + 1e-7)) ||
		(Voltage_V > (-n9 - 1e-7)) && (Voltage_V < (-n9 + 1e-7)))
		Voltage_V += 2e-7;

	alpha = FAST_K_RATE_CONSTANT * (( n1 * TemperatureFactor_3() * ((Voltage_V * 1000) + n2) ) / (1. - exp( -((Voltage_V * 1000) + n2)/n3)));
	beta =  FAST_K_RATE_CONSTANT * ((n4 * TemperatureFactor_3() * exp(-((Voltage_V * 1000) + n5)/n6)) + 
					((TemperatureFactor_3() * n8) / (1. + exp((Voltage_V * 1000) + n9)))); 
}

HH_NaActivation::HH_NaActivation(double InitialVoltage_V, double Temperature_DegreesC) : 
							HHChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	MaxConductance_Siemens = 325e-9 * TemperatureFactor_2();	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2581]
	ReversalPotential_V = 55.0e-3;	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2564]
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void HH_NaActivation::CalculateRates(double Voltage_V)
{
	/* Fast sodium activation channel */  
	static double m1 = .36;   
	static double m2 = 49.;   
	static double m3 = 3.;    
	static double m4 = -0.4;
	static double m5 = 58.;   
	static double m6 = 20.;   

	if ((Voltage_V > (-m2 - 1e-7)) && (Voltage_V < (-m2 + 1e-7)) ||
	    (Voltage_V > (-m5 - 1e-7)) && (Voltage_V < (-m5 + 1e-7)))
		Voltage_V += 2e-7;

	alpha = 1000 * ((m1 * TemperatureFactor_3() * ((Voltage_V * 1000) + m2)) / (1. - exp(-((Voltage_V * 1000) + m2)/m3)));
	beta =  1000 * ((m4 * TemperatureFactor_3() * ((Voltage_V * 1000) + m5)) / (1. - exp(((Voltage_V * 1000) + m5)/m6))); 
}

HH_NaInactivation::HH_NaInactivation(double InitialVoltage_V, double Temperature_DegreesC) : 
							  HHChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	MaxConductance_Siemens = 325e-9 * TemperatureFactor_2();	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2581]
	ReversalPotential_V = 55.0e-3;	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2564]
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void HH_NaInactivation::CalculateRates(double Voltage_V)
{
	/* Fast sodium inactivation channel */
	static double h1 = 2.4;   
	static double h2 = 68.;   
	static double h3 = 3.;    
	static double h4 = 0.8;
	static double h5 = 61.3;  
	static double h8 = 3.6;  
	static double h9 = 21.; 
	static double h10 = 10.;

	/* FAST SODIUM INACTIVATION VALUES (h) */
	alpha = 1000 * ((h1 * TemperatureFactor_3() / (1. + exp(((Voltage_V * 1000) + h2) / h3))) + 
		((h4 * TemperatureFactor_10()) / (1. + exp((Voltage_V * 1000) + h5))));
	beta =  1000 * ((h8 * TemperatureFactor_3()) / (1. + exp(-((Voltage_V * 1000) + h9) / h10)));
}

HH_Excitatory::HH_Excitatory()
{
	MaxConductance_Siemens = 1000e-9;
	ReversalPotential_V = -10.0e-3;	/* Excitatory synapse reversal potential (mV) */
	CalculateRates(0.0);
	CalculateSteadyState();
}

void HH_Excitatory::CalculateRates(double Voltage_V)
{
	alpha = 0.0;
	beta = 0.0;
}

HH_Inhibitory::HH_Inhibitory()
{
	MaxConductance_Siemens = 1000e-9;
	ReversalPotential_V = -66.5e-3;	/* Inhibitory synapse reversal potential (mV) */
	CalculateRates(0.0);
	CalculateSteadyState();
}

void HH_Inhibitory::CalculateRates(double Voltage_V)
{
	alpha = 0.0;
	beta = 0.0;
}

HH_Leakage::HH_Leakage(double Temperature_DegreesC) : 
							  HHChannelPopulation(0.0, Temperature_DegreesC)
{
	MaxConductance_Siemens = 1.7e-9 * TemperatureFactor_2();
	ReversalPotential_V = 2.8e-3;		/* leakage reversal potential (milliVolts) */
	CalculateRates(0.0);
	CalculateSteadyState();
}

void HH_Leakage::CalculateRates(double Voltage_V)
{
	alpha = 0.0;
	beta =  0.0;
}
