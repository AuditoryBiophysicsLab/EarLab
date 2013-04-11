#include "Rothman03Channels.h"
#include <math.h>

// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
Rothman03ConductanceParameters Rothman03ParameterSet_I_c = 
{
	1000e-9,
	150e-9,
	0e-9,
	0e-9,
	0.5e-9,
	2e-9,
};

Rothman03ConductanceParameters Rothman03ParameterSet_I_t = 
{
	1000e-9,
	80e-9,
	0e-9,
	65e-9,
	0.5e-9,
	2e-9,
};

Rothman03ConductanceParameters Rothman03ParameterSet_I_II = 
{
	1000e-9,
	150e-9,
	20e-9,
	0e-9,
	2e-9,
	2e-9,
};

Rothman03ConductanceParameters Rothman03ParameterSet_II_I = 
{
	1000e-9,
	150e-9,
	35e-9,
	0e-9,
	3.5e-9,
	2e-9,
};

Rothman03ConductanceParameters Rothman03ParameterSet_II = 
{
	1000e-9,
	150e-9,
	200e-9,
	0e-9,
	20e-9,
	2e-9,
};

Rothman03ConductanceParameters Brughera08ParameterSet_MSO = 
{
	1000e-9,
	150e-9,
	400e-9,
	0e-9,
	40e-9,
	2e-9,
};

//Rothman03ConductanceParameters Zhou06ParameterSet_MSO = 
//{
//	7539.822e-9,  // Yi's G/area * soma_area, T=38C (soma_area = 2.513274e-5 cm^2)
//	502.6548e-9,  // Provided for reference only
//	753.9822e-9,  // Use the values below, which are scaled for Earlab
//	0.0e-9,       // to their base values at T=22C.
//	37.6991e-9,   // Earlab scales temperature sensitive values from base value at T=22C.
//	25.1327e-9,
//};
Rothman03ConductanceParameters Zhou06ParameterSet_MSO = 
{
	2487.2e-9,  // Yi's G/area * soma_area, scaled down from T=38C to T=22C.
	165.8143e-9,// Earlab scales temperature sensitive values from base value at T=22C. 
	248.7214e-9,
	0.0e-9,
	12.4361e-9,
	8.2907e-9,
};

// Rothman03ConductanceParameters Zhou06ParameterSet_MSO = 
// { // Shows the ordering of ion channels 
// 	Na
// 	KHT
// 	KLT
// 	KA  
// 	cation_h
//  leak
// };
// Note when using Zhou06ParameterSet_MSO, 
// set membraneCapacitance = 25.133e-12 (otherwise 12e-12) Farads in Rothman03Compartment.cpp
// set lkReversalPotential = -72.73e-3 (otherwise -65.0e-3) Volts in Rothman03Channels.h

//Rothman03ConductanceParameters *Rothman03ModelParameters = &Rothman03ParameterSet_II;
//Rothman03ConductanceParameters *Rothman03ModelParameters = &Zhou06ParameterSet_MSO;
Rothman03ConductanceParameters *Rothman03ModelParameters;

Rothman03ChannelPopulation::Rothman03ChannelPopulation()
{
	SetTemperature_DegreesC(38.0);	// Default value
	tau_y = 1.0;
	y_inf = 0.0;
	InitialVoltage_V = -64e-3;	// Default voltage -64 mV, close to resting potentials in RM2003
	this->ResponseType = TYPE2;
	SetRothman03ModelParameters(this->ResponseType);
}

//Rothman03ChannelPopulation::Rothman03ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC) :
//					 HHChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
//{
//	SetTemperature_DegreesC(Temperature_DegreesC);
//	tau_y = 1.0;
//	y_inf = 0.0;
//	this->InitialVoltage_V = InitialVoltage_V;
//	this->ResponseType = TYPE2;
//	SetRothman03ModelParameters(this->ResponseType);
//}

Rothman03ChannelPopulation::Rothman03ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType) :
					 HHChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	SetTemperature_DegreesC(Temperature_DegreesC);
	tau_y = 1.0;
	y_inf = 0.0;
	this->InitialVoltage_V = InitialVoltage_V;
	this->ResponseType = ResponseType;
	SetRothman03ModelParameters(this->ResponseType);
}

