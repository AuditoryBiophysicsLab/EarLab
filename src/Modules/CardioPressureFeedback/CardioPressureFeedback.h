#include <time.h>
#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "HHSynapse.h"
#include "XmlMetadata.h"
#include "EarlabModule.h"
#include "GenericIIRFilter.h"

class CardioPressureFeedback : public EarlabModule
{
public:
    CardioPressureFeedback();
    ~CardioPressureFeedback();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	double mSampleRate_Hz;
	double mCapacitance, mInitialVoltageOrCurrent;
	int SamplesPerFrame, mNumBranches;
	bool mPressureIsInput, mIsVentricleModel;
	double *mResistances;
	FloatMatrixN *mInput, *mOutput;
	int mNumAs, mNumBs;
	double *mAs, *mBs;

	GenericIIRFilter *mCompartment;

	XmlMetadataFile *mMetadataFile;	// Metadata for optional outputs
};
