#include "stdio.h"
#include "memory.h"
#include "SpikeStream.h"

Spike::Spike()
{
	CellID = -1;
	SpikeTime_us = -1;
}

Spike::Spike(int NewCellID, double NewSpikeTime_us)
{
	CellID = NewCellID;
	SpikeTime_us = NewSpikeTime_us;
}
