#include "SpikeStreamMapper.h"
#include "EarlabException.h"
#include <memory.h>

SpikeStreamMapper::SpikeStreamMapper(int InputCellCount, int OutputCellCount, char *MapFileName)
{
	FILE *fp;
	int curChar;
	int BitNumber = 0;
	int OutputCellID = 0;

	inputCellCount = InputCellCount;
	outputCellCount = OutputCellCount;
	mapFileName = MapFileName;

	MapArray = new char *[outputCellCount];
	for (int outputCell = 0; outputCell < outputCellCount; outputCell++)
	{
		MapArray[outputCell] = new char [inputCellCount];
		memset(MapArray[outputCell], 0, inputCellCount);
	}

	fp = fopen(mapFileName, "r");
	if (fp == NULL)
		throw new EarlabException("SpikeStreamMapper: Error opening map file \"%s\"\n", mapFileName);

	while (!feof(fp))
	{
		curChar = fgetc(fp);
		if (curChar == EOF)
			break;
		switch ((char)curChar)
		{
		case '1':
			MapArray[OutputCellID][BitNumber] = 1;
			BitNumber++;
			break;
		case '0':
			MapArray[OutputCellID][BitNumber] = 0;
			BitNumber++;
			break;
		case '\n':
			OutputCellID++;
			BitNumber = 0;
			break;
		default:
			break;
		} // switch ((char)curChar)
		//if (BitNumber >= inputCellCount)
		if (BitNumber > inputCellCount) // BitNumber should reach inputCellCount, then 10 ('\n') should be the next curChar
			throw new EarlabException("SpikeStreamMapper: Error initializing output channel %d.  Input bitmap length is greater than %d", OutputCellID, inputCellCount);
		//if (OutputCellID >= outputCellCount)
		if (OutputCellID > outputCellCount) // OutputCellID should equal outputCellCount just before eof
			throw new EarlabException("SpikeStreamMapper: Error initializing output channel %d.  Specified maximum output channel is %d", OutputCellID, outputCellCount);
	} // while (!feof(fp))
} // SpikeStreamMapper::SpikeStreamMapper(int InputCellCount, int OutputCellCount, char *MapFileName)

SpikeStreamMapper::~SpikeStreamMapper()
{
	for (int outputCell = 0; outputCell < outputCellCount; outputCell++)
		delete [] MapArray[outputCell];
	delete [] MapArray;
}

int SpikeStreamMapper::CountMappedSpikes(SpikeStream *SpikeStream, int CellID)
{
	int SpikeCount = 0;

	// Loop through all the cells in the input stream
	for (int InputCellID = 0; InputCellID < inputCellCount; InputCellID++)
		// If the current input cell is selected to be mapped to the specified output cell (CellID) 
		// AND the current input cell has a spike in the current time step THEN bump the count of 
		// spikes that are feeding the current input cell in this time step
		if ((MapArray[CellID][InputCellID] != 0) && (SpikeStream->CountSpikes(InputCellID) != 0))
				SpikeCount++;
	// Return the count of selected spikes in the current time step
	return SpikeCount;
}
