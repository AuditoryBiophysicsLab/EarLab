#ifndef _ROTHMAN_93_CHANNELS_H_INCLUDED
#define _ROTHMAN_93_CHANNELS_H_INCLUDED
#include "HHChannelPopulation.h"

#define ROTHMAN_93_RATE_CONSTANT 1000

// Reversal potentials from Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2564]
const double KReversalPotential93_V  = -77.0e-3;
const double NaReversalPotential93_V =  55.0e-3;
const double lkReversalPotential93_V = 2.8e-3;
const double EReversalPotential93_V = -10.0e-3;
const double IReversalPotential93_V = -66.5e-3;

typedef struct
{
	double MaxG_Na93;
	double MaxG_KFast93;
	double MaxG_KSlow93;
	double MaxG_Leakage93;
} Rothman93ConductanceParameters;

enum Enumed93ResponseType { TYPE_I=1, TYPE_II=2 };
enum Enumed93RateType { ROTHMAN93RATES=1, BRUGHERA96RATES=2 };


class Rothman93ChannelPopulation : public HHChannelPopulation
{
public:
	Rothman93ChannelPopulation();
	Rothman93ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC);
	Rothman93ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType);
	Rothman93ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType, Enumed93RateType RateType);
	void SetRothman93ModelParameters(Enumed93ResponseType ResponseType);

	Enumed93ResponseType ResponseType;
	Enumed93RateType RateType;

	Rothman93ConductanceParameters *Rothman93ModelParameters, Rothman93ParameterSet_I, Rothman93ParameterSet_II;

};

class Rothman93Kslow : public Rothman93ChannelPopulation
{
public:
	Rothman93Kslow(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
	virtual double GetCurrent_Amperes(void);
};

class Rothman93Kfast : public Rothman93ChannelPopulation
{
public:
	Rothman93Kfast(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
	virtual double GetCurrent_Amperes(void);
};

class Rothman93NaActivation : public Rothman93ChannelPopulation
{
public:
	Rothman93NaActivation(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman93NaInactivation : public Rothman93ChannelPopulation
{
public:
	Rothman93NaInactivation(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman93Excitatory : public HHChannelPopulation
{
public:
	Rothman93Excitatory();
	virtual void CalculateRates(double Voltage_V);
};

class Rothman93Inhibitory : public HHChannelPopulation
{
public:
	Rothman93Inhibitory();
	virtual void CalculateRates(double Voltage_V);
};

class Rothman93Leakage : public Rothman93ChannelPopulation
{
public:
	Rothman93Leakage(double Temperature_DegreesC);
	void CalculateRates(double Voltage_V);
	virtual double GetCurrent_Amperes(void);
};

#endif  // _ROTHMAN_93_CHANNELS_H_INCLUDED
