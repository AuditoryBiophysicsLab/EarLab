#ifndef _BRUGHERA_96_COMPARTMENT_H_INCLUDED
#define _BRUGHERA_96_COMPARTMENT_H_INCLUDED
#include "Earlab.h"
#include "EarlabException.h"
#include "Brughera96Channels.h"
#include "Rothman93Compartment.h"

class Brughera96Compartment : public Rothman93Compartment
{
public:
	Brughera96Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC);
	Brughera96Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType);
	Brughera96Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, int AdditionalConductances);
	Brughera96Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType, int AdditionalConductances);
};

#endif
