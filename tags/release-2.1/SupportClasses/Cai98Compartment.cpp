#include "Rothman93Channels.h"
#include "Brughera96Channels.h"
#include "Cai98Channels.h"
#include "Cai98Compartment.h"
#include "RungeKuttaEquation.h"
#include "RungeKuttaSolver.h"
#include "math.h"
#include "stdio.h"

Cai98Compartment::Cai98Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC) :
	Brughera96Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC)
{
	FirstOrderExponentialDecay = exp(-SamplePeriod / TAU_AHP);
	timerQueue = new TimerQueue(SampleRate_Hz);
	AHPConductance_Siemens = 0.0;
}

double Cai98Compartment::GetDerivative(double IntermediateY, double InstantaneousVoltage_V)
{
	double dVdt;

	// All voltages are in Volts
	//     Conductances are in Siemens
	//     Currents are in Amperes
	dVdt = -(1.0 / MembraneCapacitance_F) * 
						   ((Channels[1]->MaxConductance_Siemens * Channels[1]->IntermediateY * (Vold_V - Channels[1]->ReversalPotential_V)) +	// Kslow
							(Channels[2]->MaxConductance_Siemens * Channels[2]->IntermediateY * (Vold_V - Channels[2]->ReversalPotential_V)) +	// Kfast
							(Channels[3]->MaxConductance_Siemens * Channels[3]->IntermediateY * Channels[3]->IntermediateY *					// NaActivation & NaInactivation
								(Channels[4]->IntermediateY * (Vold_V - Channels[3]->ReversalPotential_V))) +
							(AHPConductance_Siemens * (Vold_V - Channels[2]->ReversalPotential_V)) +											// KAHP (Voltage independent Potassium After Hyperpolarization conductance): From Cai 1998 (JASA) [Vol 103 (1) p. 494]
							(Leakage->MaxConductance_Siemens * (Vold_V - Leakage->ReversalPotential_V)) + 
							(ExcitatoryTotal * (Vold_V - Excitatory.ReversalPotential_V)) + 
							(InhibitoryTotal * (Vold_V - Inhibitory.ReversalPotential_V)) -
							(AuxCurrentInput_A));

	return dVdt;
}

// This assumes that the synapses have already been stepped to the new sample
bool Cai98Compartment::Step(HHSynapse *ExcitatorySynapses[], int ExcitatoryCount, HHSynapse *InhibitorySynapses[], int InhibitoryCount)
{
	int timer_count = 0;
	if (ExcitatorySynapses != NULL)
		TotalExcitatorySynapses(ExcitatorySynapses, ExcitatoryCount);
	if (InhibitorySynapses != NULL)
		TotalInhibitorySynapses(InhibitorySynapses, InhibitoryCount);
	if (timerQueue == NULL)
		throw EarlabException("Cai98Compartment: Error setting timer in Step(): Timer queue does not exist");

	timer_count = timerQueue->Step();
	AHPConductance_Siemens += (AHP_INCREMENT_SIEMENS * timer_count);
	CalculateVoltage();
	AHPConductance_Siemens *= FirstOrderExponentialDecay;	// Perform exponential decay
	if (AHPConductance_Siemens < 1e-24)
		AHPConductance_Siemens = 0;							// Make sure we don't venture into the "denormal" zone for floating point calculations

	if (Vnew_V >= ThresholdVoltage_Volts)
	{
		if (WasUnderThreshold)
		{
			WasUnderThreshold = false;
			// DJA: Start a 500 uS timer, when this timer expires, increment the AHP conductance by AHP_INCREMENT_SIEMENS
			timerQueue->Add(new Timer(500e-6));
			return true;	// This indicates a new action potential has occurred in this time step
		}
	}
	else
	{
		WasUnderThreshold = true;
	}

	return false;			// No new action potential has occurred in this time step
}

