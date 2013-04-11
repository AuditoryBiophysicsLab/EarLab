#include <iostream>

#include "stdio.h"
#include "memory.h"
#include "SpikeStream.h"
#include "Earlab.h"
#include "EarlabException.h"

using namespace std;

Spike::Spike()
{
	CellID = -1;
	SpikeTime_Seconds = -1.0;
}

Spike::Spike(int CellID, double SpikeTime_Seconds)
{
	this->CellID;
	this->SpikeTime_Seconds = SpikeTime_Seconds;

}

void Spike::Dump(void)
{
	cout << "  " << CellID << "  " << SpikeTime_Seconds << endl;
}

SpikeStream::SpikeStream()
{
}

SpikeStream::SpikeStream(Spike *Spikes, int MaxSpikes, bool IsInput)
{
	Init(Spikes, MaxSpikes, IsInput);
}

SpikeStream::SpikeStream(Spike *Spikes, int MaxSpikes, bool IsInput, double SampleRate_Hz)
{
	Init(Spikes, MaxSpikes, IsInput);
	SetSampleRate_Hz(SampleRate_Hz);
	NewFrame();
}

void SpikeStream::Init(Spike *Spikes, int MaxSpikes, bool IsInput)
{
	if (Spikes == NULL)
		throw EarlabException("SpikeStream: Cannot initialize stream with NULL spike buffer");
	if (MaxSpikes < 0)
		throw EarlabException("SpikeStream: Cannot initialize stream with negative max spike count");
	this->Spikes = Spikes;
	this->MaxSpikes = MaxSpikes;
	this->IsInput = IsInput;

	if (IsInput)
		CurSpikes = MaxSpikes;
	else
		CurSpikes = 0;

	SampleRate_Hz = 0.0;
	SecondsPerSample = 0.0;
}

void SpikeStream::Step(void)
{
	if (SecondsPerSample == 0.0)
		throw EarlabException("SpikeStream: Cannot call Step() without first setting a sample rate");

	CurStepStart_Seconds += SecondsPerSample;
	CurStepEnd_Seconds = CurStepStart_Seconds + SecondsPerSample;
	//CurStepStart_Seconds -= 0.000000000001;
	//CurStepEnd_Seconds -= 0.000000000001;
	FindStepIndices();
}

void SpikeStream::NewFrame(void)
{
	if (SecondsPerSample == 0.0)
		throw EarlabException("SpikeStream: Cannot call NewFrame() without first setting a sample rate");

	CurStepStart_Seconds = 0.0;
	CurStepEnd_Seconds = SecondsPerSample;
	//CurStepEnd_Seconds -= 0.000000000001;
	CurStepIndexStart = 0;
	CurStepIndexEnd = 0;
	FindStepIndices();
}

void SpikeStream::FindStepIndices(void)
{
	if (CurSpikes == 0)
		return;

	while ((CurStepStart_Seconds > Spikes[CurStepIndexStart].SpikeTime_Seconds) && (CurStepIndexStart < CurSpikes))
		CurStepIndexStart++;
	CurStepIndexEnd = CurStepIndexStart;
	while ((CurStepEnd_Seconds > Spikes[CurStepIndexEnd].SpikeTime_Seconds) && (CurStepIndexEnd < CurSpikes))
		CurStepIndexEnd++;
	if (CurStepIndexEnd > CurStepIndexStart)
		DataAvailableInCurrentTimeStep = true;
	else
		DataAvailableInCurrentTimeStep = false;
}

void SpikeStream::SetSampleRate_Hz(double SampleRate_Hz)
{
	if (SampleRate_Hz <= 0.0)
		throw EarlabException("SpikeStream: Zero or negative sample rates are not allowed");
	this->SampleRate_Hz = SampleRate_Hz;
	this->SecondsPerSample = 1 / SampleRate_Hz;
}

