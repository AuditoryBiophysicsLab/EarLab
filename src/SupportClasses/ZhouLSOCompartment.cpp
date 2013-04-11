//#include <iostream>
//#include <stdio.h>
//#include <memory.h>
//#include <string.h>
#include <math.h>
//#include <time.h>
#include "Earlab.h"
#include "ZhouLSOCompartment.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "EarlabException.h"
#include "FPException.h"


ZhouLSOCompartment::ZhouLSOCompartment()
{
	//memset(this, 0, sizeof(ZhouLSOCompartment));
}

ZhouLSOCompartment::ZhouLSOCompartment(double SampleRate_Hz, double C_m_Farads, double G_L_Siemens, double E_L_Volts, double G_abs_increment_Siemens, double E_abs_Volts, double t_abs_seconds, double G_AHP_increment_Siemens, double E_AHP_Volts, double Tau_AHP_Seconds, double V_th_Volts, double V_rest_Volts)
{
	//memset(this, 0, sizeof(ZhouLSOCompartment));
	Initialize(SampleRate_Hz, C_m_Farads, G_L_Siemens, E_L_Volts, G_abs_increment_Siemens, E_abs_Volts, t_abs_seconds, G_AHP_increment_Siemens, E_AHP_Volts, Tau_AHP_Seconds, V_th_Volts, V_rest_Volts);
}

ZhouLSOCompartment::ZhouLSOCompartment(double SampleRate_Hz, double C_m_Farads, double G_L_Siemens, double E_L_Volts, double G_abs_increment_Siemens, double E_abs_Volts, double t_abs_seconds, double G_AHP_increment_Siemens, double E_AHP_Volts, double Tau_AHP_Seconds, double V_th_Volts, double V_rest_Volts, double E_e_Volts, double E_i_Volts,	double G_e_Siemens, double G_i_Siemens,	double Tau_e_rise_Seconds, double Tau_e_fall_Seconds, double Tau_i_rise_Seconds, double Tau_i_fall_Seconds)
{
	//memset(this, 0, sizeof(ZhouLSOCompartment));
	Initialize(SampleRate_Hz, C_m_Farads, G_L_Siemens, E_L_Volts, G_abs_increment_Siemens, E_abs_Volts, t_abs_seconds, G_AHP_increment_Siemens, E_AHP_Volts, Tau_AHP_Seconds, V_th_Volts, V_rest_Volts, E_e_Volts, E_i_Volts, G_e_Siemens, G_i_Siemens,	Tau_e_rise_Seconds, Tau_e_fall_Seconds, Tau_i_rise_Seconds, Tau_i_fall_Seconds);
}

void ZhouLSOCompartment::Initialize(double SampleRate_Hz, double C_m_Farads, double G_L_Siemens, double E_L_Volts, double G_abs_increment_Siemens, double E_abs_Volts, double t_abs_seconds, double G_AHP_increment_Siemens, double E_AHP_Volts, double Tau_AHP_Seconds, double V_th_Volts, double V_rest_Volts)
{
	int i;
	this->SamplePeriod_Seconds = 1.0 / SampleRate_Hz;

	this->C_m_Farads = C_m_Farads;
	this->G_L_Siemens = G_L_Siemens;
	this->E_L_Volts = E_L_Volts;
	this->G_abs_increment_Siemens = G_abs_increment_Siemens;
	this->G_abs_Siemens = 0.0;
	this->E_abs_Volts = E_abs_Volts;
	this->t_abs_seconds = t_abs_seconds;
	this->G_AHP_increment_Siemens = G_AHP_increment_Siemens;
	this->G_AHP_Siemens = 0.0;
	this->E_AHP_Volts = E_AHP_Volts;
	this->Tau_AHP_Seconds = Tau_AHP_Seconds;
	this->reduction_factor_G_AHP = exp(-SamplePeriod_Seconds/Tau_AHP_Seconds);
	this->V_th_Volts = V_th_Volts;
	this->V_rest_Volts = V_rest_Volts;
	this->V_m_Volts = V_rest_Volts; // set the membrane voltage to resting potential
	this->V_m_old_Volts = V_rest_Volts;
	this->Vnew_V = V_rest_Volts;
	this->InitialY = V_rest_Volts; // initial voltage for the RungeKuttaEquation
	this->dVmdt = 0.0; // assume the membrane voltage is not changing initially
	this->AbsoluteRefractoryTimerQueue = new TimerQueue(SampleRate_Hz);
	// this->ThresholdReductionFactor = (1 - (SamplePeriod_Seconds / RelativeRefractoryPeriod_Seconds));
	// Integrator = new FirstOrderLowpass();
	// Integrator->SetSampleRate_Hz(SampleRate_Hz);
	// Integrator->SetTimeConstant_mS(FilterTimeConstant_Seconds * 1000);
	// Integrator->ResetFilterTo(RestingPotential_Volts);
	this->Initialized = true;
	this->InAbsoluteRefractoryPeriod = false;
	this->ExcitatoryTotal = 0.0;
	this->InhibitoryTotal = 0.0;

	this->E_e_Volts = 0.0;
	this->E_i_Volts = -0.070;
	this->G_e_Siemens = 0.0;
	this->G_i_Siemens = 0.0;
	this->Tau_e_rise_Seconds = 0.0001;
	this->Tau_e_fall_Seconds = 0.001;
	this->Tau_i_rise_Seconds = 0.0001;
	this->Tau_i_fall_Seconds = 0.001;

	this->NumZLCurrentBranches = 2;	// ExcitatorySynapseBranch
									// InhibitorySynapseBranch
	// number of ZLCurrentBranches requiring CalculateVoltage calling rkSolver
	this->NumZLCurrentBranchesRK = 0;

	CurrentBranches = new ZLCurrentBranch *[NumZLCurrentBranches + 1];
	// CurrentBranches not requiring rkSolver (Synapses)
	ESBIndex = 1;
	ISBIndex = 2;
	CurrentBranches[ESBIndex] = new ZLSynapseBranch(E_e_Volts); // Excitatory synapse branch
	CurrentBranches[ISBIndex] = new ZLSynapseBranch(E_i_Volts); // Inhibitory synapse branch 1

	for (i = 1; i <= NumZLCurrentBranches; i++)
		CurrentBranches[i]->SetCompartment(this);

// NumZLCurrentBranchesRK = 0, so only the compartment below is an Element (Elements[1]).  Elements[0] is undefined.
//	for (i = 1; i <= NumZLCurrentBranchesRK; i++) // Elements[0] is unused, consistent with rkSolver architecture
//		Elements[i] = CurrentBranches[i];
//	Elements[i] = this; // Elements[NumZLCurrentBranchesRK+1] is the compartment
	Elements[NumZLCurrentBranchesRK+1] = this; // Elements[NumZLCurrentBranchesRK+1] is the compartment
}

