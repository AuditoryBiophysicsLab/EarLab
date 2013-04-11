//#include "Rothman93AHPChannels.h"
#include "Rothman93AHPCompartment.h"
#include "RungeKuttaEquation.h"
#include "RungeKuttaSolver.h"
#include "math.h"
#include "stdio.h"

Rothman93AHPCompartment::Rothman93AHPCompartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC) :
	Rothman93Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC)
{
	mTauAHP_Seconds = 1/SampleRate_Hz;  // choose a short time constant for the default no-adaptation case.
	mAHPIncrement_Siemens = 0.0e-9;
	FirstOrderExponentialDecay = exp(-SamplePeriod / mTauAHP_Seconds);
	timerQueue = new TimerQueue(SampleRate_Hz);
	AHPConductance_Siemens = 0.0;
}

Rothman93AHPCompartment::Rothman93AHPCompartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, double TauAHP_Seconds, double AHPIncrement_Siemens) :
	Rothman93Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC)
{
	mTauAHP_Seconds = TauAHP_Seconds;
	mAHPIncrement_Siemens = AHPIncrement_Siemens;
	FirstOrderExponentialDecay = exp(-SamplePeriod / mTauAHP_Seconds);
	timerQueue = new TimerQueue(SampleRate_Hz);
	AHPConductance_Siemens = 0.0;
}

Rothman93AHPCompartment::Rothman93AHPCompartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType) :
	Rothman93Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, ResponseType)
{
	mTauAHP_Seconds = 1/SampleRate_Hz;  // choose a short time constant for the default no-adaptation case.
	mAHPIncrement_Siemens = 0.0e-9;
	FirstOrderExponentialDecay = exp(-SamplePeriod / mTauAHP_Seconds);
	timerQueue = new TimerQueue(SampleRate_Hz);
	AHPConductance_Siemens = 0.0;
}

Rothman93AHPCompartment::Rothman93AHPCompartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType, double TauAHP_Seconds, double AHPIncrement_Siemens) :
	Rothman93Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, ResponseType)
{
	mTauAHP_Seconds = TauAHP_Seconds;
	mAHPIncrement_Siemens = AHPIncrement_Siemens;
	FirstOrderExponentialDecay = exp(-SamplePeriod / mTauAHP_Seconds);
	timerQueue = new TimerQueue(SampleRate_Hz);
	AHPConductance_Siemens = 0.0;
}

double Rothman93AHPCompartment::GetDerivative(double IntermediateY, double InstantaneousVoltage_V)
{
	double dVdt;

	// All voltages are in Volts
	//     Conductances are in Siemens
	//     Currents are in Amperes
	dVdt = -(1.0 / MembraneCapacitance_F) * 
						   ((Channels[1]->GetCurrent_Amperes())		// Kslow
							+ (Channels[2]->GetCurrent_Amperes())	// Kfast
							+ (Channels[3]->MaxConductance_Siemens * Channels[3]->IntermediateY * Channels[3]->IntermediateY *	// NaActivation & NaInactivation
								(Channels[4]->IntermediateY * (Vold_V - Channels[3]->ReversalPotential_V)))
							+ (Leakage->GetCurrent_Amperes())
							+ (ExcitatoryTotal * (Vold_V - Excitatory.ReversalPotential_V)) 
							+ (InhibitoryTotal * (Vold_V - Inhibitory.ReversalPotential_V))
							+ (AHPConductance_Siemens * (Vold_V - Channels[2]->ReversalPotential_V)) 	// Compute i_AHP						
							- (AuxCurrentInput_A));

	// Compute i_AHP
	// KAHP (Voltage independent Potassium After Hyperpolarization conductance):\
	// From Cai 1998 (JASA) [Vol 103 (1) p. 494]
	// i_AHP = (AHPConductance_Siemens * (V - VKHT));
	// i_AHP = (AHPConductance_Siemens * (Vold_V - Channels[2]->ReversalPotential_V));

	return dVdt;
}

// This assumes that the synapses have already been stepped to the new sample
bool Rothman93AHPCompartment::Step(HHSynapse *ExcitatorySynapses[], int ExcitatoryCount, HHSynapse *InhibitorySynapses[], int InhibitoryCount)
{
	int timer_count = 0;
	if (ExcitatorySynapses != NULL)
		TotalExcitatorySynapses(ExcitatorySynapses, ExcitatoryCount);
	if (InhibitorySynapses != NULL)
		TotalInhibitorySynapses(InhibitorySynapses, InhibitoryCount);
	if (timerQueue == NULL)
		throw EarlabException("Rothman93AHPCompartment: Error setting timer in Step(): Timer queue does not exist");

	timer_count = timerQueue->Step();
	AHPConductance_Siemens += (mAHPIncrement_Siemens * timer_count);
	CalculateVoltage();
	AHPConductance_Siemens *= FirstOrderExponentialDecay;	// Perform exponential decay
	if (AHPConductance_Siemens < 1e-24)
		AHPConductance_Siemens = 0;							// Make sure we don't venture into the "denormal" zone for floating point calculations

	if (Vnew_V >= ThresholdVoltage_Volts)
	{
		if (WasUnderThreshold)
		{
			WasUnderThreshold = false;
			// DJA: Start a 500 uS timer, when this timer expires, increment the AHP conductance by mAHPIncrement_Siemens
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
