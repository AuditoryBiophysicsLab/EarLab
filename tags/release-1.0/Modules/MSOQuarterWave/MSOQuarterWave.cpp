#include <iostream>
#include "Earlab.h"
#include "MSOQuarterWave.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include "EarlabException.h"

using namespace std;

void Q_wave(double left[], double right[], long n, double lag[], long shiftmax);
double compute_lag(double left[], double right[], long n);

MSOQuarterWave::MSOQuarterWave()
{
    memset(this, 0, sizeof(MSOQuarterWave));
	mLogger = new Logger();
    SetModuleName("MSOQuarterWave");
}

MSOQuarterWave::~MSOQuarterWave()
{
    if (mModuleName != NULL)
        delete [] mModuleName;
    if (mSampLeft != NULL)
        delete [] mSampLeft;
    if (mSampRight != NULL)
        delete [] mSampRight;
    if (mLag != NULL)
        delete [] mLag;
    if (mCFArray != NULL)
		delete [] mCFArray;
    if (mQLags != NULL)
		delete [] mQLags;
    if (mScale != NULL)
		delete [] mScale;
}

int MSOQuarterWave::ReadParameters(char *ParameterFileName)
{
    return ReadParameters(ParameterFileName, mModuleName);
}

int MSOQuarterWave::ReadParameters(char *ParameterFileName, char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

    cout << "    ReadParameters: " << mModuleName << endl;

    // Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, mSampleRate_Hz);
    Status = theParamFile.GetParameter(SectionName, "MaxLag_us", mMaxLag_us, mMaxLag_us);
	Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), NULL);
    return 1;
}

