#ifndef _BRUGHERA_96_COMPARTMENT_H_INCLUDED
#define _BRUGHERA_96_COMPARTMENT_H_INCLUDED
#include "Earlab.h"
#include "EarlabException.h"
#include "HHChannelPopulation.h"
#include "Brughera96Channels.h"
#include "Rothman93Compartment.h"
#include "HHSynapse.h"
#include "RungeKuttaEquation.h"
#include "RungeKuttaSolver.h"
#include "HHCompartment.h"

class Brughera96Compartment : public Rothman93Compartment
{
public:
	Brughera96Compartment() {};
	Brughera96Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC);
};

#endif
