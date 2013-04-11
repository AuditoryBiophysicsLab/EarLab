#ifndef _HH_COMPARTMENT_H_INCLUDED
#define _HH_COMPARTMENT_H_INCLUDED
#include "Earlab.h"
#include "EarlabException.h"
#include "HHSynapse.h"
#include "RungeKuttaEquation.h"
#include "RungeKuttaSolver.h"

const int MaxSynapseCount = 100;
const int ODECount = 5; // Number of Ordinary Differential Equations we're solving here

class HHCompartment : public RungeKuttaEquation
{
public:
	HHCompartment(double SampleRate_Hz, double InitialVoltage_mV, double Temperature_DegreesC);

	virtual double GetDerivative(double Voltage_mV, double InstantaneousVoltage_mV);
	void SetTemperature_DegreesC(double Temperature_DegreesC);

	void SetCurrentInjection_pA(double CurrentInjection_pA);

	void Settle(void);
	inline void TotalExcitatorySynapses(HHSynapse *Synapses[], int SynapseCount) {ExcitatoryTotal = TotalSynapses(Synapses, SynapseCount);}
	inline void TotalInhibitorySynapses(HHSynapse *Synapses[], int SynapseCount) {InhibitoryTotal = 0.0;}

	inline void SetSampleRate_Hz(double SampleRate_Hz) {this->SampleRate_Hz = SampleRate_Hz; SamplePeriod = 1.0 / SampleRate_Hz;}

	void Step(HHSynapse *ExcitatorySynapses[], int ExcitatoryCount, HHSynapse *InhibitorySynapses[], int InhibitoryCount);

	double Conductance_Siemens;

	inline double GetVoltage_mV(void) {return Vnew_mV;}

private:
	void CalculateVoltage(void);

	double TotalSynapses(HHSynapse *Synapses[], int SynapseCount);

	double Temperature_DegreesC;
	double MembraneCapacitance_uF;
	double AuxCurrentInput_pA;	// Auxilliary Current Input, in microAmperes
	double ExcitatoryTotal;
	double InhibitoryTotal;
	double SamplePeriod;
	double SampleRate_Hz;
	double InitialVoltage_mV, Vold_mV, Vnew_mV;

	// Channel Conductances
	HH_Kslow Kslow;						// Passed as Element[0]
	HH_Kfast Kfast;						// Passed as Element[1]
	HH_NaActivation NaActivation;		// Passed as Element[2]
	HH_NaInactivation NaInactivation;	// Passed as Element[3]

	// Input conductances
	HH_Excitatory Excitatory;
	HH_Inhibitory Inhibitory;

	// Leakage conductance
	HH_Leakage Leakage;

	RungeKuttaEquation *Elements[ODECount + 1];
	RungeKuttaSolver rkSolver;
};

#endif
