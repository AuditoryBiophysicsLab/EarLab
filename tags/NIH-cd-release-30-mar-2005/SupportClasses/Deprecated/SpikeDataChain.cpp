#include "stdio.h"
#include "memory.h"
#include "SpikeStream.h"
#include "EarlabException.h"

SpikeDataChain::SpikeDataChain()
{
	memset(this, 0, sizeof(SpikeDataChain));
	mGrowSize = DefaultInitialSpikeCount;
	mBuf = new SpikeDataChunk(mGrowSize);
}

SpikeDataChain::SpikeDataChain(int GrowSize)
{
	memset(this, 0, sizeof(SpikeDataChain));
	mGrowSize = GrowSize;
	mBuf = new SpikeDataChunk(mGrowSize);
}

SpikeDataChain::SpikeDataChain(SpikeDataChunk *NewChunk)
{
	memset(this, 0, sizeof(SpikeDataChain));
	mGrowSize = DefaultInitialSpikeCount;
	mBuf = NewChunk;
}

SpikeDataChain::~SpikeDataChain()
{
	if (mNext != NULL)
		delete mNext;
	if (mBuf != NULL)
		delete mBuf;
}

SpikeDataChain *SpikeDataChain::Grow(void)
{
	SpikeDataChain *Cur;

	Cur = this;
	while (Cur->mNext != NULL)
		Cur = Cur->mNext;
	Cur->mNext = new SpikeDataChain(mGrowSize);
	Cur->mNext->mPrev = this;
	return Cur->mNext;
}

SpikeDataChain *SpikeDataChain::AddChunk(SpikeDataChunk *NewChunk)
{
	SpikeDataChain *Cur;

	Cur = this;
	while (Cur->mNext != NULL)
		Cur = Cur->mNext;
	Cur->mNext = new SpikeDataChain(mGrowSize);
	Cur->mNext->mPrev = this;
	return Cur->mNext;
}

SpikeDataChain *SpikeDataChain::Next(void)
{
	return mNext;
}

SpikeDataChain *SpikeDataChain::Prev(void)
{
	return mPrev;
}

SpikeDataChunk *SpikeDataChain::GetChunk()
{
	return mBuf;
}

SpikeDataChunk *SpikeDataChain::operator[](int Index) const
{
	if (Index < 0)
		throw EarlabException("SpikeDataChain: Illegal index: %d", Index);
	if (Index > 0)
	{
		if (mNext == NULL)
			return NULL;
		else
			return (*mNext)[Index - 1];
	}
	else
		return mBuf;
}
