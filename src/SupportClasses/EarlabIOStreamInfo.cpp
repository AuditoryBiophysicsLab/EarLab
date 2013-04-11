#include "EarlabIOStreamInfo.h"

EarlabIOStreamInfo::EarlabIOStreamInfo(int Index, const char *Name, EarlabDataStreamType Type, const char *Description, const char *Constraints) : XMLInfo("Stream")
{
	AddElement("Index", Index);
	AddElement("Name", Name);
	AddElement("Type", StreamTypeToString(Type));
	AddElement("Description", Description);
	AddElement("Constraints", Constraints);
}

char *EarlabIOStreamInfo::StreamTypeToString(EarlabDataStreamType Type)
{
	switch (Type)
	{
	case WaveformData:
		return "Waveform";
	case SpikeData:
		return "Spikes";
	default:
	case Reserved:
	case StreamNotPresent:
		return "Unknown";
	}
}
