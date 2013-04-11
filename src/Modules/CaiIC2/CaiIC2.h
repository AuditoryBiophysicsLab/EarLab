#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
//#include "Cai98Compartment.h"
//#include "Rothman03Compartment.h"
//#include "Rothman03AHPCompartment.h"
#include "HHCompartmentFactory.h"
#include "HHSynapse.h"
#include <time.h>
#include "EarlabModule.h"

//#define COMPARTMENT_TYPE Rothman03AHPCompartment


class CaiIC2 : public EarlabModule
{
public:
    CaiIC2();
    ~CaiIC2();

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
	double mExSynapticStrength;
	double mExSynapticTau;
	double mInhSynapticStrength;
	double mInhSynapticTau;
	double mCellTemperature_DegreesC;
	double mInhInputDelay_Seconds;
	int SamplesPerFrame;

	double mTauAHP_Seconds;
	double mAHPIncrement_Siemens;
	
	int mResponseType;// 1=TypeI-c, 11=TypeI-t, 12=TypeI-II, 21=TypeII-I, 2=TypeII, 19=Zhou06Gmaxs
					  // 0 = CUSTOMTYPE which allows custom capacitance and Gmax values with TypeII default values
						// All types may be made adaptive by setting 
						// non-zero values for mTauAHP_Seconds and mAHPIncrement_Siemens
	EnumedResponseType newmResponseType;	

	//COMPARTMENT_TYPE ***ICCells;
	HHCompartment ***ICCells;
	char *mCompartmentClassName;

	HHSynapse ***ExcitatorySynapses, ***InhibitorySynapses;
	bool **WasUnderThreshold;
	// SpikeStream *TempInputContra;
	SpikeStream *InputContra;

	int NumChannels;
	int NumInputReps, NumLags;
	int InputCellCount, OutputCellCount, mExcitatoryMSOLagNumber;

	SpikeDelayBuffer *InhInputDelayBuffer;

	Rothman03CustomParameterValues mRothman03CustomParameterValues;

	char mgeFilename[256], mgiFilename[256], mgNaFilename[256], mgKAFilename[256], mgKHTFilename[256], mgKLTFilename[256], mghFilename[256], mgAHPFilename[256];
	FILE *mgeFile, *mgiFile, *mgNaFile, *mgKAFile, *mgKHTFile, *mgKLTFile, *mghFile, *mgAHPFile;
	char mVmemFilename[256];
	FILE *mVmemFile;

};
