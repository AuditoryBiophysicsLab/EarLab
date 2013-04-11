#ifndef _CAI_98_COMPARTMENT_H_INCLUDED
#define _CAI_98_COMPARTMENT_H_INCLUDED
#include "Earlab.h"
#include "EarlabException.h"
#include "HHChannelPopulation.h"
#include "Rothman93Channels.h"
#include "Cai98Channels.h"
#include "HHSynapse.h"
#include "RungeKuttaEquation.h"
#include "RungeKuttaSolver.h"
#include "HHCompartment.h"
#include "Brughera96Compartment.h"
#include "TimerQueue.h"

const double TAU_AHP = 0.5;
const double AHP_INCREMENT_SIEMENS = 10e-9;	// DJA: Change this when we find the real value in the code or in the paper

class Cai98Compartment : public Brughera96Compartment
{
public:
	Cai98Compartment() {};
	Cai98Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC);
	~Cai98Compartment();

	virtual double GetDerivative(double Voltage_V, double InstantaneousVoltage_V);
	virtual bool Step(HHSynapse *ExcitatorySynapses[], int ExcitatoryCount, HHSynapse *InhibitorySynapses[], int InhibitoryCount);

private:
	double FirstOrderExponentialDecay;
	double AHPConductance_Siemens;
	TimerQueue *timerQueue;
};

#endif
