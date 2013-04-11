#include "MatrixN.h"
#include "Earlab.h"
#include <stdio.h>
#include "Logger.h"
#include "BinaryWriter.h"
#include "EarlabDataStream.h"

class MSOCombiner
{
public:
    MSOCombiner();
    ~MSOCombiner();

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
	int mFrameCount;
	double mSampleRate_Hz;
	int mNumChannels, mMaxLag_Samples, mZeroLag_Index;
	double mMaxITD_uS, mLagResolution_uS;
	int mInputLagCount;
	double *mCFArray_Hz;
	double *mChannelBuffer;
	BinaryWriter *mWriter;
};
