#include <time.h>
#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "HHSynapse.h"
#include "XmlMetadata.h"
#include "EarlabModule.h"

class CardioDriver
{
public:
	CardioDriver(double SampleRate_Hz, double SystolicElastance, double DiastolicElastance, double InitialPeriod_Seconds);
	double Step(double NewPeriod_Seconds);
private:
	void NewBeat(void);
	double mSamplePeriod_Seconds, mSystolicElastance, mDiastolicElastance, mCurPeriod_Seconds, mLastPeriod_Seconds;;
	double mCurTime_Seconds, mSystolicPhase1EndTime_Seconds, mSystolicPhase2EndTime_Seconds, mBeatEndTime_Seconds;
	double mSystolicElasticityConstant, mPeriodConstant;
};

class CardioVentricularDriver : public EarlabModule
{
public:
    CardioVentricularDriver();
    ~CardioVentricularDriver();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	double mInitialHeartrate_BeatsPerMinute, mSystolicElastance, mDiastolicElastance;
	double mSampleRate_Hz, mOutputGain;
	int mSamplesPerFrame;

	CardioDriver *mCardioDriver;
	XmlMetadataFile *mMetadataFile;	// Metadata for optional outputs
};
