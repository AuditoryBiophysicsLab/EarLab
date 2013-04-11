#include "math.h"
#include "CarneySpikeGenerator.h"

time_t CarneySpikeGenerator::seed = 0;

CarneySpikeGenerator::CarneySpikeGenerator(double SampleRate_Hz, double c0, double c1, double s0, double s1, double Ra, double Scale, double Offset)
{
	this->mSampleRate_Hz = SampleRate_Hz;
	this->c0 = c0;
	this->c1 = c1;
	this->s0 = s0;
	this->s1 = s1;
	this->Ra = Ra;
	this->Scale = Scale;
	this->Offset = Offset;
	teffect = 0.0;
	spike_count = 0.0;
    time_skip = (Ra / 1000) * mSampleRate_Hz;	
    time(&seed);     // use processor clock to set random seed
    seed = -1 * seed;
}

bool CarneySpikeGenerator::Step(float CurIFRValue)
{
	double SpikeProbability;
	double random_number;

	if ((c0 == 0) && (c1 == 0))
		SpikeProbability = 1.0;
	else
		SpikeProbability = (1 - (c0 * (exp(-1 * (teffect - Ra) / s0)) + c1 * (exp(-1 * (teffect - Ra) / s1))));

	if (SpikeProbability < 0)
		SpikeProbability = 0;
	SpikeProbability = SpikeProbability * (Scale * CurIFRValue / mSampleRate_Hz);
	if (SpikeProbability < 0)
		SpikeProbability = 0;

	random_number = ran1(); 

	if (random_number < (SpikeProbability + Offset))
	{
		spike_count = 1;
		teffect = (1.0 * spike_count / (mSampleRate_Hz / 1000.0)); // time in ms after spike occurance
		return true;
	} 
	else
	{
		spike_count++;
		teffect = 1000.0 * spike_count / mSampleRate_Hz; // time after spike in msecs 
		return false;
	}
}

void CarneySpikeGenerator::SetSeed(int NewSeed)
{
	seed = NewSeed;
}

//  Random number generator definitions
#define  IA 16807
#define  IM 2147483647
#define  AM (1.0/IM)
#define  IQ 127773
#define  IR 2836
#define  NTAB  32
#define  NDIV (1+(IM-1)/NTAB)
#define  EPS 1.2e-7
#define  RNMX (1.0-EPS)

double CarneySpikeGenerator::ran1(void)
{
    int  j;
    int k;
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

    return temp;
}
