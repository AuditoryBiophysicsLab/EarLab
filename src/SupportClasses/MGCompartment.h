#ifndef __MG_COMPARTMENT_H_INCLUDED
#define __MG_COMPARTMENT_H_INCLUDED

#include "Earlab.h"
#include "EarlabException.h"
#include "MGCurrentBranch.h"
#include "MGSynapseFirstOrder.h"
#include "RungeKuttaEquation.h"
#include "RungeKuttaSolver.h"

// The MGCompartment SupportClass is the single-compartment in the MacGregor neuromime model 
// implemented by Hancock KE and Voigt HF (1999) in "A Computational Model of DCN Type IV Units". 
// Annals of Biomedical Engineering. Vol. 278, pp. 73-87.


const int MGMaxSynapseCount = 100;
const int MGODECount = 20;	// (HHCompartment uses ODECount = 20; 
							// Used in MGCompartment.h: RungeKuttaEquation *Elements[MGODECount + 1];)
							// MGODECount is at least the number of Ordinary Differential Equations we're solving here 
							// Elements[0] never used (the active indices start with 1)
							// Elements[1]: the g_K RungeKuttaEquation
							// Elements[2]: the MGCompartment RungeKuttaEquation
							// For this incarnation of MGCompartment, MGODECount must be at least 2.

//const int NumExcSynapseSets = 1;  // SynapseSet members will have the same input module, same strength and time constant
//const int NumInhSynapseSets = 2;

class MGCurrentBranch;
class MGESynapseBranch;
class MGISynapseBranch;
class MGSynapseFirstOrder;

class MGCompartment : public RungeKuttaEquation
{
public:
	MGCompartment();
	MGCompartment(double SampleRate_Hz, double Tau_m_Seconds, double b_K, double V_th_Volts, double Tau_K_Seconds, double E_K_Volts, double E_ex_Volts, double E_in_Volts);
	double GetDerivative(double IntermediateY, double InstantaneousVoltage_V);
	double GetSolutionY(int MGCurrentBranchRKIndex);
	void SetCurrentInjection_A(double CurrentInjection_A);
	//virtual bool Step(MGSynapseFirstOrder *ExcitatorySynapses[], int ExcitatoryCount, MGSynapseFirstOrder *InhibitorySynapses[], int InhibitoryCount);
	//virtual bool Step(MGSynapseFirstOrder *ExcitatorySynapses[], int ExcitatoryCount, MGSynapseFirstOrder *InhibitorySynapses1[], int InhibitoryCount1, MGSynapseFirstOrder *InhibitorySynapses2[], int InhibitoryCount2);
	virtual bool Step(MGSynapseFirstOrder *ExcitatorySynapses[], int ExcitatoryCount, MGSynapseFirstOrder *ExcitatorySynapsesNSA[], int ExcitatoryCountNSA, MGSynapseFirstOrder *InhibitorySynapses1[], int InhibitoryCount1, MGSynapseFirstOrder *InhibitorySynapses2[], int InhibitoryCount2);

	inline double GetExcitatoryTotal(void) {return ExcitatoryTotal;}
	inline double GetExcitatoryTotalNSA(void) {return ExcitatoryTotalNSA;}
	inline double GetInhibitoryTotal1(void) {return InhibitoryTotal1;}
	inline double GetInhibitoryTotal2(void) {return InhibitoryTotal2;}
	inline double GetMembraneVoltage_V(void) {return V_m_new_Volts;}

