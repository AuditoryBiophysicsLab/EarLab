#ifndef _CEARLAB_DATA_STREAM_H
#define _CEARLAB_DATA_STREAM_H

#include "CMatrixN.h"
#include "CSpike.h"

/*
 * ENUM defines so they can be used in other contexts
 */
#ifndef EFI_STREAM_NONE

#define EFI_STREAM_WAVEFORM 0
#define EFI_STREAM_SPIKE    1
#define EFI_STREAM_RESERVED 2
#define EFI_STREAM_NONE     999

typedef enum
{
	WaveformData            = EFI_STREAM_WAVEFORM,
	SpikeData               = EFI_STREAM_SPIKE,
	Reserved                = EFI_STREAM_RESERVED,
	StreamNotPresent        = EFI_STREAM_NONE,
} EarlabDataStreamType;

#endif

typedef union {
	TMatrixN_CLASS *waveform;
	Spike_CLASS    *spike;
	void           *ptr;    /* Used to reference the uncast pointer */
} EarlabDataStream;

typedef struct {
	EarlabDataStreamType datatype;  /* 0 = waveform, 1 = spikes */
	EarlabDataStream     data;      /* Data pointer             */
} Stream;

#endif
