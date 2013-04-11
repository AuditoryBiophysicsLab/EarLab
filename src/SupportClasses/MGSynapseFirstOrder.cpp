#include "Earlab.h"
#include "MGSynapseFirstOrder.h"

MGSynapseFirstOrder::MGSynapseFirstOrder(double SampleRate_Hz, double Tau_seconds, double EPSP_strength, MGCompartment *theCompartment)
{
	this->myCompartment = theCompartment;
	this->in = 0.0;
	this->out = 0.0;
	this->Conductance_Siemens = 0.0;
	this->SamplePeriod = 1.0 / SampleRate_Hz;
	this->Tau_seconds = Tau_seconds;
	this->EPSP_strength = EPSP_strength;
	//this->EPSP_ScaleFactor_Siemens = (EPSP_strength * 1.0e-9);
	// Note that in MGSynapse there is no 1.0e-9 conversion from nS to S (as in the HHSynapse) because MacGregor conductances are normalized 
	// by dividing by leakage G (assumed 10e-9 in setting AuxCurrentInput_A).
	// Also the (E / Tau) factor is not present because the conductance profile is a simple step
	// followed by exponential decay.
	this->EPSP_ScaleFactor_Siemens = EPSP_strength;

	Integrator = new FirstOrderLowpass();
	Integrator->SetSampleRate_Hz(SampleRate_Hz);
	Integrator->SetTimeConstant_mS(Tau_seconds*1000.0);
	Integrator->ResetFilterTo(myCompartment->Get_V_rest_Volts());
}

MGSynapseFirstOrder::~MGSynapseFirstOrder()
{
	delete Integrator;
}

void MGSynapseFirstOrder::Step(void)
{
	out = Integrator->Filter(in);
	in = 0.0;  // always reset the input

	/* Convert epsp strength to nS */
	Conductance_Siemens = out * EPSP_ScaleFactor_Siemens;
}

void MGSynapseFirstOrder::Fire(void)
{
	//out += 1.0;
	// Increment out by slightly more than 1.0 to compensate for the decay 
	// when using first MGSynapseFirstOrder::Fire, and second MGSynapseFirstOrder::Step.  
	// Approximate the exponential decay using the first two terms of the Taylor series for e.
	out += 1.0 + (SamplePeriod/Tau_seconds) + 0.5*(SamplePeriod/Tau_seconds)*(SamplePeriod/Tau_seconds);
	
	/* Convert epsp strength to nS */
	Conductance_Siemens = out * EPSP_ScaleFactor_Siemens;
	// Set the filter's previous values: output Y to out, input X to 0.0
	//Integrator->ResetFilterTo(out);
	Integrator->ResetFilterYTo(out);
	Integrator->ResetFilterXTo(0.0);
}

void MGSynapseFirstOrder::Fire(int SpikeCount)
{
	double dSpikeCount;

	dSpikeCount = (double) SpikeCount;
	out += dSpikeCount * (1.0 + (SamplePeriod/Tau_seconds) + 0.5*(SamplePeriod/Tau_seconds)*(SamplePeriod/Tau_seconds));
	/* Convert epsp strength to nS */
	Conductance_Siemens = out * EPSP_ScaleFactor_Siemens;
	//Integrator->ResetFilterTo(out);
	Integrator->ResetFilterYTo(out);
	Integrator->ResetFilterXTo(0.0);
}
