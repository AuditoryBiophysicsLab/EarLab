#ifndef _EARLAB_DATA_STREAM_H_INCLUDED
#define _EARLAB_DATA_STREAM_H_INCLUDED
#include "MatrixN.h"
#include "SpikeStream.h"
#include "CMatrixN.h"

typedef enum
{
	Waveform	= 0,
	Spike		= 1,
	Reserved	= 2,
} EarlabDataStreamType;

typedef struct
{
	int CellID;
	double SpikeTime_Seconds;
} SpikeType;

typedef struct
{
	unsigned long NumCells;           // Total number of cells in the originating module
	unsigned long TotalSpikes;        // Total number of spikes in the current frame
	SpikeType *Spikes;                // Array of spikes
} SpikeDataType;

typedef struct
{
	EarlabDataStreamType DataType;           // 0 = waveform, 1 = spikes, other values reserved
	union
	{
		doubleMatrixN WaveformData;			// Also changing waveform data type to float to save space
		SpikeDataType SpikeData;        
	} PayloadData;
} EarlabDataStreamStruct;

class EarlabDataStream
{
public:
	EarlabDataStream(FloatMatrixN &WaveformData);
	EarlabDataStream(SpikeStream &SpikeData);
	EarlabDataStream(EarlabDataStreamStruct *DataStream);
	~EarlabDataStream();

	inline EarlabDataStreamType GetStreamType(void) {return StreamType;}
	FloatMatrixN &GetWaveformData(void);
	SpikeStream &GetSpikeData(void);
private:
	EarlabDataStreamType StreamType;
	FloatMatrixN *WaveformData;
	SpikeStream *SpikeData;
};
#endif
