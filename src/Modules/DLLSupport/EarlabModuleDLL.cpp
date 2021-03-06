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
#include "EarlabException.h"

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
	try
	{
		return TheModule->ReadParameters(ParameterFileName);
	}
	catch (EarlabException e)
	{
		printf("%s: ReadParameters() threw exception: %s\n", TheModule->GetModuleName(), e.GetExceptionString());
		fflush(stdout);
		return 0;
	}
}

EARLABMODULEDLL_API int ReadParametersEx(MODULE_CLASS_NAME *TheModule, char *ParameterFileName, char *SectionName)
{
	try
	{
		return TheModule->ReadParameters(ParameterFileName, SectionName);
	}
	catch (EarlabException e)
	{
		printf("%s: ReadParametersEx() threw exception: %s\n", TheModule->GetModuleName(), e.GetExceptionString());
		fflush(stdout);
		return 0;
	}
}

EARLABMODULEDLL_API int Start(MODULE_CLASS_NAME *TheModule, 
			      int NumInputs, 
                              EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount], 
                              int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			      int NumOutputs, 
                              EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], 
                              int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
                              unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	try
	{
		return TheModule->Start(NumInputs, InputTypes, InputSize, NumOutputs, OutputTypes, OutputSize, OutputElementCounts);
	}
 	catch (EarlabException e)
	{
		printf("%s: Start() threw exception: %s\n", TheModule->GetModuleName(), e.GetExceptionString());
		fflush(stdout);
		return 0;
	}
}

EARLABMODULEDLL_API int Advance(MODULE_CLASS_NAME *TheModule, EarlabDataStream *Input[EarlabMaxIOStreamCount], EarlabDataStream *Output[EarlabMaxIOStreamCount])
{
	try
	{
		return TheModule->Advance(Input, Output);
	}
	catch (EarlabException e)
	{
		printf("%s: Advance() threw exception: %s\n", TheModule->GetModuleName(), e.GetExceptionString());
		fflush(stdout);
		return 0;
	}
}

EARLABMODULEDLL_API int Stop(MODULE_CLASS_NAME *TheModule)
{
	try
	{
		return TheModule->Stop();
	}
	catch (EarlabException e)
	{
		printf("%s: Stop() threw exception: %s\n", TheModule->GetModuleName(), e.GetExceptionString());
		fflush(stdout);
		return 0;
	}
}

EARLABMODULEDLL_API int Unload(MODULE_CLASS_NAME *TheModule)
{
	try
	{
		return TheModule->Unload();
	}
	catch (EarlabException e)
	{
		printf("%s: Unload() threw exception: %s\n", TheModule->GetModuleName(), e.GetExceptionString());
		fflush(stdout);
		return 0;
	}
}

EARLABMODULEDLL_API void SetModuleName(MODULE_CLASS_NAME *TheModule, char *ModuleName)
{
	try
	{
		TheModule->SetModuleName(ModuleName);
	}
	catch (EarlabException e)
	{
		printf("%s: SetModuleName() threw exception: %s\n", TheModule->GetModuleName(), e.GetExceptionString());
		fflush(stdout);
	}
}

EARLABMODULEDLL_API void SetLogger(MODULE_CLASS_NAME *TheModule, Logger *TheLogger)
{
	try
	{
		TheModule->SetLogger(TheLogger);
	}
	catch (EarlabException e)
	{
		printf("%s: SetLogger() threw exception: %s\n", TheModule->GetModuleName(), e.GetExceptionString());
		fflush(stdout);
	}
}

EARLABMODULEDLL_API void SetEnvironment(MODULE_CLASS_NAME *TheModule, EarlabEnvironment *theEnvironment)
{
	try
	{
		TheModule->SetEnvironment(theEnvironment);
	}
	catch (EarlabException e)
	{
		printf("%s: SetEnvironment() threw exception: %s\n", TheModule->GetModuleName(), e.GetExceptionString());
		fflush(stdout);
	}
}

EARLABMODULEDLL_API MODULE_CLASS_NAME *NewInstance(void)
{
	return new MODULE_CLASS_NAME();
}

EARLABMODULEDLL_API void DeleteInstance(MODULE_CLASS_NAME *TheModule)
{
	delete TheModule;
}
