#include <time.h>
#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
//#include "Rothman93Compartment.h"
//#include "Brughera96Compartment.h"
//#include "Rothman03Compartment.h"
//#include "Rothman03AHPCompartment.h"
#include "HHCompartmentFactory.h"
#include "HHSynapse.h"
#include "XmlMetadata.h"
#include "EarlabModule.h"

//#define COMPARTMENT_TYPE Rothman03Compartment
//#define COMPARTMENT_TYPE Rothman03AHPCompartment


class BrugheraMSO2 : public EarlabModule
{
public:
    BrugheraMSO2();
    ~BrugheraMSO2();

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
	double mAHPIncrement_Siemens;
	double mTauAHP_Seconds;
	int SamplesPerFrame;
	bool Inhibitory; // ***NEW***

	int mResponseType;// 1=TypeI-c, 11=TypeI-t, 12=TypeI-II, 21=TypeII-I, 2=TypeII, 19=Zhou06Gmaxs
					  // 0 = CUSTOMTYPE which allows custom capacitance and Gmax values with TypeII default values
						// All types may be made adaptive by setting
						// non-zero values for mTauAHP_Seconds and mAHPIncrement_Siemens
	EnumedResponseType newmResponseType;	

	//COMPARTMENT_TYPE ***MSOCells;
	HHCompartment ***MSOCells;
	char *mCompartmentClassName;


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

	Rothman03CustomParameterValues mRothman03CustomParameterValues;

	XmlMetadataFile **mMetadataFiles;	// Metadata for optional outputs

	char mgeFilename[256], mgiFilename[256], mgNaFilename[256], mgKAFilename[256], mgKHTFilename[256], mgKLTFilename[256], mghFilename[256], mgAHPFilename[256];
	FILE *mgeFile, *mgiFile, *mgNaFile, *mgKAFile, *mgKHTFile, *mgKLTFile, *mghFile, *mgAHPFile;
	char mVmemFilename[256];
	FILE *mVmemFile;

};
