#ifndef _BRUGHERA_96_CHANNELS_H_INCLUDED
#define _BRUGHERA_96_CHANNELS_H_INCLUDED
#include "Rothman93Channels.h"
//#include "HHChannelPopulation.h"

// Note: This factor is used to compensate for the fact that in the original Bushy Cell code by
// Hong Mei Cai and Andy Brughera (Cai et al. 1998; Brughera et al. 1996), the millisecond-to-second scaling factor of 1000 was 
// omitted from the second terms in the expressions for beta_n and alpha_h (Rothman et al. 1993).
#define BRUGHERA_96_RATE_CONSTANT .001

const double lkReversalPotentialBrughera96_V = 22.0e-3;


class Brughera96ChannelPopulation : public Rothman93ChannelPopulation
{
public:
	Brughera96ChannelPopulation();
	Brughera96ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC);
	Brughera96ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType);
	Brughera96ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType, Enumed93RateType RateType);
};

// The following are identical to Rothman93 except that Brughera96ChannelPopulation is used in the constructor

class Brughera96Kslow : public Brughera96ChannelPopulation
{
public:
	Brughera96Kslow(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
	virtual double GetCurrent_Amperes(void);
};

class Brughera96Kfast : public Brughera96ChannelPopulation
{
public:
	Brughera96Kfast(double InitialVoltage_V, double Temperature_DegreesC);
	Brughera96Kfast(double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType, Enumed93RateType RateType);
	virtual void CalculateRates(double Voltage_V);
	virtual double GetCurrent_Amperes(void);
};

class Brughera96NaActivation : public Brughera96ChannelPopulation
{
public:
	Brughera96NaActivation(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};

class Brughera96NaInactivation : public Brughera96ChannelPopulation
{
public:
	Brughera96NaInactivation(double InitialVoltage_V, double Temperature_DegreesC);
	Brughera96NaInactivation(double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType, Enumed93RateType RateType);
	virtual void CalculateRates(double Voltage_V);
};

class Brughera96Leakage : public Brughera96ChannelPopulation
{
public:
	Brughera96Leakage(double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
	virtual double GetCurrent_Amperes(void);
};

#endif
