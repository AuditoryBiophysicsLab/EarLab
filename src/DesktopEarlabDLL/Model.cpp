#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <sys/time.h>
#endif
#include "Earlab.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include "Model.h"
#include "MatrixN.h"
#include "EarlabException.h"
#include "EarlabModuleInstance.h"
#include "Logger.h"
#include <sys/stat.h>
#include <sys/types.h>
#ifdef USE_PTHREADS
#include <pthread.h>
#endif

using namespace std;

Model::Model()
{
	memset(this, 0, sizeof(Model));
	theEnvironment = new EarlabEnvironment();
#ifdef USE_PTHREADS
	pthread_mutex_init(&mModuleCountMutex, NULL);
	pthread_cond_init(&mModuleBarrierCond, NULL);
#endif
}

Model::~Model()
{
	int i;

	for (i = 0; i < mModuleCount; i++)
	{
		if (mModuleNames[i] != NULL)
			delete [] mModuleNames[i];
		if (mModules[i] != NULL)
			delete mModules[i];
		for (int j = 0; j < EarlabMaxIOStreamCount; j++)
		{
			if (mInputStreamNames[i][j] != NULL)
				delete [] mInputStreamNames[i][j];
		}
	}
	delete theEnvironment;
}

int Model::SetModuleDirectory(char *ModuleDirectoryPath)
{
	strncpy((char *)mModuleDirectory, ModuleDirectoryPath, sizeof(mModuleDirectory));
	return 1;
}

int Model::SetInputDirectory(char *InputDirectoryPath)
{
	theEnvironment->SetInputDirectoryPath(InputDirectoryPath);
	return 1;
}

int Model::SetOutputDirectory(char *OutputDirectoryPath)
{
	
#ifdef _WIN32
	if (mkdir(OutputDirectoryPath) == -1)
#else
	if (mkdir(OutputDirectoryPath, 0777) == -1)
#endif 
	{
		if (errno != EEXIST)
			throw EarlabException("Model: Error setting output directory to \"%s\": One or more components of the specified path do not exist", OutputDirectoryPath);
	}

	theEnvironment->SetOutputDirectoryPath(OutputDirectoryPath);
	return 1;
}

int Model::SetLogger(Logger *TheLogger)
{
	mLogger = TheLogger;
	return 1;
}

