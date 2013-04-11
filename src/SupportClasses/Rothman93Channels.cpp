#include "Rothman93Channels.h"
#include <math.h>



void Rothman93ChannelPopulation::SetRothman93ModelParameters(Enumed93ResponseType ResponseType)
{

	// Set_I obtained by testing in Earlab to obtain Type1 current-clamp response
	Rothman93ConductanceParameters Rothman93ParameterSet_I = {1000e-9, 150e-9, 0e-9, 1.7e-9};
	// The Set_II conductance parameters are from Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2564]
	// and produce a Type2 current-clamp in the paper and in Earlab
	Rothman93ConductanceParameters Rothman93ParameterSet_II = {325e-9, 40e-9, 20e-9, 1.7e-9};

	// Rothman93Model Parameters will be used to set the maximum conductances according to ResponseType
	switch (ResponseType)
	{
	// 	enum ResponseType { TYPE_I=1, TYPE_II=2};
		case TYPE_I:
			Rothman93ModelParameters = &Rothman93ParameterSet_I;
			break;
		case TYPE_II:
			Rothman93ModelParameters = &Rothman93ParameterSet_II;
			break;
		default: //exception
			throw EarlabException("Rothman93ChannelPopulation::SetRothman93ModelParameters: Invalid ResponseType code.  Sorry!");
	}
}

Rothman93ChannelPopulation::Rothman93ChannelPopulation() : HHChannelPopulation()
{
	
	SetTemperature_DegreesC(38.0);
	this->ResponseType = TYPE_II;
	SetRothman93ModelParameters(this->ResponseType);
	this->RateType = ROTHMAN93RATES;
}

Rothman93ChannelPopulation::Rothman93ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC) : 
					HHChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	SetTemperature_DegreesC(Temperature_DegreesC);
	this->ResponseType = TYPE_II;
	SetRothman93ModelParameters(this->ResponseType);
	this->RateType = ROTHMAN93RATES;
}

Rothman93ChannelPopulation::Rothman93ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType) :
					 HHChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	SetTemperature_DegreesC(Temperature_DegreesC);	
	this->ResponseType = ResponseType;
	SetRothman93ModelParameters(this->ResponseType);
	this->RateType = ROTHMAN93RATES;
}
Rothman93ChannelPopulation::Rothman93ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType, Enumed93RateType RateType) :
					 HHChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	SetTemperature_DegreesC(Temperature_DegreesC);
	this->ResponseType = ResponseType;
	SetRothman93ModelParameters(this->ResponseType);
	this->RateType = RateType;
}

Rothman93Kslow::Rothman93Kslow(double InitialVoltage_V, double Temperature_DegreesC) : 
					 Rothman93ChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	/* SLOW POTASSIUM ACTIVATION VALUES (w) */
	// Was: 20e-9 when we started fiddling
	// But IC model current clamp responses from Cai 1998 (II) are Type I,
	// which suggest no/low Kslow conductance.
	//MaxConductance_Siemens = 20e-9 * TemperatureFactor_2_5();	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2581]
																// GmaxKLT=20nS good for Type II current clamp response.

	//MaxConductance_Siemens = 0.0e-9 * TemperatureFactor_2_5();	// GmaxKLT=0 good for Type I current clamp response.

	MaxConductance_Siemens = this->Rothman93ModelParameters->MaxG_KSlow93 * TemperatureFactor_2_5();

	ReversalPotential_V = KReversalPotential93_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void Rothman93Kslow::CalculateRates(double Voltage_V)
{
	static double w1 = .107;  
	static double w2 = 33.0;  
	static double w3 = 13.1;  
	static double w4 = .01881;
	static double w5 = 30.0;  
	static double w6 = 30.3;
	double TemperatureFactor;

	if (this->RateType == BRUGHERA96RATES)
		TemperatureFactor = TemperatureFactor_2_6655();
	else
		TemperatureFactor = TemperatureFactor_3();

	alpha = ROTHMAN_93_RATE_CONSTANT * ((w1 * TemperatureFactor) / (1. + exp(-((Voltage_V * 1000) + w2) / w3)));	// Activation rate
	beta =  ROTHMAN_93_RATE_CONSTANT * (w4 * TemperatureFactor * exp(-((Voltage_V * 1000) + w5) / w6));				// Deactivation rate
}

