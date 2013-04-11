#include "EarlabModuleInstance.h"
#include "EarlabException.h"
#ifndef _WIN32
#include <dlfcn.h>
#endif

typedef MatrixN<float> FloatMatrixN;

EarlabModuleInstance::EarlabModuleInstance()
{
	memset(this, 0, sizeof(EarlabModuleInstance));
}

EarlabModuleInstance::EarlabModuleInstance(const char *ModuleFileName)
{
	memset(this, 0, sizeof(EarlabModuleInstance));
	MapToModule(ModuleFileName);
}

EarlabModuleInstance::EarlabModuleInstance(const char *ModuleFileName, const char *ModuleName)
{
	memset(this, 0, sizeof(EarlabModuleInstance));
	MapToModule(ModuleFileName);
	SetModuleName(ModuleName);
}

EarlabModuleInstance::EarlabModuleInstance(const char *ModuleFileName, const char *ParameterFileName, const char *ModuleName)
{
	memset(this, 0, sizeof(EarlabModuleInstance));
	MapToModule(ModuleFileName);
	SetModuleName(ModuleName);
	ReadParameters(ParameterFileName);
}

void EarlabModuleInstance::LoadModuleFile(const char *ModuleFileName)
{
#ifdef _WIN32
	mModule = LoadLibrary(mModuleFileName);
	if (mModule == NULL)
		throw EarlabException("EarlabModuleInstance: LoadModuleFile failed to load module %s", ModuleFileName);

#else
    mModule = dlopen(ModuleFileName, RTLD_LAZY);
    if (mModule == NULL)
		throw EarlabException("EarlabModuleInstance: LoadModuleFile failed to load module %s: %s", ModuleFileName, dlerror());
#endif
}

void EarlabModuleInstance::FreeModuleFile(void)
{
#ifdef _WIN32
	FreeLibrary(mModule);
#else
#endif
}