int MSOQuarterWave::Start(int NumInputs, int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], int NumOutputs, int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions])
{
	double m1,m2,b1,b2,xo,sxo;
	FILE *CFFile;
	float tmp;
	// Because this module produces data in a "Frequency-Time" format, the following statements apply:
    // OutputSize[i][0] is the number of samples in a frame (the term "samples" includes data for all frequency channels at one time index)
    // OutputSize[i][1] is the number of frequency channels provided
    // OutputSize[i][2] must be 0 to mark the end of the dimension count

    cout << "    Start: " << mModuleName << endl;
    // Perform some validation on my parameters to make sure I can handle the requested input and output streams...
    if (NumInputs != 2)
        throw EarlabException("%s: Currently this module can only handle two input streams.  Sorry!", mModuleName);

    if (NumOutputs != 1)
        throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", mModuleName);

    if (InputSize[0][0] != InputSize[1][0])
        throw EarlabException("%s: Input frame lengths must be identical.  Sorry!", mModuleName);

    if (InputSize[0][1] != InputSize[1][1])
        throw EarlabException("%s: Input channel counts must be identical.  Sorry!", mModuleName);

    if (InputSize[0][1] != OutputSize[0][1])
        throw EarlabException("%s: Input and output channel counts must be identical.  Sorry!", mModuleName);

    if (InputSize[0][2] != 0)
        throw EarlabException("%s: Input data must be two-dimensional array.  Sorry!", mModuleName);

    if (OutputSize[0][2] != 0)
        throw EarlabException("%s: Output signal must be two-dimensional array.  Sorry!", mModuleName);

    mFrameSize_Samples = InputSize[0][0];
    mNumChannels = InputSize[0][1];
    mNumDelays_Samples = OutputSize[0][1];
    mMaxLag_Samples = (int)((mMaxLag_us * mSampleRate_Hz) / 1000000);
    mSampLeft = new double[mFrameSize_Samples + 1];
    mSampRight = new double[mFrameSize_Samples + 1];
    mLag = new double[mNumDelays_Samples + 1];

	mFrameBuf[0].Init(InputSize[0][0], InputSize[0][1]);
	mFrameBuf[1].Init(InputSize[1][0], InputSize[1][1]);
	for (int i = 0; i < InputSize[0][0]; i++)
		for (int j = 0; j < InputSize[0][1]; j++)
		{
			mFrameBuf[0].Data(i, j) = 0.0;
			mFrameBuf[1].Data(i, j) = 0.0;
		}

	m1 = 0.2300;								// Scale factor variables, estimated 4/22/03
    m2 = 0.0005;
    b1 = 0.0951;
    b2 = -0.2822;
    xo = 375;
    sxo = 10;

	mCFArray = new double[mNumChannels];
	mQLags = new int[mNumChannels];
	mScale = new double[mNumChannels];

    if (mMaxLag_Samples > mNumDelays_Samples)
        mMaxLag_Samples = mNumDelays_Samples;

	CFFile = fopen(mCFFileName, "r");
	if (CFFile == NULL)
		return 0;

	for(int i = 0; i < mNumChannels; i++)
	{
		fscanf(CFFile,"%f", &tmp);
		mCFArray[i] = tmp;
		mQLags[i]= (int)((0.25 / mCFArray[i]) * mSampleRate_Hz);
		mScale[i]=(m1 * log(mCFArray[i]) + b1) * (1 / (1 + exp((mCFArray[i] - xo) / sxo))) + pow(10.0, (m2 * mCFArray[i] + b2)) * (1 / (1 + exp((xo - mCFArray[i]) / sxo)));
	}

	fclose(CFFile);
    return 1;
}
//#define NEW
#ifdef NEW
// Note: We assume here that Input 0 is Ipsilateral and Input 1 is Contralateral
int MSOXcorr::Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount])
{
	int i, j, k, NumLags, CurIndex;
	double TestThreshold = 37;
	int OutputLagNumber = 0;

    cout << "    Advance: " << mModuleName << endl;

	if ((Input[0]->Rank(0) != mFrameSize_Samples) || (Input[0]->Rank(1) != mNumChannels) ||
		(Input[1]->Rank(0) != mFrameSize_Samples) || (Input[1]->Rank(1) != mNumChannels))
		throw EarlabException("%s: Input size mismatch with Start(): %d %d %d %d %d %d", mModuleName, Input[0]->Rank(0), Input[0]->Rank(1), Input[1]->Rank(0), Input[1]->Rank(1), mFrameSize_Samples, mNumChannels);
	if ((Output[0]->Rank(0) != mFrameSize_Samples) || (Output[0]->Rank(1) != mNumChannels))
		throw EarlabException("%s: Output size mismatch with Start()", mModuleName);

	//Input[0]->EnableBoundsChecking(true);
	//Input[1]->EnableBoundsChecking(true);
	//mFrameBuf[0].EnableBoundsChecking(true);
	//mFrameBuf[1].EnableBoundsChecking(true);
	//Output[0]->EnableBoundsChecking(true);
	for (j = 0; j < mFrameSize_Samples; j += mDownsampleStepCount)
	{
		for (i = 0; i < mNumChannels; i++)
		{
			cout << "Processing channel " << i + 1 << " of " << mNumChannels << "\r";
			for (k = 0; k < mFrameSize_Samples; k++)
			{
				CurIndex = j + k + 1;
				// When we're checking lags from current frame's data
				if (CurIndex < mFrameSize_Samples)
				{
					//cout << "  Copied past sample " << CurIndex << " into correlation buffer index " << k << endl;
					mSampLeft[k] = mFrameBuf[0].Data(CurIndex, i);
					mSampRight[k] = mFrameBuf[1].Data(CurIndex, i);
				}
				else
				{
					//cout << "  Copied present sample " << CurIndex << " into correlation buffer index " << k << endl;
					CurIndex -= mFrameSize_Samples;
					mSampLeft[k] = Input[0]->Data(CurIndex, i);
					mSampRight[k] = Input[1]->Data(CurIndex, i);
				}
			}
			NumLags = xcorr2(mSampRight, mSampLeft, mFrameSize_Samples, mLag, mMaxLag_Samples);
			//cout << "  xcorr2 returned " << NumLags << " lags" << endl;
			for (k = 0; k < NumLags; k++)
			{
				Output[0]->Data(OutputLagNumber, i, k) = mLag[k] / (37.5 * 37.5); // the 37.5*37.5 term is to normalize by the square of the spont rate
				//fprintf(mFP, "%g\n", mLag[k] / (37.5 * 37.5));
			}
		}
		//cout << "  Copied lag data into output buffer index [" << OutputSampleNumber << ", " << i << "]" << endl;
		OutputLagNumber++;
	}
	for (i = 0; i < mFrameSize_Samples; i++)
	{
		for (j = 0; j < mNumChannels; j++)
		{
			mFrameBuf[0].Data(i, j) = Input[0]->Data(i, j);
			mFrameBuf[1].Data(i, j) = Input[1]->Data(i, j);
		}
	}
	//cout << "  Copied current input buffers to past sample buffer" << endl;
	cout << "                                                                              \r";
    return 1;
}
#else
// Note: We assume here that Input 0 is Left and Input 1 is Right
int MSOQuarterWave::Advance(DoubleMatrixN *Input[EarlabMaxIOStreamCount], DoubleMatrixN *Output[EarlabMaxIOStreamCount])
{
	int i, j;

    cout << "    Advance: " << mModuleName << endl;
	if ((Input[0]->Rank(0) != mFrameSize_Samples) || (Input[0]->Rank(1) != mNumChannels) ||
		(Input[1]->Rank(0) != mFrameSize_Samples) || (Input[1]->Rank(1) != mNumChannels))
		throw EarlabException("%s: Input size mismatch with Start()", mModuleName);
	if ((Output[0]->Rank(0) != mNumDelays_Samples) || (Output[0]->Rank(1) != mNumChannels))
		throw EarlabException("%s: Output size mismatch with Start()", mModuleName);

	for (i = 0; i < mNumChannels; i++)
    {
        for (j = 0; j < mFrameSize_Samples; j++)
        {
			// CVS Test
            mSampLeft[j] = Input[0]->Data(j, i);
            mSampRight[j] = Input[1]->Data(j, i);
        }

		Q_wave(mSampRight, mSampLeft, mFrameSize_Samples, mLag, mQLags[i]);
		Output[0]->Data(0, i) = (1 / mCFArray[i]) * (mLag[2] / (2 * PI));
		//Output[0].Data(0, i) = (1 / mCFArray[i]) * mLag[2];
    }
    return mNumChannels;
}
#endif

