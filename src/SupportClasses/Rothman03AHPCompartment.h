#ifndef __ROTHMAN_03_AHP_COMPARTMENT_H_INCLUDED
#define __ROTHMAN_03_AHP_COMPARTMENT_H_INCLUDED
#include "Earlab.h"
#include "EarlabException.h"
#include "Rothman03AHPChannels.h"
#include "Rothman03Compartment.h"
#include "TimerQueue.h"

//#define	NUM_R03AHP_CONDUCTANCES	NUM_R03_CONDUCTANCES+1


class Rothman03AHPCompartment : public Rothman03Compartment
{
public:
	Rothman03AHPCompartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC);
	Rothman03AHPCompartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, double TauAHP_Seconds, double AHPIncrement_Siemens);
	Rothman03AHPCompartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType);
	Rothman03AHPCompartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, double TauAHP_Seconds, double AHPIncrement_Siemens);
	Rothman03AHPCompartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues);
	Rothman03AHPCompartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, double TauAHP_Seconds, double AHPIncrement_Siemens, Rothman03CustomParameterValues MyCustomParameterValues);
	// GetDerivative and Step in form of Cai98Compartment.h
	double GetDerivative(double Voltage_V, double InstantaneousVoltage_V);
	//void GetConductances_nS(float &ge, float &gi, float &gNa, float &gKA, float &gKHT, float &gKLT, float &gh, float &gAHP);
	virtual void CalculateConductances(void);
	bool Step(HHSynapse *ExcitatorySynapses[], int ExcitatoryCount, HHSynapse *InhibitorySynapses[], int InhibitoryCount);
//protected:
	// Input conductances -- Rothman03AHPCompartment will use
	// the following declared in Rothman03Compartment:
	//Rothman03Excitatory Excitatory;
	//Rothman03Inhibitory Inhibitory;

	// Leakage conductance -- Rothman03AHPCompartment will use
	// the following declared in Rothman03Compartment: 
	//Rothman03Leakage *Leakage;

private:
	// The AHP member variables are the same as in Cai98Compartment.h
	double FirstOrderExponentialDecay;
	double AHPConductance_Siemens;
	TimerQueue *timerQueue;

	double mTauAHP_Seconds;							// In Cai et al. mTauAHP_Seconds = 0.5

	double mAHPIncrement_Siemens;			// DJA: Change this when we find the real value in the code or in the paper
												// This first estimate appeared way too large, judging by simulation results

//const double mAHPIncrement_Siemens = 0.0;	// AB: mAHPIncrement_Siemens = 0.0 to shut off AHP adaptation used in combination 
												// with a type II current-clamp response (the default in Brughera96Compartment).

												// Use non-zero mAHPIncrement_Siemens in combination with
												// the settings for a Type I current-clamp response in 
												// Brughera96Channels.cpp
												// Rothman93Compartment.cpp
												// Rothman93Channels.cpp
//const double mAHPIncrement_Siemens = 1.0e-9;	// 1.0e-9 works better than 0.2e-9 for adaptaion with
												// GmaxNa=1000nS, GmaxKHT=150nS, GmaxKHT=0, Cmembrane=12pF
												// and mTauAHP_Seconds = 0.5 seconds
												//
												// Calculate mAHPIncrement_Siemens
												// AB: Calculated from Cai, Carney & Colburn 1998 (II) 
												// Figure 2: A single action potential is induced at time=2ms,
												// According to the model, G_AHP is incremented 0.5 ms later.
												// G_AHP = mAHPIncrement_Siemens at time=2.5ms
												// From the Figure, at time 15 ms (12.5 ms after onset of AHP),
												// we read that I_AHP=0.0033nA and Vmembrane~=Vrest=-60mV.
												// E_K = -77mV
												// G_AHP_15ms = I_AHP/(Vmembrane-E_K)= 3.3e-12A/(-60e-3V + 77e-3V)
												// G_AHP_15ms =  3.3e-12A/17e-3V = 1.9412e-010 S
												// G_AHP decays exponentially with time constant mTauAHP_Seconds=0.5s, therefore
												// AHP_INCREMENT = G_AHP_15ms*(1/(exp(-12.5ms/500ms)) 
												// AHP_INCREMENT = 1.9903e-010 SIEMENS
												// which is very close to the round value of
												// mAHPIncrement_Siemens = 0.2e-9

};

#endif
