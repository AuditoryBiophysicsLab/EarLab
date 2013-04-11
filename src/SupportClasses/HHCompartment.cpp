#include "HHChannelPopulation.h"
#include "HHCompartment.h"
#include "math.h"
#include "stdio.h"

HHCompartment::HHCompartment(int NumConductances, double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC)
{
	this->NumConductances = NumConductances;
	SetSampleRate_Hz(SampleRate_Hz);
	this->InitialVoltage_V = InitialVoltage_V;
	this->ThresholdVoltage_Volts = -25e-3;
	this->Temperature_DegreesC = Temperature_DegreesC;

	ConductanceInfo = new HHConductanceInfo *[this->NumConductances];
	for (int i = 0; i < NumConductances; i++)
		ConductanceInfo[i] = new HHConductanceInfo();
}

void HHCompartment::SetTemperature_DegreesC(double Temperature_DegreesC)
{
	int i;

	for (i = 1; i <= NumHHChannels; i++)
			Channels[i]->SetTemperature_DegreesC(Temperature_DegreesC);
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

void HHCompartment::CalculateVoltage(void)
{
	int nbad, nok;
	double eps = 1.0e-4;
	double hstart = 10.0e-6;	/* 'Best guess' for time step */
	double hmin = 1.0e-6; /* Minimum time step */
	double x1, x2;	  // Howitt 12Dec01
	int i;

	Vold_V = Vnew_V; 

	/* Try  ODEINT here , from Numerical Recipes */
	rkSolver.kmax=100;
	x1 = 1.; 
	x2 = 1. + SamplePeriod; 
	rkSolver.dxsav=(x2-x1)/20.0;

	// DJA: Set the present instananeous voltage of the compartment in each of my dependent conductances
	// Note: NumHHChannels is the number of HH Channels in the compartment, plus one for the compartment
	rkSolver.ode_rk(NumHHChannels + 1,x1,x2,eps,hstart,hmin,&nok,&nbad,Elements);
	for (i = 1; i <= NumHHChannels; i++)
			Channels[i]->activation = Elements[i]->InitialY;
	Vnew_V = Elements[i]->InitialY;
}

// This assumes that the synapses have already been stepped to the new sample
bool HHCompartment::Step(HHSynapse *ExcitatorySynapses[], int ExcitatoryCount, HHSynapse *InhibitorySynapses[], int InhibitoryCount)
{
	if (ExcitatorySynapses != NULL)
		TotalExcitatorySynapses(ExcitatorySynapses, ExcitatoryCount);
	if (InhibitorySynapses != NULL)
		TotalInhibitorySynapses(InhibitorySynapses, InhibitoryCount);

	CalculateVoltage();

	CalculateConductances();

	if (Vnew_V >= ThresholdVoltage_Volts)
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

double HHCompartment::GetActivations(int ChannelIndex)
{
	if ((ChannelIndex < 1) || (ChannelIndex <= NumHHChannels))
		return Channels[ChannelIndex]->activation;
	else
		throw EarlabException("HHCompartment::GetActivations: Channel Index (%d) out of range", ChannelIndex);
}
