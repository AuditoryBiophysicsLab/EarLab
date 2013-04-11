#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "MGCompartment.h"
//#include "MGSynapseFirstOrder.h"
#include "SpikeStreamMapper.h"
#include "CarneySpikeGenerator.h"
#include <time.h>
#include "EarlabModule.h"

class VoigtDCNCell : public EarlabModule
{
public:
    VoigtDCNCell();
    ~VoigtDCNCell();

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
	double Tau_m_Seconds, b_K, V_th_Volts, Tau_K_Seconds, E_K_Volts, E_ex_Volts, E_in_Volts;
	//double EPSP_strength;
	double mEPSP_strength;
	double mEPSP_strengthNSA;
	double mIPSP_strength1;
	double mIPSP_strength2;
	//double SynapticTau_Seconds;
	double mExcitatorySynapticTau_Seconds;
	double mExcitatorySynapticTau_SecondsNSA;
	double mInhibitorySynapticTau_Seconds1;
	double mInhibitorySynapticTau_Seconds2;
	int SamplesPerFrame;

	MGCompartment ***MGCells;
	CarneySpikeGenerator ***NSAinputs; // Non-specific Afferents used in P-type DCN cells

	//MGSynapseFirstOrder ***Synapses;
	MGSynapseFirstOrder ***ExcitatorySynapses;
	MGSynapseFirstOrder ***ExcitatorySynapsesNSA;
	MGSynapseFirstOrder ***InhibitorySynapses1;
	MGSynapseFirstOrder ***InhibitorySynapses2;
	bool **WasUnderThreshold;


	//int ***SynapseList;

	char mInputFileName[256];
	FILE *mInputFile;


	char mSpikeMapFileNameExcitatory[256];
	char mSpikeMapFileNameInhibitory1[256];
	char mSpikeMapFileNameInhibitory2[256];
	FILE *mSpikeMapFileExcitatory;
	FILE *mSpikeMapFileInhibitory1;
	FILE *mSpikeMapFileInhibitory2;

	SpikeStreamMapper *SpikeMapExcitatory;
	SpikeStreamMapper *SpikeMapInhibitory1;
	SpikeStreamMapper *SpikeMapInhibitory2;

//	int NumChannels;
//	int NumInputReps, NumOutputReps;
	int NumInputFreqChansExcitatory, NumInputFreqChansInhibitory1, NumInputFreqChansInhibitory2;
	int NumInputCellsPerFreqChanExcitatory, NumInputCellsPerFreqChanInhibitory1, NumInputCellsPerFreqChanInhibitory2;
	int	NumOutputFreqChans, NumOutputCellsPerFreqChan;
	//int mMaxSynapseCount;
};
