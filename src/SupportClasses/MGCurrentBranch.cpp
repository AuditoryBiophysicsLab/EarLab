#include "MGCurrentBranch.h"

MGKChannelBranch::MGKChannelBranch(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds, double ReversalPotential_V)
{
	this->ReversalPotential_V = ReversalPotential_V;
	CalculateSteadyState(InitialVoltage_V, ThresholdVoltage_V, b_K, Tau_K_Seconds);
}

void MGKChannelBranch::CalculateSteadyState(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds)
{
	// Calculate steady-state rate
	SetS(InitialVoltage_V, ThresholdVoltage_V);
	InitialY = (b_K * S) / Tau_K_Seconds; // Typically on initialization S = 0, giving InitialY = 0.
	IntermediateY = InitialY;
}	

void MGKChannelBranch::SetS(double Voltage_V, double ThresholdVoltage_V)
{
	if (Voltage_V >= ThresholdVoltage_V)
		S = 1;
	else
		S = 0;
}

double MGKChannelBranch::GetDerivative(double IntermediateY, double InstantaneousVoltage_V)
{
	double dydt;
	//double Voltage_V, VoltageThreshold_V;
	//double VoltageThreshold_V;

	// Input IntermediateY from RungeKuttaSolver::rkck represents g_K. 
	// Update IntermediateY in the MGKChannelBranch object.
	this->IntermediateY = IntermediateY;
	
	// argument IntermediateY is the intermediate value of membrane voltage
	// used in RungeKuttaSolver.cpp.  In HH compartments where GetDerivative 
	// calculates ion channel activation, membrane voltage is needed as an
	// additional parameter.  Here it is redundant.

	//Voltage_V = myCompartment->GetMembraneVoltage_V();
	//VoltageThreshold_V = myCompartment->Get_V_th_Volts();

	SetS(myCompartment->GetMembraneVoltage_V(), myCompartment->Get_V_th_Volts());
	dydt = (1.0/myCompartment->Get_Tau_K_Seconds())*(-IntermediateY + (myCompartment->Get_b_K() * S)); 

	return dydt;
}

MGESynapseBranch::MGESynapseBranch(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds, double ReversalPotential_V)	
{
	this->ReversalPotential_V = ReversalPotential_V;	/* Excitatory synapse reversal potential (V) */
	CalculateSteadyState(InitialVoltage_V, ThresholdVoltage_V, b_K, Tau_K_Seconds);
}

void MGESynapseBranch::CalculateSteadyState(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds)
{
	// Calculate steady-state rate
	InitialY = 0;	// InitialY arbitrarily set to zero because this synapse doesn't require rkSolver 
	IntermediateY = InitialY;
}	

double MGESynapseBranch::GetDerivative(double IntermediateY, double InstantaneousVoltage_V)
{
	this->IntermediateY = IntermediateY;
	return -IntermediateY;
}

MGISynapseBranch::MGISynapseBranch(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds, double ReversalPotential_V)	
{
	this->ReversalPotential_V = ReversalPotential_V;	/* Excitatory synapse reversal potential (V) */
	CalculateSteadyState(InitialVoltage_V, ThresholdVoltage_V, b_K, Tau_K_Seconds);
}

void MGISynapseBranch::CalculateSteadyState(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds)
{
	// Calculate steady-state rate
	InitialY = 0;	// InitialY arbitrarily set to zero because this synapse doesn't require rkSolver
	IntermediateY = InitialY;
}	

double MGISynapseBranch::GetDerivative(double IntermediateY, double InstantaneousVoltage_V)
{
	this->IntermediateY = IntermediateY;
	return -IntermediateY;
}
