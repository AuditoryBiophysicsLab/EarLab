#include "earlab.h"
#include "EarlabException.h"
#include "EarlabDataStream.h"
#include "stdio.h"

EarlabDataStream::EarlabDataStream(FloatMatrixN &WaveformData)
{
	StreamType = Waveform;
	this->WaveformData = &WaveformData;
	SpikeData = NULL;
}

EarlabDataStream::EarlabDataStream(SpikeStream &SpikeData)
{
	StreamType = Spike;
	this->SpikeData = &SpikeData;
	WaveformData = NULL;
}

EarlabDataStream::EarlabDataStream(EarlabDataStreamStruct *DataStream)
{
}

EarlabDataStream::~EarlabDataStream()
{
}

FloatMatrixN &EarlabDataStream::GetWaveformData(void)
{
	if (StreamType != Waveform)
		throw EarlabException("EarlabDataStream: Requested waveform data when data stream is not of waveform type");

	return *WaveformData;
}

SpikeStream &EarlabDataStream::GetSpikeData(void)
{
	if (StreamType != Spike)
		throw EarlabException("EarlabDataStream: Requested spike data when data stream is not of spike type");

	return *SpikeData;
}
