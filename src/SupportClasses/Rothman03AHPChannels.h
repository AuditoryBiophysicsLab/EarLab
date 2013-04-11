#ifndef _ROTHMAN_03_AHP_CHANNELS_H_INCLUDED
#define _ROTHMAN_03_AHP_CHANNELS_H_INCLUDED
#include "Rothman03Channels.h"

//const int ROTHMAN_03_RATE_CONSTANT = 1000;

// AHP stands for After (action potential) Hyperpolarization Adaptation
// The AHP current is assumed to be via calcium-activated potassium channels 
// We are adding the Cai98AHP channel to the Rothman03 compartment
class Rothman03AHP : public HHChannelPopulation
{
public:
	Rothman03AHP(double InitialVoltage_V, double Temperature_DegreesC);
	virtual void CalculateRates(double Voltage_V);
};
#endif