Rothman03ChannelPopulation::Rothman03ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues) :
					 HHChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{

	SetTemperature_DegreesC(Temperature_DegreesC);
	tau_y = 1.0;
	y_inf = 0.0;
	this->InitialVoltage_V = InitialVoltage_V;
	this->ResponseType = ResponseType;
	if (ResponseType == CUSTOMTYPE)
	{
		Rothman03ModelParameters->MaxG_Na =	MyCustomParameterValues.MaxG_Na;
		Rothman03ModelParameters->MaxG_HT = MyCustomParameterValues.MaxG_HT;
		Rothman03ModelParameters->MaxG_LT = MyCustomParameterValues.MaxG_LT;
		Rothman03ModelParameters->MaxG_A  = MyCustomParameterValues.MaxG_A;
		Rothman03ModelParameters->MaxG_h  = MyCustomParameterValues.MaxG_h;
		Rothman03ModelParameters->MaxG_lk = MyCustomParameterValues.MaxG_lk;
	}
	else
		SetRothman03ModelParameters(this->ResponseType);

}

void Rothman03ChannelPopulation::SetRothman03ModelParameters(EnumedResponseType ResponseType)
{
	// Rothman03Model Paraters will be used to set the maximum conductances according to ResponseType
	switch (ResponseType)
	{
	//	enum EnumedResponseType { TYPE1C=1, TYPE1T=11, TYPE12=12, TYPE21=21, TYPE2=2, TYPEZHOU2006=19, TYPEBRUGHERA2008 = 22};
		case TYPE1C:
			Rothman03ModelParameters = &Rothman03ParameterSet_I_c;
			//lkReversalPotential_V = -65.0e-3;
			break;
		case TYPE1T:
			Rothman03ModelParameters = &Rothman03ParameterSet_I_t;
			//lkReversalPotential_V = -65.0e-3;
			break;
		case TYPE12:
			Rothman03ModelParameters = &Rothman03ParameterSet_I_II;
			//lkReversalPotential_V = -65.0e-3;
			break;
		case TYPE21:
			Rothman03ModelParameters = &Rothman03ParameterSet_II_I;
			//lkReversalPotential_V = -65.0e-3;
			break;
		case TYPE2:
			Rothman03ModelParameters = &Rothman03ParameterSet_II;
			//lkReversalPotential_V = -65.0e-3;
			break;
		case TYPEBRUGHERA2008:
			Rothman03ModelParameters = &Brughera08ParameterSet_MSO;
			//lkReversalPotential_V = -65.0e-3;
			break;
		case TYPEZHOU2006:
			Rothman03ModelParameters = &Zhou06ParameterSet_MSO;
			//lkReversalPotential_V = -72.73e-3;
			break;
		default: //exception
			throw EarlabException("Rothman03ChannelPopulation::SetRothman03ModelParameters: Invalid ResponseType code.  Sorry!");
	}
}

double Rothman03ChannelPopulation::GetDerivative(double IntermediateY, double InstantaneousVoltage_V)
{
	double dydt;

	this->IntermediateY = IntermediateY;

	CalculateRates(InstantaneousVoltage_V);
	
	dydt = (y_inf - IntermediateY) / tau_y;
	
	return dydt;
}

