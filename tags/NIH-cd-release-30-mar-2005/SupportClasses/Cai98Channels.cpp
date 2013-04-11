#include "Cai98Channels.h"
#include "Rothman93Channels.h"
#include <math.h>

Cai98AHP::Cai98AHP(double InitialVoltage_V, double Temperature_DegreesC) : 
				     HHChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	/* FAST POTASSIUM ACTIVATION VALUES (w) */
	MaxConductance_Siemens = 40e-9 * TemperatureFactor_2_5();	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2581]
	ReversalPotential_V = -77.0e-3;	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2564]
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void Cai98AHP::CalculateRates(double Voltage_V)
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

	alpha = ROTHMAN_93_RATE_CONSTANT * (( n1 * TemperatureFactor_3() * ((Voltage_V * 1000) + n2) ) / (1. - exp( -((Voltage_V * 1000) + n2)/n3)));
	beta =  ROTHMAN_93_RATE_CONSTANT * ((n4 * TemperatureFactor_3() * exp(-((Voltage_V * 1000) + n5)/n6)) + 
					(BRUGHERA_96_RATE_CONSTANT * (TemperatureFactor_3() * n8) / (1. + exp((Voltage_V * 1000) + n9)))); 
}