void SpikeStream::Fire(int CellID, double SpikeTime_Seconds)
{
	int i;
	SpikeTimeT firetime = SpikeTime_Seconds; // Do comparison in same precision as stored data

	if (CurSpikes >= MaxSpikes)
		throw EarlabException("SpikeStream: Error adding spike to output stream: Spike buffer is full");

	// Make sure times are non-decreasing
	if (CurSpikes > 0 && Spikes[CurSpikes-1].SpikeTime_Seconds > firetime) {
	   printf("Spike::Fire: cell %d at time %f (< %f)\n",
		  CellID, firetime,  Spikes[CurSpikes-1].SpikeTime_Seconds);
	   throw EarlabException("SpikeStream: Fire() must be called with constantly ascending spike times");

	}
    //  {
    //          for (i = 0; i < CurSpikes; i++)
    //                  if (Spikes[i].SpikeTime_Seconds > SpikeTime_Seconds)
    //                          throw EarlabException("SpikeStream: Fire() must be called with constantly ascending spike times");
    //  }

	Spikes[CurSpikes].CellID = CellID;
	Spikes[CurSpikes].SpikeTime_Seconds = SpikeTime_Seconds;
	CurSpikes++;
}

void SpikeStream::Fire(int CellID)
{
	Fire(CellID, CurStepStart_Seconds);
}

 int SpikeStream::CountSpikes(int CellID, double StartTime_Seconds, double EndTime_Seconds)
{
	int i;
	int total = 0;

	for (i = 0; i < CurSpikes; i++)
	{
		if (Spikes[i].CellID == CellID)
		{
			if ((StartTime_Seconds <= Spikes[i].SpikeTime_Seconds) && (Spikes[i].SpikeTime_Seconds <= EndTime_Seconds))
				total++;
		}
	}
	return total;
}

int SpikeStream::CountSpikes(int *CellIDs, int NumCellIDs, double StartTime_Seconds, double EndTime_Seconds)
{
	int i, j, CurCellID;
	int total = 0;
	double SpikeTime;

	for (i = 0; i < CurSpikes; i++)
	{
		CurCellID = Spikes[i].CellID;
		SpikeTime = Spikes[i].SpikeTime_Seconds;
		for (j = 0; j < NumCellIDs; j++)
		{
			if (CurCellID == CellIDs[j])
			{
				if ((StartTime_Seconds <= SpikeTime) && (SpikeTime <= EndTime_Seconds))
					total++;
			}
		}
	}
	return total;
}

int SpikeStream::CountSpikes(int *CellIDs, int NumCellIDs)
{
	int i, j, CurCellID;
	int total = 0;

	if (!DataAvailableInCurrentTimeStep)
		return 0;

	for (i = CurStepIndexStart; i < CurStepIndexEnd; i++)
	{
		CurCellID = Spikes[i].CellID;
		for (j = 0; j < NumCellIDs; j++)
			if (CurCellID == CellIDs[j])
				total++;
	}
	return total;
}

int SpikeStream::CountSpikes(int CellID)
{
	int i;
	int total = 0;

	if (!DataAvailableInCurrentTimeStep)
		return 0;

	for (i = CurStepIndexStart; i < CurStepIndexEnd; i++)
	{
		if (Spikes[i].CellID == CellID)
			total++;
	}
	return total;
}

int SpikeStream::CountSpikes(void)
{
	if (!DataAvailableInCurrentTimeStep)
		return 0;
	return CurStepIndexEnd - CurStepIndexStart;
}

int SpikeStream::SpikeCount(void)
{
	return CurSpikes;
}

Spike *SpikeStream::GetSpike(int SpikeIndex)
{
	if (SpikeIndex >= CurSpikes)
		return NULL;
	if (SpikeIndex < 0)
		throw EarlabException("SpikeStream: GetSpike error: Spike index must be greater than or equal to zero");

	return &(Spikes[SpikeIndex]);
}

void SpikeStream::Clear(void)
{
	CurSpikes = 0;
	if (Spikes != NULL)
		memset(Spikes, 0, sizeof(Spike) * MaxSpikes);
}

void SpikeStream::Dump(void)
{
	int i;

	cout << "Spike stream dump:" << endl;
	cout << "  CellID  SpikeTime_Seconds" << endl;

	for (i = 0; i < CurSpikes; i++)
		Spikes[i].Dump();
}