Rothman03KFastTransient_a::Rothman03KFastTransient_a(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType)
{
	/* FAST TRANSIENT POTASSIUM ACTIVATION VALUE (a) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_A * TemperatureFactor_2();		
	ReversalPotential_V = KReversalPotential_V;								
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

Rothman03KFastTransient_a::Rothman03KFastTransient_a(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType, MyCustomParameterValues)
{
	/* FAST TRANSIENT POTASSIUM ACTIVATION VALUE (a) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_A * TemperatureFactor_2();		
	ReversalPotential_V = KReversalPotential_V;								
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void Rothman03KFastTransient_a::CalculateRates(double Voltage_V)
{
	// Units of Volts and seconds are used in Earlab HH models.
	// Formulae in the Rothman03 Calculate Rates functions are taken directly
	// from Rothman and Manis 2003 model, which are for units mV and ms.
	// So in the  Voltage_V is converted to mV using (Voltage_V * 1000)
	// and the output tau_y is converted to seconds by dividing by ROTHMAN_03_RATE_CONSTANT = 1000. 

	y_inf = pow(1 + exp(-((Voltage_V * 1000) + 31) / 6), -0.25);
	tau_y = (100 * (1 / ((7 * exp(((Voltage_V * 1000) + 60) / 14)) + (29 * exp(-((Voltage_V * 1000) + 60) / 24))))) + 0.1;
	tau_y = tau_y / (TemperatureFactor_3() * ROTHMAN_03_RATE_CONSTANT);
	alpha = y_inf / tau_y;
	beta = (1 - y_inf) / tau_y;
}

Rothman03KFastTransient_b::Rothman03KFastTransient_b(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType)
{
	/* FAST TRANSIENT POTASSIUM ACTIVATION VALUE (b) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_A * TemperatureFactor_2();
	ReversalPotential_V = KReversalPotential_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

Rothman03KFastTransient_b::Rothman03KFastTransient_b(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType, MyCustomParameterValues)
{
	/* FAST TRANSIENT POTASSIUM ACTIVATION VALUE (b) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_A * TemperatureFactor_2();
	ReversalPotential_V = KReversalPotential_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void Rothman03KFastTransient_b::CalculateRates(double Voltage_V)
{
	y_inf = pow(1 + exp(((Voltage_V * 1000) + 66) / 7), -0.5);
	tau_y = (1000 * (1 / ((14 * exp(((Voltage_V * 1000) + 60) / 27)) + (29 * exp(-((Voltage_V * 1000) + 60) / 24))))) + 1;
	tau_y = tau_y / (TemperatureFactor_3() * ROTHMAN_03_RATE_CONSTANT);
	alpha = y_inf / tau_y;
	beta = (1 - y_inf) / tau_y;
}

Rothman03KFastTransient_c::Rothman03KFastTransient_c(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType)
{
	/* FAST TRANSIENT POTASSIUM ACTIVATION VALUE (b) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_A * TemperatureFactor_2();
	ReversalPotential_V = KReversalPotential_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

Rothman03KFastTransient_c::Rothman03KFastTransient_c(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType, MyCustomParameterValues)
{
	/* FAST TRANSIENT POTASSIUM ACTIVATION VALUE (b) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_A * TemperatureFactor_2();
	ReversalPotential_V = KReversalPotential_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void Rothman03KFastTransient_c::CalculateRates(double Voltage_V)
{
	y_inf = pow(1 + exp(((Voltage_V * 1000) + 66) / 7), -0.5);
	tau_y = (90 * (1 / (1 + exp(-((Voltage_V * 1000) + 66) / 17)))) + 10;
	tau_y = tau_y / (TemperatureFactor_3() * ROTHMAN_03_RATE_CONSTANT);
	alpha = y_inf / tau_y;
	beta = (1 - y_inf) / tau_y;
}

Rothman03KLT_w::Rothman03KLT_w(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType)
{
	/* LOW THRESHOLD POTASSIUM ACTIVATION VALUE (w) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_LT * TemperatureFactor_2();
	ReversalPotential_V = KReversalPotential_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

Rothman03KLT_w::Rothman03KLT_w(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType, MyCustomParameterValues)
{
	/* LOW THRESHOLD POTASSIUM ACTIVATION VALUE (w) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_LT * TemperatureFactor_2();
	ReversalPotential_V = KReversalPotential_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void Rothman03KLT_w::CalculateRates(double Voltage_V)
{
	y_inf = pow(1 + exp(-((Voltage_V * 1000) + 48) / 6), -0.25);
	tau_y = (100 * (1 / ((6 * exp(((Voltage_V * 1000) + 60) / 6)) + (16 * exp(-((Voltage_V * 1000) + 60) / 45))))) + 1.5;
	tau_y = tau_y / (TemperatureFactor_3() * ROTHMAN_03_RATE_CONSTANT);
	alpha = y_inf / tau_y;
	beta = (1 - y_inf) / tau_y;
}

Rothman03KLT_z::Rothman03KLT_z(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType)
{
	/* LOW THRESHOLD POTASSIUM ACTIVATION VALUE (w) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_LT * TemperatureFactor_2();
	ReversalPotential_V = KReversalPotential_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

Rothman03KLT_z::Rothman03KLT_z(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType, MyCustomParameterValues)
{
	/* LOW THRESHOLD POTASSIUM ACTIVATION VALUE (w) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_LT * TemperatureFactor_2();
	ReversalPotential_V = KReversalPotential_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}
void Rothman03KLT_z::CalculateRates(double Voltage_V)
{
	const double zeta = 0.5;
	y_inf = (1 - zeta) * (1 / (1 + exp(((Voltage_V * 1000) + 71) / 10))) + zeta;
	tau_y = (1000 * (1 / (exp(((Voltage_V * 1000) + 60) / 20) + exp(-((Voltage_V * 1000) + 60) / 8)))) + 50;
	tau_y = tau_y / (TemperatureFactor_3() * ROTHMAN_03_RATE_CONSTANT);
	alpha = y_inf / tau_y;
	beta = (1 - y_inf) / tau_y;
}

Rothman03KHT_n::Rothman03KHT_n(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType)
{
	/* HIGH THRESHOLD POTASSIUM ACTIVATION VALUE (w) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_HT * TemperatureFactor_2();
	ReversalPotential_V = KReversalPotential_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

Rothman03KHT_n::Rothman03KHT_n(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType, MyCustomParameterValues)
{
	/* HIGH THRESHOLD POTASSIUM ACTIVATION VALUE (w) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_HT * TemperatureFactor_2();
	ReversalPotential_V = KReversalPotential_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void Rothman03KHT_n::CalculateRates(double Voltage_V)
{
	if (this->ResponseType == TYPEZHOU2006)
		y_inf = pow(1 + exp(-((Voltage_V * 1000) + 15) / 5), -2.0); // -2.0 is used to compare with Rothman's PhD Thesis and Yi's NEURON implementation
	else 
		y_inf = pow(1 + exp(-((Voltage_V * 1000) + 15) / 5), -0.5);

	tau_y = (100 * (1 / ((11 * exp(((Voltage_V * 1000) + 60) / 24)) + (21 * exp(-((Voltage_V * 1000) + 60) / 23))))) + 0.7;
	tau_y = tau_y / (TemperatureFactor_3() * ROTHMAN_03_RATE_CONSTANT);
	alpha = y_inf / tau_y;
	beta = (1 - y_inf) / tau_y;
}

Rothman03KHT_p::Rothman03KHT_p(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType)
{
	/* HIGH THRESHOLD POTASSIUM ACTIVATION VALUE (w) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_HT * TemperatureFactor_2();
	ReversalPotential_V = KReversalPotential_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

Rothman03KHT_p::Rothman03KHT_p(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType, MyCustomParameterValues)
{
	/* HIGH THRESHOLD POTASSIUM ACTIVATION VALUE (w) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_HT * TemperatureFactor_2();
	ReversalPotential_V = KReversalPotential_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void Rothman03KHT_p::CalculateRates(double Voltage_V)
{
	y_inf = 1 / (1 + exp(-((Voltage_V * 1000) + 23) / 6));
	tau_y = (100 * (1 / ((4 * exp(((Voltage_V * 1000) + 60) / 32)) + (5 * exp(-((Voltage_V * 1000) + 60) / 22))))) + 5;
	tau_y = tau_y / (TemperatureFactor_3() * ROTHMAN_03_RATE_CONSTANT);
	alpha = y_inf / tau_y;
	beta = (1 - y_inf) / tau_y;
}

Rothman03NaFast_m::Rothman03NaFast_m(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType)
{
	/* FAST SODIUM CURRENT (m) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_Na * TemperatureFactor_2();
	ReversalPotential_V = NaReversalPotential_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

Rothman03NaFast_m::Rothman03NaFast_m(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType, MyCustomParameterValues)
{
	/* FAST SODIUM CURRENT (m) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_Na * TemperatureFactor_2();
	ReversalPotential_V = NaReversalPotential_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void Rothman03NaFast_m::CalculateRates(double Voltage_V)
{
	/* FAST SODIUM CURRENT (m) */
	y_inf = 1 / (1 + exp(-((Voltage_V * 1000) + 38) / 7));
	tau_y = (10 * (1 / ((5.0 * exp(((Voltage_V * 1000) + 60) / 18)) + (36.0 * exp(-((Voltage_V * 1000) + 60) / 25))))) + 0.04;
	tau_y = tau_y / (TemperatureFactor_3() * ROTHMAN_03_RATE_CONSTANT);
	//tau_y = tau_y * 0.17 / (ROTHMAN_03_RATE_CONSTANT);
	alpha = y_inf / tau_y;
	beta = (1 - y_inf) / tau_y;
}