int Model::Load(char *ModelConfigFileName, float FrameSize_uS)
{
	char ModuleList[2048];
	char *tmp;
    ParameterStatus Status;
	char CurInput[256];
	int InputCount;

	mParamFile.ReadParameterFile(ModelConfigFileName);

	Status = mParamFile.GetParameter("Model", "ModuleList", ModuleList, sizeof(ModuleList), "");
	if (Status != Success)
		throw EarlabException("Model: Could not find required string parameter \"ModuleList\" in required section [Model] in file \"%s\"", ModelConfigFileName);

	//Status = mParamFile.GetParameter("Model", "ModuleDirectory", mModuleDirectory, sizeof(mModuleDirectory), "");
	//if (Status != Success)
	//	throw EarlabException("Model: Could not find required string parameter \"mModuleDirectory\" in required section [Model] in file \"%s\"", ModelConfigFileName);

	tmp = GetNextToken(",", ModuleList);
	mModuleCount = 0;
	while ((tmp != NULL) && (mModuleCount <= MaxEarlabModuleCount))
	{
		mModuleNames[mModuleCount] = new char[strlen(tmp) + 1];
		strcpy(mModuleNames[mModuleCount], tmp);
		tmp = GetNextToken(",", NULL);
		mModuleCount++;
	}

	mLogger->Log("Model: Loading modules...");
	for (int i = 0; i < mModuleCount; i++)
	{
		AddModule(mModuleNames[i]);
	}
	mLogger->Log("Model: Modules loaded");
	mLogger->Log("Model: Connecting modules...");
	for (int i = 0; i < mModuleCount; i++)
	{
		for (InputCount = 0; InputCount < EarlabMaxIOStreamCount; InputCount++)
		{
			char InputName[50];
			sprintf(InputName, "Input.%d", InputCount + 1);
			Status = mParamFile.GetParameter(mModuleNames[i], InputName, CurInput, sizeof(CurInput), NULL);
			if (Status != Success)
				break;
			mInputStreamNames[i][InputCount] = new char[strlen(CurInput) + 1];
			strcpy(mInputStreamNames[i][InputCount], CurInput);
		}

		for (int j = 0; j < InputCount; j++)
		{
			char ModuleName[50], OutputNumber[5], *tmp;
			int Output, SourceModule, *SourceOutputSizes, *DestInputSizes;
			tmp = GetNextToken(".", mInputStreamNames[i][j]);
			if (tmp == NULL)
				throw EarlabException("Model: Input stream description format error while processing module %s", mModuleNames[i]);
			strncpy((char *)ModuleName, tmp, sizeof(ModuleName));
			tmp = GetNextToken(".", NULL);
			if (tmp == NULL)
				throw EarlabException("Model: Input stream description format error while processing module %s", mModuleNames[i]);
			strncpy((char *)OutputNumber, tmp, sizeof(OutputNumber));
			Output = strtol(OutputNumber, &tmp, 10);
			if (*tmp != '\0')
				throw EarlabException("Model: Input stream description format error while processing module %s", mModuleNames[i]);
			if ((Output < 1) || (Output > EarlabMaxIOStreamCount))
				throw EarlabException("Model: Module: %s Stream indices must be between 1 and %d, inclusive", mModuleNames[i], EarlabMaxIOStreamCount);
			Output--;
			SourceModule = FindModule(ModuleName);
			if (SourceModule == -1)
				throw EarlabException("Model: Error: Module %s refers to nonexistent module %s as its input source", mModuleNames[i], ModuleName);
			SourceOutputSizes = mModules[SourceModule]->OutputSizes(Output);
			DestInputSizes = mModules[i]->InputSizes(j);
			// Copy dimension sizes for source output stream to destination input stream
			for (int k = 0; k < EarlabMaxIOStreamDimensions; k++)
				DestInputSizes[k] = SourceOutputSizes[k];
			// Now, subscribe the current (destination) module to the output stream of its source module
			mModules[i]->Subscribe(mModules[SourceModule], Output);
			mModules[i]->SetInputCount(j + 1);
			mModules[i]->SetInputType(j, mModules[SourceModule]->GetOutputType(Output));
		}
	}
	mLogger->Log("Model: Modules Connected");
	return 1;
}

int Model::LoadModuleParameters(char *ModuleParameterFileName)
{
	for (int i = 0; i < MaxEarlabModuleCount; i++)
	{
		if (mModules[i] == NULL)
			break;
		mModules[i]->ReadParameters(ModuleParameterFileName);
	}
	return 1;
}

#ifdef USE_PTHREADS
int Model::StartModules(void)
{
	int i;
	struct timespec ts;

	mLogger->Log("Model: Initializing module threads...");

	mModulesWaitingForStart = 0;
	for (i = 0; i < mModuleCount; i++)
	{
		mThreadInfo[i].theModel = this;
		mThreadInfo[i].moduleIndex = i;
		pthread_create(&mModuleThreads[i], NULL, ThreadStub, &(mThreadInfo[i]));
	}
	ts.tv_sec = 0;
	ts.tv_nsec = 10000000; // 10 milliseconds = 10,000,000 nanoseconds
	pthread_mutex_lock(&mModuleCountMutex);
	while (mModulesWaitingForStart != mModuleCount)
	{
		pthread_cond_timedwait(&mModuleBarrierCond, &mModuleCountMutex, &ts);
	}

	//mLogger->Log("Model: Starting modules...");
	pthread_cond_broadcast(&mModuleBarrierCond);
	
	while (mModulesWaitingForStart != mModuleCount)
	{
		pthread_cond_timedwait(&mModuleBarrierCond, &mModuleCountMutex, &ts);
	}
	pthread_mutex_unlock(&mModuleCountMutex);
	
	return 1;
}

