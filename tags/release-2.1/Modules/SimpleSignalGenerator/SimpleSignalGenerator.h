/******************************************************************/
/*  SimpleSignalGenerator.h                                             */
/*  Header file for Signal Generator module                       */
/*                                                                */
/*  8 May 2003      1.0    DJA    Initial C version               */
/* 29 Jul 2003      1.1    DJA    C++ version                     */
/******************************************************************/
#ifndef SIGNAL_GENERATOR_H_INCLUDED
#define SIGNAL_GENERATOR_H_INCLUDED

#include "Earlab.h"
#include "MatrixN.h"
#include <stdio.h>
#include "Logger.h"
#include "EarlabDataStream.h"
#include "EarlabModule.h"

class SimpleSignalGenerator : public EarlabModule
{
public:
    SimpleSignalGenerator();
    ~SimpleSignalGenerator();

    int ReadParameters(const char *ParameterFileName);
    int ReadParameters(const char *ParameterFileName, const char *SectionName);

    int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount]);
    int Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

private:
	bool mProduceSineSignal, mProduceImpulseSignal, mProduceOutputFile, mSineModulateSignal, mProduceNoiseSignal;
	double mSampleRate_Hz;
	int mSamplesPerFrame;
	double mSineAmplitude_dB, mImpulseAmplitude_dB, mNoiseAmplitude_dB;
	int mCurImpulseCounter;
	double mSineFrequency_Hz, mImpulseFrequency_Hz;
    double mModulationDepth_Percent, mModulationFrequency_Hz;
	float mImpulseAmplitude, mImpulsePeriod;
	float mSineAmplitude, mSinePeriod, mSineOmega, mSineOmegaDelta;
	float mModulationOmega, mModulationDepth, mModulationFloor, mModulationOmegaDelta;
	float mNoiseAmplitude;
	char mOutputFileName[64];
	FILE *mOutputFile;
};

#endif // SIGNAL_GENERATOR_H_INCLUDED
