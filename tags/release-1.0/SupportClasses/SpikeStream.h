#ifndef _SPIKE_H_INCLUDED
#define _SPIKE_H_INCLUDED

class Spike
{
public:
	Spike();
	Spike(int CellID, double SpikeTime_Seconds);
	void Dump(void);
	int CellID;
	double SpikeTime_Seconds;	// Spike time from start of frame, in seconds
};

class SpikeStream
{
public:
	// Spike stream default constructor
	SpikeStream();
	// Construct a SpikeStream from a given array of spikes
	SpikeStream(Spike *Spikes, int MaxSpikes, bool IsInput);
	// Construct a SpikeStream from a given array of spikes, using a provided sample rate
	SpikeStream(Spike *Spikes, int MaxSpikes, bool IsInput, double SampleRate_Hz);

	// Set the sample rate for the current SpikeStream
	void SetSampleRate_Hz(double SampleRate_Hz);

	// Resets the current step to the start of the frame.  Requires SampleRate_Hz to be set
	void NewFrame(void);
	// Steps the SpikeStream to the next sample.  Requires SampleRate_Hz to be set
	void Step(void);

	// Causes a spike to be logged for the given CellID at the given time.  Times must never decrease across subsequent calls to Fire in any frame
	void Fire(int CellID, double SpikeTime_Seconds);
	// Causes a spike to be logged for the given CellID at the current sample step time.  Requires SampleRate_Hz to be set
	void Fire(int CellID);

	// This method returns the number of spikes between the given start and end times, (inclusive), for the given CellID
	int CountSpikes(int CellID, double StartTime_Seconds, double EndTime_Seconds);
	// This method returns the number of spikes between the given start and end times, (inclusive), for the given set of CellIDs
	int CountSpikes(int *CellIDs, int NumCellIDs, double StartTime_Seconds, double EndTime_Seconds);
	// This method returns the number of spikes in the current sample interval for the given CellID
	int CountSpikes(int CellID);
	// This method returns the number of spikes in the current sample interval for the given set of CellIDs
	int CountSpikes(int *CellIDs, int NumCellIDs);

	// Writes a formatted display of the current stream contents to cout
	void Dump(void);

private:
	void Init(Spike *Spikes, int MaxSpikes, bool IsInput);
	void FindStepIndices(void);

	bool IsInput;
	int MaxSpikes;
	int CurSpikes;
	Spike *Spikes;
	double SampleRate_Hz;
	double SecondsPerSample;
	double CurStepStart_Seconds;
	double CurStepEnd_Seconds;
	int CurStepIndexStart;
	int CurStepIndexEnd;
	bool DataAvailableInCurrentTimeStep;
};

#endif
