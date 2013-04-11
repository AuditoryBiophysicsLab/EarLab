#ifndef _MG_CURRENT_BRANCH_H_INCLUDED
#define _MG_CURRENT_BRANCH_H_INCLUDED
#include "RungeKuttaEquation.h"
#include "MGCompartment.h"

class MGCompartment;

// Resting Conductance G, used by Hancock and Voigt (1999), is similar to Leakage Conductance in HH modules,
// but always has zero as its reversal potential. In the MacGregor compartment model the differential equation 
// describing current is scaled (divided) by G, and the scaled current I_G/G always equals the membrane potential.
// So apart from the V_m_Volts term in the membrane current equation, it is unnecesary to explicitly describe the 
// current branch of Resting Conductance G.

class MGCurrentBranch : public RungeKuttaEquation
{
public:
	//MGCurrentBranch();
	virtual void CalculateSteadyState(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds) = 0;
	void SetCompartment(MGCompartment *theCompartment) {myCompartment = theCompartment;}

	double ReversalPotential_V;			// Set up by the derived class constructor
	double SolutionY;					// Y-value solution from the RungeKuttaSolver
										// Y in the case of MGKChannelBranch is g_K.
	//(double IntermediateY);	// IntermediateY, inherited from RungeKuttaEquation public,
								// contains the branch conductance divided by the membrane leakage conductance. 
								// potassium channel:  g_K
								// excitatory synapse: g_ex
								// inhibitory synapse: g_in 

protected:
	MGCompartment *myCompartment;
};

class MGKChannelBranch : public MGCurrentBranch
{
public:
	MGKChannelBranch(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds, double ReversalPotential_V);
	void CalculateSteadyState(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds);
	void SetS(double Voltage_V, double ThresholdVoltage_V);
	double GetDerivative(double IntermediateY, double InstantaneousVoltage_V);
	inline int Get_S(void) {return S;}

private:
// (g_branch);	// potassium conductance divided by the leakage condunctance
// (double g_K)	// g_K is determined by a first-order differential equation
				// Tau_K*dg_K/dt = -g_K + b_K*S  (S=1 for V_m >= V_th, otherwise S=0)
				// To obtain Tau_K_Seconds, V_th_Volts, b_K
				// use MGCompartment::Get_Tau_K_Seconds() etc.
	int S;
};

class MGESynapseBranch : public MGCurrentBranch
{
public:
	MGESynapseBranch(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds, double ReversalPotential_V);
	void CalculateSteadyState(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds);
	double GetDerivative(double IntermediateY, double InstantaneousVoltage_V);

//private:
//	double g_ex;		// excitatory synaptic conductance divided by the leakage condunctance
// (g_branch)			// g_ex in Hancock and Voigt (1999)

};

class MGISynapseBranch : public MGCurrentBranch
{
public:
	MGISynapseBranch(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds, double ReversalPotential_V);
	void CalculateSteadyState(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds);
	double GetDerivative(double IntermediateY, double InstantaneousVoltage_V);

//private:
//	double g_in;		// inhibitory synaptic conductance divided by the leakage condunctance
// (g_branch)			// g_in in Hancock and Voigt (1999)

};
#endif // _MG_CURRENT_BRANCH_H_INCLUDED
