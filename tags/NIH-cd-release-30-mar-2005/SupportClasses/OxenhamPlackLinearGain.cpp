#include "OxenhamPlackLinearGain.h"
#include <math.h>
#include <memory.h>

OxenhamPlackLinearGain::OxenhamPlackLinearGain()
{
	memset(this, 0, sizeof(OxenhamPlackLinearGain));
}

void OxenhamPlackLinearGain::Init(double BestFrequency_Hz)
{
	//                     g_p0       g_m
	g = pow(10.0, (double)(4.20405 + (-0.47909 * log10(BestFrequency_Hz))));
}

double OxenhamPlackLinearGain::Filter(double InputSample)
{
	return g * InputSample;
}
