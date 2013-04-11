#ifndef _EARLAB_DATA_STREAM_H_INCLUDED
#define _EARLAB_DATA_STREAM_H_INCLUDED
#include "MatrixN.h"
#include "SpikeStream.h"

#ifndef _WIN32
// Glenn: I put this in here because CMatrixN generates compile errors under the MSVC compiler
#include "CMatrixN.h"
#endif

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

class EarlabDataStream
{
public:
	EarlabDataStream() {;}
	virtual EarlabDataStreamType GetStreamType(void) = 0;   // All subclasses are required to override this pure virtual function
};

class EarlabWaveformStream : public EarlabDataStream
{
public:
	EarlabWaveformStream(FloatMatrixN *theData) {TheData = theData;}
	~EarlabWaveformStream();

	virtual EarlabDataStreamType GetStreamType(void) {return WaveformData;}
	inline FloatMatrixN *GetData(void) {return TheData;}
private:
	FloatMatrixN *TheData;
};

class EarlabSpikeStream : public EarlabDataStream
{
public:
	EarlabSpikeStream(SpikeStream *theData) {TheData = theData;}
	~EarlabSpikeStream();

	virtual EarlabDataStreamType GetStreamType(void) {return SpikeData;}
	inline SpikeStream *GetData(void) {return TheData;}
private:
	SpikeStream *TheData;
};
#endif
