#ifndef _HH_COMPARTMENT_H_INCLUDED
#define _HH_COMPARTMENT_H_INCLUDED
#include "Earlab.h"
#include "EarlabException.h"
#include "HHChannelPopulation.h"
#include "HHSynapse.h"
#include "RungeKuttaEquation.h"
#include "RungeKuttaSolver.h"

const int MaxSynapseCount = 100;
const int ODECount = 20; // Number of Ordinary Differential Equations we're solving here

class HHChannelPopulation;

class HHCompartment : public RungeKuttaEquation
{
public:
	HHCompartment() {ThresholdVoltage_Volts = -25e-3; Vold_V = Vnew_V = -60e-3;}
	HHCompartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC);
	virtual double GetDerivative(double Voltage_V, double InstantaneousVoltage_V) = 0;
	virtual void Settle(void) = 0;

	void SetTemperature_DegreesC(double Temperature_DegreesC);
	void SetCurrentInjection_A(double CurrentInjection_A);

	inline void SetSampleRate_Hz(double SampleRate_Hz) {this->SampleRate_Hz = SampleRate_Hz; SamplePeriod = 1.0 / SampleRate_Hz;}

	virtual bool Step(HHSynapse *ExcitatorySynapses[], int ExcitatoryCount, HHSynapse *InhibitorySynapses[], int InhibitoryCount);

	inline double GetVoltage_V(void) {return Vnew_V;}
	double GetActivations(int ChannelIndex);

	double ThresholdVoltage_Volts;

protected:
	virtual void CalculateVoltage(void);
	inline void TotalExcitatorySynapses(HHSynapse *Synapses[], int SynapseCount) {ExcitatoryTotal = TotalSynapses(Synapses, SynapseCount);}
	inline void TotalInhibitorySynapses(HHSynapse *Synapses[], int SynapseCount) {InhibitoryTotal = TotalSynapses(Synapses, SynapseCount);}
	double TotalSynapses(HHSynapse *Synapses[], int SynapseCount);

	double Temperature_DegreesC;
	double MembraneCapacitance_F;	// Farads
	double AuxCurrentInput_A;	// Auxilliary Current Input, in Amperes
	double ExcitatoryTotal;
	double InhibitoryTotal;
	double SamplePeriod;
	double SampleRate_Hz;
	double InitialVoltage_V, Vold_V, Vnew_V;
	bool WasUnderThreshold;

	// Channel Conductances
	HHChannelPopulation **Channels;
	int NumHHChannels;

	RungeKuttaEquation *Elements[ODECount + 1];
	RungeKuttaSolver rkSolver;
};

#endif