Spike *SpikeStream::GetBuffer(int *SpikeCount)
{
	if (SpikeCount == NULL)
		throw EarlabException("SpikeStream::GetBuffer: Must provide an int pointer for output spike count");
	*SpikeCount = CurSpikes;
	return Spikes;
}

void SpikeStream::SetBuffer(Spike *NewSpikeBuffer, int SpikeCount)
{
	Spikes = NewSpikeBuffer;
	MaxSpikes = SpikeCount;
	CurSpikes = SpikeCount;
	IsInput = true;
	NewFrame();
}

SpikeStream &SpikeStream::operator+(SpikeStream &rhs)
{
	int outSpikeCount = CurSpikes + rhs.CurSpikes;
	Spike *ReturnSpikes = new Spike[outSpikeCount];
	int lhsSpikeIndex, rhsSpikeIndex, outSpikeIndex;

	lhsSpikeIndex = rhsSpikeIndex = 0;
	for (outSpikeIndex = 0; outSpikeIndex > outSpikeCount; outSpikeIndex++)
	{
		if (Spikes[lhsSpikeIndex].SpikeTime_Seconds <= rhs.Spikes[rhsSpikeIndex].SpikeTime_Seconds)
		{
			ReturnSpikes[outSpikeIndex].CellID = Spikes[lhsSpikeIndex].CellID;
			ReturnSpikes[outSpikeIndex].SpikeTime_Seconds = Spikes[lhsSpikeIndex].SpikeTime_Seconds;
			lhsSpikeIndex++;
		}
		else
		{
			ReturnSpikes[outSpikeIndex].CellID = rhs.Spikes[rhsSpikeIndex].CellID;
			ReturnSpikes[outSpikeIndex].SpikeTime_Seconds = rhs.Spikes[rhsSpikeIndex].SpikeTime_Seconds;
			rhsSpikeIndex++;
		}
	}
	return *(new SpikeStream(ReturnSpikes, MaxSpikes + rhs.MaxSpikes, IsInput, SampleRate_Hz));
}

#if 0
void SpikeStream::operator=(SpikeStream &rhs)
{
	if (Spikes != NULL)
		delete [] Spikes;
	Init(rhs.Spikes, rhs.MaxSpikes, rhs.IsInput);
	if (rhs.SampleRate_Hz != 0)
		SetSampleRate_Hz(rhs.SampleRate_Hz);
	NewFrame();
	return *this;
}
#endif

SpikeDelayBuffer::SpikeDelayBuffer(int MaxSpikes, double DelayTime_Seconds, double FrameLength_Seconds, double SampleRate_Hz)
{
	if (FrameLength_Seconds <= 0.0)
		throw EarlabException("SpikeDelayBuffer: Frame length cannot be zero or negative");

	if (DelayTime_Seconds <= 0.0)
		throw EarlabException("SpikeDelayBuffer: Spike delay time cannot be zero or negative");

	if (SampleRate_Hz <= 0.0)
		throw EarlabException("SpikeDelayBuffer: Sample Rate cannot be zero or negative");

	if (MaxSpikes <= 0)
		throw EarlabException("SpikeDelayBuffer: Maximum spike count cannot be zero or negative");

	memset(this, 0, sizeof(SpikeDelayBuffer));
	this->MaxSpikes = MaxSpikes;
	this->DelayTime_Seconds = DelayTime_Seconds;
	this->FrameLength_Seconds = FrameLength_Seconds;
	this->SampleRate_Hz = SampleRate_Hz;
	DelayedBufferSize = -1;
	Spikes = new Spike[MaxSpikes];
}

SpikeDelayBuffer::~SpikeDelayBuffer()
{
	if (Spikes != NULL)
		delete [] Spikes;

	if (DelayedSpikes != NULL)
		delete [] DelayedSpikes;

	if (DelayedStream != NULL)
		delete DelayedStream;
}

