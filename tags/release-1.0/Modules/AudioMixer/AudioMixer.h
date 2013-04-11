#include "MatrixN.h"
#include "Earlab.h"
#include "FirstOrderHighpass.h"
#include "Logger.h"

class AudioMixer
{
public:
    AudioMixer();
    ~AudioMixer();

    int ReadParameters(char *ParameterFileName);
    int ReadParameters(char *ParameterFileName, char *SectionName);

    int Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions]);
    int Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount]);
    int Stop(void);
    int Unload(void);

	void SetModuleName(char *ModuleName);
	void SetLogger(Logger *TheLogger);

private:
	double InputScaleFactorArray[EarlabMaxIOStreamCount];
	int mNumInputs;
	bool mScaleFactorsSpecified;
	int mNumSamples;

	Logger *mLogger;
	char *mModuleName;
};
