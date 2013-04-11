#include "Rothman03Compartment.h"
#include "math.h"
#include "stdio.h"

Rothman03Compartment::Rothman03Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC)
: HHCompartment(NUM_R03_CONDUCTANCES, SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC)
{
	SetSampleRate_Hz(SampleRate_Hz);
	this->InitialVoltage_V = InitialVoltage_V;
	this->Temperature_DegreesC = Temperature_DegreesC;
	//this->ResponseType = TYPEZHOU2006;  // TYPEZHOU2006 used in collaboration with Zhou.
	this->ResponseType = TYPE2;  // TYPE2 is the standard default ResponseType
	//this->ResponseType = TYPE21;  // TYPE21 tested with BrugheraMSO for unmodulated stimuli

	Initialize();
}
Rothman03Compartment::Rothman03Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType)
: HHCompartment(NUM_R03_CONDUCTANCES, SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC)
{
	SetSampleRate_Hz(SampleRate_Hz);
	this->InitialVoltage_V = InitialVoltage_V;
	this->Temperature_DegreesC = Temperature_DegreesC;
	this->ResponseType = ResponseType;

	Initialize();
}

Rothman03Compartment::Rothman03Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues)
: HHCompartment(NUM_R03_CONDUCTANCES, SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC)
{
	SetSampleRate_Hz(SampleRate_Hz);
	this->InitialVoltage_V = InitialVoltage_V;
	this->Temperature_DegreesC = Temperature_DegreesC;
	this->ResponseType = ResponseType;
	this->CustomParameterValues = MyCustomParameterValues;
	Initialize(MyCustomParameterValues);
}

Rothman03Compartment::Rothman03Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, int AdditionalConductances)
: HHCompartment(NUM_R03_CONDUCTANCES + AdditionalConductances, SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC)
{
	SetSampleRate_Hz(SampleRate_Hz);
	this->InitialVoltage_V = InitialVoltage_V;
	this->Temperature_DegreesC = Temperature_DegreesC;
	//this->ResponseType = TYPEZHOU2006;  // TYPEZHOU2006 used in collaboration with Zhou.
	this->ResponseType = TYPE2;  // TYPE2 is the standard default ResponseType
	//this->ResponseType = TYPE21;  // TYPE21 tested with BrugheraMSO for unmodulated stimuli

	Initialize();
}
Rothman03Compartment::Rothman03Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, int AdditionalConductances)
: HHCompartment(NUM_R03_CONDUCTANCES + AdditionalConductances, SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC)
{
	SetSampleRate_Hz(SampleRate_Hz);
	this->InitialVoltage_V = InitialVoltage_V;
	this->Temperature_DegreesC = Temperature_DegreesC;
	this->ResponseType = ResponseType;

	Initialize();
}

Rothman03Compartment::Rothman03Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, int AdditionalConductances, Rothman03CustomParameterValues MyCustomParameterValues)
: HHCompartment(NUM_R03_CONDUCTANCES + AdditionalConductances, SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC)
{
	SetSampleRate_Hz(SampleRate_Hz);
	this->InitialVoltage_V = InitialVoltage_V;
	this->Temperature_DegreesC = Temperature_DegreesC;
	this->ResponseType = ResponseType;

	Initialize(MyCustomParameterValues);
}


