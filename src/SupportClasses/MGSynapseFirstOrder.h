#ifndef _MG_SYNAPSEFIRSTORDER_H_INCLUDED
#define _MG_SYNAPSEFIRSTORDER_H_INCLUDED

#include "FirstOrderLowpass.h"
#include "MGCompartment.h"

class MGCompartment;

class MGSynapseFirstOrder
{
public:
	MGSynapseFirstOrder(double SampleRate_Hz, double Tau_seconds, double EPSP_strength, MGCompartment *theCompartment);
	~MGSynapseFirstOrder();
	void Fire(void);
	void Fire(int SpikeCount);
	void Step(void);
	inline double GetConductance_Siemens(void) {return Conductance_Siemens;}
private:
	double in, out;
	double SamplePeriod;
	double Tau_seconds;
	double EPSP_strength;
	double EPSP_ScaleFactor_Siemens;
	double Conductance_Siemens;
	FirstOrderLowpass *Integrator;
	MGCompartment *myCompartment;
};

#endif
