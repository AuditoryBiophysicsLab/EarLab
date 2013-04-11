#ifndef _MG_SYNAPSE_H_INCLUDED
#define _MG_SYNAPSE_H_INCLUDED

class MGSynapse
{
public:
	MGSynapse(double SampleRate_Hz, double Tau, double EPSP_strength);
	inline void Fire(void) {in += 1.0;}
	inline void Fire(int SpikeCount) {in += SpikeCount;}
	void Step(void);
	inline double GetConductance_Siemens(void) {return Conductance_Siemens;}
private:
	double first, second, in;
	double SamplePeriod;
	double Tau;
	double A;
	double EPSP_strength;
	double EPSP_ScaleFactor_Siemens;
	double Conductance_Siemens;
};

#endif
