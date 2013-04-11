#include "Earlab.h"
#include "EarlabException.h"
#include "EarlabDataStream.h"
#include "stdio.h"

EarlabWaveformStream::~EarlabWaveformStream()
{
	if (TheData != NULL)
		delete TheData;
}

EarlabSpikeStream::~EarlabSpikeStream()
{
	if (TheData != NULL)
		delete TheData;
}
