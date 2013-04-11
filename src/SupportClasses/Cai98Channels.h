#ifndef _CAI_98_CHANNELS_H_INCLUDED
#define _CAI_98_CHANNELS_H_INCLUDED
#include "Brughera96Channels.h"

#define BRUGHERA_96_RATE_CONSTANT .001

// AHP stands for Calcium Activated Hyperpolarization Adapatation
class Cai98AHP : public Brughera96ChannelPopulation
//class Cai98AHP : public Brughera96ChannelPopulation
{
public:
	Cai98AHP(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};
#endif
