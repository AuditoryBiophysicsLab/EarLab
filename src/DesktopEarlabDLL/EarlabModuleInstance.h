#ifndef EARLAB_MODULE_INSTANCE_H_INCLUDED
#define EARLAB_MODULE_INSTANCE_H_INCLUDED
#include <stdio.h>
#include <memory>
#ifdef _WIN32
#include <windows.h>
#endif
#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"
#include "MatrixN.h"
#include "SpikeStream.h"
#include "EarlabEnvironment.h"

typedef int EFIReadParameters(void *Instance, const char *ParameterFileName);
typedef int EFIReadParametersEx(void *Instance, const char *ParameterFileName, const char *SectionName);
typedef int EFIStart(void *Instance, int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			  int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			  unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);
typedef int EFIAdvance(void *Instance, EarlabDataStream *Input[EarlabMaxIOStreamCount], EarlabDataStream *Output[EarlabMaxIOStreamCount]);
typedef int EFIStop(void *Instance);
typedef int EFIUnload(void *Instance);

typedef void EFISetModuleName(void *Instance, const char *ModuleName);
typedef void EFISetLogger(void *Instance, Logger *TheLogger);
typedef void EFISetEnvironment(void *Instance, EarlabEnvironment *theEnvironment);

typedef void *EFINewInstance(void);
typedef void EFIDeleteInstance(void *Instance);

class EarlabModuleInstance
{
public:
	EarlabModuleInstance();
	EarlabModuleInstance(const char *ModuleFileName);
	EarlabModuleInstance(const char *ModuleFileName, const char *ModuleName);
	EarlabModuleInstance(const char *ModuleFileName, const char *ParameterFileName, const char *ModuleName);
	~EarlabModuleInstance();

	int ReadParameters(const char *ParameterFileName);
	int ReadParametersEx(const char *ParameterFileName, const char *SectionName);

	//int Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions]);
	int Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			  int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			  unsigned int OutputElementCounts[EarlabMaxIOStreamCount]);

	int Advance(EarlabDataStream *Input[EarlabMaxIOStreamCount], EarlabDataStream *Output[EarlabMaxIOStreamCount]);
	int Stop(void);
	int Unload(void);

	void Subscribe(EarlabModuleInstance *Source, int OutputNumber);
	int Advance(void);
	int Start(void);

	void SetModuleName(const char *ModuleName);
	void SetLogger(Logger *TheLogger);
	void SetEnvironment(EarlabEnvironment *theEnvironment);

	int *InputSizes(int InputNum);
	int *OutputSizes(int OutputNum);

	void SetInputCount(int NewInputCount);
	void SetOutputCount(int NewOutputCount);

	void SetOutputType(int OutputNumber, EarlabDataStreamType StreamType);
	void SetInputType(int InputNumber, EarlabDataStreamType StreamType);

	EarlabDataStreamType GetOutputType(int OutputNumber);
	EarlabDataStreamType GetInputType(int InputNumber);

	EarlabDataStream *FrameBuf[EarlabMaxIOStreamCount][2];
private:
	void *MapAddress(char *FunctionName);
	void MapToModule(const char *ModuleFileName);
	void CreateOutputStreams(void);
	void LoadModuleFile(const char *ModuleFileName);
	void FreeModuleFile(void);

	unsigned int mOutputElementCounts[EarlabMaxIOStreamCount];

	EFIReadParameters *mReadParameters;
	EFIReadParametersEx *mReadParametersEx;
	EFIStart *mStart;
	EFIAdvance *mAdvance;
	EFIStop *mStop;
	EFIUnload *mUnload;

	EFISetModuleName *mSetModuleName;
	EFISetLogger *mSetLogger;
	EFISetEnvironment *mSetEnvironment;

	EFINewInstance *mNewInstance;
	EFIDeleteInstance *mDeleteInstance;

	EarlabModuleInstance *mInput[EarlabMaxIOStreamCount];
	int mInputNum[EarlabMaxIOStreamCount];
	int mSrcFrame, mDstFrame, mInputCount, mOutputCount;
	EarlabDataStreamType mOutputStreamTypes[EarlabMaxIOStreamCount];
	EarlabDataStreamType mInputStreamTypes[EarlabMaxIOStreamCount];
	int mInputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions];
	int mOutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions];

	EarlabModuleInstance *mNext;

	char *mModuleFileName;
	char *mModuleName;
	void *mInstance;
#ifdef _WIN32
	HMODULE mModule;
#else
	void *mModule;
#endif
};

#endif // EARLAB_MODULE_INSTANCE_H_INCLUDED
