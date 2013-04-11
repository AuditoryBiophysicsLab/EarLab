#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "XmlMetadata.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"

enum OutputType
{
	Rectifier = 1,
	Step = 2,
	Sigmoid = 3,
};

class SimpleIFR : public EarlabModule
{
public:
    SimpleIFR();
    ~SimpleIFR();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	OutputType mOutputType;
	double mSampleRate_Hz;
	int mNumElectrodes, mNumChannels, mFrameSize_Samples, mSampleCount;
	double mGain;

	XmlMetadataFile *mMetadataFile;

	float *mBiasVector;
	float **mCurrentSpreadMatrix;
	float *mOutputBuffer;
	double mOutputGain;
	char mCFFileName[256];
	char mWeightingMatrixFilename[256];
	char mBiasVectorFilename[256];
	char mOutputTypeName[256];
};
