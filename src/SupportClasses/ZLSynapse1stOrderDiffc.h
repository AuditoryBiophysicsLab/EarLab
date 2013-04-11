#ifndef _ZL_SYNAPSE_1ST_ORDER_DIFFC_H_INCLUDED
#define _ZL_SYNAPSE_1ST_ORDER_DIFFC_H_INCLUDED

#include "FirstOrderLowpass.h"
#include "ZhouLSOCompartment.h"

class ZhouLSOCompartment;

class ZLSynapse1stOrderDiffc
{
public:
	ZLSynapse1stOrderDiffc(double SampleRate_Hz, double Tau_rise_seconds, double Tau_fall_seconds, double EPSP_strength_Siemens, ZhouLSOCompartment *theCompartment);
	~ZLSynapse1stOrderDiffc();
	void Fire(void);
	void Fire(int SpikeCount);
	void Step(void);
	inline double GetConductance_Siemens(void) {return Conductance_Siemens;}
private:
	double in_rise, out_rise;
	double in_fall, out_fall;
	double SamplePeriod_seconds;
	double Tau_rise_seconds, Tau_fall_seconds;
	double EPSP_strength_Siemens;
	double EPSP_ScaleFactor;
	double Conductance_Siemens;
	double t_peak, g_t_peak;
	FirstOrderLowpass *Integrator_rise, *Integrator_fall;
	ZhouLSOCompartment *myCompartment;
};

#endif
