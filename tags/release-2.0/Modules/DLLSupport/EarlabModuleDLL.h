// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EARLABMODULEDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EARLABMODULEDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef EARLABMODULEDLL_EXPORTS
#define EARLABMODULEDLL_API __declspec(dllexport)
#else
#define EARLABMODULEDLL_API __declspec(dllimport)
#endif

#include "Earlab.h"
#include "Logger.h"
#include "EarlabDataStream.h"

extern "C"
{
	EARLABMODULEDLL_API int ReadParameters(MODULE_CLASS_NAME *TheModule, char *ParameterFileName);
	EARLABMODULEDLL_API int ReadParametersEx(MODULE_CLASS_NAME *TheModule, char *ParameterFileName, char *SectionName);
	//EARLABMODULEDLL_API int Start(MODULE_CLASS_NAME *TheModule, int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions]);
	//EARLABMODULEDLL_API int Advance(MODULE_CLASS_NAME *TheModule, DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount]);
	EARLABMODULEDLL_API int Start(MODULE_CLASS_NAME *TheModule, int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			  int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			  unsigned long OutputElementCounts[EarlabMaxIOStreamCount]);

	EARLABMODULEDLL_API int Advance(MODULE_CLASS_NAME *TheModule, EarlabDataStream *Input[EarlabMaxIOStreamCount], EarlabDataStream *Output[EarlabMaxIOStreamCount]);
	EARLABMODULEDLL_API int Stop(MODULE_CLASS_NAME *TheModule);
	EARLABMODULEDLL_API int Unload(MODULE_CLASS_NAME *TheModule);

	EARLABMODULEDLL_API void SetModuleName(MODULE_CLASS_NAME *TheModule, char *ModuleName);
	EARLABMODULEDLL_API void SetLogger(MODULE_CLASS_NAME *TheModule, Logger *TheLogger);
	
	EARLABMODULEDLL_API MODULE_CLASS_NAME *NewInstance(void);
	EARLABMODULEDLL_API void DeleteInstance(MODULE_CLASS_NAME *TheModule);
};
