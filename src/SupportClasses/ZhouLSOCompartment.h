#ifndef __ZL_COMPARTMENT_H_INCLUDED
#define __ZL_COMPARTMENT_H_INCLUDED

#include "Earlab.h"
#include "EarlabException.h"
#include "TimerQueue.h"
#include "ZLCurrentBranch.h"
#include "ZLSynapse1stOrderDiffc.h"
#include "RungeKuttaEquation.h"
#include "RungeKuttaSolver.h"

const int ZLMaxSynapseCount = 100;
const int ZLODECount = 1; // Number of Ordinary Differential Equations we're solving here

class ZLCurrentBranch;
class ZLSynapse1stOrderDiffc;

class ZhouLSOCompartment : RungeKuttaEquation
{
public:
	ZhouLSOCompartment();

	ZhouLSOCompartment(double SampleRate_Hz, double C_m_Farads, double G_L_Siemens, double E_L_Volts, double G_abs_increment_Siemens, double E_abs_Volts, 
		double t_abs_seconds, double G_AHP_increment_Siemens, double E_AHP_Volts, double Tau_AHP_Seconds, double V_th_Volts, double V_rest_Volts);

	ZhouLSOCompartment(double SampleRate_Hz, double C_m_Farads, double G_L_Siemens, double E_L_Volts, double G_abs_increment_Siemens, double E_abs_Volts, 
		double t_abs_seconds, double G_AHP_increment_Siemens, double E_AHP_Volts, double Tau_AHP_Seconds, double V_th_Volts, double V_rest_Volts,
		double E_e_Volts, double E_i_Volts,	double G_e_Siemens, double G_i_Siemens,	
		double Tau_e_rise_Seconds, double Tau_e_fall_Seconds, double Tau_i_rise_Seconds, double Tau_i_fall_Seconds);

	~ZhouLSOCompartment();

	void Initialize(double SampleRate_Hz, double C_m_Farads, double G_L_Siemens, double E_L_Volts, double G_abs_increment_Siemens, double E_abs_Volts, 
		double t_abs_seconds, double G_AHP_increment_Siemens, double E_AHP_Volts, double Tau_AHP_Seconds, double V_th_Volts, double V_rest_Volts);

	void Initialize(double SampleRate_Hz, double C_m_Farads, double G_L_Siemens, double E_L_Volts, double G_abs_increment_Siemens, double E_abs_Volts,
		double t_abs_seconds, double G_AHP_increment_Siemens, double E_AHP_Volts, double Tau_AHP_Seconds, double V_th_Volts, double V_rest_Volts,
		double E_e_Volts, double E_i_Volts,	double G_e_Siemens, double G_i_Siemens,
		double Tau_e_rise_Seconds, double Tau_e_fall_Seconds, double Tau_i_rise_Seconds, double Tau_i_fall_Seconds);

	//bool Step(float InjectedCurrent_Amps);
	bool Step(double InjectedCurrent_Amps);
//	bool Step(ZLSynapse1stOrderDiffc **ExcitatorySynapses, int ExcitatoryCount, ZLSynapse1stOrderDiffc **InhibitorySynapses, int InhibitoryCount);
//	bool Step(ZLSynapse1stOrderDiffc *ExcitatorySynapses[], int ExcitatoryCount, ZLSynapse1stOrderDiffc *InhibitorySynapses[], int InhibitoryCount);
	virtual bool Step(ZLSynapse1stOrderDiffc *ExcitatorySynapses[], int ExcitatoryCount, ZLSynapse1stOrderDiffc *InhibitorySynapses[], int InhibitoryCount);
	//double GetdVdt();

	inline double GetExcitatoryTotal(void) {return ExcitatoryTotal;}
	inline double GetInhibitoryTotal(void) {return InhibitoryTotal;}
	inline double GetGabs_Siemens(void) {return G_abs_Siemens;}
	inline double GetGAHP_Siemens(void) {return G_AHP_Siemens;}
	inline double GetMembraneVoltage_V(void) {return V_m_Volts;}
	void GetConductances_nS(float &Ge, float &Gi, float &Gabs, float &GAHP);

	inline double Get_V_rest_Volts(void) {return V_rest_Volts;}
	inline double Get_V_th_Volts(void) {return V_th_Volts;}
//	inline double Get_V_rest_Volts(void) {return V_rest_Volts;}

private:
	void CalculateVoltage(void);
	virtual double GetDerivative(double IntermediateY, double InstantaneousVoltage_V);
	inline void TotalExcitatorySynapses(ZLSynapse1stOrderDiffc *Synapses[], int SynapseCount) {ExcitatoryTotal = TotalSynapses(Synapses, SynapseCount);}
	inline void TotalInhibitorySynapses(ZLSynapse1stOrderDiffc *Synapses[], int SynapseCount) {InhibitoryTotal = TotalSynapses(Synapses, SynapseCount);}
	double TotalSynapses(ZLSynapse1stOrderDiffc *Synapses[], int SynapseCount);

	bool Initialized;
	double InjectedCurrent_Amps;

	double SamplePeriod_Seconds;

	double C_m_Farads;
	double G_L_Siemens;
	double E_L_Volts;
	double G_abs_increment_Siemens;
	double G_abs_Siemens;
	double E_abs_Volts;
	double t_abs_seconds;
	double G_AHP_increment_Siemens;
	double G_AHP_Siemens;
	double E_AHP_Volts;
	double Tau_AHP_Seconds;
	double reduction_factor_G_AHP;
	double V_th_Volts;
	double V_rest_Volts;
	double V_m_Volts; // membrane voltage
	double dVmdt; // time deriviative of V_m_Volts
	//double InitialVoltage_V, 
	double V_m_old_Volts, Vnew_V;
	double ExcitatoryTotal;
	double InhibitoryTotal;
	double E_e_Volts;
	double E_i_Volts;
	double G_e_Siemens;
	double G_i_Siemens;
	double Tau_e_rise_Seconds;
	double Tau_e_fall_Seconds;
	double Tau_i_rise_Seconds;
	double Tau_i_fall_Seconds;

	bool InAbsoluteRefractoryPeriod;
	TimerQueue *AbsoluteRefractoryTimerQueue;

	// FirstOrderLowpass *Integrator;

	// Current Branches (In the case of ZhouLSO, synaptic current branches only)
	ZLCurrentBranch **CurrentBranches;
	int NumZLCurrentBranches; // number of CurrentBranches (In the case of ZhouLSO, synaptic current branches only)
	int NumZLCurrentBranchesRK; // In the case of ZhouLSO, zero.
    int ESBIndex, ISBIndex;  // Indices for the current branches

	RungeKuttaEquation *Elements[ZLODECount + 1];
	RungeKuttaSolver rkSolver;

};

#endif
