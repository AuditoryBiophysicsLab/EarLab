#ifndef _ROTHMAN_03_CHANNELS_H_INCLUDED
#define _ROTHMAN_03_CHANNELS_H_INCLUDED
#include "HHChannelPopulation.h"

const int ROTHMAN_03_RATE_CONSTANT = 1000;

typedef struct
{
	double MaxG_Na;
	double MaxG_HT;
	double MaxG_LT;
	double MaxG_a;
	double MaxG_h;
	double MaxG_lk;
} Rothman03ConductanceParameters;

class Rothman03ChannelPopulation : public HHChannelPopulation
{
public:
	Rothman03ChannelPopulation();
	Rothman03ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V) = 0;
	virtual double GetDerivative(double IntermediateY, double InstantaneousVoltage_V);
	double tau_y, y_inf;
	double InitialVoltage_v;
};

class Rothman03KFastTransient_a : public Rothman03ChannelPopulation
{
public:
	Rothman03KFastTransient_a(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03KFastTransient_b : public Rothman03ChannelPopulation
{
public:
	Rothman03KFastTransient_b(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03KFastTransient_c : public Rothman03ChannelPopulation
{
public:
	Rothman03KFastTransient_c(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03KLT_w : public Rothman03ChannelPopulation
{
public:
	Rothman03KLT_w(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03KLT_z : public Rothman03ChannelPopulation
{
public:
	Rothman03KLT_z(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03KHT_n : public Rothman03ChannelPopulation
{
public:
	Rothman03KHT_n(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03KHT_p : public Rothman03ChannelPopulation
{
public:
	Rothman03KHT_p(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03NaFast_m : public Rothman03ChannelPopulation
{
public:
	Rothman03NaFast_m(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03NaFast_h : public Rothman03ChannelPopulation
{
public:
	Rothman03NaFast_h(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03CATIONh_r : public Rothman03ChannelPopulation
{
public:
	Rothman03CATIONh_r(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03Excitatory : public Rothman03ChannelPopulation
{
public:
	Rothman03Excitatory();
	virtual void CalculateRates(double Voltage_V);
};

class Rothman03Leakage : public Rothman03ChannelPopulation
{
public:
	Rothman03Leakage(double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

#endif // _ROTHMAN_03_CHANNELS_H_INCLUDED