void EarlabModuleInstance::MapToModule(const char *ModuleFileName)
{
	if (ModuleFileName == NULL)
		throw EarlabException("EarlabModuleInstance: Unable to initialize with NULL module filename");

  	mModuleFileName = new char [strlen(ModuleFileName) + 1];
	strcpy(mModuleFileName, ModuleFileName);

	LoadModuleFile(ModuleFileName);
	
	mReadParameters	= (EFIReadParameters *)MapAddress("ReadParameters");
	if (mReadParameters == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain a ReadParameters entry point", mModuleFileName);

	mReadParametersEx = (EFIReadParametersEx *)	MapAddress("ReadParametersEx");
	if (mReadParametersEx == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain a ReadParametersEx entry point", mModuleFileName);

	mStart = (EFIStart *)MapAddress("Start");
	if (mStart == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain a Start entry point", mModuleFileName);

	mAdvance = (EFIAdvance *)MapAddress("Advance");
	if (mAdvance == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain an Advance entry point", mModuleFileName);

	mStop = (EFIStop *)MapAddress("Stop");
	if (mStop == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain a Stop entry point", mModuleFileName);

	mUnload = (EFIUnload *)MapAddress("Unload");
	if (mUnload == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain an Unload entry point", mModuleFileName);

	mSetModuleName = (EFISetModuleName *)MapAddress("SetModuleName");
	if (mSetModuleName == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain a SetModuleName entry point", mModuleFileName);

	mSetLogger = (EFISetLogger *)MapAddress("SetLogger");
	if (mSetLogger == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain a SetLogger entry point", mModuleFileName);

	mSetEnvironment = (EFISetEnvironment *)MapAddress("SetEnvironment");
	if (mSetEnvironment == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain a SetEnvironment entry point", mModuleFileName);

	mNewInstance = (EFINewInstance *)MapAddress("NewInstance");
	if (mNewInstance == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain a NewInstance entry point", mModuleFileName);

	mDeleteInstance = (EFIDeleteInstance *)MapAddress("DeleteInstance");
	if (mDeleteInstance == NULL)
		throw EarlabException("EarlabModuleInstance: Module DLL %s does not contain a DeleteInstance entry point", mModuleFileName);

	mInstance = (*mNewInstance)();
}

EarlabModuleInstance::~EarlabModuleInstance()
{
	(*mDeleteInstance)(mInstance);
	if (mModule != NULL)
			FreeModuleFile();
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

int EarlabModuleInstance::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
								int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
								unsigned int OutputElementCounts[EarlabMaxIOStreamCount])
{
	int result;

	if ((mStart != NULL) && (mInstance != NULL))
		result = (*mStart)(mInstance, NumInputs, InputTypes, InputSize, NumOutputs, OutputTypes, OutputSize, OutputElementCounts);
	else
		return 0;

	mInputCount = NumInputs;
	mOutputCount = NumOutputs;
	memcpy(mInputSize, InputSize, sizeof(mInputSize));
	memcpy(mOutputSize, OutputSize, sizeof(mOutputSize));
	memcpy(mInputStreamTypes, InputTypes, sizeof(mInputStreamTypes));
	memcpy(mOutputStreamTypes, OutputTypes, sizeof(mOutputStreamTypes));
	memcpy(mOutputElementCounts, OutputElementCounts, sizeof(mOutputElementCounts));

	CreateOutputStreams();
	return result;
}

void EarlabModuleInstance::CreateOutputStreams(void)
{
	int DimensionCount, CellCount;
	int i, j;

	for (i = 0; i < mOutputCount; i++)
	{
		DimensionCount = 0;
		CellCount = 1;
		for (j = 0; j < EarlabMaxIOStreamDimensions; j++)
		{
			switch (mOutputStreamTypes[i])
			{
			case WaveformData:
				if (mOutputSize[i][j] != 0)
					DimensionCount++;
				break;
			case SpikeData:
				CellCount *= mOutputSize[i][j];
				break;
			}
		}
		for (j = 0; j < 2; j++)
		{
			switch (mOutputStreamTypes[i])
			{
			case WaveformData:
				switch (DimensionCount)
				{
				case 1:
					FrameBuf[i][j] = new EarlabWaveformStream(new FloatMatrixN(mOutputSize[i][0]));
					break;
				case 2:
					FrameBuf[i][j] = new EarlabWaveformStream(new FloatMatrixN(mOutputSize[i][0], mOutputSize[i][1]));
					break;
				case 3:
					FrameBuf[i][j] = new EarlabWaveformStream(new FloatMatrixN(mOutputSize[i][0], mOutputSize[i][1], mOutputSize[i][2]));
					break;
				case 4:
					FrameBuf[i][j] = new EarlabWaveformStream(new FloatMatrixN(mOutputSize[i][0], mOutputSize[i][1], mOutputSize[i][2],
													mOutputSize[i][3]));
					break;
				case 5:
					FrameBuf[i][j] = new EarlabWaveformStream(new FloatMatrixN(mOutputSize[i][0], mOutputSize[i][1], mOutputSize[i][2],
													mOutputSize[i][3], mOutputSize[i][4]));
					break;
				case 6:
					FrameBuf[i][j] = new EarlabWaveformStream(new FloatMatrixN(mOutputSize[i][0], mOutputSize[i][1], mOutputSize[i][2],
													mOutputSize[i][3], mOutputSize[i][4], mOutputSize[i][5]));
					break;
				case 7:
					FrameBuf[i][j] = new EarlabWaveformStream(new FloatMatrixN(mOutputSize[i][0], mOutputSize[i][1], mOutputSize[i][2],
													mOutputSize[i][3], mOutputSize[i][4], mOutputSize[i][5],
													mOutputSize[i][6]));
					break;
				case 8:
					FrameBuf[i][j] = new EarlabWaveformStream(new FloatMatrixN(mOutputSize[i][0], mOutputSize[i][1], mOutputSize[i][2],
													mOutputSize[i][3], mOutputSize[i][4], mOutputSize[i][5],
													mOutputSize[i][6], mOutputSize[i][7]));
					break;
				case 9:
					FrameBuf[i][j] = new EarlabWaveformStream(new FloatMatrixN(mOutputSize[i][0], mOutputSize[i][1], mOutputSize[i][2],
													mOutputSize[i][3], mOutputSize[i][4], mOutputSize[i][5],
													mOutputSize[i][6], mOutputSize[i][7], mOutputSize[i][8]));
					break;
				case 10:
					FrameBuf[i][j] = new EarlabWaveformStream(new FloatMatrixN(mOutputSize[i][0], mOutputSize[i][1], mOutputSize[i][2],
													mOutputSize[i][3], mOutputSize[i][4], mOutputSize[i][5],
													mOutputSize[i][6], mOutputSize[i][7], mOutputSize[i][8],
													mOutputSize[i][9]));
					break;
				default:
					throw EarlabException("EarlabModuleInstance: Invalid number of dimensions (%d) specified for output stream", DimensionCount);
					break;
				}
				break;
			case SpikeData:
				FrameBuf[i][j] = new EarlabSpikeStream(new SpikeStream(new Spike[mOutputElementCounts[i]], mOutputElementCounts[i], false)); 
				break;
			}
		}
	}
}
int EarlabModuleInstance::Advance(EarlabDataStream *Input[EarlabMaxIOStreamCount], EarlabDataStream *Output[EarlabMaxIOStreamCount])
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
	EarlabDataStream  *Input[EarlabMaxIOStreamCount];
	EarlabDataStream *Output[EarlabMaxIOStreamCount];


	// Initialize all the stream pointers in the arrays to NULL for EOF detection
	memset(Input, 0, sizeof(Input));
	memset(Output, 0, sizeof(Input));

	// Swap source and destination frame indices on every other loop
	mDstFrame = (mSrcFrame + 1) & 0x01;

	for (i = 0; i < mInputCount; i++)
	{
		Input[i] = mInput[i]->FrameBuf[mInputNum[i]][mDstFrame];
		mInputStreamTypes[i] = Input[i]->GetStreamType();
	}

	for (i = 0; i < mOutputCount; i++)
	{
		Output[i] = FrameBuf[i][mSrcFrame];
		if (Output[i]->GetStreamType() == SpikeData)
			((EarlabSpikeStream *)Output[i])->GetData()->Clear();
		mOutputStreamTypes[i] = Output[i]->GetStreamType();
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
		result = (*mStart)(mInstance, mInputCount, mInputStreamTypes, mInputSize, mOutputCount, mOutputStreamTypes, mOutputSize, mOutputElementCounts);
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

void EarlabModuleInstance::SetEnvironment(EarlabEnvironment *theEnvironment)
{
	if ((mSetEnvironment != NULL) && (mInstance != NULL))
		(*mSetEnvironment)(mInstance, theEnvironment);
}

void *EarlabModuleInstance::MapAddress(char *FunctionName)
{
	if (mModule == NULL)
		return NULL;
	else
#ifdef _WIN32
		return GetProcAddress(mModule, FunctionName);
#else
		return dlsym(mModule, FunctionName);
#endif
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
		throw EarlabException("EarlabModuleInstance: Attempted to set output stream count to illegal value: %d", NewOutputCount);
	mOutputCount = NewOutputCount;
}

void EarlabModuleInstance::SetOutputType(int OutputNumber, EarlabDataStreamType StreamType)
{
	if ((OutputNumber < 0) || (OutputNumber > EarlabMaxIOStreamCount))
		throw EarlabException("EarlabModuleInstance: Attempted to set output stream type of illegal stream number: %d", OutputNumber);
	mOutputStreamTypes[OutputNumber] = StreamType;
}

void EarlabModuleInstance::SetInputType(int InputNumber, EarlabDataStreamType StreamType)
{
	if ((InputNumber < 0) || (InputNumber > EarlabMaxIOStreamCount))
		throw EarlabException("EarlabModuleInstance: Attempted to set input stream type of illegal stream number: %d", InputNumber);
	mInputStreamTypes[InputNumber] = StreamType;
}

EarlabDataStreamType EarlabModuleInstance::GetOutputType(int OutputNumber)
{
	if ((OutputNumber < 0) || (OutputNumber > EarlabMaxIOStreamCount))
		throw EarlabException("EarlabModuleInstance: Attempted to set output stream type of illegal stream number: %d", OutputNumber);
	return mOutputStreamTypes[OutputNumber];
}

EarlabDataStreamType EarlabModuleInstance::GetInputType(int InputNumber)
{
	if ((InputNumber < 0) || (InputNumber > EarlabMaxIOStreamCount))
		throw EarlabException("EarlabModuleInstance: Attempted to set input stream type of illegal stream number: %d", InputNumber);
	return mInputStreamTypes[InputNumber];
}
