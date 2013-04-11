#include "HHChannels.h"
#include "HHCompartment.h"
#include "RungeKuttaEquation.h"
#include "RungeKuttaSolver.h"
#include "math.h"
#include "stdio.h"

HHCompartment::HHCompartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC) :
				Kslow(InitialVoltage_V, Temperature_DegreesC),
				Kfast(InitialVoltage_V, Temperature_DegreesC),
				NaActivation(InitialVoltage_V, Temperature_DegreesC),
				NaInactivation(InitialVoltage_V, Temperature_DegreesC),
				Leakage(Temperature_DegreesC)
{
	// Was 23e-12 before fiddling began
	MembraneCapacitance_F = 23e-12;	 /* Membrane Capacitance (Farads) */
	this->InitialVoltage_V = InitialVoltage_V;
	Vold_V = InitialVoltage_V;
	Vnew_V = Vold_V;
	InitialY = InitialVoltage_V;
	this->SampleRate_Hz = SampleRate_Hz; 
	SamplePeriod = 1.0 / SampleRate_Hz;
	AuxCurrentInput_A = 0.0;
	ExcitatoryTotal = 0.0;
	InhibitoryTotal = 0.0;
	Settle();
}

double HHCompartment::GetDerivative(double IntermediateY, double InstantaneousVoltage_V)
{
	double dVdt;

	// All voltages are in Volts
	//     Conductances are in Siemens
	//     Currents are in Amperes
	dVdt = -(1.0 / MembraneCapacitance_F) * 
						   ((Kslow.MaxConductance_Siemens * Kslow.IntermediateY * (Vold_V - Kslow.ReversalPotential_V)) +
							(Kfast.MaxConductance_Siemens * Kfast.IntermediateY * (Vold_V - Kfast.ReversalPotential_V)) +
							(NaActivation.MaxConductance_Siemens * NaActivation.IntermediateY * NaActivation.IntermediateY * 
								(NaInactivation.IntermediateY * (Vold_V - NaActivation.ReversalPotential_V))) +
							(Leakage.MaxConductance_Siemens * (Vold_V - Leakage.ReversalPotential_V)) + 
							(ExcitatoryTotal * (Vold_V - Excitatory.ReversalPotential_V)) + 
							(InhibitoryTotal * (Vold_V - Inhibitory.ReversalPotential_V)) -
							(AuxCurrentInput_A));

	return dVdt;
}

void HHCompartment::SetTemperature_DegreesC(double Temperature_DegreesC)
{
	Kslow.SetTemperature_DegreesC(Temperature_DegreesC);
	Kfast.SetTemperature_DegreesC(Temperature_DegreesC);
	NaActivation.SetTemperature_DegreesC(Temperature_DegreesC);
	NaInactivation.SetTemperature_DegreesC(Temperature_DegreesC);
	Leakage.SetTemperature_DegreesC(Temperature_DegreesC);
}

void HHCompartment::SetCurrentInjection_A(double CurrentInjection_A)
{
	AuxCurrentInput_A = CurrentInjection_A;
}

double HHCompartment::TotalSynapses(HHSynapse *Synapses[], int SynapseCount)
{
	int i;
	double total = 0.0;

	for (i = 0; i < SynapseCount; i++)
		total += Synapses[i]->GetConductance_Siemens();
	return total;
}

void HHCompartment::Settle(void)
{
	int settle_samples, i;

	Elements[1] = &Kslow;
	Elements[2] = &Kfast;
	Elements[3] = &NaActivation;
	Elements[4] = &NaInactivation;
	Elements[5] = this;

	// Settle time is 15 milliseconds
	settle_samples = (int)ceil(15.0 * (SampleRate_Hz / 1000.0));

	for (i = 0; i < settle_samples; i++)
	{
		CalculateVoltage();
		Leakage.ReversalPotential_V = (1./Leakage.MaxConductance_Siemens) * ( (Kslow.MaxConductance_Siemens  * Kslow.activation * (InitialVoltage_V - Kfast.ReversalPotential_V) )
			+ (Kfast.MaxConductance_Siemens  * Kfast.activation * (InitialVoltage_V - Kfast.ReversalPotential_V) )
			+ (NaActivation.MaxConductance_Siemens * NaActivation.activation * NaActivation.activation * 
			   NaInactivation.activation * (InitialVoltage_V - NaActivation.ReversalPotential_V) ) ) + InitialVoltage_V;
	}
}

void HHCompartment::CalculateVoltage(void)
{
	int nbad, nok;
	double eps = 1.0e-4;
	double hstart = 10.0e-6;	/* 'Best guess' for time step */
	double hmin = 1.0e-6; /* Minimum time step */
	double x1, x2;	  // Howitt 12Dec01

	Vold_V = Vnew_V; 

	/* Try  ODEINT here , from Numerical Recipes */
	rkSolver.kmax=100;
	x1 = 1.; 
	x2 = 1. + SamplePeriod; 
	rkSolver.dxsav=(x2-x1)/20.0;

	// DJA: Set the present instananeous voltage of the compartment in each of my dependent conductances
	rkSolver.ode_rk(ODECount,x1,x2,eps,hstart,hmin,&nok,&nbad,Elements);
	Kslow.activation = Elements[1]->InitialY;
	Kfast.activation = Elements[2]->InitialY;
	NaActivation.activation = Elements[3]->InitialY;
	NaInactivation.activation = Elements[4]->InitialY;
	Vnew_V = Elements[5]->InitialY;
}

// This assumes that the synapses have already been stepped to the new sample
void HHCompartment::Step(HHSynapse *ExcitatorySynapses[], int ExcitatoryCount, HHSynapse *InhibitorySynapses[], int InhibitoryCount)
{
	if (ExcitatorySynapses != NULL)
		TotalExcitatorySynapses(ExcitatorySynapses, ExcitatoryCount);
	if (InhibitorySynapses != NULL)
		TotalInhibitorySynapses(InhibitorySynapses, InhibitoryCount);

	CalculateVoltage();
}
void HHCompartment::GetActivations(double *Kslow, double *Kfast, double *NaAct, double *NaDeact)
{
	*Kslow = this->Kslow.activation;
	*Kfast = this->Kfast.activation;
	*NaAct = this->NaActivation.activation;
	*NaDeact = this->NaInactivation.activation;
}
