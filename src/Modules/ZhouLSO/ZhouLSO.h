#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"
#include "ZhouLSOCompartment.h"

class ZhouLSO : public EarlabModule
{
public:
    ZhouLSO();
    ~ZhouLSO();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	int FrameLength_Samples;
	
	double mFrameLength_Seconds; // used with spike inputs
	double mSampleRate_Hz;
	double mC_m_Farads;
	double mG_L_Siemens;
	double mE_L_Volts;
	double mG_abs_increment_Siemens;
	// double mG_abs_Siemens;
	double mE_abs_Volts;
	double mt_abs_seconds;
	double mG_AHP_increment_Siemens;
	// double mG_AHP_Siemens;
	double mE_AHP_Volts;
	double mTau_AHP_Seconds;
	double mV_th_Volts;
	double mV_rest_Volts;

	double mE_e_Volts;
	double mE_i_Volts;
	double mG_e_Siemens;
	double mG_i_Siemens;
	double mTau_e_rise_Seconds;
	double mTau_e_fall_Seconds;
	double mTau_i_rise_Seconds;
	double mTau_i_fall_Seconds;

	int NumChannels;
	int NumInputReps;

	ZhouLSOCompartment **Neurons;
	ZLSynapse1stOrderDiffc **ExcitatorySynapses;
	ZLSynapse1stOrderDiffc **InhibitorySynapses;
	int **SynapseList;

	char mGeFilename[256], mGiFilename[256], mGabsFilename[256], mGAHPFilename[256];
	FILE *mGeFile, *mGiFile, *mGabsFile, *mGAHPFile;
	char mVmemFilename[256];
	FILE *mVmemFile;
};
