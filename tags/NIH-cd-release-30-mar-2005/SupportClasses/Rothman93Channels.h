#ifndef _ROTHMAN_93_CHANNELS_H_INCLUDED
#define _ROTHMAN_93_CHANNELS_H_INCLUDED
#include "HHChannelPopulation.h"

#define ROTHMAN_93_RATE_CONSTANT 1000

class Rothman93Kslow : public HHChannelPopulation
{
public:
	Rothman93Kslow(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
	virtual double GetCurrent_Amperes(void);
};

class Rothman93Kfast : public HHChannelPopulation
{
public:
	Rothman93Kfast(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
	virtual double GetCurrent_Amperes(void);
};

class Rothman93NaActivation : public HHChannelPopulation
{
public:
	Rothman93NaActivation(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

class Rothman93NaInactivation : public HHChannelPopulation
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

class Rothman93Leakage : public HHChannelPopulation
{
public:
	Rothman93Leakage(double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
	virtual double GetCurrent_Amperes(void);
};

#endif
