#include "Rothman03Channels.h"
#include "Rothman03Compartment.h"
#include "RungeKuttaEquation.h"
#include "RungeKuttaSolver.h"
#include "math.h"
#include "stdio.h"

Rothman03Compartment::Rothman03Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC)
{
	SetSampleRate_Hz(SampleRate_Hz);
	this->InitialVoltage_V = InitialVoltage_V;
	this->Temperature_DegreesC = Temperature_DegreesC;

	NumHHChannels = 10;
	Channels = new HHChannelPopulation *[NumHHChannels + 1];
	Channels[ 1] = new Rothman03KFastTransient_a(InitialVoltage_V, Temperature_DegreesC);
	Channels[ 2] = new Rothman03KFastTransient_b(InitialVoltage_V, Temperature_DegreesC);
	Channels[ 3] = new Rothman03KFastTransient_c(InitialVoltage_V, Temperature_DegreesC);
	Channels[ 4] = new Rothman03KLT_w(InitialVoltage_V, Temperature_DegreesC);
	Channels[ 5] = new Rothman03KLT_z(InitialVoltage_V, Temperature_DegreesC);
	Channels[ 6] = new Rothman03KHT_n(InitialVoltage_V, Temperature_DegreesC);
	Channels[ 7] = new Rothman03KHT_p(InitialVoltage_V, Temperature_DegreesC);
	Channels[ 8] = new Rothman03NaFast_m(InitialVoltage_V, Temperature_DegreesC);
	Channels[ 9] = new Rothman03NaFast_h(InitialVoltage_V, Temperature_DegreesC);
	Channels[10] = new Rothman03CATIONh_r(InitialVoltage_V, Temperature_DegreesC);

	for (int i = 1; i <= NumHHChannels; i++)
		Channels[i]->SetCompartment(this);

	Initialize();
}

void Rothman03Compartment::Initialize(void)
{
	Leakage = new Rothman03Leakage(Temperature_DegreesC);
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

double Rothman03Compartment::GetDerivative(double IntermediateY, double InstantaneousVoltage_V)
{
	double dVdt;
	double tmpa, tmpw, tmpn, tmpm;
	const double phi = 0.85;

	// All voltages are in Volts
	//     Conductances are in Siemens
	//     Currents are in Amperes
	// Rothman03ModelParameters
	tmpa = Channels[1]->IntermediateY * Channels[1]->IntermediateY * Channels[1]->IntermediateY * Channels[1]->IntermediateY;
	tmpw = Channels[4]->IntermediateY * Channels[4]->IntermediateY * Channels[4]->IntermediateY * Channels[4]->IntermediateY;
	tmpn = Channels[6]->IntermediateY * Channels[6]->IntermediateY;
	tmpm = Channels[8]->IntermediateY * Channels[8]->IntermediateY * Channels[8]->IntermediateY;

	dVdt = -(1.0 / MembraneCapacitance_F) * (
		(Channels[3]->MaxConductance_Siemens * tmpa * Channels[2]->IntermediateY * Channels[3]->IntermediateY * (InstantaneousVoltage_V - Channels[3]->ReversalPotential_V)) +
		(Channels[5]->MaxConductance_Siemens * tmpw * Channels[5]->IntermediateY * (InstantaneousVoltage_V - Channels[5]->ReversalPotential_V)) + 
		(Channels[7]->MaxConductance_Siemens * (phi * tmpn + (1 - phi) * Channels[7]->IntermediateY) * (InstantaneousVoltage_V - Channels[7]->ReversalPotential_V)) + 
		(Channels[9]->MaxConductance_Siemens * tmpm * Channels[9]->IntermediateY * (InstantaneousVoltage_V - Channels[9]->ReversalPotential_V)) + 
		(Channels[10]->MaxConductance_Siemens * Channels[10]->IntermediateY * (InstantaneousVoltage_V - Channels[10]->ReversalPotential_V)) + 
		(Channels[2]->MaxConductance_Siemens * (InstantaneousVoltage_V - lkReversalPotential_V)));
	return dVdt;
}

void Rothman03Compartment::Settle(void)
{
	int settle_samples, i;

	for (i = 1; i <= NumHHChannels; i++)
		Elements[i] = Channels[i];
	Elements[i] = this;

	// Settle time is 15 milliseconds
	settle_samples = (int)ceil(15.0 * (SampleRate_Hz / 1000.0));

	for (i = 0; i < settle_samples; i++)
		CalculateVoltage();
}
