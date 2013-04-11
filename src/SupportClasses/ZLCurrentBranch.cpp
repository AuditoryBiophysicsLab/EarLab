#include "ZLCurrentBranch.h"


ZLSynapseBranch::ZLSynapseBranch(double ReversalPotential_V)	
{
	this->ReversalPotential_V = ReversalPotential_V;	/* Excitatory synapse reversal potential (V) */
	//CalculateSteadyState(InitialVoltage_V, ThresholdVoltage_V, b_K, Tau_K_Seconds);
}

//ZLESynapseBranch::ZLESynapseBranch(double ReversalPotential_V)	
//{
//	this->ReversalPotential_V = ReversalPotential_V;	/* Excitatory synapse reversal potential (V) */
//	//CalculateSteadyState(InitialVoltage_V, ThresholdVoltage_V, b_K, Tau_K_Seconds);
//}

//void ZLESynapseBranch::CalculateSteadyState(double InitialVoltage_V)
//{
//	// Calculate steady-state rate
//	InitialY = 0;	// InitialY arbitrarily set to zero because this synapse doesn't require rkSolver 
//	IntermediateY = InitialY;
//}	

//double ZLESynapseBranch::GetDerivative(double IntermediateY, double InstantaneousVoltage_V)
//{
//	this->IntermediateY = IntermediateY;
//	return -IntermediateY;
//}

//ZLISynapseBranch::ZLISynapseBranch(double ReversalPotential_V)	
//{
//	this->ReversalPotential_V = ReversalPotential_V;	/* Excitatory synapse reversal potential (V) */
//	//CalculateSteadyState(InitialVoltage_V, ThresholdVoltage_V, b_K, Tau_K_Seconds);
//}

//void ZLISynapseBranch::CalculateSteadyState(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds)
//{
//	// Calculate steady-state rate
//	InitialY = 0;	// InitialY arbitrarily set to zero because this synapse doesn't require rkSolver
//	IntermediateY = InitialY;
//}	

//double ZLISynapseBranch::GetDerivative(double IntermediateY, double InstantaneousVoltage_V)
//{
//	this->IntermediateY = IntermediateY;
//	return -IntermediateY;
//}
