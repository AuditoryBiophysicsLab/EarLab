#include "Brughera96Channels.h"
#include "Rothman93Channels.h"
#include <math.h>


Brughera96ChannelPopulation::Brughera96ChannelPopulation() : Rothman93ChannelPopulation()
{
	
	//SetTemperature_DegreesC(38.0);
	//this->ResponseType = TYPE_II;
	//SetRothman93ModelParameters(this->ResponseType);
	this->RateType = BRUGHERA96RATES;
}

Brughera96ChannelPopulation::Brughera96ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC) :
				Rothman93ChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
{
	//SetTemperature_DegreesC(Temperature_DegreesC);
	//this->ResponseType = TYPE_II;
	//SetRothman93ModelParameters(this->ResponseType);
	this->RateType = BRUGHERA96RATES;
}

Brughera96ChannelPopulation::Brughera96ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType) :
					 Rothman93ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType)
{
	//SetTemperature_DegreesC(Temperature_DegreesC);	
	//this->ResponseType = ResponseType;
	//SetRothman93ModelParameters(this->ResponseType);
	this->RateType = BRUGHERA96RATES;
}
// ????WITH ALL 4 ARGUMENTS NO DEFAULT SETTINGS ARE REQUIRED, AND WE CAN REVERT BACK TO Rothman93ChannelPopulation 
Brughera96ChannelPopulation::Brughera96ChannelPopulation(double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType, Enumed93RateType RateType) :
					 Rothman93ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType, RateType)
{
	//SetTemperature_DegreesC(Temperature_DegreesC);
	//this->ResponseType = ResponseType;
	//SetRothman93ModelParameters(this->ResponseType);
	//this->RateType = RateType;
}

Brughera96Kslow::Brughera96Kslow(double InitialVoltage_V, double Temperature_DegreesC) : 
					 Brughera96ChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
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

void Brughera96Kslow::CalculateRates(double Voltage_V)
{
	static double w1 = .107;  
	static double w2 = 33.0;  
	static double w3 = 13.1;  
	static double w4 = .01881;
	static double w5 = 30.0;  
	static double w6 = 30.3;
	double TemperatureFactor;

	if (this->RateType == ROTHMAN93RATES)
		TemperatureFactor = TemperatureFactor_3();
	else
		TemperatureFactor = TemperatureFactor_2_6655();

	alpha = ROTHMAN_93_RATE_CONSTANT * ((w1 * TemperatureFactor) / (1. + exp(-((Voltage_V * 1000) + w2) / w3)));	// Activation rate
	beta =  ROTHMAN_93_RATE_CONSTANT * (w4 * TemperatureFactor * exp(-((Voltage_V * 1000) + w5) / w6));				// Deactivation rate
}

double Brughera96Kslow::GetCurrent_Amperes(void)
{
	return MaxConductance_Siemens * IntermediateY * (myCompartment->GetVoltage_V() - ReversalPotential_V);
}

Brughera96Kfast::Brughera96Kfast(double InitialVoltage_V, double Temperature_DegreesC) : 
				     Brughera96ChannelPopulation(InitialVoltage_V, Temperature_DegreesC) // Added ResponseType, RateType
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

	ReversalPotential_V = -77.0e-3;	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2564]
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

Brughera96Kfast::Brughera96Kfast(double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType, Enumed93RateType RateType) : 
				     Brughera96ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType, RateType) // Added ResponseType, RateType
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

	ReversalPotential_V = -77.0e-3;	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2564]
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

double Brughera96Kfast::GetCurrent_Amperes(void)
{
	return MaxConductance_Siemens * IntermediateY * (myCompartment->GetVoltage_V() - ReversalPotential_V);
}


