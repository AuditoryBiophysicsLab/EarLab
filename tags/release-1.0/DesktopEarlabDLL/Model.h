#include "MatrixN.h"
#include "Earlab.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include "EarlabModuleInstance.h"
#include "Logger.h"

class Model
{
public:
	Model();
	~Model();

	int SetModuleDirectory(char *ModuleDirectoryPath);
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

	int mModuleCount;
	int mCurFrame;
	char *mModuleNames[MaxEarlabModuleCount];
	EarlabModuleInstance *mModules[MaxEarlabModuleCount];
	char *mInputStreamNames[MaxEarlabModuleCount][EarlabMaxIOStreamCount];
	char mModuleDirectory[128];
	CParameterFile mParamFile;
};
