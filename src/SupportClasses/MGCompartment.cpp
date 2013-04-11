#include "MGCompartment.h"
//#include "RungeKuttaEquation.h"
//#include "RungeKuttaSolver.h"
#include "math.h"
#include "stdio.h"

MGCompartment::MGCompartment(double SampleRate_Hz, double Tau_m_Seconds, double b_K, double V_th_Volts, double Tau_K_Seconds, double E_K_Volts, double E_ex_Volts, double E_in_Volts)
{
	int i;

	this->SampleRate_Hz = SampleRate_Hz;
	this->SamplePeriod_Seconds = 1.0/this->SampleRate_Hz;
	this->Tau_m_Seconds = Tau_m_Seconds;
	this->b_K = b_K;
	this->V_th_Volts = V_th_Volts;

	this->V_rest_Volts = 0.0;
	this->V_m_Volts = V_rest_Volts;
	this->V_m_old_Volts = V_m_Volts;
	this->V_m_new_Volts = V_m_Volts;
	this->InitialY = V_m_Volts;
	this->IntermediateY = InitialY;
	this->dVmdt = 0.0;
	this->dgKdt = 0.0;
	this->WasUnderThreshold = true;
	this->Tau_K_Seconds = Tau_K_Seconds; // Tau_K_Seconds = 0.001 in Voigt and Hancock 1999
	this->AuxCurrentInput_A = 0.0;
	this->ExcitatoryTotal = 0.0;
	this->ExcitatoryTotalNSA = 0.0;
	this->InhibitoryTotal1 = 0.0;
	this->InhibitoryTotal2 = 0.0;


	// number of MGCurrentBranches excluding leakage, including K+, Exc, Inh
	this->NumMGCurrentBranches = 5;	// MGKChannelBranch
									// ExcitatorySynapseBranch
									// ExcitatorySynapseBranchNSA
									// InhibitorySynapseBranch1
									// InhibitorySynapseBranch2
	// number of MGCurrentBranches requiring CalculateVoltage calling rkSolver
	this->NumMGCurrentBranchesRK = 1;

	CurrentBranches = new MGCurrentBranch *[NumMGCurrentBranches + 1];
	 // First the CurrentBranches requiring rkSolver (Channels)
	KCBIndex = 1;
	CurrentBranches[KCBIndex] = new MGKChannelBranch(V_m_Volts, V_th_Volts, b_K, Tau_K_Seconds, E_K_Volts); // Potassium channel branch (scaled conductance g_K is the variable)
	// Last the CurrentBranches not requiring rkSolver (Synapses)
	ESBIndex = 2;
	ESBNSAIndex = 3;
	ISB1Index = 4;
	ISB2Index = 5;
	CurrentBranches[ESBIndex] = new MGESynapseBranch(V_m_Volts, V_th_Volts, b_K, Tau_K_Seconds, E_ex_Volts); // Excitatory synapse branch
	CurrentBranches[ESBNSAIndex] = new MGESynapseBranch(V_m_Volts, V_th_Volts, b_K, Tau_K_Seconds, E_ex_Volts); // Excitatory synapse branch for non-specific afferents (see Zheng and Voigt 2006)
	CurrentBranches[ISB1Index] = new MGISynapseBranch(V_m_Volts, V_th_Volts, b_K, Tau_K_Seconds, E_in_Volts); // Inhibitory synapse branch 1
	CurrentBranches[ISB2Index] = new MGISynapseBranch(V_m_Volts, V_th_Volts, b_K, Tau_K_Seconds, E_in_Volts); // Inhibitory synapse branch 2


	for (i = 1; i <= NumMGCurrentBranches; i++)
		CurrentBranches[i]->SetCompartment(this);

	for (i = 1; i <= NumMGCurrentBranchesRK; i++) // Elements[0] is unused, consistent with rkSolver architecture
		Elements[i] = CurrentBranches[i];
	Elements[i] = this; // Elements[NumMGCurrentBranchesRK+1] is the compartment
	
}

void MGCompartment::SetCurrentInjection_A(double CurrentInjection_A)
{
	AuxCurrentInput_A = CurrentInjection_A;
}

double MGCompartment::TotalSynapses(MGSynapseFirstOrder *Synapses[], int SynapseCount)
{
	int i;
	double total = 0.0;

	for (i = 0; i < SynapseCount; i++)
		total += Synapses[i]->GetConductance_Siemens();
	return total;
}

void MGCompartment::CalculateVoltage(void)
{
	int nbad, nok;
	double eps = 1.0e-4;
	double hstart = 10.0e-6;	/* 'Best guess' for time step */
	double hmin = 1.0e-6; /* Minimum time step */
	double x1, x2;	  // Howitt 12Dec01
	int i;

	V_m_old_Volts = V_m_new_Volts; 

	/* Try  ODEINT here , from Numerical Recipes */
	rkSolver.kmax=100;
	x1 = 1.; 
	x2 = 1. + SamplePeriod_Seconds; 
	rkSolver.dxsav=(x2-x1)/20.0;

	// DJA: Set the present instananeous voltage of the compartment in each of my dependent conductances
	rkSolver.ode_rk(NumMGCurrentBranchesRK + 1,x1,x2,eps,hstart,hmin,&nok,&nbad,Elements);
	for (i = 1; i <= NumMGCurrentBranchesRK; i++)
			CurrentBranches[i]->SolutionY = Elements[i]->InitialY; // rkSolver writes solutions to Elements[i]->InitialY
	V_m_new_Volts = Elements[i]->InitialY;
}

