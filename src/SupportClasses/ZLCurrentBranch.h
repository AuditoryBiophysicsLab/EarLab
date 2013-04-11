#ifndef _ZL_CURRENT_BRANCH_H_INCLUDED
#define _ZL_CURRENT_BRANCH_H_INCLUDED
//#include "RungeKuttaEquation.h"
#include "ZhouLSOCompartment.h"

class ZhouLSOCompartment;


class ZLCurrentBranch //: public RungeKuttaEquation
{
public:
	//ZLCurrentBranch();
	//virtual void CalculateSteadyState(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds) = 0;
	virtual void SetCompartment(ZhouLSOCompartment *theCompartment) = 0;

	double ReversalPotential_V;			// Set up by the derived class constructor
	//double SolutionY;					// Y-value solution from the RungeKuttaSolver
										// Y in the case of ZLKChannelBranch is g_K.
	//(double IntermediateY);	// IntermediateY, inherited from RungeKuttaEquation public,
								// contains the branch conductance divided by the membrane leakage conductance. 
								// potassium channel:  g_K
								// excitatory synapse: g_ex
								// inhibitory synapse: g_in 

//private:
//	ZhouLSOCompartment *myCompartment;
};

class ZLSynapseBranch : public ZLCurrentBranch
{
public:
	//ZLSynapseBranch(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds, double ReversalPotential_V);
	ZLSynapseBranch(double ReversalPotential_V);
	void SetCompartment(ZhouLSOCompartment *theCompartment) {myCompartment = theCompartment;}
	
	//void CalculateSteadyState(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds);
	//double GetDerivative(double IntermediateY, double InstantaneousVoltage_V);

private:
	ZhouLSOCompartment *myCompartment;

};

//class ZLESynapseBranch : public ZLCurrentBranch
//{
//public:
//	//ZLESynapseBranch(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds, double ReversalPotential_V);
//	ZLESynapseBranch(double ReversalPotential_V);
//	void SetCompartment(ZhouLSOCompartment *theCompartment) {myCompartment = theCompartment;}
//	
//	//void CalculateSteadyState(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds);
//	//double GetDerivative(double IntermediateY, double InstantaneousVoltage_V);
//
//private:
//	ZhouLSOCompartment *myCompartment;
//
//};

//class ZLISynapseBranch : public ZLCurrentBranch
//{
//public:
//	//ZLISynapseBranch(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds, double ReversalPotential_V);
//	ZLISynapseBranch(double ReversalPotential_V);
//	void SetCompartment(ZhouLSOCompartment *theCompartment) {myCompartment = theCompartment;}
//
//	//void CalculateSteadyState(double InitialVoltage_V, double ThresholdVoltage_V, double b_K, double Tau_K_Seconds);
//	//double GetDerivative(double IntermediateY, double InstantaneousVoltage_V);
//
//private:
//	ZhouLSOCompartment *myCompartment;

//};
#endif // _ZL_CURRENT_BRANCH_H_INCLUDED