double Rothman93Kslow::GetCurrent_Amperes(void)
{
	return MaxConductance_Siemens * IntermediateY * (myCompartment->GetVoltage_V() - ReversalPotential_V);
}


Rothman93Kfast::Rothman93Kfast(double InitialVoltage_V, double Temperature_DegreesC) : 
				     Rothman93ChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	/* FAST POTASSIUM ACTIVATION VALUES (w) */
	//MaxConductance_Siemens = 40e-9 * TemperatureFactor_2_5();	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2581]
																// GmaxKHT=40nS good for Type II current clamp response.

	//MaxConductance_Siemens = 150e-9 * TemperatureFactor_2_5();	// GmaxKHT=150nS best for Type I current clamp response as in Cai 1998 (II)
																	// GmaxKHT between 80 nS and 360nS were all okay for Type I.
																	// However, with smaller Gs smaller Cmembrane values 
																	// were required to avoid depolarization block when
																	// depolarizing currents were increased.

	MaxConductance_Siemens = Rothman93ModelParameters->MaxG_KFast93 * TemperatureFactor_2_5();

	ReversalPotential_V = KReversalPotential93_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void Rothman93Kfast::CalculateRates(double Voltage_V)
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
	double TemperatureFactor;

	if ((Voltage_V > (-n2 - 1e-7)) && (Voltage_V < (-n2 + 1e-7)) ||
		(Voltage_V > (-n5 - 1e-7)) && (Voltage_V < (-n5 + 1e-7)) ||
		(Voltage_V > (-n9 - 1e-7)) && (Voltage_V < (-n9 + 1e-7)))
		Voltage_V += 2e-7;

	if (this->RateType == BRUGHERA96RATES)
		TemperatureFactor = TemperatureFactor_2_6655();
	else
		TemperatureFactor = TemperatureFactor_3();

	alpha = ROTHMAN_93_RATE_CONSTANT * (( n1 * TemperatureFactor * ((Voltage_V * 1000) + n2) ) / (1. - exp( -((Voltage_V * 1000) + n2)/n3)));
	beta =  ROTHMAN_93_RATE_CONSTANT * ((n4 * TemperatureFactor * exp(-((Voltage_V * 1000) + n5)/n6)) + 
					((TemperatureFactor * n8) / (1. + exp((Voltage_V * 1000) + n9)))); 
}

double Rothman93Kfast::GetCurrent_Amperes(void)
{
	return MaxConductance_Siemens * IntermediateY * (myCompartment->GetVoltage_V() - ReversalPotential_V);
}

Rothman93NaActivation::Rothman93NaActivation(double InitialVoltage_V, double Temperature_DegreesC) : 
							Rothman93ChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	//MaxConductance_Siemens = 325e-9 * TemperatureFactor_2();	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2581]
																// Gives Type II current clamp response with
																// other parameters values set as in Rothman 1993.

	//MaxConductance_Siemens = 1000e-9 * TemperatureFactor_2();	// GmaxNa values between 275e-9 and 1000e-9  
																// were all okay for Type I current clamp response
																// with GmaxKHT=150nS, GmaxKHT=0.
																// However, with smaller Gs smaller Cmembrane values 
																// were required to avoid depolarization block when
																// depolarizing currents were increased.
																// For GmaxKHT=1000nS, Cmembrane=12pF works well.

	MaxConductance_Siemens = Rothman93ModelParameters->MaxG_Na93 * TemperatureFactor_2();

	ReversalPotential_V = NaReversalPotential93_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void Rothman93NaActivation::CalculateRates(double Voltage_V)
{
	/* Fast sodium activation channel */  
	static double m1 = .36;   
	static double m2 = 49.;   
	static double m3 = 3.;    
	static double m4 = -0.4;
	static double m5 = 58.;   
	static double m6 = 20.;
	double TemperatureFactor;

	if ((Voltage_V > (-m2 - 1e-7)) && (Voltage_V < (-m2 + 1e-7)) ||
	    (Voltage_V > (-m5 - 1e-7)) && (Voltage_V < (-m5 + 1e-7)))
		Voltage_V += 2e-7;

	if (this->RateType == BRUGHERA96RATES)
		TemperatureFactor = TemperatureFactor_2_6655();
	else
		TemperatureFactor = TemperatureFactor_3();

	alpha = ROTHMAN_93_RATE_CONSTANT * ((m1 * TemperatureFactor * ((Voltage_V * 1000) + m2)) / (1. - exp(-((Voltage_V * 1000) + m2)/m3)));
	beta =  ROTHMAN_93_RATE_CONSTANT * ((m4 * TemperatureFactor * ((Voltage_V * 1000) + m5)) / (1. - exp(((Voltage_V * 1000) + m5)/m6))); 
}

