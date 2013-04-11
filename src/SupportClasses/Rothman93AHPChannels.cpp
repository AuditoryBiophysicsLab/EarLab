//#include "HHChannelPopulation.h"
#include "Rothman93AHPChannels.h"
#include "Rothman93Channels.h"
#include <math.h>

Rothman93AHP::Rothman93AHP(double InitialVoltage_V, double Temperature_DegreesC) : 
				     HHChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	/* FAST POTASSIUM ACTIVATION VALUES (w) */
	// MaxConductance_Siemens is not used; mAHPIncrement_Siemens used to increment conductance
	//MaxConductance_Siemens = 40e-9 * TemperatureFactor_2_5();	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2581]
	MaxConductance_Siemens = 10000e-9;	// Not used, but specify large value just in case
	//ReversalPotential_V = -77.0e-3;	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2564]
	ReversalPotential_V = KReversalPotential93_V; // KReversalPotential_V set in Rothman93Channels.h
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void Rothman93AHP::CalculateRates(double Voltage_V)
{
	// At present the rates alpha and beta are not used,
	// so set them to zero.
	// The steady state InitialY should default to 1 in HHChannelPopulation::CalculateSteadyState(void).
	alpha = 0;
	beta =  0;
}

// For now use the same Rothman93ConductanceParameters *Rothman93ModelParameters
// as in Rothman93Channels.cpp.
