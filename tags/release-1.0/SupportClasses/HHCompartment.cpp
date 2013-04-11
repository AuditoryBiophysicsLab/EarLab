#include "HHChannels.h"
#include "HHCompartment.h"
#include "RungeKuttaEquation.h"
#include "RungeKuttaSolver.h"
#include "math.h"
#include "stdio.h"

HHCompartment::HHCompartment(double SampleRate_Hz, double InitialVoltage_mV, double Temperature_DegreesC) :
				Kslow(InitialVoltage_mV, Temperature_DegreesC),
				Kfast(InitialVoltage_mV, Temperature_DegreesC),
				NaActivation(InitialVoltage_mV, Temperature_DegreesC),
				NaInactivation(InitialVoltage_mV, Temperature_DegreesC),
				Leakage(Temperature_DegreesC)
{
	MembraneCapacitance_uF = 23.0 * 1.0e-6;	 /* Membrane Capacitance (uF) */
	this->InitialVoltage_mV = InitialVoltage_mV;
	Vold_mV = InitialVoltage_mV;
	Vnew_mV = Vold_mV;
	InitialY = InitialVoltage_mV;
	this->SampleRate_Hz = SampleRate_Hz; 
	SamplePeriod = 1.0 / SampleRate_Hz;
	AuxCurrentInput_pA = 0.0;
	ExcitatoryTotal = 0.0;
	InhibitoryTotal = 0.0;
	Settle();
}

double HHCompartment::GetDerivative(double IntermediateY, double InstantaneousVoltage_mV)
{
	double dVdt;

	// All voltages are in milliVolts
	//     Conductances are in Siemens
	//     Currents are in milliAmps (we think)
	dVdt = -(1.0 / MembraneCapacitance_uF) * 
						   ((Kslow.MaxConductance_Siemens * Kslow.IntermediateY * (Vold_mV - Kslow.ReversalPotential_mV)) +
							(Kfast.MaxConductance_Siemens * Kfast.IntermediateY * (Vold_mV - Kfast.ReversalPotential_mV)) +
							(NaActivation.MaxConductance_Siemens * NaActivation.IntermediateY * NaActivation.IntermediateY * 
								(NaInactivation.IntermediateY * (Vold_mV - NaActivation.ReversalPotential_mV))) +
							(Leakage.MaxConductance_Siemens * (Vold_mV - Leakage.ReversalPotential_mV)) + 
							(ExcitatoryTotal * (Vold_mV - Excitatory.ReversalPotential_mV)) + 
							(InhibitoryTotal * (Vold_mV - Inhibitory.ReversalPotential_mV)) +
							(AuxCurrentInput_pA * 1.0e-6));

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

void HHCompartment::SetCurrentInjection_pA(double CurrentInjection_pA)
{
	AuxCurrentInput_pA = CurrentInjection_pA;
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
		Leakage.ReversalPotential_mV = (1./Leakage.MaxConductance_Siemens) * ( (Kslow.MaxConductance_Siemens  * Kslow.activation * (InitialVoltage_mV - Kfast.ReversalPotential_mV) )
			+ (Kfast.MaxConductance_Siemens  * Kfast.activation * (InitialVoltage_mV - Kfast.ReversalPotential_mV) )
			+ (NaActivation.MaxConductance_Siemens * NaActivation.activation * NaActivation.activation * 
			   NaInactivation.activation * (InitialVoltage_mV - NaActivation.ReversalPotential_mV) ) ) + InitialVoltage_mV;
	}
}

void HHCompartment::CalculateVoltage(void)
{
	int nbad, nok;
	double eps = 1.0e-4;
	double hstart = 10.0e-6;	/* 'Best guess' for time step */
	double hmin = 1.0e-6; /* Minimum time step */
	double x1, x2;	  // Howitt 12Dec01

	Vold_mV = Vnew_mV; 

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
	Vnew_mV = Elements[5]->InitialY;
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