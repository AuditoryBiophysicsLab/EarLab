#include "Cai98Channels.h"
//#include "Brughera96Channels.h"
//#include "Rothman93Channels.h"
#include <math.h>

Cai98AHP::Cai98AHP(double InitialVoltage_V, double Temperature_DegreesC) : 
				     Brughera96ChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	/* FAST POTASSIUM ACTIVATION VALUES (w) */
	//MaxConductance_Siemens = 40e-9 * TemperatureFactor_2_5();	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2581]
	//MaxConductance_Siemens = Rothman93ModelParameters->MaxG_KFast93 * TemperatureFactor_2_5();
	MaxConductance_Siemens = 1.0;  // The AHP conductance does not use MaxConductance_Siemens, so its value here is arbitrary
	ReversalPotential_V = -77.0e-3;	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2564]
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void Cai98AHP::CalculateRates(double Voltage_V)
{
	// Rates alpha and beta always zero for AHP.
	alpha = 0;
	beta = 0;
}