void Rothman03Compartment::Initialize(void)
{
	NumHHChannels = 10;
	Channels = new HHChannelPopulation *[NumHHChannels + 1];
	Channels[ 1] = new Rothman03KFastTransient_a(InitialVoltage_V, Temperature_DegreesC, this->ResponseType);
	Channels[ 2] = new Rothman03KFastTransient_b(InitialVoltage_V, Temperature_DegreesC, this->ResponseType);
	Channels[ 3] = new Rothman03KFastTransient_c(InitialVoltage_V, Temperature_DegreesC, this->ResponseType);
	Channels[ 4] = new Rothman03KLT_w(InitialVoltage_V, Temperature_DegreesC, this->ResponseType);
	Channels[ 5] = new Rothman03KLT_z(InitialVoltage_V, Temperature_DegreesC, this->ResponseType);
	Channels[ 6] = new Rothman03KHT_n(InitialVoltage_V, Temperature_DegreesC, this->ResponseType);
	Channels[ 7] = new Rothman03KHT_p(InitialVoltage_V, Temperature_DegreesC, this->ResponseType);
	Channels[ 8] = new Rothman03NaFast_m(InitialVoltage_V, Temperature_DegreesC, this->ResponseType);
	Channels[ 9] = new Rothman03NaFast_h(InitialVoltage_V, Temperature_DegreesC, this->ResponseType);
	Channels[10] = new Rothman03CATIONh_r(InitialVoltage_V, Temperature_DegreesC, this->ResponseType);

	for (int i = 1; i <= NumHHChannels; i++)
		Channels[i]->SetCompartment(this);

	strcpy(ConductanceInfo[0]->ConductanceName, "ge");
	strcpy(ConductanceInfo[1]->ConductanceName, "gi");
	strcpy(ConductanceInfo[2]->ConductanceName, "gNa");
	strcpy(ConductanceInfo[3]->ConductanceName, "gKA");
	strcpy(ConductanceInfo[4]->ConductanceName, "gKHT");
	strcpy(ConductanceInfo[5]->ConductanceName, "gKLT");
	strcpy(ConductanceInfo[6]->ConductanceName, "gh");

	Leakage = new Rothman03Leakage(Temperature_DegreesC, ResponseType);
	Leakage->SetCompartment(this);
	
	if (this->ResponseType == TYPEZHOU2006)
		MembraneCapacitance_F = 25.1327e-12;	/* for Zhou06ParameterSet_MSO only */
	else
	{
		if (this->ResponseType == CUSTOMTYPE)
			MembraneCapacitance_F = this->CustomParameterValues.MembraneCapacitance_F;
		else
			MembraneCapacitance_F = 12e-12;		/* Membrane Capacitance (Farads) */
	}

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

void Rothman03Compartment::Initialize(Rothman03CustomParameterValues MyCustomParameterValues)
{
	NumHHChannels = 10;
	Channels = new HHChannelPopulation *[NumHHChannels + 1];
	Channels[ 1] = new Rothman03KFastTransient_a(InitialVoltage_V, Temperature_DegreesC, this->ResponseType, MyCustomParameterValues);
	Channels[ 2] = new Rothman03KFastTransient_b(InitialVoltage_V, Temperature_DegreesC, this->ResponseType, MyCustomParameterValues);
	Channels[ 3] = new Rothman03KFastTransient_c(InitialVoltage_V, Temperature_DegreesC, this->ResponseType, MyCustomParameterValues);
	Channels[ 4] = new Rothman03KLT_w(InitialVoltage_V, Temperature_DegreesC, this->ResponseType, MyCustomParameterValues);
	Channels[ 5] = new Rothman03KLT_z(InitialVoltage_V, Temperature_DegreesC, this->ResponseType, MyCustomParameterValues);
	Channels[ 6] = new Rothman03KHT_n(InitialVoltage_V, Temperature_DegreesC, this->ResponseType, MyCustomParameterValues);
	Channels[ 7] = new Rothman03KHT_p(InitialVoltage_V, Temperature_DegreesC, this->ResponseType, MyCustomParameterValues);
	Channels[ 8] = new Rothman03NaFast_m(InitialVoltage_V, Temperature_DegreesC, this->ResponseType, MyCustomParameterValues);
	Channels[ 9] = new Rothman03NaFast_h(InitialVoltage_V, Temperature_DegreesC, this->ResponseType, MyCustomParameterValues);
	Channels[10] = new Rothman03CATIONh_r(InitialVoltage_V, Temperature_DegreesC, this->ResponseType, MyCustomParameterValues);

	for (int i = 1; i <= NumHHChannels; i++)
		Channels[i]->SetCompartment(this);

	strcpy(ConductanceInfo[0]->ConductanceName, "ge");
	strcpy(ConductanceInfo[1]->ConductanceName, "gi");
	strcpy(ConductanceInfo[2]->ConductanceName, "gNa");
	strcpy(ConductanceInfo[3]->ConductanceName, "gKA");
	strcpy(ConductanceInfo[4]->ConductanceName, "gKHT");
	strcpy(ConductanceInfo[5]->ConductanceName, "gKLT");
	strcpy(ConductanceInfo[6]->ConductanceName, "gh");

	Leakage = new Rothman03Leakage(Temperature_DegreesC, ResponseType, MyCustomParameterValues);
	Leakage->SetCompartment(this);
	
	if (this->ResponseType == TYPEZHOU2006)
		MembraneCapacitance_F = 25.1327e-12;	/* for Zhou06ParameterSet_MSO only */
	else
	{
		if (this->ResponseType == CUSTOMTYPE)
			MembraneCapacitance_F = this->CustomParameterValues.MembraneCapacitance_F;
		else
			MembraneCapacitance_F = 12e-12;		/* Membrane Capacitance (Farads) */
	}

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
// double Rothman03Compartment::GetDerivative(double IntermediateY, double InstantaneousVoltage_V)
// {
//	double dVdt;
//	double tmpa, tmpw, tmpn, tmpm;
//	const double phi = 0.85;
//
//	// All voltages are in Volts
//	//     Conductances are in Siemens
//	//     Currents are in Amperes
//	// Rothman03ModelParameters
//	tmpa = Channels[1]->IntermediateY * Channels[1]->IntermediateY * Channels[1]->IntermediateY * Channels[1]->IntermediateY;
//	tmpw = Channels[4]->IntermediateY * Channels[4]->IntermediateY * Channels[4]->IntermediateY * Channels[4]->IntermediateY;
//	tmpn = Channels[6]->IntermediateY * Channels[6]->IntermediateY;
//	tmpm = Channels[8]->IntermediateY * Channels[8]->IntermediateY * Channels[8]->IntermediateY;
//
//	dVdt = -(1.0 / MembraneCapacitance_F) * (
//		(Channels[3]->MaxConductance_Siemens * tmpa * Channels[2]->IntermediateY * Channels[3]->IntermediateY * (InstantaneousVoltage_V - Channels[3]->ReversalPotential_V)) +
//		(Channels[5]->MaxConductance_Siemens * tmpw * Channels[5]->IntermediateY * (InstantaneousVoltage_V - Channels[5]->ReversalPotential_V)) + 
//		(Channels[7]->MaxConductance_Siemens * (phi * tmpn + (1 - phi) * Channels[7]->IntermediateY) * (InstantaneousVoltage_V - Channels[7]->ReversalPotential_V)) + 
//		(Channels[9]->MaxConductance_Siemens * tmpm * Channels[9]->IntermediateY * (InstantaneousVoltage_V - Channels[9]->ReversalPotential_V)) + 
//		(Channels[10]->MaxConductance_Siemens * Channels[10]->IntermediateY * (InstantaneousVoltage_V - Channels[10]->ReversalPotential_V)) + 
//		(Channels[2]->MaxConductance_Siemens * (InstantaneousVoltage_V - lkReversalPotential_V)));
//	return dVdt;
// }

	double dVdt;

	// All voltages are in Volts
	//     Conductances are in Siemens
	//     Currents are in Amperes
	// Rothman03ModelParameters

	// Brughera 2006/09/12
	// Simplified variable names match those in the Rothman and Manis 2003 paper,
	// and allow for a readable membrane current equation:
	//
    // Instantaneous membrane voltage use by RungeKuttaSolver
	double V;
	// ion-channel activiation parameters
	double a4, b, c, w4, z, n2, p, m3, m2, h, r;
	// maximum conductance for ion channels
	double gmaxKA, gmaxKLT, gmaxKHT, gmaxNa, gmaxh;
	// reversal potentials for ion channels (typically all VKs will be equal)
	double VKA, VKLT, VKHT, VNa, Vh;
	// additional parameter for KHT
	const double phi = 0.85;
	// leakage current variable (primarily for viewing during debug)
	double i_LEAK;

	// membrane voltage
	V = InstantaneousVoltage_V;

	// activation parameter values (number indicates exponent, a4 is a^4)
	a4 = Channels[1]->IntermediateY * Channels[1]->IntermediateY * Channels[1]->IntermediateY * Channels[1]->IntermediateY;
	b  = Channels[2]->IntermediateY;
	c  = Channels[3]->IntermediateY;
	w4 = Channels[4]->IntermediateY * Channels[4]->IntermediateY * Channels[4]->IntermediateY * Channels[4]->IntermediateY;
	z  = Channels[5]->IntermediateY;
	n2 = Channels[6]->IntermediateY * Channels[6]->IntermediateY;
	p  = Channels[7]->IntermediateY;
	m3 = Channels[8]->IntermediateY * Channels[8]->IntermediateY * Channels[8]->IntermediateY;
	m2 = Channels[8]->IntermediateY * Channels[8]->IntermediateY;
	h  = Channels[9]->IntermediateY;
	r  = Channels[10]->IntermediateY;

	// maximum conductance values for the ion channels
	gmaxKA  = Channels[1]->MaxConductance_Siemens; // for fast transient K+ current
	gmaxKLT = Channels[4]->MaxConductance_Siemens; // for low-threshold K+ current
	gmaxKHT = Channels[6]->MaxConductance_Siemens; // for high-threshold K+ current 
	gmaxNa = Channels[8]->MaxConductance_Siemens; // for fast Na+ current
	gmaxh  = Channels[10]->MaxConductance_Siemens; // for hyperpolarization-activated cation current

	// corresponding reversal potentials for the ion channels
	VKA  = Channels[1]->ReversalPotential_V;
	VKLT = Channels[4]->ReversalPotential_V;
	VKHT = Channels[6]->ReversalPotential_V;
	VNa = Channels[8]->ReversalPotential_V;
	Vh  = Channels[10]->ReversalPotential_V;

	// Compute leakage current
	i_LEAK = Leakage->GetCurrent_Amperes();

	// the membrane current equation divided on both sides by MembraneCapacitance_F
	// provides a differential equation in membrane voltage

	if (this->ResponseType == TYPEZHOU2006) // use m2 in place of m3
		dVdt = -(1.0 / MembraneCapacitance_F) * (
				(gmaxKA * a4 * b * c * (V - VKA))
			  + (gmaxKLT * w4 * z * (V - VKLT))
			  + (gmaxKHT * (phi * n2 + (1 - phi) * p) * (V - VKHT))
			  + (gmaxNa * m2 * h * (V - VNa))
			  + (gmaxh * r * (V - Vh))
			  + (i_LEAK)
			  + (ExcitatoryTotal * (V - Excitatory.ReversalPotential_V))
			  + (InhibitoryTotal * (V - Inhibitory.ReversalPotential_V))
			  - (AuxCurrentInput_A)
			  );
	else
		dVdt = -(1.0 / MembraneCapacitance_F) * (
			    (gmaxKA * a4 * b * c * (V - VKA))
			  + (gmaxKLT * w4 * z * (V - VKLT))
			  + (gmaxKHT * (phi * n2 + (1 - phi) * p) * (V - VKHT))
			  + (gmaxNa * m3 * h * (V - VNa))
			  + (gmaxh * r * (V - Vh))
			  + (i_LEAK)
			  + (ExcitatoryTotal * (V - Excitatory.ReversalPotential_V))
			  + (InhibitoryTotal * (V - Inhibitory.ReversalPotential_V))
			  - (AuxCurrentInput_A)
			  );

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

// put the conductance evaluations in GetDerivative, store them in HHConductanceInfo **ConductanceInfo
#if 0
void Rothman03Compartment::GetConductances_nS(float &ge, float &gi, float &gNa, float &gKA, float &gKHT, float &gKLT, float &gh)
{
	// ion-channel activiation parameters
	double a4, b, c, w4, z, n2, p, m3, m2, h, r;
	// maximum conductance for ion channels
	double gmaxKA, gmaxKLT, gmaxKHT, gmaxNa, gmaxh;
	// additional parameter for KHT
	const double phi = 0.85;

	// activation parameter values (number indicates exponent, a4 is a^4)
	a4 = Channels[1]->IntermediateY * Channels[1]->IntermediateY * Channels[1]->IntermediateY * Channels[1]->IntermediateY;
	b  = Channels[2]->IntermediateY;
	c  = Channels[3]->IntermediateY;
	w4 = Channels[4]->IntermediateY * Channels[4]->IntermediateY * Channels[4]->IntermediateY * Channels[4]->IntermediateY;
	z  = Channels[5]->IntermediateY;
	n2 = Channels[6]->IntermediateY * Channels[6]->IntermediateY;
	p  = Channels[7]->IntermediateY;
	m3 = Channels[8]->IntermediateY * Channels[8]->IntermediateY * Channels[8]->IntermediateY;
	m2 = Channels[8]->IntermediateY * Channels[8]->IntermediateY;
	h  = Channels[9]->IntermediateY;
	r  = Channels[10]->IntermediateY;

	// maximum conductance values for the ion channels
	gmaxNa = Channels[8]->MaxConductance_Siemens; // for fast Na+ current
	gmaxKA  = Channels[1]->MaxConductance_Siemens; // for fast transient K+ current
	gmaxKLT = Channels[4]->MaxConductance_Siemens; // for low-threshold K+ current
	gmaxKHT = Channels[6]->MaxConductance_Siemens; // for high-threshold K+ current 
	gmaxh  = Channels[10]->MaxConductance_Siemens; // for hyperpolarization-activated cation current

	if (this->ResponseType == TYPEZHOU2006) // use m2 in place of m3
	{
		ge =   (float)(ExcitatoryTotal * 1e9);
		gi =   (float)(InhibitoryTotal * 1e9);
		gNa =  (float)(gmaxNa * m2 * h * 1e9);
		gKA =  (float)(gmaxKA * a4 * b * c * 1e9);
		gKLT = (float)(gmaxKLT * w4 * z);
		gKHT = (float)(gmaxKHT * (phi * n2 + (1 - phi) * p) * 1e9);
		gh =   (float)(gmaxh * r * 1e9);
	}
	else
	{
		ge =   (float)(ExcitatoryTotal * 1e9);
		gi =   (float)(InhibitoryTotal * 1e9);
		gNa =  (float)(gmaxNa * m3 * h * 1e9);
		gKA =  (float)(gmaxKA * a4 * b * c * 1e9);
		gKLT = (float)(gmaxKLT * w4 * z * 1e9);
		gKHT = (float)(gmaxKHT * (phi * n2 + (1 - phi) * p) * 1e9);
		gh =   (float)(gmaxh * r * 1e9);
	}
}

#endif

void Rothman03Compartment::CalculateConductances(void)
{
	// synaptic and voltage-dependent ion-channel conductances
	float ge, gi, gNa, gKA, gKHT, gKLT, gh;
	// ion-channel activiation parameters
	double a4, b, c, w4, z, n2, p, m3, m2, h, r;
	// maximum conductance for ion channels
	double gmaxKA, gmaxKLT, gmaxKHT, gmaxNa, gmaxh;
	// additional parameter for KHT
	const double phi = 0.85;

	// activation parameter values (number indicates exponent, a4 is a^4)
	a4 = Channels[1]->IntermediateY * Channels[1]->IntermediateY * Channels[1]->IntermediateY * Channels[1]->IntermediateY;
	b  = Channels[2]->IntermediateY;
	c  = Channels[3]->IntermediateY;
	w4 = Channels[4]->IntermediateY * Channels[4]->IntermediateY * Channels[4]->IntermediateY * Channels[4]->IntermediateY;
	z  = Channels[5]->IntermediateY;
	n2 = Channels[6]->IntermediateY * Channels[6]->IntermediateY;
	p  = Channels[7]->IntermediateY;
	m3 = Channels[8]->IntermediateY * Channels[8]->IntermediateY * Channels[8]->IntermediateY;
	m2 = Channels[8]->IntermediateY * Channels[8]->IntermediateY;
	h  = Channels[9]->IntermediateY;
	r  = Channels[10]->IntermediateY;

	// maximum conductance values for the ion channels
	gmaxNa = Channels[8]->MaxConductance_Siemens; // for fast Na+ current
	gmaxKA  = Channels[1]->MaxConductance_Siemens; // for fast transient K+ current
	gmaxKLT = Channels[4]->MaxConductance_Siemens; // for low-threshold K+ current
	gmaxKHT = Channels[6]->MaxConductance_Siemens; // for high-threshold K+ current 
	gmaxh  = Channels[10]->MaxConductance_Siemens; // for hyperpolarization-activated cation current

	if (this->ResponseType == TYPEZHOU2006) // use m2 in place of m3
	{
		ge =   (float)(ExcitatoryTotal * 1e9);
		gi =   (float)(InhibitoryTotal * 1e9);
		gNa =  (float)(gmaxNa * m2 * h * 1e9);
		gKA =  (float)(gmaxKA * a4 * b * c * 1e9);
		gKLT = (float)(gmaxKLT * w4 * z);
		gKHT = (float)(gmaxKHT * (phi * n2 + (1 - phi) * p) * 1e9);
		gh =   (float)(gmaxh * r * 1e9);
	}
	else
	{
		ge =   (float)(ExcitatoryTotal * 1e9);
		gi =   (float)(InhibitoryTotal * 1e9);
		gNa =  (float)(gmaxNa * m3 * h * 1e9);
		gKA =  (float)(gmaxKA * a4 * b * c * 1e9);
		gKLT = (float)(gmaxKLT * w4 * z * 1e9);
		gKHT = (float)(gmaxKHT * (phi * n2 + (1 - phi) * p) * 1e9);
		gh =   (float)(gmaxh * r * 1e9);
	}

	// Notice that KHT is [4] and KLT is [5], 
	// consistent with the order used in BushyCell2.cpp
	ConductanceInfo[0]->Conductance_nS = ge;
	ConductanceInfo[1]->Conductance_nS = gi;
	ConductanceInfo[2]->Conductance_nS = gNa;
	ConductanceInfo[3]->Conductance_nS = gKA;
	ConductanceInfo[4]->Conductance_nS = gKHT;
	ConductanceInfo[5]->Conductance_nS = gKLT;
	ConductanceInfo[6]->Conductance_nS = gh;

}
