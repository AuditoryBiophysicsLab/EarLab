#include <iostream>
#include "Earlab.h"
#include "CParameterFile.h"
#include "ParameterStatus.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <fcntl.h>
#include "BasilarMembraneLinearGammatone.h"
#include "EarlabException.h"

using namespace std;

BasilarMembraneLinearGammatone::BasilarMembraneLinearGammatone()
{
	memset(this, 0, sizeof(BasilarMembraneLinearGammatone));
	mLogger = new Logger();
	SetModuleName("BasilarMembraneLinearGammatone");
}

BasilarMembraneLinearGammatone::~BasilarMembraneLinearGammatone()
{
	if (mModuleName != NULL)
		delete [] mModuleName;
	if (mWriter != NULL)
		delete mWriter;
}

int BasilarMembraneLinearGammatone::ReadParameters(char *ParameterFileName)
{
	return ReadParameters(ParameterFileName, mModuleName);
}

int BasilarMembraneLinearGammatone::ReadParameters(char *ParameterFileName, char *SectionName)
{
    CParameterFile theParamFile(ParameterFileName);
    ParameterStatus Status;

	mLogger->Log("    ReadParameters: %s \"%s\"", mModuleName, ParameterFileName);

	// Number of channels and Frame Size are passed as parameters to Start, see that function for details
    Status = theParamFile.GetParameter(SectionName, "SampleRate_Hz", mSampleRate_Hz, mSampleRate_Hz);

	// Greenwood frequency placemap parameters
	Status = theParamFile.GetParameter(SectionName, "A", mA, 165);
    Status = theParamFile.GetParameter(SectionName, "Alpha", mAlpha, 2.1);
    Status = theParamFile.GetParameter(SectionName, "K", mK, 1.0);
    Status = theParamFile.GetParameter(SectionName, "CochleaLength_mm", mCochleaLength_mm, 35.0);
    Status = theParamFile.GetParameter(SectionName, "BMStartPercent", mBMStartPercent, 0.0);
    Status = theParamFile.GetParameter(SectionName, "BMEndPercent", mBMEndPercent, 100.0);

	Status = theParamFile.GetParameter(SectionName, "FilterQ", mFilterQ, 4.0);

	Status = theParamFile.GetParameter(SectionName, "ProduceOutputFile", mProduceOutputFile, false);
    Status = theParamFile.GetParameter(SectionName, "BinaryOutput", mBinaryOutput, false);
    Status = theParamFile.GetParameter(SectionName, "CFFileName", mCFFileName, sizeof(mCFFileName), NULL);
	return 1;
}

