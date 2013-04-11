#include "HHChannelPopulation.h"
#include "Rothman03Channels.h"
#include <math.h>

// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
Rothman03ConductanceParameters Rothman03ParameterSet_I_c = 
{
	1000,
	150,
	0,
	0,
	0.5,
	2,
};

Rothman03ConductanceParameters Rothman03ParameterSet_I_t = 
{
	1000,
	80,
	0,
	65,
	0.5,
	2,
};

Rothman03ConductanceParameters Rothman03ParameterSet_I_II = 
{
	1000,
	150,
	20,
	0,
	2,
	2,
};

Rothman03ConductanceParameters Rothman03ParameterSet_II_I = 
{
	1000,
	150,
	35,
	0,
	3.5,
	2,
};

Rothman03ConductanceParameters Rothman03ParameterSet_II = 
{
	1000,
	150,
	200,
	0,
	20,
	2,
};

Rothman03ConductanceParameters *Rothman03ModelParameters = &Rothman03ParameterSet_I_c;

const double KReversalPotential_V  = -70.0e-3;
const double NaReversalPotential_V =  55.0e-3;
const double CATIONhReversalPotential_V  = -43.0e-3;
const double lkReversalPotential_V = -65.0e-3;

Rothman03ChannelPopulation::Rothman03ChannelPopulation()
{
	SetTemperature_DegreesC(38.0);	// Default value
	tau_y = 1.0;
	y_inf = 0.0;
	InitialVoltage_v = -60e-3;	// Default voltage -60 mV
}

Rothman03ChannelPopulation::Rothman03ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC) :
					 HHChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	SetTemperature_DegreesC(Temperature_DegreesC);
	tau_y = 1.0;
	y_inf = 0.0;
	this->InitialVoltage_v = InitialVoltage_v;
}

double Rothman03ChannelPopulation::GetDerivative(double IntermediateY, double InstantaneousVoltage_V)
{
	double dydt;

	this->IntermediateY = IntermediateY;

	CalculateRates(InstantaneousVoltage_V);
	
	dydt = (y_inf - IntermediateY) / tau_y;
	
	return dydt;
}