Rothman03NaFast_h::Rothman03NaFast_h(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType)
{
	/* FAST SODIUM CURRENT (h) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_Na * TemperatureFactor_2();
	ReversalPotential_V = NaReversalPotential_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

Rothman03NaFast_h::Rothman03NaFast_h(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType, MyCustomParameterValues)
{
	/* FAST SODIUM CURRENT (h) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_Na * TemperatureFactor_2();
	ReversalPotential_V = NaReversalPotential_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void Rothman03NaFast_h::CalculateRates(double Voltage_V)
{
	/* FAST SODIUM CURRENT (h) */
	y_inf = 1 / (1 + exp(((Voltage_V * 1000) + 65) / 6));
	tau_y = (100 * (1 / ((7.0 * exp(((Voltage_V * 1000) + 60) / 11)) + (10.0 * exp(-((Voltage_V * 1000) + 60) / 25))))) + 0.6;
	tau_y = tau_y / (TemperatureFactor_3() * ROTHMAN_03_RATE_CONSTANT);
	alpha = y_inf / tau_y;
	beta = (1 - y_inf) / tau_y;
}

Rothman03CATIONh_r::Rothman03CATIONh_r(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType)
{
	/* HYPERPOLARIZATION-ACTIVATED CATION CURRENT (r) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_h * TemperatureFactor_2();
	ReversalPotential_V = CATIONhReversalPotential_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

Rothman03CATIONh_r::Rothman03CATIONh_r(double InitialVoltage_V, double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType, MyCustomParameterValues)
{
	/* HYPERPOLARIZATION-ACTIVATED CATION CURRENT (r) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_h * TemperatureFactor_2();
	ReversalPotential_V = CATIONhReversalPotential_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void Rothman03CATIONh_r::CalculateRates(double Voltage_V)
{
	/* HYPERPOLARIZATION-ACTIVATED CATION CURRENT (r) */
	y_inf = 1 / (1 + exp(((Voltage_V * 1000) + 76) / 7));
	tau_y = (1.0e5 * (1 / ((237.0 * exp(((Voltage_V * 1000) + 60) / 12)) + (17.0 * exp(-((Voltage_V * 1000) + 60) / 14))))) + 25;
	tau_y = tau_y / (TemperatureFactor_3() * ROTHMAN_03_RATE_CONSTANT);
	alpha = y_inf / tau_y;
	beta = (1 - y_inf) / tau_y;
}