	inline double Get_V_rest_Volts(void) {return V_rest_Volts;}
	inline double Get_V_th_Volts(void) {return V_th_Volts;}
	inline double Get_b_K(void) {return b_K;}
	inline double Get_Tau_K_Seconds(void) {return Tau_K_Seconds;}
	inline double Get_KCBIndex(void) {return KCBIndex;}

private:
	void CalculateVoltage(void);
	inline void TotalExcitatorySynapses(MGSynapseFirstOrder *Synapses[], int SynapseCount) {ExcitatoryTotal = TotalSynapses(Synapses, SynapseCount);}
	inline void TotalExcitatorySynapsesNSA(MGSynapseFirstOrder *Synapses[], int SynapseCount) {ExcitatoryTotalNSA = TotalSynapses(Synapses, SynapseCount);}
	inline void TotalInhibitorySynapses1(MGSynapseFirstOrder *Synapses[], int SynapseCount) {InhibitoryTotal1 = TotalSynapses(Synapses, SynapseCount);}
	inline void TotalInhibitorySynapses2(MGSynapseFirstOrder *Synapses[], int SynapseCount) {InhibitoryTotal2 = TotalSynapses(Synapses, SynapseCount);}
	double TotalSynapses(MGSynapseFirstOrder *Synapses[], int SynapseCount);

	double SamplePeriod_Seconds;
	double SampleRate_Hz;
	//bool Initialized;
	double Tau_m_Seconds;	// membrane time constant
							// Tau_m = C_m/G; Only the time constant is used explicitly

	double AuxCurrentInput_A;	// Auxilliary Current Input, in Amperes
	double ExcitatoryTotal;
	double ExcitatoryTotalNSA;
	double InhibitoryTotal1;
	double InhibitoryTotal2;
//	double ExcitatoryTotal[NumExcSynapseGroups];// array, excitatory conductance divided by the leakage condunctance
//	double InhibitoryTotal[NumInhSynapseGroups];// array, inhibitory conductance divided by the leakage condunctance
	double V_m_Volts;		// membrane potential
	double V_th_Volts;		// threshold membrane potential for output event
	double V_rest_Volts;	// resting membrane potential
	double dVmdt;			// time deriviative of V_m_Volts
	double dgKdt;			// time deriviative of g_K
	//double InitialV_m_Volts;// initial value of V_m_Volts, inherited from and used extensively in HH constructors 
	double V_m_old_Volts;
	double V_m_new_Volts;
	bool WasUnderThreshold;

	// Relating to the potassium channel, the MGKCurrentBranch:
	//double g_K;			// potassium conductance divided by the leakage condunctance
	// (in MGCurrentBranch)	// g_K is determined by a first-order differential equation
							// Tau_K*dg_K/dt = -g_K + B_K*S  (S=1 for V_m >= V_th, otherwise S=0)
	double b_K;				// Constant used to increment dg_K/dt (and g_K) 
							// when membrane voltage exceeds threshold.
	//int S;				// S=1 for V_m >= V_th, otherwise S=0
	// (in MGCurrentBranch)
	double Tau_K_Seconds;	// time constant for g_K

	// Channel Conductances
	MGCurrentBranch **CurrentBranches;
	int NumMGCurrentBranches; // number of MGCurrentBranches excluding leakage, including K+, Exc, Inh
	int NumMGCurrentBranchesRK; // number of MGCurrentBranches requiring CalculateVoltage calling rkSolver
    int KCBIndex, ESBIndex, ESBNSAIndex, ISB1Index, ISB2Index;

	RungeKuttaEquation *Elements[MGODECount + 1];
	RungeKuttaSolver rkSolver;
	
	//// Potassium Current Branch
	//MGKChannelBranch *KCurrentBranch
	//// Input Synapses containing series conductance and reversal potential
	//MGESynapseBranch *ExcitatorySynapseBranch;
	//MGISynapseBranch *InhibitorySynapseBranch;

	// Leakage conductance (is not used)
	//MGLeakage *Leakage;
	// Resting Conductance G, used by Hancock and Voigt (1999), is similar to Leakage Conductance in HH modules,
	// but always has zero as its reversal potential. In the MacGregor compartment model the differential equation 
	// describing current is scaled (divided) by G, and the scaled current I_G/G always equals the membrane potential.
	// So apart from the V_m_Volts term in the membrane current equation, it is unnecesary to explicitly describe the 
	// current branch of Resting Conductance G.

};

#endif
