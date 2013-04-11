#include "HHChannelPopulation.h"
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
	alpha = 0.0;
	beta = 0.0;
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
	T2_6655 = pow(2.6655, tempTemperature); // T2_6655 is used in Brughera96Compartment where Rothman93Compartment uses T3
	T5_6569 = pow(5.6569, tempTemperature); // T5_6569 is used in Brughera96Compartment where Rothman93Compartment uses T10
}

void HHChannelPopulation::CalculateSteadyState(void)
{
	// Calculate steady-state rate
	if ((alpha + beta) == 0)
		InitialY = 1;
	else
		InitialY = alpha / (alpha + beta);	
}	

double HHChannelPopulation::GetCurrent_Amperes(void)
{
	return 0;
}

double HHChannelPopulation::GetDerivative(double IntermediateY, double InstantaneousVoltage_V)
{
	double dydt;

	this->IntermediateY = IntermediateY;

	CalculateRates(InstantaneousVoltage_V);
	
	dydt = ((1.0 - IntermediateY) * alpha) - (IntermediateY * beta);
	
	return dydt;
}