// Note for clarity: We need to add spikes to the delay buffer before removing any delayed spikes therefrom
// in case we are dealing with a delay interval of less than one frame.  In that case, at least some of the
//  spikes that arrive as input to the current frame will be processed in the current frame.  Therefore it is
// recommended that the spike buffer accommodate a minimum of two frames of worst-case spike data.
SpikeStream *SpikeDelayBuffer::Add(SpikeStream *newSpikes)
{
	int NewSpikeCount, i, CurSpikeCount, CurFrameStartIndex;
	Spike *CurSpike;
	double SpikeTimeAdjustment_Seconds = FrameStartTime_Seconds + DelayTime_Seconds;
	double FrameEndTime_Seconds = FrameStartTime_Seconds + FrameLength_Seconds;

	// First, copy the new spikes into the delay buffer
	NewSpikeCount = newSpikes->SpikeCount();
	for (i = 0; i < NewSpikeCount; i++)
	{
		CurSpike = newSpikes->GetSpike(i);
		Spikes[EndIndex].CellID = CurSpike->CellID;
		Spikes[EndIndex].SpikeTime_Seconds = CurSpike->SpikeTime_Seconds + SpikeTimeAdjustment_Seconds;

		EndIndex++;

		// If we've hit the end of the spike list, move to the beginning
		if (EndIndex == MaxSpikes)
			EndIndex = 0;

		// Note: This should *never* happen
		if (EndIndex > MaxSpikes)
			throw EarlabException("SpikeDelayBuffer::Add - Buffer overflow!");

		// If the buffer is full,
		if (EndIndex == StartIndex)
			throw EarlabException("SpikeDelayBuffer::Add: Error - delay buffer is full.  Try increasing the size of the delay buffer");
	}

	// Next, count the number of spikes that should be considered part of the "current" frame
	CurSpikeCount = 0;
	CurFrameStartIndex = StartIndex;
	while (1)
	{
		// If we've run into the end of the circular buffer, continue at the beginning of the circular buffer
		// After all, this makes it a circle, no?
		if (CurFrameStartIndex == MaxSpikes)
			CurFrameStartIndex = 0;

		// If we've run into the end of the spike list, stop advancing
		if (CurFrameStartIndex == EndIndex)
			break;

		// Note: This should *never* happen
		if (CurFrameStartIndex > MaxSpikes)
			throw EarlabException("SpikeDelayBuffer::Add: Error - Searching past end of delay buffer");

		// If we've found a spike with a time equal to or after the end of the current frame, we're done
		if (Spikes[CurFrameStartIndex].SpikeTime_Seconds >= FrameEndTime_Seconds)
			break;

		CurFrameStartIndex++;
		CurSpikeCount++;
	}

	// Now, allocate a buffer with sufficent storage space to hold the spikes which are part of the current frame
	if (DelayedBufferSize < CurSpikeCount)
	{
		if (DelayedSpikes != NULL)
			delete [] DelayedSpikes;
		DelayedBufferSize = CurSpikeCount + 1;
		DelayedSpikes = new Spike[DelayedBufferSize];
	}

	// Next, copy the current frame's spikes into the buffer
	for (i = 0; i < CurSpikeCount; i++)
	{
		DelayedSpikes[i].CellID = Spikes[StartIndex].CellID;
		DelayedSpikes[i].SpikeTime_Seconds = Spikes[StartIndex].SpikeTime_Seconds - FrameStartTime_Seconds;

		// If we've run into the end of the circular buffer, continue at the beginning of the circular buffer
		// After all, this makes it a circle, no?
		if (StartIndex == MaxSpikes)
			StartIndex = 0;

		// Note: This should *never* happen
		if (StartIndex > MaxSpikes)
			throw EarlabException("SpikeDelayBuffer::Add: Error - Copying past end of delay buffer");

		StartIndex++;
	}

	// Set the output buffer to the freshly-copied-and-modified spike buffer
	if (DelayedStream == NULL)
		DelayedStream = new SpikeStream(DelayedSpikes, CurSpikeCount, true, SampleRate_Hz);
	else
		DelayedStream->SetBuffer(DelayedSpikes, CurSpikeCount);

	// Set up for next time we'll be called
	FrameStartTime_Seconds += FrameLength_Seconds;

	// and return the delayed spike stream
	return DelayedStream;
}
