#include <math.h>
#include "Earlab.h"
#include "ZLSynapse1stOrderDiffc.h"

ZLSynapse1stOrderDiffc::ZLSynapse1stOrderDiffc(double SampleRate_Hz, double Tau_rise_seconds, double Tau_fall_seconds, double EPSP_strength_Siemens, ZhouLSOCompartment *theCompartment)
{
	this->myCompartment = theCompartment;
	this->in_rise = 0.0;
	this->out_rise = 0.0;
	this->in_fall = 0.0;
	this->out_fall = 0.0;
	this->Conductance_Siemens = 0.0;
	this->SamplePeriod_seconds = 1.0 / SampleRate_Hz;
	this->Tau_rise_seconds = Tau_rise_seconds;
	this->Tau_fall_seconds = Tau_fall_seconds;
	this->EPSP_strength_Siemens = EPSP_strength_Siemens;

	t_peak = (this->Tau_rise_seconds * this->Tau_fall_seconds/(this->Tau_fall_seconds - this->Tau_rise_seconds)) * log(this->Tau_fall_seconds/this->Tau_rise_seconds);
	g_t_peak = exp(-t_peak/this->Tau_fall_seconds) - exp(-t_peak/this->Tau_rise_seconds);

	//this->EPSP_ScaleFactor_Siemens = (EPSP_strength * 1.0e-9);
	// Note that in ZLSynapse there is no 1.0e-9 conversion from nS to S (as in the HHSynapse).
	// Also the (E / Tau) factor is not present because the conductance profile is a simple step
	// followed by exponential decay.  The g_t_peak factor is used to normalize the difference
	// of the two first order exponentials (see Zhou and Colburn 2008).
	this->EPSP_ScaleFactor = EPSP_strength_Siemens/g_t_peak;

	Integrator_rise = new FirstOrderLowpass();
	Integrator_rise->SetSampleRate_Hz(SampleRate_Hz);
	Integrator_rise->SetTimeConstant_mS(Tau_rise_seconds*1000.0);
	Integrator_rise->ResetFilterTo(myCompartment->Get_V_rest_Volts());


	Integrator_fall = new FirstOrderLowpass();
	Integrator_fall->SetSampleRate_Hz(SampleRate_Hz);
	Integrator_fall->SetTimeConstant_mS(Tau_fall_seconds*1000.0);
	Integrator_fall->ResetFilterTo(myCompartment->Get_V_rest_Volts());
}

ZLSynapse1stOrderDiffc::~ZLSynapse1stOrderDiffc()
{
	delete Integrator_rise;
	delete Integrator_fall;
}

void ZLSynapse1stOrderDiffc::Step(void)
{
	out_rise = Integrator_rise->Filter(in_rise);
	in_rise = 0.0;  // always reset the input

	out_fall = Integrator_fall->Filter(in_fall);
	in_fall = 0.0;  // always reset the input

	/* Convert epsp strength to S */
	Conductance_Siemens = (out_fall - out_rise) * EPSP_ScaleFactor;
	if (Conductance_Siemens < 0.0)
		Conductance_Siemens = 0.0;

}

void ZLSynapse1stOrderDiffc::Fire(void)
{
	//out += 1.0;
	// Increment out by slightly more than 1.0 to compensate for the decay 
	// when using first ZLSynapse1stOrderDiffc::Fire, and second ZLSynapse1stOrderDiffc::Step.  
	// Approximate the exponential decay using the first two terms of the Taylor series for e.
	out_rise += 1.0 + (SamplePeriod_seconds/Tau_rise_seconds) + 0.5*(SamplePeriod_seconds/Tau_rise_seconds)*(SamplePeriod_seconds/Tau_rise_seconds);
	out_fall += 1.0 + (SamplePeriod_seconds/Tau_fall_seconds) + 0.5*(SamplePeriod_seconds/Tau_fall_seconds)*(SamplePeriod_seconds/Tau_fall_seconds);
	
	/* Convert epsp strength to S */
	Conductance_Siemens = (out_fall - out_rise) * EPSP_ScaleFactor;
	if (Conductance_Siemens < 0.0)
		Conductance_Siemens = 0.0;

	// Set the filter's previous values: output Y to out, input X to 0.0
	Integrator_rise->ResetFilterYTo(out_rise);
	Integrator_rise->ResetFilterXTo(0.0);
	Integrator_fall->ResetFilterYTo(out_fall);
	Integrator_fall->ResetFilterXTo(0.0);
}

void ZLSynapse1stOrderDiffc::Fire(int SpikeCount)
{
	double dSpikeCount;

	dSpikeCount = (double) SpikeCount;
	out_rise += dSpikeCount * (1.0 + (SamplePeriod_seconds/Tau_rise_seconds) + 0.5*(SamplePeriod_seconds/Tau_rise_seconds)*(SamplePeriod_seconds/Tau_rise_seconds));
	out_fall += dSpikeCount * (1.0 + (SamplePeriod_seconds/Tau_fall_seconds) + 0.5*(SamplePeriod_seconds/Tau_fall_seconds)*(SamplePeriod_seconds/Tau_fall_seconds));
	/* Convert epsp strength to S */
	Conductance_Siemens = (out_fall - out_rise) * EPSP_ScaleFactor;
	if (Conductance_Siemens < 0.0)
		Conductance_Siemens = 0.0;
	Integrator_rise->ResetFilterYTo(out_rise);
	Integrator_rise->ResetFilterXTo(0.0);
	Integrator_fall->ResetFilterYTo(out_fall);
	Integrator_fall->ResetFilterXTo(0.0);
}