void ZhouLSOCompartment::Initialize(double SampleRate_Hz, double C_m_Farads, double G_L_Siemens, double E_L_Volts, double G_abs_increment_Siemens, double E_abs_Volts, double t_abs_seconds, double G_AHP_increment_Siemens, double E_AHP_Volts, double Tau_AHP_Seconds, double V_th_Volts, double V_rest_Volts, double E_e_Volts, double E_i_Volts, double G_e_Siemens, double G_i_Siemens, double Tau_e_rise_Seconds, double Tau_e_fall_Seconds, double Tau_i_rise_Seconds, double Tau_i_fall_Seconds)
{
	int i;
	this->SamplePeriod_Seconds = 1.0 / SampleRate_Hz;

	this->C_m_Farads = C_m_Farads;
	this->G_L_Siemens = G_L_Siemens;
	this->E_L_Volts = E_L_Volts;
	this->G_abs_increment_Siemens = G_abs_increment_Siemens;
	this->G_abs_Siemens = 0.0;
	this->E_abs_Volts = E_abs_Volts;
	this->t_abs_seconds = t_abs_seconds;
	this->G_AHP_increment_Siemens = G_AHP_increment_Siemens;
	this->G_AHP_Siemens = 0.0;
	this->E_AHP_Volts = E_AHP_Volts;
	this->Tau_AHP_Seconds = Tau_AHP_Seconds;
	this->reduction_factor_G_AHP = exp(-SamplePeriod_Seconds/Tau_AHP_Seconds);
	this->V_th_Volts = V_th_Volts;
	this->V_rest_Volts = V_rest_Volts;
	this->V_m_Volts = V_rest_Volts; // set the membrane voltage to resting potential
	this->V_m_old_Volts = V_rest_Volts;
	this->Vnew_V = V_rest_Volts;
	this->InitialY = V_rest_Volts; // initial voltage for the RungeKuttaEquation
	this->dVmdt = 0.0; // assume the membrane voltage is not changing initially
	this->AbsoluteRefractoryTimerQueue = new TimerQueue(SampleRate_Hz);
	// this->ThresholdReductionFactor = (1 - (SamplePeriod_Seconds / RelativeRefractoryPeriod_Seconds));
	// Integrator = new FirstOrderLowpass();
	// Integrator->SetSampleRate_Hz(SampleRate_Hz);
	// Integrator->SetTimeConstant_mS(FilterTimeConstant_Seconds * 1000);
	// Integrator->ResetFilterTo(RestingPotential_Volts);
	this->Initialized = true;
	this->InAbsoluteRefractoryPeriod = false;
	this->ExcitatoryTotal = 0.0;
	this->InhibitoryTotal = 0.0;

	this->InjectedCurrent_Amps = 0.0;
	this->E_e_Volts = E_e_Volts;
	this->E_i_Volts = E_i_Volts;
	this->G_e_Siemens = G_e_Siemens;
	this->G_i_Siemens = G_i_Siemens;
	this->Tau_e_rise_Seconds = Tau_e_rise_Seconds;
	this->Tau_e_fall_Seconds = Tau_e_fall_Seconds;
	this->Tau_i_rise_Seconds = Tau_i_rise_Seconds;
	this->Tau_i_fall_Seconds = Tau_i_fall_Seconds;

	this->NumZLCurrentBranches = 2;	// ExcitatorySynapseBranch
									// InhibitorySynapseBranch
	// number of ZLCurrentBranches requiring CalculateVoltage calling rkSolver
	this->NumZLCurrentBranchesRK = 0;

	CurrentBranches = new ZLCurrentBranch *[NumZLCurrentBranches];
	// CurrentBranches not requiring rkSolver (Synapses)
	ESBIndex = 0;
	ISBIndex = 1;
	CurrentBranches[ESBIndex] = new ZLSynapseBranch(E_e_Volts); // Excitatory synapse branch
	CurrentBranches[ISBIndex] = new ZLSynapseBranch(E_i_Volts); // Inhibitory synapse branch 1

	for (i = 0; i < NumZLCurrentBranches; i++)
		CurrentBranches[i]->SetCompartment(this);

// NumZLCurrentBranchesRK = 0, so only the compartment below is an Element (Elements[1]).  Elements[0] is undefined.
//	for (i = 1; i <= NumZLCurrentBranchesRK; i++) // Elements[0] is unused, consistent with rkSolver architecture
//		Elements[i] = CurrentBranches[i-1];
//	Elements[i] = this; // Elements[NumZLCurrentBranchesRK+1] is the compartment
	Elements[NumZLCurrentBranchesRK+1] = this; // Elements[NumZLCurrentBranchesRK+1] is the compartment
}

