#include "MatrixN.h"
#include "Earlab.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "EarlabModuleInstance.h"
#include "Logger.h"
#include "EarlabEnvironment.h"
#ifdef USE_PTHREADS
#include <pthread.h>
#endif

class Model;

typedef struct
{
	Model *theModel;
	int moduleIndex;
} ThreadInfo;

class Model
{
public:
	Model();
	~Model();

	int SetModuleDirectory(char *ModuleDirectoryPath);
	int SetInputDirectory(char *InputDirectoryPath);
	int SetOutputDirectory(char *OutputDirectoryPath);
	int SetLogger(Logger *TheLogger);
	int Load(char *ModelConfigFileName, float FrameSize_uS);
	int LoadModuleParameters(char *ModuleParameterFileName);
	int StartModules(void);
	int Run(int FrameCount);
	int AdvanceModules(void);
	int StopModules(void);
	int Unload(void);

private:
	Logger *mLogger;
	char *GetNextToken(char *Delimiters, char *SourceString);
	void AddModule(char *ModuleName);
	int FindModule(char *ModuleName);
#ifdef USE_PTHREADS
	static void *ThreadStub(void *parameter);
	void *ThreadProc(int ModuleIndex);

	pthread_t       mModuleThreads[MaxEarlabModuleCount];
	pthread_mutex_t mModuleCountMutex;
	pthread_cond_t  mModuleBarrierCond;
	ThreadInfo		mThreadInfo[MaxEarlabModuleCount];
	int mModulesWaitingForStart;
	int mModulesWaitingForAdvance;
	int mFailedModule;
#endif

	int mModuleCount;
	int mCurFrame;
	char *mModuleNames[MaxEarlabModuleCount];
	EarlabModuleInstance *mModules[MaxEarlabModuleCount];
	char *mInputStreamNames[MaxEarlabModuleCount][EarlabMaxIOStreamCount];
	char mModuleDirectory[256];
	CParameterFile mParamFile;
	EarlabEnvironment *theEnvironment;
};
