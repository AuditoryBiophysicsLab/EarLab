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

using namespace std;
void Usage(char *Program);
void __stdcall Logger(char *Message);

int ConsoleEarlab(int argc, char* argv[])
{
	void *Model;
	long frameCount;
	char *test;
	int status;

	if (argc != 5)
		Usage(argv[0]);
	if (strcmp("-n", argv[1]) != 0)
		Usage(argv[0]);

	frameCount = strtol(argv[2], &test, 10);
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
#ifdef _DEBUG
		status = SetModuleDirectory(Model, "..\\..\\bin\\Debug");
#else
		status = SetModuleDirectory(Model, "..\\..\\bin\\Release");
#endif
		if (status == 0)
		{
			Logger("Error setting module directory");
			exit(1);
		}
		if (LoadModelConfigFile(Model, argv[3], 1000.0f) == 0)
		{
			Logger("Error setting diagram file");
			exit(1);
		}
		if (LoadModuleParameters(Model, argv[4]) == 0)
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
			if (AdvanceModules(Model) == 0)
			{
				Logger("Advance failed");
				exit(1);
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
	cerr << "Usage: " << Program << " -n <NumFrames> <DiagramFile> <ParameterFile>" << endl;
	cerr << endl;
	cerr << "Where: <NumFrames> is a positive integer which is the number of frames over " << endl;
	cerr << "                   which the simulation will be run" << endl;
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