ZhouLSOCompartment::~ZhouLSOCompartment()
{
	//delete Integrator;
	//if (AbsoluteRefractoryTimerQueue != NULL)
	//	delete AbsoluteRefractoryTimerQueue;
	delete AbsoluteRefractoryTimerQueue;
}

bool ZhouLSOCompartment::Step(double InputCurrent_Amps)
{
	bool FiredThisStep = false;
	//double IntegratorOutput;

	InjectedCurrent_Amps = InputCurrent_Amps;

	if (!(IS_ALMOST_DENORMAL(G_AHP_Siemens)))
		G_AHP_Siemens *= reduction_factor_G_AHP;
	else
		G_AHP_Siemens = 0;
    
	// Determine dVm/dt from the old value of Vm 
	//dVmdt = GetdVdt();
	dVmdt = ZhouLSOCompartment::GetDerivative(V_m_Volts, V_m_Volts);


	// Pass dVmdt and other parameters to the integrating differential equation solver
	// and solve for a new value of Vm_Volts
	CalculateVoltage();

	if (!InAbsoluteRefractoryPeriod)
	{
		// If we're not in the absolute refractory period
		if (V_m_Volts >= V_th_Volts)
		{
			FiredThisStep = true;
			//Integrator->ResetFilterTo(RestingPotential_Volts);
			AbsoluteRefractoryTimerQueue->Add(new Timer(t_abs_seconds));
			InAbsoluteRefractoryPeriod = true;
			G_abs_Siemens = G_abs_increment_Siemens;
		}
	}
	else
	{
		if (AbsoluteRefractoryTimerQueue->Step() != 0)
		{
			G_abs_Siemens = 0;
			G_AHP_Siemens += G_AHP_increment_Siemens;
			InAbsoluteRefractoryPeriod = false;
		}
	}

	return FiredThisStep;
}


