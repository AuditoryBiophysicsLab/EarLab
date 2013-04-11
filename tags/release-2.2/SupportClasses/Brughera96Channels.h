#ifndef _BRUGHERA_96_CHANNELS_H_INCLUDED
#define _BRUGHERA_96_CHANNELS_H_INCLUDED
#include "HHChannelPopulation.h"

// Note: This factor is used to compensate for the fact that in the original Bushy Cell code by
// Hong Mei Cai and Andy Brughera, the millisecond-to-second scaling factor of 1000 was accidentally
// omitted from the code
#define BRUGHERA_96_RATE_CONSTANT .001

class Brughera96Kfast : public HHChannelPopulation
{
public:
	Brughera96Kfast(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
	virtual double GetCurrent_Amperes(void);
};

class Brughera96NaInactivation : public HHChannelPopulation
{
public:
	Brughera96NaInactivation(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
	virtual double GetCurrent_Amperes(void);
};

#endif
