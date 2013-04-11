#include "MatrixN.h"
#include "Earlab.h"
#include "FirstOrderHighpass.h"
#include "Logger.h"
#include "EarlabDataStream.h"

class MiddleEarSimpleFilter
{
public:
    MiddleEarSimpleFilter();
    ~MiddleEarSimpleFilter();

    int ReadParameters(char *ParameterFileName);
    int ReadParameters(char *ParameterFileName, char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

	void SetModuleName(char *ModuleName);
	void SetLogger(Logger *TheLogger);

private:
	Logger *mLogger;
	char *mModuleName;

	FirstOrderHighpass *mHighpassFilter;
	double mHighpassCornerFreq_Hz;
	int mHighpassFilterOrder;

	int mFrameSize_Samples;

	double FrameDuration_ms;
	double mSampleRate_Hz;
	double mGain;
	int SamplesPerFrame;
};