Rothman03KFastTransient_a::Rothman03KFastTransient_a(double InitialVoltage_V, double Temperature_DegreesC) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	/* FAST TRANSIENT POTASSIUM ACTIVATION VALUE (a) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_a * TemperatureFactor_2();		
	ReversalPotential_V = KReversalPotential_V;								
	CalculateRates(InitialVoltage_V);
}

void Rothman03KFastTransient_a::CalculateRates(double Voltage_V)
{
	y_inf = pow(1 + exp(-(Voltage_V + 31) / 6), -0.25);
	tau_y = (100 * (1 / ((7 * exp((Voltage_V + 60) / 14)) + (29 * exp(-(Voltage_V + 60) / 24))))) + 0.1;
	tau_y = tau_y / TemperatureFactor_3();
}

Rothman03KFastTransient_b::Rothman03KFastTransient_b(double InitialVoltage_V, double Temperature_DegreesC) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	/* FAST TRANSIENT POTASSIUM ACTIVATION VALUE (b) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_a * TemperatureFactor_2();
	ReversalPotential_V = KReversalPotential_V;
	CalculateRates(InitialVoltage_V);
}

void Rothman03KFastTransient_b::CalculateRates(double Voltage_V)
{
	y_inf = pow(1 + exp((Voltage_V + 66) / 7), -0.5);
	tau_y = (1000 * (1 / ((14 * exp((Voltage_V + 60) / 27)) + (29 * exp(-(Voltage_V + 60) / 24))))) + 1;
	tau_y = tau_y / TemperatureFactor_3();
}

Rothman03KFastTransient_c::Rothman03KFastTransient_c(double InitialVoltage_V, double Temperature_DegreesC) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	/* FAST TRANSIENT POTASSIUM ACTIVATION VALUE (b) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_a * TemperatureFactor_2();
	ReversalPotential_V = KReversalPotential_V;
	CalculateRates(InitialVoltage_V);
}

void Rothman03KFastTransient_c::CalculateRates(double Voltage_V)
{
	y_inf = pow(1 + exp((Voltage_V + 66) / 7), -0.5);
	tau_y = (90 * (1 / ((exp(-(Voltage_V + 66) / 17))))) + 10;
	tau_y = tau_y / TemperatureFactor_3();
}

Rothman03KLT_w::Rothman03KLT_w(double InitialVoltage_V, double Temperature_DegreesC) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	/* LOW THRESHOLD POTASSIUM ACTIVATION VALUE (w) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_LT * TemperatureFactor_2();
	ReversalPotential_V = KReversalPotential_V;
	CalculateRates(InitialVoltage_V);
}

void Rothman03KLT_w::CalculateRates(double Voltage_V)
{
	static double w1 = .107;  
	static double w2 = 33.0;  
	static double w3 = 13.1;  
	static double w4 = .01881;
	static double w5 = 30.0;  
	static double w6 = 30.3;     

	alpha = ROTHMAN_03_RATE_CONSTANT * ((w1 * TemperatureFactor_3()) / (1. + exp(-((Voltage_V * 1000) + w2) / w3)));	// Activation rate
	beta =  ROTHMAN_03_RATE_CONSTANT * (w4 * TemperatureFactor_3() * exp(-((Voltage_V * 1000) + w5) / w6));				// Deactivation rate
	tau_y = tau_y / TemperatureFactor_3();
}

Rothman03KLT_z::Rothman03KLT_z(double InitialVoltage_V, double Temperature_DegreesC) : 
					 Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	/* LOW THRESHOLD POTASSIUM ACTIVATION VALUE (w) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_LT * TemperatureFactor_2();
	ReversalPotential_V = KReversalPotential_V;
	CalculateRates(InitialVoltage_V);
}

void Rothman03KLT_z::CalculateRates(double Voltage_V)
{
	static double w1 = .107;  
	static double w2 = 33.0;  
	static double w3 = 13.1;  
	static double w4 = .01881;
	static double w5 = 30.0;  
	static double w6 = 30.3;     

	alpha = ROTHMAN_03_RATE_CONSTANT * ((w1 * TemperatureFactor_3()) / (1. + exp(-((Voltage_V * 1000) + w2) / w3)));	// Activation rate
	beta =  ROTHMAN_03_RATE_CONSTANT * (w4 * TemperatureFactor_3() * exp(-((Voltage_V * 1000) + w5) / w6));				// Deactivation rate
	tau_y = tau_y / TemperatureFactor_3();
}

Rothman03KHT_n::Rothman03KHT_n(double InitialVoltage_V, double Temperature_DegreesC) : 
				     Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	/* HIGH THRESHOLD POTASSIUM ACTIVATION VALUE (w) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_HT * TemperatureFactor_2();
	ReversalPotential_V = KReversalPotential_V;
	CalculateRates(InitialVoltage_V);
}

void Rothman03KHT_n::CalculateRates(double Voltage_V)
{
	/* Fast potassium activation channel */
	static double n1 = .0282; 
	static double n2 = 9.;    
	static double n3 = 12.;   
	static double n4 = 6.;
	static double n5 = 144;   
	static double n6 = 30.;   
	static double n8 = 6.;    
	static double n9 = 62.;

	if ((Voltage_V > (-n2 - 1e-7)) && (Voltage_V < (-n2 + 1e-7)) ||
		(Voltage_V > (-n5 - 1e-7)) && (Voltage_V < (-n5 + 1e-7)) ||
		(Voltage_V > (-n9 - 1e-7)) && (Voltage_V < (-n9 + 1e-7)))
		Voltage_V += 2e-7;

	alpha = ROTHMAN_03_RATE_CONSTANT * (( n1 * TemperatureFactor_3() * ((Voltage_V * 1000) + n2) ) / (1. - exp( -((Voltage_V * 1000) + n2)/n3)));
	beta =  ROTHMAN_03_RATE_CONSTANT * ((n4 * TemperatureFactor_3() * exp(-((Voltage_V * 1000) + n5)/n6)) + 
					((TemperatureFactor_3() * n8) / (1. + exp((Voltage_V * 1000) + n9)))); 
	tau_y = tau_y / TemperatureFactor_3();
}

