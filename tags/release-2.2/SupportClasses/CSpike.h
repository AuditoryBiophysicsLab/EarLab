#ifndef _CSPIKE_H
#define _CSPIKE_H

/* typedef double Spike_T; */
typedef float Spike_T;

typedef struct {
	int     cell;   /* Cell ID         */
	Spike_T time;   /* Time in seconds */
} TSpike;

typedef struct {
	int     IsInput;
	int     MaxSpikes;
	int     CurSpikes;
	TSpike *Spikes;
	double  SampleRate_Hz;
	double  SecondsPerSample;
	double  CurStepStart_Seconds;
	double  CurStepEnd_Seconds;
	int     CurStepIndexStart;
	int     CurStepIndexEnd;
	int     DataAvailableInCurrentTimeStep;
} Spike_CLASS;

#endif
