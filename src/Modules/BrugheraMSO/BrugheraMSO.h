#include <time.h>
#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "Brughera96Compartment.h"
#include "HHSynapse.h"
#include "XmlMetadata.h"
#include "EarlabModule.h"

#define COMPARTMENT_TYPE Brughera96Compartment

class BrugheraMSO : public EarlabModule
{
public:
    BrugheraMSO();
    ~BrugheraMSO();

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
	double mFrameLength_Seconds;
	double mExcitatorySynapticStrength, mInhibitorySynapticStrength;
	double mExcitatorySynapticTau, mInhibitorySynapticTau;
	double mCellTemperature_DegreesC;
	double mMaxLag_Seconds;
	int SamplesPerFrame;
	bool Inhibitory; // ***NEW***

	COMPARTMENT_TYPE ***MSOCells;
	HHSynapse ***ExcitatorySynapses;
	HHSynapse ***InhibitorySynapses;
	bool **WasUnderThreshold;
	int **SynapseList;
	//SpikeStream **InputContra; //	***OLD***
	SpikeStream **InputContraExcit;// ***NEW***
	SpikeStream **InputContraInhib;// ***NEW***

	int NumChannels;
	int NumInputReps, NumLags;
	int InputCellCount, OutputCellCount;
	int mNumOptionalOutputs;

	//SpikeDelayBuffer **DelayBuffers; // ***OLD***
	SpikeDelayBuffer **InhibitoryDelayBuffers;// ***NEW***
	SpikeDelayBuffer **ExcitatoryDelayBuffers;// ***NEW***

	XmlMetadataFile **mMetadataFiles;	// Metadata for optional outputs

	char mgeFilename[256], mgiFilename[256], mgNaFilename[256], mgKHTFilename[256], mgKLTFilename[256];
	FILE *mgeFile, *mgiFile, *mgNaFile, *mgKHTFile, *mgKLTFile;  // may not need *mOutputFile: FILE *mOutputFile, ...;
	char mVmemFilename[256];
	FILE *mVmemFile;

};
