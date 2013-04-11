#include "EarlabModuleInstance.h"
#include "EarlabException.h"

EarlabModuleInstance::EarlabModuleInstance()
{
	memset(this, 0, sizeof(EarlabModuleInstance));
}

EarlabModuleInstance::EarlabModuleInstance(const char *ModuleDLLFileName)
{
	memset(this, 0, sizeof(EarlabModuleInstance));
	MapToDLL(ModuleDLLFileName);
}

EarlabModuleInstance::EarlabModuleInstance(const char *ModuleDLLFileName, const char *ModuleName)
{
	memset(this, 0, sizeof(EarlabModuleInstance));
	MapToDLL(ModuleDLLFileName);
	SetModuleName(ModuleName);
}

EarlabModuleInstance::EarlabModuleInstance(const char *ModuleDLLFileName, const char *ParameterFileName, const char *ModuleName)
{
	memset(this, 0, sizeof(EarlabModuleInstance));
	MapToDLL(ModuleDLLFileName);
	SetModuleName(ModuleName);
	ReadParameters(ParameterFileName);
}

void EarlabModuleInstance::MapToDLL(const char *ModuleDLLFileName)
{
	if (ModuleDLLFileName == NULL)
		throw EarlabException("EarlabModuleInstance: Unable to initialize with NULL DLL filename");

  	mModuleDLLFileName = new char [strlen(ModuleDLLFileName) + 1];
	strcpy(mModuleDLLFileName, ModuleDLLFileName);

	mModule = LoadLibrary(mModuleDLLFileName);
	if (mModule == NULL)
		throw EarlabException("EarlabModuleInstance: LoadLibrary failed to load DLL: %s", mModuleDLLFileName);

	mReadParameters	= (EFIReadParameters *)MapAddress("ReadParameters");
	if (mReadParameters == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain a ReadParameters entry point", mModuleDLLFileName);

	mReadParametersEx = (EFIReadParametersEx *)	MapAddress("ReadParametersEx");
	if (mReadParametersEx == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain a ReadParametersEx entry point", mModuleDLLFileName);

	mStart = (EFIStart *)MapAddress("Start");
	if (mStart == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain a Start entry point", mModuleDLLFileName);

	mAdvance = (EFIAdvance *)MapAddress("Advance");
	if (mAdvance == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain an Advance entry point", mModuleDLLFileName);

	mStop = (EFIStop *)MapAddress("Stop");
	if (mStop == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain a Stop entry point", mModuleDLLFileName);

	mUnload = (EFIUnload *)MapAddress("Unload");
	if (mUnload == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain an Unload entry point", mModuleDLLFileName);

	mSetModuleName = (EFISetModuleName *)MapAddress("SetModuleName");
	if (mSetModuleName == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain a SetModuleName entry point", mModuleDLLFileName);

	mSetLogger = (EFISetLogger *)MapAddress("SetLogger");
	if (mSetLogger == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain a SetLogger entry point", mModuleDLLFileName);

	mNewInstance = (EFINewInstance *)MapAddress("NewInstance");
	if (mNewInstance == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain a NewInstance entry point", mModuleDLLFileName);

	mDeleteInstance = (EFIDeleteInstance *)MapAddress("DeleteInstance");
	if (mDeleteInstance == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain a DeleteInstance entry point", mModuleDLLFileName);

	mInstance = (*mNewInstance)();
}

EarlabModuleInstance::~EarlabModuleInstance()
{
	(*mDeleteInstance)(mInstance);
	if (mModule != NULL)
		FreeLibrary(mModule);
	if (mModuleName != NULL)
		delete [] mModuleName;
}

int EarlabModuleInstance::ReadParameters(const char *ParameterFileName)
{
	if ((mReadParameters != NULL) && (mInstance != NULL))
		return (*mReadParameters)(mInstance, ParameterFileName);
	else
		return 0;
}

int EarlabModuleInstance::ReadParametersEx(const char *ParameterFileName, const char *SectionName)
{
	if ((mReadParametersEx != NULL) && (mInstance != NULL))
		return (*mReadParametersEx)(mInstance, ParameterFileName, SectionName);
	else
		return 0;
}

int EarlabModuleInstance::Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions])
{
	int result;

	if ((mStart != NULL) && (mInstance != NULL))
		result = (*mStart)(mInstance, NumInputs, InputSize, NumOutputs, OutputSize);
	else
		return 0;

	mInputCount = NumInputs;
	mOutputCount = NumOutputs;
	memcpy(mInputSize, InputSize, sizeof(mInputSize));
	memcpy(mOutputSize, OutputSize, sizeof(mOutputSize));

	CreateOutputStreams();
	return result;
}

void EarlabModuleInstance::CreateOutputStreams(void)
{
	int DimensionCount;
	int i, j;

	for (i = 0; i < mOutputCount; i++)
	{
		DimensionCount = 0;
		for (j = 0; i < EarlabMaxIOStreamDimensions; j++)
		{
			if (mOutputSize[i][j] != 0)
				DimensionCount++;
			else
				break;
		}
		for (j = 0; j < 2; j++)
		{
			switch (DimensionCount)
			{
			case 1:
				FrameBuf[i][j] = new DoubleMatrixN(mOutputSize[i][0]);
				break;
			case 2:
				FrameBuf[i][j] = new DoubleMatrixN(mOutputSize[i][0], mOutputSize[i][1]);
				break;
			case 3:
				FrameBuf[i][j] = new DoubleMatrixN(mOutputSize[i][0], mOutputSize[i][1], mOutputSize[i][2]);
				break;
			case 4:
				FrameBuf[i][j] = new DoubleMatrixN(mOutputSize[i][0], mOutputSize[i][1], mOutputSize[i][2],
												mOutputSize[i][3]);
				break;
			case 5:
				FrameBuf[i][j] = new DoubleMatrixN(mOutputSize[i][0], mOutputSize[i][1], mOutputSize[i][2],
												mOutputSize[i][3], mOutputSize[i][4]);
				break;
			case 6:
				FrameBuf[i][j] = new DoubleMatrixN(mOutputSize[i][0], mOutputSize[i][1], mOutputSize[i][2],
												mOutputSize[i][3], mOutputSize[i][4], mOutputSize[i][5]);
				break;
			case 7:
				FrameBuf[i][j] = new DoubleMatrixN(mOutputSize[i][0], mOutputSize[i][1], mOutputSize[i][2],
												mOutputSize[i][3], mOutputSize[i][4], mOutputSize[i][5],
												mOutputSize[i][6]);
				break;
			case 8:
				FrameBuf[i][j] = new DoubleMatrixN(mOutputSize[i][0], mOutputSize[i][1], mOutputSize[i][2],
												mOutputSize[i][3], mOutputSize[i][4], mOutputSize[i][5],
												mOutputSize[i][6], mOutputSize[i][7]);
				break;
			case 9:
				FrameBuf[i][j] = new DoubleMatrixN(mOutputSize[i][0], mOutputSize[i][1], mOutputSize[i][2],
												mOutputSize[i][3], mOutputSize[i][4], mOutputSize[i][5],
												mOutputSize[i][6], mOutputSize[i][7], mOutputSize[i][8]);
				break;
			case 10:
				FrameBuf[i][j] = new DoubleMatrixN(mOutputSize[i][0], mOutputSize[i][1], mOutputSize[i][2],
												mOutputSize[i][3], mOutputSize[i][4], mOutputSize[i][5],
												mOutputSize[i][6], mOutputSize[i][7], mOutputSize[i][8],
												mOutputSize[i][9]);
				break;
			default:
				throw EarlabException("EarlabModuleInstance: Invalid number of dimensions (%d) specified for output stream", DimensionCount);
				break;
			}
		}
	}
}
int EarlabModuleInstance::Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount])
{
	if ((mAdvance != NULL) && (mInstance != NULL))
		return (*mAdvance)(mInstance, Input, Output);
	else
		return 0;
}

int EarlabModuleInstance::Stop(void)
{
	if ((mStop != NULL) && (mInstance != NULL))
		return (*mStop)(mInstance);
	else
		return 0;
}

int EarlabModuleInstance::Unload(void)
{
	if ((mUnload != NULL) && (mInstance != NULL))
		return (*mUnload)(mInstance);
	else
		return 0;
}

int  EarlabModuleInstance::Advance(void)
{
	int result = 0;
	int i;
	DoubleMatrixN  *Input[EarlabMaxIOStreamCount];
	DoubleMatrixN *Output[EarlabMaxIOStreamCount];

	memset(Input, 0, sizeof(Input));
	memset(Output, 0, sizeof(Input));
	// Swap source and destination frame indices on every other loop
	mDstFrame = (mSrcFrame + 1) & 0x01;

	for (i = 0; i < mInputCount; i++)
	{
		Input[i] = mInput[i]->FrameBuf[mInputNum[i]][mDstFrame];
	}

	for (i = 0; i < mOutputCount; i++)
	{
		Output[i] = FrameBuf[i][mSrcFrame];
	}

	if ((mAdvance != NULL) && (mInstance != NULL))
		result = (*mAdvance)(mInstance, Input, Output);
	else
		return 0;

	// Swap source and destination frame indices on every other loop
	mSrcFrame = (mSrcFrame + 1) & 0x01;

	return result;
}

int EarlabModuleInstance::Start(void)
{
	int result;

	if ((mStart != NULL) && (mInstance != NULL))
		result = (*mStart)(mInstance, mInputCount, mInputSize, mOutputCount, mOutputSize);
	else
		return 0;

	CreateOutputStreams();
	return result;
}

void EarlabModuleInstance::Subscribe(EarlabModuleInstance *Source, int OutputNumber)
{
	int i;

	for (i = 0; i < EarlabMaxIOStreamCount; i++)
	{
		if (mInput[i] == NULL)
			break;
	}
	if (i == EarlabMaxIOStreamCount)
		throw EarlabException("EarlabModuleInstance: Maximum number of inputs exceeded");

	if (Source->FrameBuf[OutputNumber] == NULL)
		throw EarlabException("EarlabModuleInstance: Attempted to subscribe to nonexistent output stream");

	mInput[i] = Source;
	mInputNum[i] = OutputNumber;
}

void EarlabModuleInstance::SetModuleName(const char *ModuleName)
{
	if (mModuleName != NULL)
		delete [] mModuleName;

	mModuleName = new char[strlen(ModuleName) + 1];
	strcpy(mModuleName, ModuleName);

	if ((mSetModuleName != NULL) && (mInstance != NULL))
		(*mSetModuleName)(mInstance, mModuleName);
}

void EarlabModuleInstance::SetLogger(Logger *TheLogger)
{
	if ((mSetLogger != NULL) && (mInstance != NULL))
		(*mSetLogger)(mInstance, TheLogger);
}

void *EarlabModuleInstance::MapAddress(char *FunctionName)
{
	if (mModule == NULL)
		return NULL;
	else
		return GetProcAddress(mModule, FunctionName);
}

int *EarlabModuleInstance::InputSizes(int InputNum)
{
	if ((InputNum < 0) || (InputNum >= EarlabMaxIOStreamCount))
		throw EarlabException("EarlabModuleInstance: Attempted to retrieve size of illegal input stream number");
	else
		return mInputSize[InputNum];
}

int *EarlabModuleInstance::OutputSizes(int OutputNum)
{
	if ((OutputNum < 0) || (OutputNum >= EarlabMaxIOStreamCount))
		throw EarlabException("EarlabModuleInstance: Attempted to retrieve size of illegal output stream number");
	else
		return mOutputSize[OutputNum];
}

void EarlabModuleInstance::SetInputCount(int NewInputCount)
{
	if ((NewInputCount < 0) || (NewInputCount > EarlabMaxIOStreamCount))
		throw EarlabException("EarlabModuleInstance: Attempted to set input stream count to illegal value: %d", NewInputCount);
	mInputCount = NewInputCount;
}

void EarlabModuleInstance::SetOutputCount(int NewOutputCount)
{
	if ((NewOutputCount < 0) || (NewOutputCount > EarlabMaxIOStreamCount))
		throw EarlabException("EarlabModuleInstance: Attempted to set out stream count to illegal value: %d", NewOutputCount);
	mOutputCount = NewOutputCount;
}
