#include "Brughera96Channels.h"
#include "Rothman93Channels.h"
#include <math.h>

Brughera96Kfast::Brughera96Kfast(double InitialVoltage_V, double Temperature_DegreesC) : 
				     HHChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	/* FAST POTASSIUM ACTIVATION VALUES (w) */
	MaxConductance_Siemens = 40e-9 * TemperatureFactor_2_5();	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2581]
	ReversalPotential_V = -77.0e-3;	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2564]
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

double Brughera96Kfast::GetCurrent_Amperes(void)
{
	return MaxConductance_Siemens * IntermediateY * (myCompartment->GetVoltage_V() - ReversalPotential_V);
}


void Brughera96Kfast::CalculateRates(double Voltage_V)
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

   	/* FAST POTASSIUM ACTIVATION VALUES (n) */
	alpha = ROTHMAN_93_RATE_CONSTANT * (( n1 * TemperatureFactor_3() * ((Voltage_V * 1000) + n2) ) / (1. - exp( -((Voltage_V * 1000) + n2)/n3)));
	beta =  ROTHMAN_93_RATE_CONSTANT * ((n4 * TemperatureFactor_3() * exp(-((Voltage_V * 1000) + n5)/n6)) + 
					(BRUGHERA_96_RATE_CONSTANT * n8 / (1. + exp((Voltage_V * 1000) + n9)))); 
					
	// Note: Second half of 'beta' calculation originally read like this:
	// (BRUGHERA_96_RATE_CONSTANT * (TemperatureFactor_3() * n8) / (1. + exp((Voltage_V * 1000) + n9)))); 
	// We removed the TemperatureFactor_3() from the calculation because it does not appear in the original Bushy Cell code
	// from Hong Mei Cai.  The snippet of original code appears here:
	// alphan = 1000 * ( n[1] * TF3 * (y[5] + n[2]) ) / (1. - exp( -(y[5] + n[2])/n[3]) );
	// betan = 1000 * (n[4] * TF3 * exp(-(y[5] + n[5])/n[6]) ) + n[8] / (1. + exp(y[5] + n[9] )); 
	// dydx[2] = (1. - y[2]) * alphan - y[2] * betan; 
}

Brughera96NaInactivation::Brughera96NaInactivation(double InitialVoltage_V, double Temperature_DegreesC) : 
							  HHChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	MaxConductance_Siemens = 325e-9 * TemperatureFactor_2();	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2581]
	ReversalPotential_V = 55.0e-3;	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2564]
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

double Brughera96NaInactivation::GetCurrent_Amperes(void)
{
	return MaxConductance_Siemens * IntermediateY * (myCompartment->GetVoltage_V() - ReversalPotential_V);
}

void Brughera96NaInactivation::CalculateRates(double Voltage_V)
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
	alpha = ROTHMAN_93_RATE_CONSTANT * ((h1 * TemperatureFactor_3() / (1. + exp(((Voltage_V * 1000) + h2) / h3))) + 
		(BRUGHERA_96_RATE_CONSTANT * (h4 * TemperatureFactor_10()) / (1. + exp((Voltage_V * 1000) + h5))));
	beta =  ROTHMAN_93_RATE_CONSTANT * ((h8 * TemperatureFactor_3()) / (1. + exp(-((Voltage_V * 1000) + h9) / h10)));
}
