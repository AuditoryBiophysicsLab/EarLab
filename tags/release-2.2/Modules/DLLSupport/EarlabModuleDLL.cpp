// EarlabModuleDLL.cpp : Defines the entry point for the DLL application.
//
#define EARLABMODULEDLL_EXPORTS

#ifdef _WIN32
#include "windows.h"
#endif
#include "ModuleSpecific.h"
#include "EarlabModuleDLL.h"
#include "MatrixN.h"
#include "Earlab.h"

#ifdef _WIN32
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
    return TRUE;
}
#endif

EARLABMODULEDLL_API int ReadParameters(MODULE_CLASS_NAME *TheModule, char *ParameterFileName)
{
	return TheModule->ReadParameters(ParameterFileName);
}

EARLABMODULEDLL_API int ReadParametersEx(MODULE_CLASS_NAME *TheModule, char *ParameterFileName, char *SectionName)
{
	return TheModule->ReadParameters(ParameterFileName, SectionName);
}

EARLABMODULEDLL_API int Start(MODULE_CLASS_NAME *TheModule, 
			      int NumInputs, 
                              EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount], 
                              int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			      int NumOutputs, 
                              EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], 
                              int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
                              unsigned long OutputElementCounts[EarlabMaxIOStreamCount])
{
	return TheModule->Start(NumInputs, InputTypes, InputSize, NumOutputs, OutputTypes, OutputSize, OutputElementCounts);
}

EARLABMODULEDLL_API int Advance(MODULE_CLASS_NAME *TheModule, EarlabDataStream *Input[EarlabMaxIOStreamCount], EarlabDataStream *Output[EarlabMaxIOStreamCount])
{
	return TheModule->Advance(Input, Output);
}

EARLABMODULEDLL_API int Stop(MODULE_CLASS_NAME *TheModule)
{
	return TheModule->Stop();
}

EARLABMODULEDLL_API int Unload(MODULE_CLASS_NAME *TheModule)
{
	return TheModule->Unload();
}

EARLABMODULEDLL_API void SetModuleName(MODULE_CLASS_NAME *TheModule, char *ModuleName)
{
	TheModule->SetModuleName(ModuleName);
}

EARLABMODULEDLL_API void SetLogger(MODULE_CLASS_NAME *TheModule, Logger *TheLogger)
{
	TheModule->SetLogger(TheLogger);
}

EARLABMODULEDLL_API void SetEnvironment(MODULE_CLASS_NAME *TheModule, EarlabEnvironment *theEnvironment)
{
	TheModule->SetEnvironment(theEnvironment);
}

EARLABMODULEDLL_API MODULE_CLASS_NAME *NewInstance(void)
{
	return new MODULE_CLASS_NAME();
}

EARLABMODULEDLL_API void DeleteInstance(MODULE_CLASS_NAME *TheModule)
{
	delete TheModule;
}
