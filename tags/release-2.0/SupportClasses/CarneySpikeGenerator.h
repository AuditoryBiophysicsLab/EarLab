#ifndef _CARNEY_SPIKE_GENERATOR_H_INCLUDED
#define _CARNEY_SPIKE_GENERATOR_H_INCLUDED

class CarneySpikeGenerator
{
public:
	CarneySpikeGenerator(double SampleRate_Hz, double c0, double c1, double s0, double s1, double Ra, double Scale, double Offset);
	void SetSeed(long NewSeed);
	bool Step(float CurIFRValue);
private:
	double ran1(void);

	static long seed;
    double c0;		// s0 weighting factor [ Carney default c0 = 0.55] 
    double c1;		// s1 weighting factor [ Carney default c1 = 0.45] 
    double s0;		// Time constant ms [ Carney default s0 = 0.8] 
    double s1;		// Time constant ms [ Carney default s1 = 25] 
    double Ra;		// Absolute refractory period ms  [ Carney default Ra = 0.75] 
    double Scale;	// Scaling factor used to multiply the firing rate sample step
    double Offset;	// Base probability of given cell producing a spike for each sample interval

	double mSampleRate_Hz;
	double teffect;
	double spike_count;
    double time_skip;
};
#endif
