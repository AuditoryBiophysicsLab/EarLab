#ifndef _ROTHMAN_93_AHP_CHANNELS_H_INCLUDED
#define _ROTHMAN_93_AHP_CHANNELS_H_INCLUDED
#include "HHChannelPopulation.h"

//const int ROTHMAN_93_RATE_CONSTANT = 1000;

// AHP stands for After (action potential) Hyperpolarization Adaptation
// The AHP current is assumed to be via calcium-activated potassium channels 
// We are adding the Cai98AHP channel to the Rothman93 compartment
class Rothman93AHP : public HHChannelPopulation
{
public:
	Rothman93AHP(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};
#endif
