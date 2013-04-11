#include "Rothman93Compartment.h"
#include "Rothman93Channels.h"
#include "RungeKuttaEquation.h"
#include "RungeKuttaSolver.h"
#include "math.h"
#include "stdio.h"

Rothman93Compartment::Rothman93Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC) 
{
	SetSampleRate_Hz(SampleRate_Hz);
	this->InitialVoltage_V = InitialVoltage_V;
	this->Temperature_DegreesC = Temperature_DegreesC;

	NumHHChannels = 4;
	Channels = new HHChannelPopulation *[NumHHChannels + 1];
	Channels[1] = new Rothman93Kslow(InitialVoltage_V, Temperature_DegreesC);
	Channels[2] = new Rothman93Kfast(InitialVoltage_V, Temperature_DegreesC);
	Channels[3] = new Rothman93NaActivation(InitialVoltage_V, Temperature_DegreesC);
	Channels[4] = new Rothman93NaInactivation(InitialVoltage_V, Temperature_DegreesC);

	for (int i = 1; i <= NumHHChannels; i++)
		Channels[i]->SetCompartment(this);
	Initialize();
}

void Rothman93Compartment::Initialize(void)
{
	Leakage = new Rothman93Leakage(Temperature_DegreesC);
	Leakage->SetCompartment(this);
	
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

double Rothman93Compartment::GetDerivative(double IntermediateY, double InstantaneousVoltage_V)
{
	double dVdt;

	// All voltages are in Volts
	//     Conductances are in Siemens
	//     Currents are in Amperes
	dVdt = -(1.0 / MembraneCapacitance_F) * 
						   ((Channels[1]->GetCurrent_Amperes()) +	// Kslow
							(Channels[2]->GetCurrent_Amperes()) +	// Kfast
							(Channels[3]->MaxConductance_Siemens * Channels[3]->IntermediateY * Channels[3]->IntermediateY *					// NaActivation & NaInactivation
								(Channels[4]->IntermediateY * (Vold_V - Channels[3]->ReversalPotential_V))) +
							(Leakage->GetCurrent_Amperes()) + 
							(ExcitatoryTotal * (Vold_V - Excitatory.ReversalPotential_V)) + 
							(InhibitoryTotal * (Vold_V - Inhibitory.ReversalPotential_V)) -
							(AuxCurrentInput_A));

	return dVdt;
}

void Rothman93Compartment::Settle(void)
{
	int settle_samples, i;

	for (i = 1; i <= NumHHChannels; i++)
		Elements[i] = Channels[i];
	Elements[i] = this;

	// Settle time is 15 milliseconds
	settle_samples = (int)ceil(15.0 * (SampleRate_Hz / 1000.0));

	for (i = 0; i < settle_samples; i++)
	{
		CalculateVoltage();
		Leakage->ReversalPotential_V = (1./Leakage->MaxConductance_Siemens) * ( (Channels[1]->MaxConductance_Siemens  * Channels[1]->activation * (InitialVoltage_V - Channels[2]->ReversalPotential_V) )
			+ (Channels[2]->MaxConductance_Siemens  * Channels[2]->activation * (InitialVoltage_V - Channels[2]->ReversalPotential_V) )
			+ (Channels[3]->MaxConductance_Siemens * Channels[3]->activation * Channels[3]->activation * 
			   Channels[4]->activation * (InitialVoltage_V - Channels[3]->ReversalPotential_V) ) ) + InitialVoltage_V;
	}
}
