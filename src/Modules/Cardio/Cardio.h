#include <time.h>
#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "CardioCompartment.h"
#include "CardioModel.h"
#include "HHSynapse.h"
#include "XmlMetadata.h"
#include "EarlabModule.h"

#define COMPARTMENT_TYPE BasicPressureCompartment


class Cardio : public EarlabModule
{
public:
    Cardio();
    ~Cardio();

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
	int mNumCompartments;
	int SamplesPerFrame;
	int mVentricularFunctionInputIndex, mBiasPressureInputIndex, mNumInputs, mNumOutputs;
	FloatMatrixN *mOutput, *mElastance, *mBiasPressure;
	char mModelDescriptionFile[256];

	CardioModel *mModel;
};
