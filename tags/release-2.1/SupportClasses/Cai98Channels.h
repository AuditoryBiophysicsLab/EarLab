#ifndef _CAI_98_CHANNELS_H_INCLUDED
#define _CAI_98_CHANNELS_H_INCLUDED
#include "HHChannelPopulation.h"

#define BRUGHERA_96_RATE_CONSTANT .001

// AHP stands for Calcium Activated Hyperpolarization Adapatation
class Cai98AHP : public HHChannelPopulation
{
public:
	Cai98AHP(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};
#endif