int MSOQuarterWave::Stop(void)
{
    cout << "    Stop: " << mModuleName << endl;
    return 1;
}

int MSOQuarterWave::Unload(void)
{
    cout << "    Unload: " << mModuleName << endl;
    return 1;
}

void MSOQuarterWave::SetModuleName(char *ModuleName)
{
    if (mModuleName != NULL)
        delete [] mModuleName;

    mModuleName = new char[strlen(ModuleName) + 1];
    strcpy(mModuleName, ModuleName);
}

void MSOQuarterWave::SetLogger(Logger *TheLogger)
{
	if (mLogger != NULL)
		delete mLogger;
	mLogger = TheLogger;
}

void Q_wave(double left[], double right[], long n, double lag[], long shiftmax)
{
	int lagpos = 0;
	int cur_shift;

	// cur_shift = shiftmax-10;           	// Compute left shift lags
	//lag[lagpos++] = compute_lag(&left[cur_shift], right, n - cur_shift); 
	cur_shift = shiftmax;           	// Compute left shift lags
	lag[lagpos++] = compute_lag(&left[cur_shift], right, n - cur_shift);  // Right leading shift
	//cur_shift = shiftmax+10;           	// Compute left shift lags
	//lag[lagpos++] = compute_lag(&left[cur_shift], right, n - cur_shift); 

	//cur_shift = shiftmax+10;		
	//lag[lagpos++] = compute_lag(left, &right[cur_shift], n - cur_shift); // Compute right shift lag
	cur_shift = shiftmax;		
	lag[lagpos++] = compute_lag(left, &right[cur_shift], n - cur_shift); // Left leading shift
	//cur_shift = shiftmax-10;		
	//lag[lagpos++] = compute_lag(left, &right[cur_shift], n - cur_shift); // Compute right shift lag

    lag[2] = (lag[0]-lag[1])/(0.5*(lag[0]+lag[1]));
	//lag[8]  =(lag[0]+lag[1]+lag[2])/3;
	//lag[9]  =(lag[3]+lag[4]+lag[5])/3;
    //lag[10] = (lag[8]-lag[9])/(0.5*(lag[8]+lag[9]));
}

double compute_lag(double left[], double right[], long n)
{
    long i;
    double total = 0.0;

    for (i = 0; i < n; i++)
    {
        total += left[i] * right[i];
    }

    total /= n;

    return total;
}
