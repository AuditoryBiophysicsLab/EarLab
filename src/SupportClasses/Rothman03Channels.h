#ifndef _ROTHMAN_03_CHANNELS_H_INCLUDED
#define _ROTHMAN_03_CHANNELS_H_INCLUDED
#include "HHChannelPopulation.h"

const int ROTHMAN_03_RATE_CONSTANT = 1000;
const double KReversalPotential_V  = -70.0e-3;
const double NaReversalPotential_V =  55.0e-3;
const double CATIONhReversalPotential_V  = -43.0e-3;
const double lkReversalPotential_V = -65.0e-3;
const double ZhoulkReversalPotential_V = -72.73e-3; // for Zhou06ParameterSet_MSO only
const double EReversalPotential_V = 0.0e-3;
const double IReversalPotential_V = -66.5e-3;

typedef struct
{
	double MaxG_Na;
	double MaxG_HT;
	double MaxG_LT;
	double MaxG_A;
	double MaxG_h;
	double MaxG_lk;
} Rothman03ConductanceParameters;

typedef struct
{
	double MembraneCapacitance_F;
	double MaxG_Na;
	double MaxG_HT;
	double MaxG_LT;
	double MaxG_A;
	double MaxG_h;
	double MaxG_lk;
} Rothman03CustomParameterValues;


enum EnumedResponseType { CUSTOMTYPE=0, TYPE1C=1, TYPE1T=11, TYPE12=12, TYPE21=21, TYPE2=2, TYPEZHOU2006=19, TYPEBRUGHERA2008 = 22};


class Rothman03ChannelPopulation : public HHChannelPopulation
{
public:
	Rothman03ChannelPopulation();
	//Rothman03ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC);
	Rothman03ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType);
	Rothman03ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues);
	virtual void CalculateRates(double Voltage_V) = 0;
	virtual double GetDerivative(double IntermediateY, double InstantaneousVoltage_V);
	void SetRothman03ModelParameters(EnumedResponseType ResponseType);
	double tau_y, y_inf;
	double InitialVoltage_V;
	EnumedResponseType ResponseType;
};

class Rothman03KFastTransient_a : public Rothman03ChannelPopulation
{
public:
	Rothman03KFastTransient_a(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType);
	Rothman03KFastTransient_a(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03KFastTransient_b : public Rothman03ChannelPopulation
{
public:
	Rothman03KFastTransient_b(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType);
	Rothman03KFastTransient_b(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03KFastTransient_c : public Rothman03ChannelPopulation
{
public:
	Rothman03KFastTransient_c(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType);
	Rothman03KFastTransient_c(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03KLT_w : public Rothman03ChannelPopulation
{
public:
	Rothman03KLT_w(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType);
	Rothman03KLT_w(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03KLT_z : public Rothman03ChannelPopulation
{
public:
	Rothman03KLT_z(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType);
	Rothman03KLT_z(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03KHT_n : public Rothman03ChannelPopulation
{
public:
	Rothman03KHT_n(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType);
	Rothman03KHT_n(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03KHT_p : public Rothman03ChannelPopulation
{
public:
	Rothman03KHT_p(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType);
	Rothman03KHT_p(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03NaFast_m : public Rothman03ChannelPopulation
{
public:
	Rothman03NaFast_m(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType);
	Rothman03NaFast_m(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03NaFast_h : public Rothman03ChannelPopulation
{
public:
	Rothman03NaFast_h(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType);
	Rothman03NaFast_h(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03CATIONh_r : public Rothman03ChannelPopulation
{
public:
	Rothman03CATIONh_r(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType);
	Rothman03CATIONh_r(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03Excitatory : public Rothman03ChannelPopulation
{
public:
	Rothman03Excitatory();
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03Inhibitory : public Rothman03ChannelPopulation
{
public:
	Rothman03Inhibitory();
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03Leakage : public Rothman03ChannelPopulation
{
public:
	//Rothman03Leakage(double Temperature_DegreesC);
	Rothman03Leakage(double Temperature_DegreesC, EnumedResponseType ResponseType);
	Rothman03Leakage(double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues);
	virtual void CalculateRates(double Voltage_V);
	virtual double GetCurrent_Amperes(void);
};

#endif // _ROTHMAN_03_CHANNELS_H_INCLUDED