int BasilarMembraneLinearGammatone::Start(int NumInputs,  EarlabDataStreamType InputTypes[EarlabMaxIOStreamCount],  int InputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions], 
			int NumOutputs, EarlabDataStreamType OutputTypes[EarlabMaxIOStreamCount], int OutputSize[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions],
			unsigned long OutputElementCounts[EarlabMaxIOStreamCount])
{
	// Because this module produces data in a "Frequency-Time" format, the following statements apply:
	// OutputSize[i][0] is the number of samples in a frame (the term "samples" includes data for all frequency channels at one time index)
	// OutputSize[i][1] is the number of frequency channels provided
	// OutputSize[i][2] must be 0 to mark the end of the dimension count

    int i,process_counter;
	double PlacemapStepSize_mm, CurrentPlace_mm, CurrentCF_Hz;
	char OutputFileName[256];

	mLogger->Log("    Start: %s", mModuleName);
	// Perform some validation on my parameters to make sure I can handle the requested input and output streams...
	if (NumInputs != 1)
		throw EarlabException("%s: Currently this module can only handle one input stream.  Sorry!", mModuleName);

	if (NumOutputs != 1)
		throw EarlabException("%s: Currently this module can only handle one output stream.  Sorry!", mModuleName);

	if (InputTypes[0] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform input data streams.  Sorry!", mModuleName);

	if (OutputTypes[0] != WaveformData)
		throw EarlabException("%s: Currently this module can only handle waveform output data streams.  Sorry!", mModuleName);

	if (InputSize[0][0] != OutputSize[0][0])
		throw EarlabException("%s: Input and output frame lengths must be identical.  Sorry!", mModuleName);

	if (InputSize[0][1] != 0)
		throw EarlabException("%s: Input data must be one-dimensional array.  Sorry!", mModuleName);

	if (OutputSize[0][2] != 0)
		throw EarlabException("%s: Output signal must be two-dimensional array.  Sorry!", mModuleName);

	OutputElementCounts[0] = OutputSize[0][0] * OutputSize[0][1];

	mFrameSize_Samples = OutputSize[0][0];
	mNumChannels = OutputSize[0][1];

	/* Convert sample/sec  to sec/sample */ 
    mDeltaT = (double)(1.0 / mSampleRate_Hz);

    process_counter = 0;
/*****************************************************************************/
/*              SET BASILIAR MEMBRANE ARRAYS AND CONSTANTS                   */
/*****************************************************************************/

	mQArray.Init(mNumChannels);
	for (i = 0; i < mNumChannels; i++)
		mQArray.Data(i) = mFilterQ;

	mCFArray.Init(mNumChannels);
	mTauArray.Init(mNumChannels);

	mBMA.Init(mNumChannels);
	mBMB.Init(mNumChannels);
	mCFShift.Init(mNumChannels);
	mShiftIn.Init(mNumChannels);
	mOldIn.Init(mNumChannels);
	mBMFilter.Init(4, mNumChannels);
	mOldBMFilter.Init(4, mNumChannels);
	mHold.Init(mNumChannels);

    for (i = 0; i < mNumChannels; i++)
    {
		for (int j = 0; j < 4; j++)
		{
			mBMFilter.Data(j, i).Real = 0.0;
			mBMFilter.Data(j, i).Imag = 0.0;
			mOldBMFilter.Data(j, i).Real = 0.0;
			mOldBMFilter.Data(j, i).Imag = 0.0;
		}
        mShiftIn.Data(i).Real = 0.0;  mShiftIn.Data(i).Imag = 0.0; 
        mOldIn.Data(i).Real  = 0.0;  mOldIn.Data(i).Imag = 0.0;         
    } 

    mCounter = 0;
	if (mProduceOutputFile)
	{
#ifdef NOTWINDOWS
# define WR_BIN  "w"
# define WR_TEXT "w"
#else
# define WR_BIN  "wb"
# define WR_TEXT "wt"
#endif
		if (mBinaryOutput)
		{
#ifndef NOTWINDOWS
			_fmode = _O_BINARY;
#endif
			mOutputFile = fopen(mCFFileName, WR_BIN);
		}
		else
		{
#ifndef NOTWINDOWS
			_fmode = _O_TEXT;
#endif
			mOutputFile = fopen(mCFFileName, WR_TEXT);
		}

		if (mOutputFile == NULL)
			return 0;
	}
	else
		mOutputFile = NULL;

	mCochleaStartPos_mm = mCochleaLength_mm * (mBMStartPercent / 100);
	mCochleaEndPos_mm =   mCochleaLength_mm * (  mBMEndPercent / 100);
	mCochleaModeledLength_mm = mCochleaEndPos_mm - mCochleaStartPos_mm;
	PlacemapStepSize_mm = mCochleaModeledLength_mm / (double)mNumChannels;

	for (i = 0; i < mNumChannels; i++)
    {
		CurrentPlace_mm = (PlacemapStepSize_mm * i) + mCochleaStartPos_mm;
		CurrentCF_Hz = mA * (pow((double)10.0, (double)(mAlpha * (CurrentPlace_mm / mCochleaLength_mm)) - mK));
		mCFArray.Data(i) = (double)(CurrentCF_Hz * 2.0 * PI);

		if (mOutputFile != NULL)
		{
			if (mBinaryOutput)
				fwrite(&CurrentCF_Hz, sizeof(double), 1, mOutputFile);
			else
				fprintf(mOutputFile, "%g\n", CurrentCF_Hz);
		}
		mTauArray.Data(i) = mQArray.Data(i) / mCFArray.Data(i);
        mBMA.Data(i) = (double)((1.0 / ((mTauArray.Data(i) * 2.0)/mDeltaT + 1)));
        mBMB.Data(i) = (double)(mTauArray.Data(i) * 2.0 / mDeltaT - 1);
        mCFShift.Data(i) = (double)(mCFArray.Data(i) * mDeltaT);
    } 
	if (mOutputFile != NULL)
		fclose(mOutputFile);
	sprintf(OutputFileName, "%s.1", mModuleName);
	mWriter = new BinaryWriter(OutputFileName);
	mWriter->WriteMetadata("File_Type", "Frequency_Time");
	mWriter->WriteMetadata("Frequency_Dimension_Size", mNumChannels);
	mWriter->WriteMetadata("Frame_Length_Samples", mFrameSize_Samples);
	mWriter->WriteMetadata("Sample_Rate_Hz", mSampleRate_Hz);
	mWriter->CloseMetadataFile();

	return 1;
}

int BasilarMembraneLinearGammatone::Advance(EarlabDataStream *InputStream[EarlabMaxIOStreamCount], EarlabDataStream *OutputStream[EarlabMaxIOStreamCount])
{
    int i,j,k,process_counter;
	float *Data;
	unsigned long DataLen;
	FloatMatrixN *Input, *Output;

/*****************************************************************************/
/*               BASILAR MEMBRANE MODEL VARIABLE DECLARATIONS                */
/*****************************************************************************/

    double data,a,b,c,d;

	mLogger->Log("    Advance: %s", mModuleName);

	Input = ((EarlabWaveformStream *)InputStream[0])->GetData();	// Only supporting one output at the present moment
	Output = ((EarlabWaveformStream *)OutputStream[0])->GetData();	// Only supporting one output at the present moment

	if (Input->Rank(0) != mFrameSize_Samples)
		throw EarlabException("%s: Input size mismatch with Start()", mModuleName);
	if ((Output->Rank(0) != mFrameSize_Samples) || (Output->Rank(1) != mNumChannels))
		throw EarlabException("%s: Output size mismatch with Start()", mModuleName);

/***************************************************************************/
/* Note: The following four sections are connected so that a data point    */
/* is passed through all the channels of each filter before the next point */
/* is processed. The counter variable for the channels is [i], the counter */
/* for the time points in the frame is [j], and the counter for the number */
/* of frames is frmcount over the number of frames(numfrm).                */
/*                                                                         */
/*                Loops: [i]  over the number of channels                  */
/*                       FRMSIZ [j] outside over the NUMCH inside          */
/*                       NUMFRM  outside over FRMSIZ inside                */
/***************************************************************************/

/*****************************************************************************/
/*                   START UP THE BASILAR MEMBRANE FILTER                    */
/*****************************************************************************/

    for (j = 0; j < mFrameSize_Samples; j++)
    { // begin processing frame
/***************** RESET shftin COUNTER FOR EACH NEW FRAME ******************/
        mCounter++;

        for (i = 0; i< mNumChannels; i++)
        { // begin looping over channel
/******************** FREQUENCY SHIFT DOWN **********************************/

			//mShiftIn.Data(i) = Input.Data(j) * Complex::eToTheI(mCFShift.Data(i) * mCounter);
            //shftin[i].IMG  = input[j] * sin(-1 * cfshift[i] * counter);
			mShiftIn.Data(i).Imag = (double)(Input->Data(j) * sin(-1 * mCFShift.Data(i) * mCounter));
            //shftin[i].REAL = input[j] * cos(cfshift[i] * counter);
			mShiftIn.Data(i).Real = (double)(Input->Data(j) * cos(mCFShift.Data(i) * mCounter));

/********************** FILTER BANK ***************************************/
#ifdef USE_COMPLEX_CLASS
			mBMFilter.Data(0, i) = mBMA.Data(i) * (mShiftIn.Data(i)     + mOldIn.Data(i) + (mBMB.Data(i) * mOldBMFilter.Data(0, i)));
			mBMFilter.Data(1, i) = mBMA.Data(i) * (mBMFilter.Data(0, i) + mOldIn.Data(i) + (mBMB.Data(i) * mOldBMFilter.Data(1, i)));
#else
			// mBMFilter.Data(0, i).Imag = mBMA.Data(i) * (mShiftIn.Data(i).Imag + mOldIn.Data(i).Imag + mBMB.Data(i) * mOldBMFilter.Data(0, i).Imag);
            //bmflt1[i].IMG           = bma[i]       * (shftin[i].IMG         + oldin[i].IMG        + bmb[i]       * obmflt1[i].IMG);
            mBMFilter.Data(0, i).Imag = mBMA.Data(i) * (mShiftIn.Data(i).Imag + mOldIn.Data(i).Imag + mBMB.Data(i) * mOldBMFilter.Data(0, i).Imag);
            //bmflt1[i].REAL          = bma[i]       * (shftin[i].REAL        + oldin[i].REAL       + bmb[i]       * obmflt1[i].REAL); 
            mBMFilter.Data(0, i).Real = mBMA.Data(i) * (mShiftIn.Data(i).Real + mOldIn.Data(i).Real + mBMB.Data(i) * mOldBMFilter.Data(0, i).Real); 
            //bmflt2[i].IMG           = bma[i]       * (bmflt1[i].IMG             + obmflt1[i].IMG               + bmb[i]       * obmflt2[i].IMG);
            mBMFilter.Data(1, i).Imag = mBMA.Data(i) * (mBMFilter.Data(0, i).Imag + mOldBMFilter.Data(0, i).Imag + mBMB.Data(i) * mOldBMFilter.Data(1, i).Imag);
            //bmflt2[i].REAL          = bma[i]       * (bmflt1[i].REAL            + obmflt1[i].REAL              + bmb[i]       * obmflt2[i].REAL); 
            mBMFilter.Data(1, i).Real = mBMA.Data(i) * (mBMFilter.Data(0, i).Real + mOldBMFilter.Data(0, i).Real + mBMB.Data(i) * mOldBMFilter.Data(1, i).Real); 

            //bmflt3[i].IMG           = bma[i]       * (bmflt2[i].IMG             + obmflt2[i].IMG               + bmb[i]       * obmflt3[i].IMG);
            mBMFilter.Data(2, i).Imag = mBMA.Data(i) * (mBMFilter.Data(1, i).Imag + mOldBMFilter.Data(1, i).Imag + mBMB.Data(i) * mOldBMFilter.Data(2, i).Imag);
            //bmflt3[i].REAL          = bma[i]       * (bmflt2[i].REAL            + obmflt2[i].REAL              + bmb[i]       * obmflt3[i].REAL); 
            mBMFilter.Data(2, i).Real = mBMA.Data(i) * (mBMFilter.Data(1, i).Real + mOldBMFilter.Data(1, i).Real + mBMB.Data(i) * mOldBMFilter.Data(2, i).Real); 

            //bmflt4[i].IMG           = bma[i]       * (bmflt3[i].IMG             + obmflt3[i].IMG               + bmb[i]       * obmflt4[i].IMG);
            mBMFilter.Data(3, i).Imag = mBMA.Data(i) * (mBMFilter.Data(2, i).Imag + mOldBMFilter.Data(2, i).Imag + mBMB.Data(i) * mOldBMFilter.Data(3, i).Imag);
            //bmflt4[i].REAL          = bma[i]       * (bmflt3[i].REAL            + obmflt3[i].REAL              + bmb[i]       * obmflt4[i].REAL); 
            mBMFilter.Data(3, i).Real = mBMA.Data(i) * (mBMFilter.Data(2, i).Real + mOldBMFilter.Data(2, i).Real + mBMB.Data(i) * mOldBMFilter.Data(3, i).Real); 
#endif
/******************** FREQUENCY SHIFT UP **********************************/ 

         // a = bmflt4[i].REAL;                     b = bmflt4[i].IMG;  
            a = mBMFilter.Data(3, i).Real;          b = mBMFilter.Data(3, i).Imag;  
         // c = cos(cfshift[i]       * counter);    d = sin(cfshift[i]       * counter);
            c = cos(mCFShift.Data(i) * mCounter);   d = sin(mCFShift.Data(i) * mCounter);

            mHold.Data(i).Real = (double)((a * c) - (b * d));

/****************** PUT BM OUTPUT INTO IHC ARRAY FOR NEXT STAGE ***********/

            data = (4.0 * mHold.Data(i).Real) * 1e-3;

            Output->Data(j, i) = (float)data;
            process_counter = i + j * mNumChannels;

/******************** STORE OLD VALUES FOR NEXT DATA POINT ***************/

            mOldIn.Data(i) = mShiftIn.Data(i);
			for (k = 0; k < 4; k++)
	            mOldBMFilter.Data(k, i) = mBMFilter.Data(k, i);

/********************** END LOOP OVER EACH FRAME OF DATA **************************/
        } // end looping over channels
/*********************** END LOOP OVER EACH FRAME **********************/   
    }  // end looping over frame
	Data = Output->GetData(&DataLen);
	mWriter->CheckMinMax(Data, DataLen);
	mSampleCount += mFrameSize_Samples;
    return process_counter + 1;
}

int BasilarMembraneLinearGammatone::Stop(void)
{
	mLogger->Log("    Stop: %s", mModuleName);
	mWriter->WriteMetadata("Number_Of_Samples", mSampleCount);
	mWriter->WriteMinMaxMetadata();
	return 1;
}

int BasilarMembraneLinearGammatone::Unload(void)
{
	mLogger->Log("    Unload: %s", mModuleName);
	return 1;
}

void BasilarMembraneLinearGammatone::SetModuleName(char *ModuleName)
{
	if (mModuleName != NULL)
		delete [] mModuleName;

	mModuleName = new char[strlen(ModuleName) + 1];
	strcpy(mModuleName, ModuleName);
}

void BasilarMembraneLinearGammatone::SetLogger(Logger *TheLogger)
{
	if (mLogger != NULL)
		delete mLogger;
	mLogger = TheLogger;
}
