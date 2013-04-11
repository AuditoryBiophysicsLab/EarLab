#include "HHCompartmentFactory.h"

HHCompartment* HHCompartmentFactory::Create(char *CompartmentClassName, double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Enumed93ResponseType ResponseType93, double TauAHP_Seconds, double AHPIncrement_Siemens)
{
	if (!stricmp(CompartmentClassName, "Rothman03AHPCompartment"))
		return new Rothman03AHPCompartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, ResponseType, TauAHP_Seconds, AHPIncrement_Siemens);
	if (!stricmp(CompartmentClassName, "Rothman03Compartment"))
		return new Rothman03Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, ResponseType);
	if (!stricmp(CompartmentClassName, "Cai98Compartment"))
		return new Cai98Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, ResponseType93, TauAHP_Seconds, AHPIncrement_Siemens);
	if (!stricmp(CompartmentClassName, "Brughera96Compartment"))
		return new Brughera96Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, ResponseType93);
	// provide an Rothman93 option because sometimes BushyCell can be setup to use it
	if (!stricmp(CompartmentClassName, "Rothman93Compartment"))
		return new Rothman93Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, ResponseType93);

	// if CompartmentClassName hasn't matched a valid type by this point, an error has occurred 
	throw EarlabException("CompartmentClassName (%s) not a valid compartment.", CompartmentClassName);
};

//HHCompartment* HHCompartmentFactory::CreateCustomRothman03(char *CompartmentClassName, double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Enumed93ResponseType ResponseType93, double TauAHP_Seconds, double AHPIncrement_Siemens, Rothman03CustomParameterValues MyCustomParameterValues)
HHCompartment* HHCompartmentFactory::Create(char *CompartmentClassName, double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Enumed93ResponseType ResponseType93, double TauAHP_Seconds, double AHPIncrement_Siemens, Rothman03CustomParameterValues MyCustomParameterValues)
{
	if (ResponseType != 0)
		EarlabException("ResponseType=0 is required to use HHCompartmentFactory::CreateCustomRothman03.");
	
	if (!stricmp(CompartmentClassName, "Rothman03AHPCompartment"))
		return new Rothman03AHPCompartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, ResponseType, TauAHP_Seconds, AHPIncrement_Siemens, MyCustomParameterValues);
	if (!stricmp(CompartmentClassName, "Rothman03Compartment"))
			return new Rothman03Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, ResponseType, MyCustomParameterValues);
	//if (!stricmp(CompartmentClassName, "Cai98Compartment"))
	//	return new Cai98Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, ResponseType93, TauAHP_Seconds, AHPIncrement_Siemens);
	//if (!stricmp(CompartmentClassName, "Brughera96Compartment"))
	//	return new Brughera96Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, ResponseType93);
	//// provide an Rothman93 option because sometimes BushyCell can be setup to use it
	//if (!stricmp(CompartmentClassName, "Rothman93Compartment"))
	//	return new Rothman93Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, ResponseType93);

	//// if CompartmentClassName hasn't matched a valid type by this point, an error has occurred 
	throw EarlabException("CompartmentClassName (%s) not a valid compartment for HHCompartmentFactory::CreateCustom.", CompartmentClassName);
};

// In BrugheraMSO.cpp, remember to set a ResponseType variable to type II as pass it as the argument to HHCompartmentFactory.
// Currently, BrugheraMSO uses the constructor without a ResponseType which selects the default value, type II.