void Brughera96Kfast::CalculateRates(double Voltage_V)
{
	/* Fast potassium activation channel */
	static double n1 = .0282; 
	static double n2 = 9.;    
	static double n3 = 12.;   
	static double n4 = 6.;
	static double n5 = 144.;   
	static double n6 = 30.;   
	static double n8 = 6.;    
	static double n9 = 62.;
	double TemperatureFactor;

	if ((Voltage_V > (-n2 - 1e-7)) && (Voltage_V < (-n2 + 1e-7)) ||
		(Voltage_V > (-n5 - 1e-7)) && (Voltage_V < (-n5 + 1e-7)) ||
		(Voltage_V > (-n9 - 1e-7)) && (Voltage_V < (-n9 + 1e-7)))
		Voltage_V += 2e-7;

	if (this->RateType == ROTHMAN93RATES)
		TemperatureFactor = TemperatureFactor_3();
	else
		TemperatureFactor = TemperatureFactor_2_6655();

   	/* FAST POTASSIUM ACTIVATION VALUES (n) */
	alpha = ROTHMAN_93_RATE_CONSTANT * (( n1 * TemperatureFactor * ((Voltage_V * 1000) + n2) ) / (1. - exp( -((Voltage_V * 1000) + n2)/n3)));
	beta =  ROTHMAN_93_RATE_CONSTANT * ((n4 * TemperatureFactor * exp(-((Voltage_V * 1000) + n5)/n6)) + 
					(BRUGHERA_96_RATE_CONSTANT * n8 / (1. + exp((Voltage_V * 1000) + n9)))); 
					
	// Note 1: BRUGHERA_96_RATE_CONSTANT is .001, so that the second term for beta_n is already greatly reduced
	// before the TF factor is omitted, further reducing the second term (in Note 2)
	// Note 2: Second term of 'beta' calculation originally read like this:
	// (BRUGHERA_96_RATE_CONSTANT * (TemperatureFactor_3() * n8) / (1. + exp((Voltage_V * 1000) + n9)))); 
	// We removed the TemperatureFactor_3() from the calculation because it does not appear in the original Bushy Cell code
	// from Hong Mei Cai.  The snippet of original code appears here:
	// alphan = 1000 * ( n[1] * TF3 * (y[5] + n[2]) ) / (1. - exp( -(y[5] + n[2])/n[3]) );
	// betan = 1000 * (n[4] * TF3 * exp(-(y[5] + n[5])/n[6]) ) + n[8] / (1. + exp(y[5] + n[9] )); 
	// dydx[2] = (1. - y[2]) * alphan - y[2] * betan;

	// T2_6655 is used in Brughera96Compartment where Rothman93Compartment uses T3
	// This is because the Brughera et al. 1996 model used a constant for TF3 = 4.8 (automatic temperature scaling was not used).
	// However this is technically incorrect because by definition TF3=3^((T-22)/10), which equals 5.7995 at T = 38. 
	// At body temperature of 38 deg C, we need the value of the TemperatureFactor to be 4.8
	// The temperature factor is named according to x, the base of the exponent in the following relation which we solve for x:
	//      TF = x^((T-22)/10) = 4.8; T = 38
	//   x^1.6 = 4.8
	//       x = 4.8^(1/1.6)
	//       x = 2.6655
	// Hence to incorporate temperature scaling into Brughera96Compartment we use T2_6655 in place of T3 (both are defined in HHChannelPopulation.cpp)
}

Brughera96NaActivation::Brughera96NaActivation(double InitialVoltage_V, double Temperature_DegreesC) : 
							Brughera96ChannelPopulation(InitialVoltage_V, Temperature_DegreesC)
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

void Brughera96NaActivation::CalculateRates(double Voltage_V)
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

	if (this->RateType == ROTHMAN93RATES)
		TemperatureFactor = TemperatureFactor_3();
	else
		TemperatureFactor = TemperatureFactor_2_6655();

	alpha = ROTHMAN_93_RATE_CONSTANT * ((m1 * TemperatureFactor * ((Voltage_V * 1000) + m2)) / (1. - exp(-((Voltage_V * 1000) + m2)/m3)));
	beta =  ROTHMAN_93_RATE_CONSTANT * ((m4 * TemperatureFactor * ((Voltage_V * 1000) + m5)) / (1. - exp(((Voltage_V * 1000) + m5)/m6))); 
}

Brughera96NaInactivation::Brughera96NaInactivation(double InitialVoltage_V, double Temperature_DegreesC) : 
				     Brughera96ChannelPopulation(InitialVoltage_V, Temperature_DegreesC) // Added ResponseType, RateType
{
	//MaxConductance_Siemens = 325e-9 * TemperatureFactor_2();	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2581]
																// Gives Type II current clamp response with
																// other parameters values set as in Rothman 1993.

//	MaxConductance_Siemens = 1000e-9 * TemperatureFactor_2();	// GmaxNa values between 275e-9 and 1000e-9  
																// were all okay for Type I current clamp response
																// with GmaxKHT=150nS, GmaxKHT=0.
																// However, with smaller Gs smaller Cmembrane values 
																// were required to avoid depolarization block when
																// depolarizing currents were increased.
																// For GmaxKHT=1000nS, Cmembrane=12pF works well.

	MaxConductance_Siemens = Rothman93ModelParameters->MaxG_Na93 * TemperatureFactor_2();

	ReversalPotential_V = 55.0e-3;	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2564]
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}

