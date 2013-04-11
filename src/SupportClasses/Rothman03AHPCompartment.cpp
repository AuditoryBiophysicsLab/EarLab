#include "Rothman03AHPCompartment.h"
#include "math.h"
#include "stdio.h"

Rothman03AHPCompartment::Rothman03AHPCompartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC) :
	Rothman03Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, (int) 1)
{
	mAHPIncrement_Siemens = 0.0e-9;  // The default is no adaptation.  Adaptation must be setup using input parameters 
	mTauAHP_Seconds = 1/SampleRate_Hz;  // choose a short time constant for the default no-adaptation case. 
	FirstOrderExponentialDecay = exp(-SamplePeriod / mTauAHP_Seconds);
	timerQueue = new TimerQueue(SampleRate_Hz);
	AHPConductance_Siemens = 0.0;
	//this->NumConductances = NumConductances + 1;
	strcpy(ConductanceInfo[7]->ConductanceName, "gAHP");
}

Rothman03AHPCompartment::Rothman03AHPCompartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, double TauAHP_Seconds, double AHPIncrement_Siemens) :
	Rothman03Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, (int) 1)
{
	mAHPIncrement_Siemens = AHPIncrement_Siemens;
	mTauAHP_Seconds = TauAHP_Seconds;
	FirstOrderExponentialDecay = exp(-SamplePeriod / mTauAHP_Seconds);
	timerQueue = new TimerQueue(SampleRate_Hz);
	AHPConductance_Siemens = 0.0;
	//this->NumConductances = NumConductances + 1;
	strcpy(ConductanceInfo[7]->ConductanceName, "gAHP");
}

Rothman03AHPCompartment::Rothman03AHPCompartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType) :
	Rothman03Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, ResponseType, (int) 1)
{
	mAHPIncrement_Siemens = 0.0e-9;  // The default is no adaptation.  Adaptation must be setup using input parameters 
	mTauAHP_Seconds = 1/SampleRate_Hz;  // choose a short time constant for the default no-adaptation case. 
	FirstOrderExponentialDecay = exp(-SamplePeriod / mTauAHP_Seconds);
	timerQueue = new TimerQueue(SampleRate_Hz);
	AHPConductance_Siemens = 0.0;
	//this->NumConductances = NumConductances + 1;
	strcpy(ConductanceInfo[7]->ConductanceName, "gAHP");
}

Rothman03AHPCompartment::Rothman03AHPCompartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, double TauAHP_Seconds, double AHPIncrement_Siemens) :
	Rothman03Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, ResponseType, (int) 1)
{
	mAHPIncrement_Siemens = AHPIncrement_Siemens;
	mTauAHP_Seconds = TauAHP_Seconds;
	FirstOrderExponentialDecay = exp(-SamplePeriod / mTauAHP_Seconds);
	timerQueue = new TimerQueue(SampleRate_Hz);
	AHPConductance_Siemens = 0.0;
	//this->NumConductances = NumConductances + 1;
	strcpy(ConductanceInfo[7]->ConductanceName, "gAHP");
}

	Rothman03AHPCompartment::Rothman03AHPCompartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues) :
	Rothman03Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, ResponseType, (int) 1, MyCustomParameterValues)
{
	mAHPIncrement_Siemens = 0.0e-9;  // The default is no adaptation.  Adaptation must be setup using input parameters 
	mTauAHP_Seconds = 1/SampleRate_Hz;  // choose a short time constant for the default no-adaptation case. 
	FirstOrderExponentialDecay = exp(-SamplePeriod / mTauAHP_Seconds);
	timerQueue = new TimerQueue(SampleRate_Hz);
	AHPConductance_Siemens = 0.0;
	//this->NumConductances = NumConductances + 1;
	strcpy(ConductanceInfo[7]->ConductanceName, "gAHP");
}