void *Model::ThreadStub(void *parameter)
{
	Model *theModel;
	ThreadInfo *theInfo;
	int ModuleIndex;
	
	theInfo = (ThreadInfo *)parameter;
	theModel = theInfo->theModel;
	ModuleIndex = theInfo->moduleIndex;

	return theModel->ThreadProc(ModuleIndex);
}

void *Model::ThreadProc(int ModuleIndex)
{
	EarlabModuleInstance *myModule;
	int CurFrame = 0;

	//printf("Thread %d locking mutex\n", ModuleIndex);
	pthread_mutex_lock(&mModuleCountMutex);
	myModule = mModules[ModuleIndex];
	mModulesWaitingForStart++;
	//printf("Thread %d waiting.  Total modules waiting %d\n", ModuleIndex, mModulesWaitingForStart);
	pthread_cond_wait(&mModuleBarrierCond, &mModuleCountMutex);
	//printf("Thread %d unlocking mutex\n", ModuleIndex);
	pthread_mutex_unlock(&mModuleCountMutex);
	//printf("Thread %d calling Start()\n", ModuleIndex);
	try
	{
		if (myModule->Start() == 0)
		{
			mLogger->Log("Model: Module %s failed on Start", mModuleNames[ModuleIndex]);
			mFailedModule = ModuleIndex;
			pthread_exit(NULL);
		}
	}
	catch (EarlabException e)
	{
		mLogger->Log("Model: Module %s threw an exception: %s\n", mModuleNames[ModuleIndex], e.GetExceptionString());
	}
	mLogger->Log("           %s started.", mModuleNames[ModuleIndex]);
	while (1)
	{
		CurFrame++;
		//printf("Thread %d locking mutex\n", ModuleIndex);
		pthread_mutex_lock(&mModuleCountMutex);
		myModule = mModules[ModuleIndex];
		mModulesWaitingForAdvance++;
		//printf("Thread %d waiting.  Total modules waiting %d\n", ModuleIndex, mModulesWaitingForAdvance);
		pthread_cond_wait(&mModuleBarrierCond, &mModuleCountMutex);
		//printf("Thread %d unlocking mutex\n", ModuleIndex);
		pthread_mutex_unlock(&mModuleCountMutex);
		//printf("Thread %d calling Advance()\n", ModuleIndex);
		//printf("      %s: %d\n", mModuleNames[ModuleIndex], CurFrame);
		if (myModule->Advance() == 0)
		{
			mLogger->Log("Model: Module %s failed on Advance", mModuleNames[ModuleIndex]);
			mFailedModule = ModuleIndex;
			pthread_exit(NULL);
		}
		mLogger->Log("             %s advanced.", mModuleNames[ModuleIndex]);
	}
}
#else
int Model::StartModules(void)
{
	mLogger->Log("Model: Starting modules...");
	for (int i = 0; i < mModuleCount; i++)
	{
		if (mModules[i]->Start() == 0)
			throw EarlabException("Model: Module %s failed on Start", mModuleNames[i]);
	}
	mLogger->Log("Model: Modules Started");
	return 1;
}
#endif

int Model::Run(int FrameCount)
{
	mLogger->Log("Model: Starting simulation...");
	for (mCurFrame = 0; mCurFrame < FrameCount; mCurFrame++)
	{
		mLogger->Log("  Starting Frame %d of %d", mCurFrame + 1, FrameCount);
		for (int i = 0; i < mModuleCount; i++)
			if (mModules[i]->Advance() == 0)
				throw EarlabException("Model: Module %s failed on Advance", mModuleNames[i]);
		mLogger->Log("  Frame complete");
	}
	mLogger->Log("Model: Simulation complete");
	return 1;
}

