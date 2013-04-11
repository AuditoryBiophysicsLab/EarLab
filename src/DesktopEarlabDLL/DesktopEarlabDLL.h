// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DESKTOPEARLABDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DESKTOPEARLABDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef _WIN32
#ifdef DESKTOPEARLABDLL_EXPORTS
#define DESKTOPEARLABDLL_API __declspec(dllexport)
#else // DESKTOPEARLABDLL_EXPORTS
#define DESKTOPEARLABDLL_API __declspec(dllimport)
#endif // DESKTOPEARLABDLL_EXPORTS
#else // _WIN32
#define DESKTOPEARLABDLL_API
#endif // _WIN32
#include "Logger.h"
extern "C"
{
	DESKTOPEARLABDLL_API void *CreateModel			(void);
	DESKTOPEARLABDLL_API void  SetLogCallback   	(void *ModelPtr, LogCallback theCallback);
	DESKTOPEARLABDLL_API int   SetInputDirectory	(void *ModelPtr, char *InputDirectoryPath);
	DESKTOPEARLABDLL_API int   SetOutputDirectory	(void *ModelPtr, char *OutputDirectoryPath);
	DESKTOPEARLABDLL_API int   SetModuleDirectory	(void *ModelPtr, char *ModuleDirectoryPath);
	DESKTOPEARLABDLL_API int   LoadModelConfigFile	(void *ModelPtr, char *ModelConfigFileName, float FrameSize_uS);
	DESKTOPEARLABDLL_API int   LoadModuleParameters	(void *ModelPtr, char *ModuleParameterFileName);
	DESKTOPEARLABDLL_API int   StartModules			(void *ModelPtr);
	DESKTOPEARLABDLL_API int   RunModel				(void *ModelPtr, int NumFrames);
	DESKTOPEARLABDLL_API int   AdvanceModules		(void *ModelPtr);
	DESKTOPEARLABDLL_API int   StopModules			(void *ModelPtr);
	DESKTOPEARLABDLL_API int   UnloadModel			(void *ModelPtr);
}
