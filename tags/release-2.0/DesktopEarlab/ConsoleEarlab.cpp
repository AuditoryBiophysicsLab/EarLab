// EarlabModuleTestHarness.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include "Earlab.h"
#include "EarlabException.h"
#include "DesktopEarlabDLL.h"
#define DEFINE_GLOBAL_LOGGER
#include "Logger.h"
#include <signal.h>

using namespace std;
void Usage(char *Program);
void __stdcall Logger(char *Message);

bool StopNow = false;

void __cdecl SignalHandler(int Signal)
{
	StopNow = true;
}

int main(int argc, char* argv[])
{
	void *Model;
	long frameCount;
	char *test;
	int status, cur_arg = 0;
	char *ModuleDirectory = NULL;
	void (__cdecl *TheHandler)(int) = &SignalHandler;

	signal(SIGINT, TheHandler);
	if ((argc != 5) && (argc != 7))
		Usage(argv[0]);
	if (strcmp("-n", argv[1]) != 0)
		Usage(argv[0]);

	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
	frameCount = strtol(argv[2], &test, 10);

	cur_arg = 3;
	if (strcmp("-m", argv[3]) == 0)
	{
		ModuleDirectory = argv[4];
		cur_arg = 5;
	}

	if ((*test != '\0') || (frameCount <= 0))
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
#ifdef _DEBUG
			status = SetModuleDirectory(Model, "C:\\Earlab\\Modules\\Debug");
#else
			status = SetModuleDirectory(Model, "C:\\Earlab\\Modules\\Release");
#endif
		}
		if (status == 0)
		{
			Logger("Error setting module directory");
			exit(1);
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
	cerr << "Usage: " << Program << " -n <NumFrames> [-m <ModuleDirectory>] <DiagramFile> <ParameterFile>" << endl;
	cerr << endl;
	cerr << "Where: <NumFrames> is a positive integer which is the number of frames over " << endl;
	cerr << "                   which the simulation will be run" << endl;
	cerr << "       <ModuleDirectory> is a directory path that contains the module executables" << endl;
	cerr << "                       that will be used for this simulation.  If this option is not provided," << endl;
	cerr << "                       the module directory defaults to \"C:\\Earlab\\Modules\\Release\"" << endl;
	cerr << "       <DiagramFile> is a file that describes an Earlab model" << endl;
	cerr << "       <ParameterFile> is a parameter file that contains parameter definitions" << endl;
	cerr << "                       for all modules specified by the <DiagramFile>." << endl;
	cerr << endl;
	exit(1);
}

void __stdcall Logger(char *Message)
{
	cout << Message << endl;
}