bool ZhouLSOCompartment::Step(ZLSynapse1stOrderDiffc *ExcitatorySynapses[], int ExcitatoryCount, ZLSynapse1stOrderDiffc *InhibitorySynapses[], int InhibitoryCount)
{
	if (ExcitatorySynapses != NULL)
		TotalExcitatorySynapses(ExcitatorySynapses, ExcitatoryCount);
	if (InhibitorySynapses != NULL)
		TotalInhibitorySynapses(InhibitorySynapses, InhibitoryCount);

	bool FiredThisStep = false;
	//double IntegratorOutput;

//	InjectedCurrent_Amps = InputCurrent_Amps;
//	InjectedCurrent_Amps = 0.0;

	if (!(IS_ALMOST_DENORMAL(G_AHP_Siemens)))
		G_AHP_Siemens *= reduction_factor_G_AHP;
	else
		G_AHP_Siemens = 0;
	

	//if (CurThresholdBoost_Volts > 0)
	//{
	//	// If we're still above our default threshold, perform the exponential reduction in the boost
	//	CurThresholdBoost_Volts *= ThresholdReductionFactor;
	//	// Check to see if the boost is very small, and if it is, drop it to zero.  This is a performance enhancement on Pentium processors
	//	if (IS_ALMOST_DENORMAL(CurThresholdBoost_Volts))
	//		CurThresholdBoost_Volts = 0;
	//}

	//// Filter the output through the integrator
	//IntegratorOutput = Integrator->Filter(MembranePotential_Volts);
    
	// Determine dVm/dt from the old value of Vm 
	//dVmdt = GetdVdt();
	dVmdt = ZhouLSOCompartment::GetDerivative(V_m_Volts, V_m_Volts);

	// Pass dVmdt and other parameters to the integrating differential equation solver
	// and solve for a new value of Vm_Volts
	CalculateVoltage();

	if (!InAbsoluteRefractoryPeriod)
	{
		// If we're not in the absolute refractory period
		if (V_m_Volts >= V_th_Volts)
		{
			FiredThisStep = true;
			//Integrator->ResetFilterTo(RestingPotential_Volts);
			AbsoluteRefractoryTimerQueue->Add(new Timer(t_abs_seconds));
			InAbsoluteRefractoryPeriod = true;
			G_abs_Siemens = G_abs_increment_Siemens;
		}
	}
	else
	{
		if (AbsoluteRefractoryTimerQueue->Step() != 0)
		{
			G_abs_Siemens = 0;
			G_AHP_Siemens += G_AHP_increment_Siemens;
			InAbsoluteRefractoryPeriod = false;
		}
	}

	return FiredThisStep;
}

void ZhouLSOCompartment::CalculateVoltage(void)
{
	int nbad, nok;
	double eps = 1.0e-4;
	double hstart = 10.0e-6;	/* 'Best guess' for time step */
	//double hmin = 1.0e-6; /* Minimum time step */
	double hmin = 1.0e-8; /* Minimum time step */
	double x1, x2;	  // Howitt 12Dec01
	//int i;

	V_m_old_Volts = V_m_Volts; 

	/* Try  ODEINT here , from Numerical Recipes */
	rkSolver.kmax=100;
	x1 = 1.; 
	x2 = 1. + SamplePeriod_Seconds; 
	rkSolver.dxsav=(x2-x1)/20.0;

	// DJA: Set the present instananeous voltage of the compartment in each of my dependent conductances
	// Note: NumHHChannels is the number of HH Channels in the compartment, plus one for the compartment
	rkSolver.ode_rk(NumZLCurrentBranchesRK + 1,x1,x2,eps,hstart,hmin,&nok,&nbad,Elements);
	//for (i = 1; i <= NumHHChannels; i++)
	//		Channels[i]->activation = Elements[i]->InitialY;
	V_m_Volts = Elements[NumZLCurrentBranchesRK + 1]->InitialY; // ode_rk stores the y[i] outputs in InitialY for the next step 
}

double ZhouLSOCompartment::GetDerivative(double IntermediateY, double InstantaneousVoltage_V)
{
	double dVdt;

	// argument IntermediateY is the intermediate value of membrane voltage
	// used in RungeKuttaSolver.cpp.  In HH compartments where GetDerivative 
	// calculates ion channel activation, membrane voltage is needed as an
	// additional parameter.  Here it is redundant.


	// All voltages are in Volts
	//     Conductances are in Siemens
	//     Currents are in Amperes
	dVdt = (1.0/C_m_Farads)*(InjectedCurrent_Amps - G_L_Siemens*(IntermediateY-E_L_Volts) 
		                      - G_abs_Siemens*(IntermediateY-E_abs_Volts) 
							  - G_AHP_Siemens*(IntermediateY-E_AHP_Volts)
							  - ExcitatoryTotal*(IntermediateY-E_e_Volts)
							  - InhibitoryTotal*(IntermediateY-E_i_Volts));


	return dVdt;

}

double ZhouLSOCompartment::TotalSynapses(ZLSynapse1stOrderDiffc *Synapses[], int SynapseCount)
{
	int i;
	double total = 0.0;

	for (i = 0; i < SynapseCount; i++)
		total += Synapses[i]->GetConductance_Siemens();
	return total;
}

void ZhouLSOCompartment::GetConductances_nS(float &Ge, float &Gi, float &Gabs, float &GAHP)
{
	Ge = (float) (GetExcitatoryTotal() * 1e9); // GetExcitatoryTotal & GetInhibitoryTotal return in units Siemens.
	Gi = (float) (GetInhibitoryTotal() * 1e9);
	Gabs = (float) (GetGabs_Siemens() * 1e9);
	GAHP = (float) (GetGAHP_Siemens() * 1e9);
}
