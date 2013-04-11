#include "Rothman93Channels.h"
#include "Brughera96Compartment.h"
#include "RungeKuttaEquation.h"
#include "RungeKuttaSolver.h"
#include "math.h"
#include "stdio.h"

Brughera96Compartment::Brughera96Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC)
{
	SetSampleRate_Hz(SampleRate_Hz);
	this->InitialVoltage_V = InitialVoltage_V;
	this->Temperature_DegreesC = Temperature_DegreesC;

	Channels = new HHChannelPopulation *[5];
	Channels[1] = new Rothman93Kslow(InitialVoltage_V, Temperature_DegreesC);
	Channels[2] = new Brughera96Kfast(InitialVoltage_V, Temperature_DegreesC);
	Channels[3] = new Rothman93NaActivation(InitialVoltage_V, Temperature_DegreesC);
	Channels[4] = new Brughera96NaInactivation(InitialVoltage_V, Temperature_DegreesC);
	NumHHChannels = 4;

	for (int i = 1; i <= NumHHChannels; i++)
		Channels[i]->SetCompartment(this);

	Initialize();
}