#ifdef USE_PTHREADS
int Model::AdvanceModules(void)
{
	int i;
	struct timespec ts;
	struct timeval tv;
	double time_start, time_end;

	ts.tv_sec = 0;
	ts.tv_nsec = 10000000; // 10 milliseconds = 10,000,000 nanoseconds
	pthread_mutex_lock(&mModuleCountMutex);
	while (mModulesWaitingForAdvance != mModuleCount)
	{
		pthread_cond_timedwait(&mModuleBarrierCond, &mModuleCountMutex, &ts);
	}

	mModulesWaitingForAdvance = 0;
	mLogger->Log("  Starting Frame %d", mCurFrame + 1);
	gettimeofday(&tv, NULL);
	time_start = tv.tv_sec + (tv.tv_usec / 1000000);
	pthread_cond_broadcast(&mModuleBarrierCond);
	
	while (mModulesWaitingForAdvance != mModuleCount)
	{
		pthread_cond_timedwait(&mModuleBarrierCond, &mModuleCountMutex, &ts);
	}
	pthread_mutex_unlock(&mModuleCountMutex);
	gettimeofday(&tv, NULL);
	time_end = tv.tv_sec + (tv.tv_usec / 1000000);
	
	mLogger->Log("  Frame complete.  Elapsed time: %f sec", time_end - time_start);
	
	mCurFrame++;
	return 1;
}
#else
int Model::AdvanceModules(void)
{
#ifdef _WIN32
	BOOL CounterStatus;
	LARGE_INTEGER frequency, start, end, duration;
	double elapsed, total_elapsed;
#endif

	mLogger->Log("  Starting Frame %d", mCurFrame + 1);
#ifdef _WIN32
	CounterStatus = QueryPerformanceFrequency((LARGE_INTEGER*)(&frequency));
	total_elapsed = 0.0;
#endif	
	for (int i = 0; i < mModuleCount; i++)
	{
#ifdef _WIN32
		CounterStatus = QueryPerformanceCounter((LARGE_INTEGER*)(&start));
#endif 
		if (mModules[i]->Advance() == 0)
			throw EarlabException("Model: Module %s failed on Advance", mModuleNames[i]);
#ifdef _WIN32
		CounterStatus = QueryPerformanceCounter((LARGE_INTEGER*)(&end));
		duration.QuadPart = end.QuadPart - start.QuadPart;
		elapsed = (double)duration.QuadPart / (double)frequency.QuadPart;
		total_elapsed += elapsed;
#endif 
		//mLogger->Log("      Elapsed time: %f sec", elapsed);
	}
#ifdef _WIN32
	mLogger->Log("  Frame complete.  Elapsed time: %f sec", total_elapsed);
#else 
	mLogger->Log("  Frame complete.");
#endif
	mCurFrame++;
	return 1;
}
#endif

int Model::StopModules(void)
{
	mLogger->Log("Model: Stopping modules...");
	for (int i = 0; i < mModuleCount; i++)
		if (mModules[i]->Stop() == 0)
			throw EarlabException("Model: Module %s failed on Stop", mModuleNames[i]);
	mLogger->Log("Model: Modules stopped");
	return 1;
}

int Model::Unload(void)
{
	mLogger->Log("Model: Unloading modules...");
	for (int i = 0; i < mModuleCount; i++)
		if (mModules[i]->Unload() == 0)
			throw EarlabException("Model: Module %s failed on Unload", mModuleNames[i]);
	mLogger->Log("Model: Modules unloaded");
	return 1;
}

