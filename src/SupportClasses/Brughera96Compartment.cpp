#include "Brughera96Compartment.h"
#include "math.h"
#include "stdio.h"

Brughera96Compartment::Brughera96Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC)
: Rothman93Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC)
{
	this->ResponseType = TYPE_II;

	Channels = new HHChannelPopulation *[5];
	//Channels = new Brughera96ChannelPopulation *[5];
	//Channels[1] = new Rothman93Kslow(InitialVoltage_V, Temperature_DegreesC);
	Channels[1] = new Brughera96Kslow(InitialVoltage_V, Temperature_DegreesC);
	Channels[2] = new Brughera96Kfast(InitialVoltage_V, Temperature_DegreesC);
	Channels[3] = new Brughera96NaActivation(InitialVoltage_V, Temperature_DegreesC);
	Channels[4] = new Brughera96NaInactivation(InitialVoltage_V, Temperature_DegreesC);
	NumHHChannels = 4;

	for (int i = 1; i <= NumHHChannels; i++)
		Channels[i]->SetCompartment(this);

	Initialize();
}

Brughera96Compartment::Brughera96Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType)
: Rothman93Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC)
{
	this->ResponseType = ResponseType;

	Channels = new HHChannelPopulation *[5];
//	Channels[1] = new Rothman93Kslow(InitialVoltage_V, Temperature_DegreesC);
	Channels[1] = new Brughera96Kslow(InitialVoltage_V, Temperature_DegreesC);
	Channels[2] = new Brughera96Kfast(InitialVoltage_V, Temperature_DegreesC);
//	Channels[3] = new Rothman93NaActivation(InitialVoltage_V, Temperature_DegreesC);
	Channels[3] = new Brughera96NaActivation(InitialVoltage_V, Temperature_DegreesC);
	Channels[4] = new Brughera96NaInactivation(InitialVoltage_V, Temperature_DegreesC);
	NumHHChannels = 4;

	for (int i = 1; i <= NumHHChannels; i++)
		Channels[i]->SetCompartment(this);

	Initialize();
}

Brughera96Compartment::Brughera96Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, int AdditionalConductances)
: Rothman93Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, (int) 1)
{
	this->ResponseType = TYPE_II;

	Channels = new HHChannelPopulation *[5];
	//Channels = new Brughera96ChannelPopulation *[5];
	//Channels[1] = new Rothman93Kslow(InitialVoltage_V, Temperature_DegreesC);
	Channels[1] = new Brughera96Kslow(InitialVoltage_V, Temperature_DegreesC);
	Channels[2] = new Brughera96Kfast(InitialVoltage_V, Temperature_DegreesC);
	Channels[3] = new Brughera96NaActivation(InitialVoltage_V, Temperature_DegreesC);
	Channels[4] = new Brughera96NaInactivation(InitialVoltage_V, Temperature_DegreesC);
	NumHHChannels = 4;

	for (int i = 1; i <= NumHHChannels; i++)
		Channels[i]->SetCompartment(this);

	Initialize();
}

Brughera96Compartment::Brughera96Compartment(double SampleRate_Hz, double InitialVoltage_V, double Temperature_DegreesC, Enumed93ResponseType ResponseType, int AdditionalConductances)
: Rothman93Compartment(SampleRate_Hz, InitialVoltage_V, Temperature_DegreesC, (int) 1)
{
	this->ResponseType = ResponseType;

	Channels = new HHChannelPopulation *[5];
//	Channels[1] = new Rothman93Kslow(InitialVoltage_V, Temperature_DegreesC);
	Channels[1] = new Brughera96Kslow(InitialVoltage_V, Temperature_DegreesC);
	Channels[2] = new Brughera96Kfast(InitialVoltage_V, Temperature_DegreesC);
//	Channels[3] = new Rothman93NaActivation(InitialVoltage_V, Temperature_DegreesC);
	Channels[3] = new Brughera96NaActivation(InitialVoltage_V, Temperature_DegreesC);
	Channels[4] = new Brughera96NaInactivation(InitialVoltage_V, Temperature_DegreesC);
	NumHHChannels = 4;

	for (int i = 1; i <= NumHHChannels; i++)
		Channels[i]->SetCompartment(this);

	Initialize();
}