Rothman93NaInactivation::Rothman93NaInactivation(double InitialVoltage_V, double Temperature_DegreesC) : 
							  Rothman93ChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	//MaxConductance_Siemens = 325e-9 * TemperatureFactor_2();	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2581]
																// Gives Type II current clamp response with
																// other parameters values set as in Rothman 1993.

	//MaxConductance_Siemens = 1000e-9 * TemperatureFactor_2();	// GmaxNa values between 275e-9 and 1000e-9  
																// were all okay for Type I current clamp response
																// with GmaxKHT=150nS, GmaxKHT=0.
																// However, with smaller Gs smaller Cmembrane values 
																// were required to avoid depolarization block when
																// depolarizing currents were increased.
																// For GmaxKHT=1000nS, Cmembrane=12pF works well.

	MaxConductance_Siemens = Rothman93ModelParameters->MaxG_Na93 * TemperatureFactor_2();


	ReversalPotential_V = NaReversalPotential93_V;
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

void Rothman93NaInactivation::CalculateRates(double Voltage_V)
{
	/* Fast sodium inactivation channel */
	static double h1 = 2.4;   
	static double h2 = 68.;   
	static double h3 = 3.;    
	static double h4 = 0.8;
	static double h5 = 61.3;  
	static double h8 = 3.6;  
	static double h9 = 21.; 
	static double h10 = 10.;
	double TemperatureFactorSmall, TemperatureFactorLarge;

	if (this->RateType == BRUGHERA96RATES)
	{
		TemperatureFactorSmall = TemperatureFactor_2_6655();
		TemperatureFactorLarge = TemperatureFactor_5_6569();
	}
	else
	{
		TemperatureFactorSmall = TemperatureFactor_3();
		TemperatureFactorLarge = TemperatureFactor_10();
	}

	/* FAST SODIUM INACTIVATION VALUES (h) */
	alpha = ROTHMAN_93_RATE_CONSTANT * ((h1 * TemperatureFactorSmall / (1. + exp(((Voltage_V * 1000) + h2) / h3))) + 
		((h4 * TemperatureFactorLarge) / (1. + exp((Voltage_V * 1000) + h5))));
	beta =  ROTHMAN_93_RATE_CONSTANT * ((h8 * TemperatureFactorSmall) / (1. + exp(-((Voltage_V * 1000) + h9) / h10)));
}

Rothman93Excitatory::Rothman93Excitatory()
{
	MaxConductance_Siemens = 1000e-9;
	ReversalPotential_V = EReversalPotential93_V;	// Excitatory synapse reversal potential
	CalculateRates(0.0);
	CalculateSteadyState();
}

void Rothman93Excitatory::CalculateRates(double Voltage_V)
{
	alpha = 0.0;
	beta = 0.0;
}

Rothman93Inhibitory::Rothman93Inhibitory()
{
	MaxConductance_Siemens = 1000e-9;
	ReversalPotential_V = IReversalPotential93_V;	// Inhibitory synapse reversal potential
	CalculateRates(0.0);
	CalculateSteadyState();
}

void Rothman93Inhibitory::CalculateRates(double Voltage_V)
{
	alpha = 0.0;
	beta = 0.0;
}

Rothman93Leakage::Rothman93Leakage(double Temperature_DegreesC) : 
							  Rothman93ChannelPopulation(0.0, Temperature_DegreesC)
{
	//MaxConductance_Siemens = 1.7e-9 * TemperatureFactor_2();

	MaxConductance_Siemens = Rothman93ModelParameters->MaxG_Leakage93 * TemperatureFactor_2();

	ReversalPotential_V = lkReversalPotential93_V;
	CalculateRates(0.0);
	CalculateSteadyState();
}

void Rothman93Leakage::CalculateRates(double Voltage_V)
{
	alpha = 0.0;
	beta =  0.0;
}

double Rothman93Leakage::GetCurrent_Amperes(void)
{
	return MaxConductance_Siemens * (myCompartment->GetVoltage_V() - ReversalPotential_V);
}