Brughera96NaInactivation::Brughera96NaInactivation(double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType, Enumed93RateType RateType) : 
				     Brughera96ChannelPopulation(InitialVoltage_V, Temperature_DegreesC, ResponseType, RateType) // Added ResponseType, RateType
{
	//MaxConductance_Siemens = 325e-9 * TemperatureFactor_2();	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2581]
																// Gives Type II current clamp response with
																// other parameters values set as in Rothman 1993.

//	MaxConductance_Siemens = 1000e-9 * TemperatureFactor_2();	// GmaxNa values between 275e-9 and 1000e-9  
																// were all okay for Type I current clamp response
																// with GmaxKHT=150nS, GmaxKHT=0.
																// However, with smaller Gs smaller Cmembrane values 
																// were required to avoid depolarization block when
																// depolarizing currents were increased.
																// For GmaxKHT=1000nS, Cmembrane=12pF works well.

	MaxConductance_Siemens = Rothman93ModelParameters->MaxG_Na93 * TemperatureFactor_2();

	ReversalPotential_V = 55.0e-3;	// From Rothman 1993 (J Neurophys) [Vol 70 (6) p. 2564]
	CalculateRates(InitialVoltage_V);
	CalculateSteadyState();
}


void Brughera96NaInactivation::CalculateRates(double Voltage_V)
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

	if (this->RateType == ROTHMAN93RATES)
	{
		TemperatureFactorSmall = TemperatureFactor_3();
		TemperatureFactorLarge = TemperatureFactor_10();
	}
	else
	{
		TemperatureFactorSmall = TemperatureFactor_2_6655();
		TemperatureFactorLarge = TemperatureFactor_5_6569();
	}

	/* FAST SODIUM INACTIVATION VALUES (h) */
	alpha = ROTHMAN_93_RATE_CONSTANT * ((h1 * TemperatureFactorSmall / (1. + exp(((Voltage_V * 1000) + h2) / h3))) + 
		(BRUGHERA_96_RATE_CONSTANT * (h4 * TemperatureFactorLarge) / (1. + exp((Voltage_V * 1000) + h5))));
	beta =  ROTHMAN_93_RATE_CONSTANT * ((h8 * TemperatureFactorSmall) / (1. + exp(-((Voltage_V * 1000) + h9) / h10)));

	// T5_6569 is used in Brughera96Compartment where Rothman93Compartment uses T10
	// This is because the original model used a constant for TF10 = 16 (automatic temperature scaling was not used).
	// However this is technically incorrect because by definition TF10=10^((T-22)/10), which equals 39.8107 at T = 38. 
	// At body temperature of 38 deg C, we need the value of the TemperatureFactor to be 16
	// The temperature factor is named according to y, the base of the exponent in the following relation which we solve for y:
	//      TF = y^((T-22)/10) = 16; T = 38 (Celsius)
	//   y^1.6 = 16
	//       y = 16^(1/1.6)
	//       y = 5.6569
	// Hence to incorporate temperature scaling into Brughera96Compartment we use T5_6569 in place of T10 (both are defined in HHChannelPopulation.cpp)

}

Brughera96Leakage::Brughera96Leakage(double Temperature_DegreesC) : 
							  Brughera96ChannelPopulation(0.0, Temperature_DegreesC)
{
	MaxConductance_Siemens = Rothman93ModelParameters->MaxG_Leakage93 * TemperatureFactor_2();

	ReversalPotential_V = lkReversalPotentialBrughera96_V;
	CalculateRates(0.0);
	CalculateSteadyState();
}

void Brughera96Leakage::CalculateRates(double Voltage_V)
{
	alpha = 0.0;
	beta =  0.0;
}

double Brughera96Leakage::GetCurrent_Amperes(void)
{
	return MaxConductance_Siemens * (myCompartment->GetVoltage_V() - ReversalPotential_V);
}




