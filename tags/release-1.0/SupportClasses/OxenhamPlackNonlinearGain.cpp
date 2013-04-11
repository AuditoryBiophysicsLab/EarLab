#include "OxenhamPlackNonlinearGain.h"
#include "Earlab.h"
#include <math.h>
#include <memory.h>

double min(double a, double b);

OxenhamPlackNonlinearGain::OxenhamPlackNonlinearGain()
{
	memset(this, 0, sizeof(OxenhamPlackNonlinearGain));
}

void OxenhamPlackNonlinearGain::Init(double BestFrequency_Hz)
{
	a = pow(10.0, ( 1.40298 + ( 0.81916 * log10(BestFrequency_Hz))));
	b = pow(10.0, ( 1.61912 + (-0.81867 * log10(BestFrequency_Hz))));
                             // vvvvvvv Should this term really be zero?
	c = pow(10.0, (-0.60206 + ( 0.00000 * log10(BestFrequency_Hz))));
}

double OxenhamPlackNonlinearGain::Filter(double Sample)
{
	return sign(Sample) * min(a * fabs(Sample), b * pow(fabs(Sample), c));
}

double min(double a, double b)
{
	return (a < b ? a : b);
}

double OxenhamPlackNonlinearGain::sign(double value)
{
	if (value < 0.0)
		return -1.0;
	if (value > 0.0)
		return 1.0;
	return 0.0;
}