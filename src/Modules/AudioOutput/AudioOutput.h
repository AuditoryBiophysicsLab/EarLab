/******************************************************************/
/*  DataSink.h                                                    */
/*  Header file for generic Data Sink module                      */
/*                                                                */
/*  8 May 2003      1.0    DJA    Initial version                 */
/* 31 Jul 2003      1.1    DJA    Converted code from writer.h    */
/*                                into C++ and made it DataSink.h */
/******************************************************************/

#include "MatrixN.h"
#include "stdio.h"
#include "Earlab.h"
#include "Downsampler.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"
#include "AudioDevice.h"

class AudioOutput : public EarlabModule
{
public:
    AudioOutput();
    ~AudioOutput();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	char mAudioDeviceName[256];
	char mWaveFileName[256];
	int mSampleRate_Hz;
	bool mStereoOutput;
};
