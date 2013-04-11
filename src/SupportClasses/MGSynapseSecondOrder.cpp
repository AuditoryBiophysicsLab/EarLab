#include "Earlab.h"
#include "MGSynapseSecondOrder.h"

MGSynapseSecondOrder::MGSynapseSecondOrder(double SampleRate_Hz, double Tau, double EPSP_strength)
{
	this->first = 0.0;
	this->second = 0.0;
	this->in = 0.0;
	this->Conductance_Siemens = 0.0;
	this->SamplePeriod = 1.0 / SampleRate_Hz;
	this->Tau = Tau;
	this->A = SamplePeriod / this->Tau;
	this->EPSP_strength = EPSP_strength;
	//this->EPSP_ScaleFactor_Siemens = (EPSP_strength * 1.0e-9) * (E / Tau);
	// Note that in MGSynapse there is no 1.0e-9 conversion from nS to S (as in the HHSynapse) because MacGregor conductances are normalized 
	// by dividing by leakage G (assumed 10e-9 in setting AuxCurrentInput_A).
	// The factor (E / Tau) is maintained with a 2nd order (alpha function) conductance profile
	this->EPSP_ScaleFactor_Siemens = EPSP_strength * (E / Tau);
}

void MGSynapseSecondOrder::Step(void)
{
	double mid, out;  // temporary variables

	mid = first + in - (A * first);
	out = second + (SamplePeriod * first) - (A * second);
	in = 0.0;  // always reset the input
	first = mid;
	second = out;

	/* Convert epsp strength to nS and scale by e^1 */
	Conductance_Siemens = out * EPSP_ScaleFactor_Siemens;
}
