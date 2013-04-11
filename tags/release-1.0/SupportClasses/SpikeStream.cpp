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
	if (MaxSpikes <= 0)
		throw EarlabException("SpikeStream: Cannot initialize stream with zero or negative max spike count");
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

	if (CurSpikes >= MaxSpikes)
		throw EarlabException("SpikeStream: Error adding spike to output stream: Spike buffer is full");
	if (CurSpikes > 0)
	{
		for (i = 0; i < CurSpikes; i++)
			if (Spikes[i].SpikeTime_Seconds > SpikeTime_Seconds)
				throw EarlabException("SpikeStream: Fire() must be called with constantly ascending spike times");
	}
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

void SpikeStream::Dump(void)
{
	int i;

	cout << "Spike stream dump:" << endl;
	cout << "  CellID  SpikeTime_Seconds" << endl;

	for (i = 0; i < CurSpikes; i++)
		Spikes[i].Dump();
}
