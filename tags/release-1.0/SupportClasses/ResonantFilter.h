#ifndef __RESONANT_FILTER_H_INCLUDED
#define __RESONANT_FILTER_H_INCLUDED

class ResonantFilter
{
public:
	ResonantFilter();
	ResonantFilter(double CenterFrequency_Hz, double FilterQ, double SampleRate_Hz);
	void Initialize(double CenterFrequency_Hz, double FilterQ, double SampleRate_Hz);
	double Filter(double Sample);
private:
	double centerFrequency_Hz;
	double filterQ;
	double sampleRate_Hz;
	double damping;
	double deltaT, deltaTSquared;
	double cfAdjust;
	double omegaN, omegaNSquared;
	double A1, a1, a2, a3, b1, b2;
	double X1, X2, Y1, Y2;
};

#endif
