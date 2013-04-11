// EarlabModuleTestHarness.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "Earlab.h"
#include "EarlabException.h"
#include "DesktopEarlabDLL.h"
#define DEFINE_GLOBAL_LOGGER
#include "Logger.h"
#include <signal.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
void Usage(char *Program);
void __stdcall Logger(char *Message);

bool StopNow = false;

#ifdef _WIN32
void __cdecl SignalHandler(int Signal)
#else
void SignalHandler(int Signal)
#endif
{
	StopNow = true;
}

int main(int argc, char* argv[])
{
	void *Model;
	long frameCount = -1;
	char *test;
	int status, cur_arg = 0;
	char *ModuleDirectory = NULL;
	char *InputDirectory = NULL;
	char *OutputDirectory = NULL;
#ifdef _WIN32
	void (__cdecl *TheHandler)(int) = &SignalHandler;
#else
	void (*TheHandler)(int) = &SignalHandler;
#endif
	char ModulesPath[256];
	char ErrorMessage[256];
#ifdef _WIN32
	LONG ReturnStatus;
	HKEY hKey;
	DWORD RegType;
	DWORD PathLength;
#endif

	signal(SIGINT, TheHandler);

#ifdef _WIN32
	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
	ReturnStatus = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\EarLab", 0, KEY_QUERY_VALUE, &hKey);
	if (ReturnStatus == ERROR_SUCCESS)
	{
		ReturnStatus = RegQueryValueEx(hKey, "ModulesPath", 0, &RegType, (LPBYTE)ModulesPath, &PathLength);
		if (ReturnStatus == ERROR_SUCCESS)
			ModuleDirectory = ModulesPath;
		RegCloseKey(hKey);
	}
#endif

	for (cur_arg = 1; cur_arg < 100; cur_arg++)
	{
		if (argv[cur_arg] == NULL)
			break;
		if (strcmp("-n", argv[cur_arg]) == 0)
		{
			frameCount = strtol(argv[++cur_arg], &test, 10);
		}
		else
		{
			if (strcmp("-o", argv[cur_arg]) == 0)
			{
				OutputDirectory = argv[++cur_arg];
			}
			else
			{
				if (strcmp("-i", argv[cur_arg]) == 0)
				{
					InputDirectory = argv[++cur_arg];
				}
				else
				{
					if (strcmp("-m", argv[cur_arg]) == 0)
					{
						ModuleDirectory = argv[++cur_arg];
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	if (ModuleDirectory == NULL)
	{
#ifdef _WIN32
		memset(ErrorMessage, 0, sizeof(ErrorMessage));
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, ReturnStatus, 0, ErrorMessage, sizeof(ErrorMessage), NULL);
		cout << "Error: Module path not available from registry, please use '-m' option" << endl;
		cout << "      " << ErrorMessage << endl;
#endif
		Usage(argv[0]);
	}

	if (frameCount <= 0)
		Usage(argv[0]);
	try
	{
		Model = CreateModel();
		if (Model == NULL)
		{
			Logger("Error creating model");
			exit(1);
		}
		SetLogCallback(Model, &Logger);
		if (ModuleDirectory != NULL)
		{
			status = SetModuleDirectory(Model, ModuleDirectory);
		}
		else
		{
			status = SetModuleDirectory(Model, "");    //changed this line
		}
		if (status == 0)
		{
			Logger("Error setting module directory");
			exit(1);
		}
		if (InputDirectory != NULL)
		{
			status = SetInputDirectory(Model, InputDirectory);
		}
		if (OutputDirectory != NULL)
		{
			status = SetOutputDirectory(Model, OutputDirectory);
		}
		if (LoadModelConfigFile(Model, argv[cur_arg], 1000.0f) == 0)
		{
			Logger("Error setting diagram file");
			exit(1);
		}
		if (LoadModuleParameters(Model, argv[cur_arg + 1]) == 0)
		{
			Logger("Error setting parameters file");
			exit(1);
		}
		if (StartModules(Model) == 0)
		{
			Logger("Error starting modules");
			exit(1);
		}
		for (int j = 0; j < frameCount; j++)
		{
			if (AdvanceModules(Model) == 0)
			{
				Logger("Advance failed");
				exit(1);
			}
			if (StopNow)
				break;
		}
		if (StopModules(Model) == 0)
		{
			Logger("Error stopping modules");
			exit(1);
		}

		if (UnloadModel(Model) == 0)
		{
			Logger("Error unloading modules");
			exit(1);
		}
	} // try
	catch (EarlabException e)
	{
		cout << argv[0] << ": Error while processing file " << argv[3] << ":" << endl << "  " << e.GetExceptionString() << endl;
	}
	catch (char *ErrorMessage)
	{
		cout << argv[0] << ": Error while processing file " << argv[3] << ":" << endl << "  " << ErrorMessage << endl;
	}

	return 0;
}

void Usage(char *Program)
{
	cerr << "Usage: " << Program << " -n <NumFrames> [-m <ModuleDirectory>]" << endl;
	cerr << "                       [-i <InputDirectory>] [-o <OutputDirectory>] " << endl;
	cerr << "                       <DiagramFile> <ParameterFile>" << endl;
	cerr << endl;
	cerr << "Where: <NumFrames> is a positive integer which is the number of frames over " << endl;
	cerr << "                   which the simulation will be run" << endl;
	cerr << "       <ModuleDirectory> is a directory path that contains the module" << endl;
	cerr << "                         executables that will be used for this simulation." << endl;
	cerr << "                         If this option is not provided, the module directory" << endl;
	cerr << "                         defaults to the directory into which EarLab was" << endl;
	cerr << "                         installed." << endl;
	cerr << "       <InputDirectory> is a directory path that contains the input files for" << endl;
	cerr << "                        the model." << endl;
	cerr << "       <OutputDirectory> is a directory path that will contain the output files" << endl;
	cerr << "                          produced by the model." << endl;
	cerr << "       <DiagramFile> is a file that describes an Earlab model" << endl;
	cerr << "       <ParameterFile> is a parameter file that contains parameter definitions" << endl;
	cerr << "                       for all modules specified by the <DiagramFile>." << endl;
	cerr << endl;
	exit(1);
}

void __stdcall Logger(char *Message)
{
	cout << Message << endl;
	cout.flush();
}

