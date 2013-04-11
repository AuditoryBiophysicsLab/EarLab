#include "stdio.h"
#include "memory.h"
#include "SpikeStream.h"
#include "EarlabException.h"

SpikeDataChunk::SpikeDataChunk(int MaxSpikes)
{
	memset(this, 0, sizeof(SpikeDataChunk));
	mChunk = new Spike[MaxSpikes];
	mNumElements = MaxSpikes;
	mFirstOpenSlot = 0;
	mSlotsRemaining = MaxSpikes;
}

SpikeDataChunk::SpikeDataChunk(int NumSpikes, Spike *Spikes)
{
	memset(this, 0, sizeof(SpikeDataChunk));
	mChunk = new Spike[NumSpikes];
	mNumElements = NumSpikes;
	mSlotsRemaining = 0;
	mFirstOpenSlot = NumSpikes;
	memcpy(mChunk, Spikes, sizeof(Spike) * NumSpikes);
}

SpikeDataChunk::~SpikeDataChunk()
{
	delete [] mChunk;
}

bool SpikeDataChunk::Add(Spike newSpike)
{
	// DJA: Note that this code (in fact most of the code in the project) is not thread safe as
	// of January 2004
	if (mSlotsRemaining == 0)
		return false;

	mChunk[mFirstOpenSlot].CellID = newSpike.CellID;
	mChunk[mFirstOpenSlot].SpikeTime_us = newSpike.SpikeTime_us;
	mFirstOpenSlot++;
	mSlotsRemaining--;
	return true;
}

const Spike SpikeDataChunk::operator[](int Index) const
{
	if (Index >= mNumElements)
		throw EarlabException("SpikeDataChunk: Chunk index exceeded.  Requested index %d, max index %d", Index, mNumElements);
	return mChunk[Index];
}

int SpikeDataChunk::ElementCount(void)
{
	return mFirstOpenSlot;
}
