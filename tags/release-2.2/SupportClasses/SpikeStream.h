#ifndef _SPIKE_STREAM_H_INCLUDED
#define _SPIKE_STREAM_H_INCLUDED

typedef float SpikeTimeT;       // type of spike event time data

class Spike
{
public:
	Spike();
	Spike(int CellID, double SpikeTime_Seconds);
	void       Dump(void);
	int        CellID;
	SpikeTimeT SpikeTime_Seconds;  // Spike time from start of frame, in seconds
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
	// This method returns the number of spikes in the current sample interval for all CellIDs
	int CountSpikes(void);

	// This method returns the number of spikes in the spike buffer
	int SpikeCount(void);

	// This method returns the spike at the specified index in the spike buffer.  If the index is out of range, NULL is returned
	Spike *GetSpike(int SpikeIndex);

	// Resets the stream to an empty state
	void Clear(void);

	// Writes a formatted display of the current stream contents to cout
	void Dump(void);

	void SetBuffer(Spike *NewSpikeBuffer, int SpikeCount);

	Spike *GetBuffer(int *SpikeCount);

	// Operators
	SpikeStream &operator+(SpikeStream &rhs);
	//SpikeStream &operator=(SpikeStream &rhs);

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

class SpikeDelayBuffer
{
public:
	SpikeDelayBuffer(int MaxSpikes, double DelayTime_Seconds, double FrameLength_Seconds, double SampleRate_Hz);
	~SpikeDelayBuffer();

	// Call this function with the input buffer to be delayed.  Return value is a SpikeStream that has been delayed a sufficient amount of time to be considered "current-frame" spikes
	SpikeStream *Add(SpikeStream *newSpikes);
private:
	Spike *Spikes;
	int MaxSpikes;
	double DelayTime_Seconds, FrameLength_Seconds, FrameStartTime_Seconds, SampleRate_Hz;
	int StartIndex, EndIndex;
	int DelayedBufferSize;
	Spike *DelayedSpikes;
	SpikeStream *DelayedStream;
};
#endif
