#include "Earlab.h"
#include "ResonantFilter.h"
#include "memory.h"
#include "stdio.h"
#include "math.h"

ResonantFilter::ResonantFilter()
{
	memset(this, 0, sizeof(ResonantFilter));
}

ResonantFilter::ResonantFilter(double CenterFrequency_Hz, double FilterQ, double SampleRate_Hz)
{
	memset(this, 0, sizeof(ResonantFilter));
	Initialize(CenterFrequency_Hz, FilterQ, SampleRate_Hz);
}

void ResonantFilter::Initialize(double CenterFrequency_Hz, double FilterQ, double SampleRate_Hz)
{
	centerFrequency_Hz = CenterFrequency_Hz;
	filterQ = FilterQ;
	sampleRate_Hz = SampleRate_Hz;

	damping  = 1.0 / (2.0 * filterQ);
	deltaT   = 1.0 / sampleRate_Hz;
	cfAdjust = 1.0 / exp(damping * damping);
	centerFrequency_Hz = 1.03 * centerFrequency_Hz * (1.0 / cfAdjust);
	omegaN = TWOPI * centerFrequency_Hz;
	omegaNSquared = omegaN * omegaN;
	deltaTSquared = deltaT * deltaT;
	A1 = 1.0 / ((4.0 / deltaTSquared) + (4.0 * damping * (omegaN / deltaT)) + omegaNSquared);
	a1 = omegaNSquared;
	a2 = 2.0 * omegaNSquared;
	a3 = omegaNSquared;
	b1 = ((2.0 * omegaNSquared) - (8.0 / deltaTSquared));
	b2 = ((4.0 / deltaTSquared) - (4.0 * damping * (omegaN / deltaT)) + omegaNSquared);
	X1 = X2 = Y1 = Y2 = 0;
}

double ResonantFilter::Filter(double Sample)
{
	double Result;

	Result = A1 * ((a1 * Sample) + (a2 * X1) + (a3 * X2) - (b1 * Y1) - (b2 * Y2));

	X2 = X1;
	X1 = Sample;

	Y2 = Y1;
	Y1 = Result;

	return Result;
}