void Model::AddModule(char *ModuleName)
{
	char ModuleExecutable[128];
	char ModulePath[256];
	char FullModuleExecutablePath[256];
	ParameterStatus Status;
	int i, j;
	int OutputDimCount;
	char OutputName[50];
	char OutputDataType[50];

	Status = mParamFile.GetParameter(ModuleName, "ModuleExecutable", ModuleExecutable, sizeof(ModuleExecutable), "");
	if (Status != Success)
		throw EarlabException("AddModule: Could not find required string parameter \"ModuleExecutable\" in module section [%s]", ModuleName);
	strncpy((char *)ModulePath, mModuleDirectory, sizeof(ModulePath));

	//if (mModuleDirectory[strlen(mModuleDirectory) - 1] != '\\')

#ifdef _WIN32
	if ((strlen(mModuleDirectory) != 0) && (mModuleDirectory[strlen(mModuleDirectory) - 1] != '\\'))
		strcat(ModulePath, "\\");
#else
	if ((strlen(mModuleDirectory) != 0) && (mModuleDirectory[strlen(mModuleDirectory) - 1] != '/'))
		strcat(ModulePath, "/");
#endif
#ifdef _WIN32
	if (strstr(ModuleExecutable, ".dll") == NULL)
		sprintf(FullModuleExecutablePath, "%s%s.dll", ModulePath, ModuleExecutable);
	else
		sprintf(FullModuleExecutablePath, "%s%s", ModulePath, ModuleExecutable);
#else
	sprintf(FullModuleExecutablePath, "%slib%s.so.1.0", ModulePath, ModuleExecutable);
#endif
	for (i = 0; i < MaxEarlabModuleCount; i++)
		if (mModules[i] == NULL)
			break;
	if (i == MaxEarlabModuleCount)
		throw EarlabException("AddModule: Maximum number of modules (%d) exceeded.  Please edit Earlab.h", MaxEarlabModuleCount);
	mModules[i] = new EarlabModuleInstance(FullModuleExecutablePath, ModuleName);
	mModules[i]->SetLogger(mLogger);
	mModules[i]->SetEnvironment(theEnvironment);
	for (j = 0; j < EarlabMaxIOStreamCount; j++)
	{
		sprintf(OutputName, "Output.%d", j + 1);
		OutputDimCount = EarlabMaxIOStreamCount;
		Status = mParamFile.GetParameter(ModuleName, OutputName, mModules[i]->OutputSizes(j), OutputDimCount, NULL, 0);
		if (Status != Success)
			break;
		else
			mModules[i]->SetOutputCount(j + 1);
		sprintf(OutputName, "Output.%d.DataType", j + 1);
		OutputDimCount = EarlabMaxIOStreamCount;
		Status = mParamFile.GetParameter(ModuleName, OutputName, OutputDataType, sizeof(OutputDataType), NULL);
		if (Status != Success)
			throw EarlabException("AddModule: Missing required parameter \"%s\" for module \"%s\"", OutputName, ModuleName);
		if ((stricmp(OutputDataType, "Spike") == 0) || ((stricmp(OutputDataType, "Spikes") == 0)))
			mModules[i]->SetOutputType(j, SpikeData);
		if ((stricmp(OutputDataType, "Wave") == 0) || ((stricmp(OutputDataType, "Waveform") == 0)))
			mModules[i]->SetOutputType(j, WaveformData);
	}
}

int Model::FindModule(char *ModuleName)
{
	int i;
	for (i = 0; (i < MaxEarlabModuleCount) && (mModuleNames[i] != NULL); i++)
		if (strcmp(mModuleNames[i], ModuleName) == 0)
			return i;
	return -1;
}

char *Model::GetNextToken(char *Delimiters, char *SourceString)
{
	char *tmp;
	static char buf[1024];

	tmp = strtok(SourceString, Delimiters);
	if (tmp == NULL)
		return NULL;
	// Eat leading whitespace
	while (isspace(*tmp))
		tmp++;
	strncpy((char *)buf, tmp, sizeof(buf));
	tmp = &buf[strlen(buf) - 1];
	// Eat trailing whitespace
	while (isspace(*tmp))
		tmp--;
	tmp++;
	*tmp = '\0';
	return buf;
}

