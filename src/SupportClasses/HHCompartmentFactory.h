#ifndef _HH_COMPARTMENT_FACTORY_H_INCLUDED
#define _HH_COMPARTMENT_FACTORY_H_INCLUDED
#include "Earlab.h"
#include "EarlabException.h"
#include "Rothman03AHPCompartment.h"
#include "Cai98Compartment.h"

class HHCompartmentFactory
{
public:
	static HHCompartment* Create(char *CompartmentClassName, double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Enumed93ResponseType ResponseType93, double TauAHP_Seconds, double AHPIncrement_Siemens);
	//static HHCompartment* CreateCustomRothman03(char *CompartmentClassName, double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Enumed93ResponseType ResponseType93, double TauAHP_Seconds, double AHPIncrement_Siemens, Rothman03CustomParameterValues MyCustomParameterValues);
	static HHCompartment* Create(char *CompartmentClassName, double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Enumed93ResponseType ResponseType93, double TauAHP_Seconds, double AHPIncrement_Siemens, Rothman03CustomParameterValues MyCustomParameterValues);
};

#endif