Rothman03KHT_p::Rothman03KHT_p(double InitialVoltage_V, double Temperature_DegreesC) : 
				     Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	/* HIGH THRESHOLD POTASSIUM ACTIVATION VALUE (w) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_HT * TemperatureFactor_2();
	ReversalPotential_V = KReversalPotential_V;
	CalculateRates(InitialVoltage_V);
}

void Rothman03KHT_p::CalculateRates(double Voltage_V)
{
	/* Fast potassium activation channel */
	static double n1 = .0282; 
	static double n2 = 9.;    
	static double n3 = 12.;   
	static double n4 = 6.;
	static double n5 = 144;   
	static double n6 = 30.;   
	static double n8 = 6.;    
	static double n9 = 62.;

	if ((Voltage_V > (-n2 - 1e-7)) && (Voltage_V < (-n2 + 1e-7)) ||
		(Voltage_V > (-n5 - 1e-7)) && (Voltage_V < (-n5 + 1e-7)) ||
		(Voltage_V > (-n9 - 1e-7)) && (Voltage_V < (-n9 + 1e-7)))
		Voltage_V += 2e-7;

	alpha = ROTHMAN_03_RATE_CONSTANT * (( n1 * TemperatureFactor_3() * ((Voltage_V * 1000) + n2) ) / (1. - exp( -((Voltage_V * 1000) + n2)/n3)));
	beta =  ROTHMAN_03_RATE_CONSTANT * ((n4 * TemperatureFactor_3() * exp(-((Voltage_V * 1000) + n5)/n6)) + 
					((TemperatureFactor_3() * n8) / (1. + exp((Voltage_V * 1000) + n9)))); 
	tau_y = tau_y / TemperatureFactor_3();
}

Rothman03NaFast_m::Rothman03NaFast_m(double InitialVoltage_V, double Temperature_DegreesC) : 
							Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	/* FAST SODIUM CURRENT (m) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_Na * TemperatureFactor_2();
	ReversalPotential_V = NaReversalPotential_V;
	CalculateRates(InitialVoltage_V);
}

void Rothman03NaFast_m::CalculateRates(double Voltage_V)
{
	/* FAST SODIUM CURRENT (m) */
	y_inf = 1 / (1 + exp(-(Voltage_V + 38) / 7));
	tau_y = (10 * (1 / (5.0 * (exp((Voltage_V + 60) / 18))) + (36.0 * (exp(-(Voltage_V + 60) / 25))))) + 0.04;
	tau_y = tau_y / TemperatureFactor_3();
}

Rothman03NaFast_h::Rothman03NaFast_h(double InitialVoltage_V, double Temperature_DegreesC) : 
							  Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	/* FAST SODIUM CURRENT (h) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_Na * TemperatureFactor_2();
	ReversalPotential_V = NaReversalPotential_V;
	CalculateRates(InitialVoltage_V);
}

void Rothman03NaFast_h::CalculateRates(double Voltage_V)
{
	/* FAST SODIUM CURRENT (h) */
	y_inf = 1 / (1 + exp((Voltage_V + 65) / 6));
	tau_y = (100 * (1 / (7.0 * (exp((Voltage_V + 60) / 11))) + (10.0 * (exp((Voltage_V + 60) / 25))))) + 0.6;
	tau_y = tau_y / TemperatureFactor_3();
}

Rothman03CATIONh_r::Rothman03CATIONh_r(double InitialVoltage_V, double Temperature_DegreesC) : 
							  Rothman03ChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	/* HYPERPOLARIZATION-ACTIVATED CATION CURRENT (r) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_h * TemperatureFactor_2();
	ReversalPotential_V = CATIONhReversalPotential_V;
	CalculateRates(InitialVoltage_V);
}

void Rothman03CATIONh_r::CalculateRates(double Voltage_V)
{
	/* HYPERPOLARIZATION-ACTIVATED CATION CURRENT (r) */
	y_inf = 1 / (1 + exp((Voltage_V + 76) / 7));
	tau_y = (1.0e5 * (1 / (237.0 * (exp((Voltage_V + 60) / 12))) + (17.0 * (exp(-(Voltage_V + 60) / 14))))) + 25;
	tau_y = tau_y / TemperatureFactor_3();
}

Rothman03Excitatory::Rothman03Excitatory()
{
	MaxConductance_Siemens = 1000e-9;
	ReversalPotential_V = 0.0;	/* Excitatory synapse reversal potential (V) */
	CalculateRates(0.0);
}

void Rothman03Excitatory::CalculateRates(double Voltage_V)
{
	y_inf = 0;
	tau_y = 1;
	tau_y = tau_y / TemperatureFactor_3();
}

Rothman03Leakage::Rothman03Leakage(double Temperature_DegreesC) : 
							  Rothman03ChannelPopulation(0.0, Temperature_DegreesC)
{
	/* LEAKAGE CURRENT (lk) */
	// From Rothman 2003 (J Neurophys) [Vol 89 p. 3097]
	MaxConductance_Siemens = Rothman03ModelParameters->MaxG_lk * TemperatureFactor_2();
	ReversalPotential_V = lkReversalPotential_V;		/* leakage reversal potential (Volts) */
	CalculateRates(0.0);
}

void Rothman03Leakage::CalculateRates(double Voltage_V)
{
	y_inf = 0;
	tau_y = 1;
	tau_y = tau_y / TemperatureFactor_3();
}
