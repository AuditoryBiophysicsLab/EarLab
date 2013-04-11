#include "Random.h"
#include <math.h>

//  Random number generator definitions
const int		IA		= 16807;
const int		IM		= 2147483647;
const double	AM		= (1.0/IM);
const int		IQ		= 127773;
const int		IR		= 2836;
const int		NTAB	= 32;
const int		NDIV	= (1+(IM-1)/NTAB);
const double	EPS		= 1.2e-7;
const double	RNMX	= (1.0-EPS);

const int		MaxRandomBeforeReseeding = 100000;

#ifdef _WIN32
time_t Random::seed = 0;
#else
time_t Random::seed = 0;
#endif
int Random::RandomCount = 0;

Random::Random()
{
	if (seed == 0)
	{
		time(&seed);     // use processor clock to set random seed
		seed = -1 * seed;
	}
}

double Random::uniform(void)
{
    int  j;
    long k;
    static int iy =0;
    static int iv[NTAB];
    double temp;

    if (seed <= 0 || !iy)
    {
        if (-(seed) < 1) 
			seed = 1;
        else 
			seed = -seed;

        for (j = NTAB + 7; j >= 0; j--)
        {
            k = (int)seed / IQ;
            seed = IA * (seed - k * IQ) - IR * k;
            if (seed < 0) 
				seed += IM;
            if (j < NTAB) 
				iv[j] = (int)seed;
        }
        iy =iv[0];
    }

    k = (int)seed / IQ;
    seed = IA * (seed - k * IQ) - IR * k;
    if (seed < 0) 
		seed += IM;

	j = iy/NDIV;
    iy = iv[j];
    iv[j] = (int)seed;
    temp = AM * iy;
	if (temp > RNMX)
        temp = RNMX;

	RandomCount++;
	//if (RandomCount > MaxRandomBeforeReseeding)
	//	seed = temp;

    return temp;
}

double Random::gaussian(void)
{
	double x1, x2, w, y1; //, y2;

	do 
	{
		x1 = 2.0 * uniform() - 1.0;
		x2 = 2.0 * uniform() - 1.0;
		w = x1 * x1 + x2 * x2;
	} while (w >= 1.0);

	w = sqrt((-2.0 * log(w)) / w);
	y1 = x1 * w;
	//y2 = x2 * w;
	return y1;
}
