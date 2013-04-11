#ifndef __SPIKE_STREAM_MAPPER_H_INCLUDED
#define __SPIKE_STREAM_MAPPER_H_INCLUDED

#include "Earlab.h"
#include <stdio.h>
#include "SpikeStream.h"

class SpikeStreamMapper
{
public:
	SpikeStreamMapper(int InputCellCount, int OutputCellCount, char *MapFileName);
	~SpikeStreamMapper();
	int CountMappedSpikes(SpikeStream *SpikeStream, int CellID);  // *InputSpikeStream, OutputCellID 
private:
	char **MapArray;
	int inputCellCount, outputCellCount, inputMapLength;
	char *mapFileName;
};
#endif
