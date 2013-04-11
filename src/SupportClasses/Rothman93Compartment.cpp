#include "Rothman93Compartment.h"
#include "math.h"
#include "stdio.h"

Rothman93Compartment::Rothman93Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC)
: HHCompartment(NUM_R93_CONDUCTANCES, SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC)
{
	this->ResponseType = TYPE_II;

	Initialize();
}

Rothman93Compartment::Rothman93Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType)
: HHCompartment(NUM_R93_CONDUCTANCES, SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC)
{
	this->ResponseType = ResponseType;

	Initialize();
}

Rothman93Compartment::Rothman93Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, int AdditionalConductances)
: HHCompartment(NUM_R93_CONDUCTANCES + AdditionalConductances, SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC)
{
	this->ResponseType = TYPE_II;

	Initialize();
}

Rothman93Compartment::Rothman93Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType, int AdditionalConductances)
: HHCompartment(NUM_R93_CONDUCTANCES + AdditionalConductances, SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC)
{
	this->ResponseType = ResponseType;

	Initialize();
}


void Rothman93Compartment::Initialize(void)
{
	NumHHChannels = 4;
	Channels = new HHChannelPopulation *[NumHHChannels + 1];
	Channels[1] = new Rothman93Kslow(InitialVoltage_V, Temperature_DegreesC);
	Channels[2] = new Rothman93Kfast(InitialVoltage_V, Temperature_DegreesC);
	Channels[3] = new Rothman93NaActivation(InitialVoltage_V, Temperature_DegreesC);
	Channels[4] = new Rothman93NaInactivation(InitialVoltage_V, Temperature_DegreesC);

	for (int i = 1; i <= NumHHChannels; i++)
		Channels[i]->SetCompartment(this);

	strcpy(ConductanceInfo[0]->ConductanceName, "ge");
	strcpy(ConductanceInfo[1]->ConductanceName, "gi");
	strcpy(ConductanceInfo[2]->ConductanceName, "gNa");
	strcpy(ConductanceInfo[3]->ConductanceName, "gKfast");
	strcpy(ConductanceInfo[4]->ConductanceName, "gKslow");

	Leakage = new Rothman93Leakage(Temperature_DegreesC);
	Leakage->SetCompartment(this);
	
	//MembraneCapacitance_F = 23e-12;	/* Membrane Capacitance (Farads) */
										// 23pF from Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2564]

	//MembraneCapacitance_F = 12e-12;	// 12pF good for Type I current clamp response with adaptation
									// as in Cai 1998 (II), together with GmaxNa=1000ns, GmaxKHT=150nS, GmaxKLT=0.
									// For lower conductances, e.g. GmaxNa=325nS, GmaxKHT=100nS, GmaxKLT=0,
									// lower values of membrane capacitance (5-8pF) were required  
									// to avoid depolarization block at higher depolarizing currents.
	if (this->ResponseType == TYPE_I)
		MembraneCapacitance_F = 12e-12;	/* Membrane Capacitance (Farads) */
	else MembraneCapacitance_F = 23e-12;

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

void Rothman93Compartment::CalculateConductances(void)
{
	float ge, gi, gNa, gKfast, gKslow;

	ge = (float) (GetExcitatoryConductance_Siemens() * 1e9);
	gi = (float) (GetInhibitoryConductance_Siemens() * 1e9);
	gNa = (float) ((Channels[3]->MaxConductance_Siemens * Channels[3]->IntermediateY * Channels[3]->IntermediateY *					// NaActivation & NaInactivation
			Channels[4]->IntermediateY) * 1e9);
	gKfast = (float) ((Channels[2]->MaxConductance_Siemens * Channels[2]->IntermediateY) * 1e9);
	gKslow = (float) ((Channels[1]->MaxConductance_Siemens * Channels[1]->IntermediateY) * 1e9);

	ConductanceInfo[0]->Conductance_nS = ge;
	ConductanceInfo[1]->Conductance_nS = gi;
	ConductanceInfo[2]->Conductance_nS = gNa;
	ConductanceInfo[3]->Conductance_nS = gKfast;
	ConductanceInfo[4]->Conductance_nS = gKslow;
}

#if 0
void Rothman93Compartment::GetConductances_nS(float &ge, float &gi, float &gNa, float &gKfast, float &gKslow)
{
	ge = (float) (GetExcitatoryConductance_Siemens() * 1e9);
	gi = (float) (GetInhibitoryConductance_Siemens() * 1e9);
	gNa = (float) ((Channels[3]->MaxConductance_Siemens * Channels[3]->IntermediateY * Channels[3]->IntermediateY *					// NaActivation & NaInactivation
			Channels[4]->IntermediateY) * 1e9);
	gKfast = (float) ((Channels[2]->MaxConductance_Siemens * Channels[2]->IntermediateY) * 1e9);
	gKslow = (float) ((Channels[1]->MaxConductance_Siemens * Channels[1]->IntermediateY) * 1e9);
}
#endif