// MGCompartment::Step assumes that the synapses have already been stepped to the new sample
//bool MGCompartment::Step(MGSynapseFirstOrder *ExcitatorySynapses[], int ExcitatoryCount, MGSynapseFirstOrder *InhibitorySynapses[], int InhibitoryCount)
bool MGCompartment::Step(MGSynapseFirstOrder *ExcitatorySynapses[], int ExcitatoryCount, MGSynapseFirstOrder *ExcitatorySynapsesNSA[], int ExcitatoryCountNSA, MGSynapseFirstOrder *InhibitorySynapses1[], int InhibitoryCount1, MGSynapseFirstOrder *InhibitorySynapses2[], int InhibitoryCount2)
{
	if (ExcitatorySynapses != NULL)
		TotalExcitatorySynapses(ExcitatorySynapses, ExcitatoryCount);
	if (ExcitatorySynapsesNSA != NULL)
		TotalExcitatorySynapsesNSA(ExcitatorySynapsesNSA, ExcitatoryCountNSA);
//	if (InhibitorySynapses != NULL)
//		TotalInhibitorySynapses(InhibitorySynapses, InhibitoryCount);
	if (InhibitorySynapses1 != NULL)
		TotalInhibitorySynapses1(InhibitorySynapses1, InhibitoryCount1);
	if (InhibitorySynapses2 != NULL)
		TotalInhibitorySynapses2(InhibitorySynapses2, InhibitoryCount2);

	CalculateVoltage();

	if (V_m_new_Volts >= V_th_Volts)
	{
		if (WasUnderThreshold)
		{
			WasUnderThreshold = false;
			return true;	// This indicates a new action potential has occurred in this time step
		}
	}
	else
	{
		WasUnderThreshold = true;
	}

	return false;			// No new action potential has occurred in this time step
}

double MGCompartment::GetDerivative(double IntermediateY, double InstantaneousVoltage_V)
{
	double dVdt;
	//double g_Kbranch;

	// argument IntermediateY is the intermediate value of membrane voltage
	// used in RungeKuttaSolver.cpp.  In HH compartments where GetDerivative 
	// calculates ion channel activation, membrane voltage is needed as an
	// additional parameter.  Here it is redundant.


	// All voltages are in Volts
	// The current equation in Hancock and Voigt (1999) has been divided 
	// by the membrane leakage conductance so that Tau_m_Seconds (= C_m/G) can be used.
	// InjectedCurrent_Amps must be also divided by G (assumed to be 10e-9 for testing).
	//     Conductances are unitless because they are divided by the leakage conductance. 
	//     The leakage current divided by the leakage conductance equals the membrane voltage (IntermediateY).

	//dVdt = (1.0/Tau_m_Seconds)*((InjectedCurrent_Amps/1e-8) - IntermediateY 
	//	                      - g_K_Siemens*(IntermediateY-E_K_Volts) 
	//						  - g_e_Siemens*(IntermediateY-E_e_Volts)
	//						  - g_i_Siemens*(IntermediateY-E_i_Volts));

	//g_Kbranch = CurrentBranches[KCBIndex]->IntermediateY;
//	dVdt = (1.0/Tau_m_Seconds)*((AuxCurrentInput_A/1e-8) - IntermediateY 
//			- CurrentBranches[KCBIndex]->IntermediateY * (IntermediateY - CurrentBranches[KCBIndex]->ReversalPotential_V) 
//			- ExcitatoryTotal * (IntermediateY - CurrentBranches[ESBIndex]->ReversalPotential_V)
//			- InhibitoryTotal * (IntermediateY - CurrentBranches[ISBIndex]->ReversalPotential_V));

//	dVdt = (1.0/Tau_m_Seconds)*((AuxCurrentInput_A/1e-8) - IntermediateY 
//			- CurrentBranches[KCBIndex]->IntermediateY * (IntermediateY - CurrentBranches[KCBIndex]->ReversalPotential_V) 
//			- ExcitatoryTotal * (IntermediateY - CurrentBranches[ESBIndex]->ReversalPotential_V)
//			- InhibitoryTotal1 * (IntermediateY - CurrentBranches[ISB1Index]->ReversalPotential_V)
//			- InhibitoryTotal2 * (IntermediateY - CurrentBranches[ISB2Index]->ReversalPotential_V));

	dVdt = (1.0/Tau_m_Seconds)*((AuxCurrentInput_A/1e-8) - IntermediateY 
			- CurrentBranches[KCBIndex]->IntermediateY * (IntermediateY - CurrentBranches[KCBIndex]->ReversalPotential_V) 
			- ExcitatoryTotal * (IntermediateY - CurrentBranches[ESBIndex]->ReversalPotential_V)
			- ExcitatoryTotalNSA * (IntermediateY - CurrentBranches[ESBIndex]->ReversalPotential_V)
			- InhibitoryTotal1 * (IntermediateY - CurrentBranches[ISB1Index]->ReversalPotential_V)
			- InhibitoryTotal2 * (IntermediateY - CurrentBranches[ISB2Index]->ReversalPotential_V));

	return dVdt;
}

double MGCompartment::GetSolutionY(int MGCurrentBranchRKIndex)
{
	if ((MGCurrentBranchRKIndex >= 1) || (MGCurrentBranchRKIndex <= NumMGCurrentBranchesRK))
		return CurrentBranches[MGCurrentBranchRKIndex]->SolutionY;
	else
		throw EarlabException("MGCompartment::GetSolutionY: MGCurrentBranchRKIndex (%d) out of range", MGCurrentBranchRKIndex);
}