Rothman03Excitatory::Rothman03Excitatory()	
{
	MaxConductance_Siemens = 1000e-9;
	ReversalPotential_V = EReversalPotential_V;	/* Excitatory synapse reversal potential (V) */
	CalculateRates(0.0);
	CalculateSteadyState();
}

void Rothman03Excitatory::CalculateRates(double Voltage_V)
{
	y_inf = 0;
	tau_y = 1;
	tau_y = tau_y / (TemperatureFactor_3() * ROTHMAN_03_RATE_CONSTANT);
	alpha = y_inf / tau_y;
	beta = (1 - y_inf) / tau_y;
}

Rothman03Inhibitory::Rothman03Inhibitory()
{
	MaxConductance_Siemens = 1000e-9;
	// ReversalPotential_V = -66.5e-3;	/* Inhibitory synapse reversal potential (mV) */
	ReversalPotential_V = IReversalPotential_V;	/* Inhibitory synapse reversal potential (mV) */
	CalculateRates(0.0);
	CalculateSteadyState();
}

void Rothman03Inhibitory::CalculateRates(double Voltage_V)
{
	y_inf = 0;
	tau_y = 1;
	tau_y = tau_y / (TemperatureFactor_3() * ROTHMAN_03_RATE_CONSTANT);
	alpha = y_inf / tau_y;
	beta = (1 - y_inf) / tau_y;
}


Rothman03Leakage::Rothman03Leakage(double Temperature_DegreesC, EnumedResponseType ResponseType) : 
							  Rothman03ChannelPopulation(0.0, Temperature_DegreesC, ResponseType)
{
	/* LEAKAGE CURRENT (lk) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_lk * TemperatureFactor_2();
	if (ResponseType == TYPEZHOU2006)
		ReversalPotential_V = ZhoulkReversalPotential_V;	/* leakage reversal potential (Volts) */
	else
		ReversalPotential_V = lkReversalPotential_V;		/* leakage reversal potential (Volts) */
	CalculateRates(0.0);
	CalculateSteadyState();
}

Rothman03Leakage::Rothman03Leakage(double Temperature_DegreesC, EnumedResponseType ResponseType, Rothman03CustomParameterValues MyCustomParameterValues) : 
							  Rothman03ChannelPopulation(0.0, Temperature_DegreesC, ResponseType, MyCustomParameterValues)
{
	/* LEAKAGE CURRENT (lk) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_lk * TemperatureFactor_2();
	if (ResponseType == TYPEZHOU2006)
		ReversalPotential_V = ZhoulkReversalPotential_V;	/* leakage reversal potential (Volts) */
	else
		ReversalPotential_V = lkReversalPotential_V;		/* leakage reversal potential (Volts) */
	CalculateRates(0.0);
	CalculateSteadyState();
}

void Rothman03Leakage::CalculateRates(double Voltage_V)
{
	y_inf = 0;
	tau_y = 1;
	tau_y = tau_y / (TemperatureFactor_3() * ROTHMAN_03_RATE_CONSTANT);
	alpha = y_inf / tau_y;
	beta = (1 - y_inf) / tau_y;
}

double Rothman03Leakage::GetCurrent_Amperes(void)
{
	return MaxConductance_Siemens * (myCompartment->GetVoltage_V() - ReversalPotential_V);
}
