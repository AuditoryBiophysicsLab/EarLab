#ifndef EARLAB_MODULE_INSTANCE_H_INCLUDED
#define EARLAB_MODULE_INSTANCE_H_INCLUDED
#include <stdio.h>
#include <memory>
#include <windows.h>
#include "MatrixN.h"
#include "Earlab.h"
#include "Logger.h"

typedef int EFIReadParameters(void *Instance, const char *ParameterFileName);
typedef int EFIReadParametersEx(void *Instance, const char *ParameterFileName, const char *SectionName);
typedef int EFIStart(void *Instance, int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions]);
typedef int EFIAdvance(void *Instance, DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount]);
typedef int EFIStop(void *Instance);
typedef int EFIUnload(void *Instance);

typedef void EFISetModuleName(void *Instance, const char *ModuleName);
typedef void EFISetLogger(void *Instance, Logger *TheLogger);

typedef void *EFINewInstance(void);
typedef void EFIDeleteInstance(void *Instance);

class EarlabModuleInstance
{
public:
	EarlabModuleInstance();
	EarlabModuleInstance(const char *ModuleDLLFileName);
	EarlabModuleInstance(const char *ModuleDLLFileName, const char *ModuleName);
	EarlabModuleInstance(const char *ModuleDLLFileName, const char *ParameterFileName, const char *ModuleName);
	~EarlabModuleInstance();

	int ReadParameters(const char *ParameterFileName);
	int ReadParametersEx(const char *ParameterFileName, const char *SectionName);
	int Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions]);
	int Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount]);
	int Stop(void);
	int Unload(void);

	void Subscribe(EarlabModuleInstance *Source, int OutputNumber);
	int Advance(void);
	int Start(void);

	void SetModuleName(const char *ModuleName);
	void SetLogger(Logger *TheLogger);

	int *InputSizes(int InputNum);
	int *OutputSizes(int OutputNum);
	void SetInputCount(int NewInputCount);
	void SetOutputCount(int NewOutputCount);

	DoubleMatrixN *FrameBuf[EarlabMaxIOStreamCount][2];
private:
	void *MapAddress(char *FunctionName);
	void MapToDLL(const char *ModuleDLLFileName);
	void CreateOutputStreams(void);

	EFIReadParameters *mReadParameters;
	EFIReadParametersEx *mReadParametersEx;
	EFIStart *mStart;
	EFIAdvance *mAdvance;
	EFIStop *mStop;
	EFIUnload *mUnload;

	EFISetModuleName *mSetModuleName;
	EFISetLogger *mSetLogger;

	EFINewInstance *mNewInstance;
	EFIDeleteInstance *mDeleteInstance;

	EarlabModuleInstance *mInput[EarlabMaxIOStreamCount];
	int mInputNum[EarlabMaxIOStreamCount];
	int mSrcFrame, mDstFrame, mInputCount, mOutputCount;
	int mInputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions];
	int mOutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions];

	EarlabModuleInstance *mNext;

	char *mModuleDLLFileName;
	char *mModuleName;
	void *mInstance;
	HMODULE mModule;
};

#endif // EARLAB_MODULE_INSTANCE_H_INCLUDED