Rothman03AHPCompartment::Rothman03AHPCompartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, double TauAHP_Seconds, double AHPIncrement_Siemens, Rothman03CustomParameterValues MyCustomParameterValues) :
	Rothman03Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, ResponseType, (int) 1, MyCustomParameterValues)
{
	mAHPIncrement_Siemens = AHPIncrement_Siemens;
	mTauAHP_Seconds = TauAHP_Seconds;
	FirstOrderExponentialDecay = exp(-SamplePeriod / mTauAHP_Seconds);
	timerQueue = new TimerQueue(SampleRate_Hz);
	AHPConductance_Siemens = 0.0;
	//this->NumConductances = NumConductances + 1;
	strcpy(ConductanceInfo[7]->ConductanceName, "gAHP");
}

double Rothman03AHPCompartment::GetDerivative(double IntermediateY, double InstantaneousVoltage_V)
{

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
	double a4, b, c, w4, z, n2, p, m3, h, r;
	// maximum conductance for ion channels
	double gmaxKA, gmaxKLT, gmaxKHT, gmaxNa, gmaxh;
	// reversal potentials for ion channels (typically all VKs will be equal)
	double VKA, VKLT, VKHT, VNa, Vh;
	// additional parameter for KHT
	const double phi = 0.85;
	// leakage current variable (primarily for viewing during debug)
	double i_LEAK;
	// adaptation AHP current variable
	double i_AHP;

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

	// Compute i_AHP
	// KAHP (Voltage independent Potassium After Hyperpolarization conductance):\
	// From Cai 1998 (JASA) [Vol 103 (1) p. 494]
	i_AHP = (AHPConductance_Siemens * (V - VKHT)); 

	// the membrane current equation divided on both sides by MembraneCapacitance_F
	// provides a differential equation in membrane voltage
	dVdt = -(1.0 / MembraneCapacitance_F) * (
			  (gmaxKA * a4 * b * c * (V - VKA))
			+ (gmaxKLT * w4 * z * (V - VKLT))
			+ (gmaxKHT * (phi * n2 + (1 - phi) * p) * (V - VKHT))
			+ (gmaxNa * m3 * h * (V - VNa))
			+ (gmaxh * r * (V - Vh))
			+ (i_LEAK)
			+ (i_AHP)
			+ (ExcitatoryTotal * (V - Excitatory.ReversalPotential_V))
			+ (InhibitoryTotal * (V - Inhibitory.ReversalPotential_V))
			- (AuxCurrentInput_A)
			);

	return dVdt;
}

// This assumes that the synapses have already been stepped to the new sample
bool Rothman03AHPCompartment::Step(HHSynapse *ExcitatorySynapses[], int ExcitatoryCount, HHSynapse *InhibitorySynapses[], int InhibitoryCount)
{
	int timer_count = 0;
	if (ExcitatorySynapses != NULL)
		TotalExcitatorySynapses(ExcitatorySynapses, ExcitatoryCount);
	if (InhibitorySynapses != NULL)
		TotalInhibitorySynapses(InhibitorySynapses, InhibitoryCount);
	if (timerQueue == NULL)
		throw EarlabException("Rothman03AHPCompartment: Error setting timer in Step(): Timer queue does not exist");

	timer_count = timerQueue->Step();
	AHPConductance_Siemens += (mAHPIncrement_Siemens * timer_count);
	CalculateVoltage();
	AHPConductance_Siemens *= FirstOrderExponentialDecay;	// Perform exponential decay
	if (AHPConductance_Siemens < 1e-24)
		AHPConductance_Siemens = 0;							// Make sure we don't venture into the "denormal" zone for floating point calculations
	CalculateConductances();

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

// put the conductance evaluations in GetDerivative and Step, store them in HHConductanceInfo **ConductanceInfo
#if 0
void Rothman03AHPCompartment::GetConductances_nS(float &ge, float &gi, float &gNa, float &gKA, float &gKHT, float &gKLT, float &gh, float &gAHP)
{
	Rothman03Compartment::GetConductances_nS(ge, gi, gNa, gKA, gKHT, gKLT, gh); 
	gAHP = (float)(AHPConductance_Siemens * 1e9);
}
#endif

void Rothman03AHPCompartment::CalculateConductances(void)
{
	float gAHP;

	Rothman03Compartment::CalculateConductances(); 
	gAHP = (float)(AHPConductance_Siemens * 1e9);
	ConductanceInfo[7]->Conductance_nS = gAHP;
}
