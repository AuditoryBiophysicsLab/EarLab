// SupportClassTester.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <stdio.h>
#include "OxenhamPlackLinearGain.h"
#include "OxenhamPlackNonlinearGain.h"
#include "OxenhamPlackGammatone.h"
#include "SecondOrderButterworthLowpass.h"
#include "FirstOrderHighpass.h"
#include "FirstOrderLowpass.h"
#include "LopezPovedaMeddisDualResonanceNonlinear.h"
#include "math.h"
#include "DynamicVector.h"
#include "Downsampler.h"
#include "Earlab.h"
#include "HHChannels.h"
#include "HHCompartment.h"
#include "RungeKuttaEquation.h"
#include "RungeKuttaSolver.h"
#include "SpikeStream.h"
#include "stdlib.h"
#include "time.h"

using namespace std;

const int SignalLength = 1 << 20;

int TestFilter(double *TestSignal, int SignalLen, double FilterFreq_Hz, double SampleRate_Hz, char *FreqFileName);
void WriteOutput(double *InputSignalBuf, int InputSignalBufLen, char *BaseFileName, char *FreqFileName);
void GenerateImpulseSignal(double *SignalBuf, int SignalBufLen, double ImpulseAmplitude_dB);
void GenerateNoiseSignal(double *SignalBuf, int SignalBufLen, double NoiseAmplitude_dB);

double FilteredSignal[SignalLength];
double TestSignal[SignalLength];

void TestDynamicVector(void);
void TestDownsampler(void);
void TestAutocorrelationGenerator(double SampleRate_Hz, double CenterFrequency_Hz, double HeadSize_uS);
int xcorr(double sig1[], double sig2[], long n, double lag[], long shiftmax);
double compute_lag(double sig1[], double sig2[], long n);
void TestBushyCell(void);
void TestSpikeStream(void);

int main(int argc, char* argv[])
{
	TestSpikeStream();
	return 0;
	TestBushyCell();
	return 0;
	TestAutocorrelationGenerator(100000.0, 2345.6, 700.0);
	return 0;
	TestDownsampler();
	TestDynamicVector();
	GenerateNoiseSignal(TestSignal, SignalLength, 60.0);
	WriteOutput(TestSignal, SignalLength, "NoiseSignal", NULL); 
	TestFilter(TestSignal, SignalLength,    20.0, 100000.0, "00020Hz");
	TestFilter(TestSignal, SignalLength,   100.0, 100000.0, "00100Hz");
	TestFilter(TestSignal, SignalLength,   500.0, 100000.0, "00500Hz");
	TestFilter(TestSignal, SignalLength,  1000.0, 100000.0, "01000Hz");
	TestFilter(TestSignal, SignalLength,  5000.0, 100000.0, "05000Hz");
	TestFilter(TestSignal, SignalLength, 10000.0, 100000.0, "10000Hz");
}

#define SPIKE_CELLS 10
#define NUM_SAMPLES 10
#define SPIKE_COUNT (SPIKE_CELLS * NUM_SAMPLES)
#define SPIKE_SAMPLE_RATE_HZ 100000.0
#define SPIKE_TIME_STEP (2 / SPIKE_SAMPLE_RATE_HZ);
#define SPIKE_PROBABILITY 0.6

void TestSpikeStream(void)
{
	Spike Spikes[SPIKE_COUNT];
	SpikeStream Stream(Spikes, SPIKE_COUNT, false, SPIKE_SAMPLE_RATE_HZ);
	int i, j;
	int CurSpike = 0;
	double CurTime = 0;
	double result;
	int total;
	int odd_cells[SPIKE_CELLS / 2] = {1, 3, 5, 7, 9};
	int even_cells[SPIKE_CELLS / 2] = {0, 2, 4, 6, 8};

	srand((unsigned)time(NULL));
	for (i = 0; i < NUM_SAMPLES; i++)
	{
		total = 0;
		for (j = 0; j < SPIKE_CELLS; j++)
		{
			result = (double)rand() / (double)RAND_MAX;
			if (result < SPIKE_PROBABILITY)
			{
				Stream.Fire(j);
				total++;
			}
		}
		cout << "Total spikes for sample " << i << ": " << total << endl;
		Stream.Step();
	}
	Stream.NewFrame();
	for (i = 0; i < NUM_SAMPLES; i++)
	{
		cout << "Number of spikes in odd  cells in frame " << i << ": " << Stream.CountSpikes(odd_cells, SPIKE_CELLS / 2) << endl;
		cout << "Number of spikes in even cells in frame " << i << ": " << Stream.CountSpikes(even_cells, SPIKE_CELLS / 2) << endl;
		Stream.Step();
	}
}

#define BUSHY_STEPS 10000
double BushyBuf[BUSHY_STEPS];

void TestBushyCell(void)
{
	int i;

	HHCompartment theCompartment(100000.0, -60.0, 38.0);
	HHSynapse Excitatory(100000.0, .0001, 6);

	for (i = 0; i < BUSHY_STEPS; i++)
	{
		if (i == 1000)
			theCompartment.SetCurrentInjection_pA(-130.0);
		if (i == (BUSHY_STEPS - 1000))
			theCompartment.SetCurrentInjection_pA(0.0);

		theCompartment.Step(NULL, 0, NULL, 0);
		BushyBuf[i] = theCompartment.GetVoltage_mV();
	}
	WriteOutput(BushyBuf, BUSHY_STEPS, "BushyTest", NULL);
}

void TestAutocorrelationGenerator(double SampleRate_Hz, double CenterFrequency_Hz, double HeadSize_uS)
{
	int HeadSize_Samples, AutocorrelationBufferLength, i;
	double *AutocorrelationInputBuffer, *AutocorrelationOutputBuffer;
	double CurTime, TimeStep;
	int InputBufferLength_Samples;
	double ScaleFactor;
	FILE *fp;

	// Convert sample rate in hertz to microseconds per sample, then divide head size by that
	HeadSize_Samples = (int)(HeadSize_uS / (1e6 / SampleRate_Hz));
	AutocorrelationBufferLength = (HeadSize_Samples * 2) + 1;
	CurTime = 0.0;
	TimeStep = 1.0 / SampleRate_Hz;
	InputBufferLength_Samples = (int)ceil((1.0 / CenterFrequency_Hz) / TimeStep);
	ScaleFactor = (double)InputBufferLength_Samples / (double)HeadSize_Samples;
	if (ScaleFactor < 1.0)
	{
		ScaleFactor = ceil(1.0 / ScaleFactor);
		InputBufferLength_Samples *= (int)ScaleFactor;
	}
	AutocorrelationInputBuffer = new double[InputBufferLength_Samples];
	AutocorrelationOutputBuffer = new double[AutocorrelationBufferLength];
	for (i = 0; i < InputBufferLength_Samples; i++)
	{
		AutocorrelationInputBuffer[i] = sin(TWOPI * CenterFrequency_Hz * CurTime);
		CurTime += TimeStep;
	}
	xcorr(AutocorrelationInputBuffer, AutocorrelationInputBuffer, InputBufferLength_Samples, 
		AutocorrelationOutputBuffer, HeadSize_Samples);
	fp = fopen("signal.txt", "w");
	if (fp != NULL)
	{
		for (i = 0; i < InputBufferLength_Samples; i++)
			fprintf(fp, "%g\n", AutocorrelationInputBuffer[i]);
		fclose(fp);
	}
	fp = fopen("autocorr.txt", "w");
	if (fp != NULL)
	{
		for (i = 0; i < AutocorrelationBufferLength; i++)
			fprintf(fp, "%g\n", AutocorrelationOutputBuffer[i]);
		fclose(fp);
	}
	delete [] AutocorrelationOutputBuffer;
	delete [] AutocorrelationInputBuffer;
}

int xcorr(double sig1[], double sig2[], long n, double lag[], long shiftmax)
{
    int lagpos = 0;
    int cur_shift;
	double tmp;

    for (cur_shift = shiftmax; cur_shift > 0; cur_shift--)              // Compute sig1 shift lags
	{
		tmp = compute_lag(&sig1[cur_shift], sig2, n - cur_shift);
        lag[lagpos++] = tmp;
	}

	tmp = compute_lag(sig1, sig2, n);									// Compute zero lag
    lag[lagpos++] = tmp;

    for (cur_shift = 1; cur_shift <= shiftmax; cur_shift++)             // Compute sig2 shift lag   
	{
        tmp = compute_lag(sig1, &sig2[cur_shift], n - cur_shift);
        lag[lagpos++] = tmp;
	}

	return lagpos;
}

double compute_lag(double sig1[], double sig2[], long n)
{
    long i;
    double total = 0.0;

    for (i = 0; i < n; i++)
        total += sig1[i] * sig2[i];

    total /= n;

    return total;
}

void TestDownsampler(void)
{
	const int SignalLength = 256;
	const int NumReps = 10000;
	double *TestSignal, r, rmax;
	Downsampler *ds = new Downsampler(0, SignalLength, 100000, 8192, "TestDownsampler");
	int i, j;

	ds->SetMaxLevel(32);
	ds->SetMinimumSampleCountToKeep(400);
	ds->SetType(DownsampleTypeAverage);
	TestSignal = new double[SignalLength];
	rmax = (double)RAND_MAX;

	for (j = 0; j < NumReps; j++)
	{
		for (i = 0; i < SignalLength; i++)
		{
			r = (double)rand();
			TestSignal[i] = r / rmax;
		}
		ds->AddSample(TestSignal);
	}
	delete [] TestSignal;
	delete ds;
}

int TestFilter(double *TestSignal, int SignalLen, double FilterFreq_Hz, double SampleRate_Hz, char *FreqFileName)
{
	OxenhamPlackLinearGain OPLG;
	OxenhamPlackNonlinearGain OPNLG;
	OxenhamPlackGammatone OPG_L, OPG_NL;
	SecondOrderButterworthLowpass SOB_L, SOB_NL;
	LopezPovedaMeddisDualResonanceNonlinear DRNL_Lin, DRNL_Nonlin, DRNL;
	int i;

	OPLG.Init(FilterFreq_Hz);
	for (i = 0; i < SignalLen; i++) FilteredSignal[i] = OPLG.Filter(TestSignal[i]);
	WriteOutput(FilteredSignal, SignalLen, "OxenhamPlackLinearGain", FreqFileName); 

	OPNLG.Init(FilterFreq_Hz);
	for (i = 0; i < SignalLen; i++) FilteredSignal[i] = OPNLG.Filter(TestSignal[i]);
	WriteOutput(FilteredSignal, SignalLen, "OxenhamPlackNonlinearGain", FreqFileName); 

	OPG_L.LinearInit(FilterFreq_Hz, SampleRate_Hz);
	for (i = 0; i < SignalLen; i++) FilteredSignal[i] = OPG_L.Filter(TestSignal[i]);
	WriteOutput(FilteredSignal, SignalLen, "OxenhamPlackLinearGammatone", FreqFileName); 

	OPG_NL.NonlinearInit(FilterFreq_Hz, SampleRate_Hz);
	for (i = 0; i < SignalLen; i++) FilteredSignal[i] = OPG_NL.Filter(TestSignal[i]);
	WriteOutput(FilteredSignal, SignalLen, "OxenhamPlackNonlinearGammatone", FreqFileName); 

	SOB_L.LinearInit(FilterFreq_Hz, SampleRate_Hz);
	for (i = 0; i < SignalLen; i++) FilteredSignal[i] = SOB_L.Filter(TestSignal[i]);
	WriteOutput(FilteredSignal, SignalLen, "SecondOrderButterworthLinear", FreqFileName); 

	SOB_NL.NonlinearInit(FilterFreq_Hz, SampleRate_Hz);
	for (i = 0; i < SignalLen; i++) FilteredSignal[i] = SOB_NL.Filter(TestSignal[i]);
	WriteOutput(FilteredSignal, SignalLen, "SecondOrderButterworthNonlinear", FreqFileName); 

	DRNL_Lin.Init(FilterFreq_Hz, SampleRate_Hz);
	for (i = 0; i < SignalLen; i++) FilteredSignal[i] = DRNL_Lin.LinearCascade(TestSignal[i]);
	WriteOutput(FilteredSignal, SignalLen, "DRNL_Linear", FreqFileName); 

	DRNL_Nonlin.Init(FilterFreq_Hz, SampleRate_Hz);
	for (i = 0; i < SignalLen; i++) FilteredSignal[i] = DRNL_Nonlin.NonlinearCascade(TestSignal[i]);
	WriteOutput(FilteredSignal, SignalLen, "DRNL_Nonlinear", FreqFileName); 

	DRNL.Init(FilterFreq_Hz, SampleRate_Hz);
	for (i = 0; i < SignalLen; i++) FilteredSignal[i] = DRNL.Filter(TestSignal[i]);
	WriteOutput(FilteredSignal, SignalLen, "DRNL", FreqFileName); 

	return 0;
}

void WriteOutput(double *InputSignalBuf, int InputSignalBufLen, char *BaseFileName, char *FreqFileName)
{
	FILE *outfile;
	int i;
	//float FloatSample;
	char RealFileName[256];

	strncpy(RealFileName, BaseFileName, sizeof(RealFileName));
	if (FreqFileName != NULL)
	{
		strcat(RealFileName, "_");
		strcat(RealFileName, FreqFileName);
	}
	strcat(RealFileName, ".txt");
	outfile = fopen(RealFileName, "w");
	if (outfile == NULL)
	{
		cerr << "Error: Unable to create output file \"" << RealFileName << "\"" << endl;
		return;
	}

	cout << "  Creating file \"" << RealFileName << "\"" << endl;

	for (i = 0; i < InputSignalBufLen; i++)
	{
		//FloatSample = (float)InputSignalBuf[i];
		fprintf(outfile, "%g\n", InputSignalBuf[i]);
	}

	fclose(outfile);
}

void GenerateImpulseSignal(double *SignalBuf, int SignalBufLen, double ImpulseAmplitude_dB)
{
	int i, j;

	if (SignalBuf == NULL)
		return;

	if (SignalBufLen <= 0)
		return;

	for (i = 0; i < SignalBufLen / 20; i++)
		SignalBuf[i] = 0.0;

	SignalBuf[i++] = ((20.0 * pow(10.0, ImpulseAmplitude_dB / 20.0)) * 2.0e-9);
	SignalBuf[i++] = ((20.0 * pow(10.0, ImpulseAmplitude_dB / 20.0)) * 2.0e-9);
	SignalBuf[i++] = ((20.0 * pow(10.0, ImpulseAmplitude_dB / 20.0)) * 2.0e-9) * -1.0;
	SignalBuf[i++] = ((20.0 * pow(10.0, ImpulseAmplitude_dB / 20.0)) * 2.0e-9) * -1.0;

	for (j = i; j < SignalBufLen; j++)
		SignalBuf[j] = 0.0;
}

void GenerateNoiseSignal(double *SignalBuf, int SignalBufLen, double NoiseAmplitude_dB)
{
	int i, j, k;
	double r1, rmax, rtotal;
	int NumberToAdd = 50;

	if (SignalBuf == NULL)
		return;

	if (SignalBufLen <= 0)
		return;

	rmax = RAND_MAX;
	for (i = 0; i < SignalBufLen; i++)
	{
		rtotal = 0.0;
		for (k = 0; k < NumberToAdd; k++)
		{
			j = rand();
			r1 = j;
			r1 = ((r1 / rmax) - 0.5) * 2.0;		// Normalize to range -1.0 <--> 1.0
			rtotal += r1;		// Add 'em up.  Use that Central Limit Theorem!
		}
		//SignalBuf[i] = ((20.0 * pow(10.0, NoiseAmplitude_dB / 20.0)) * 2.0e-9 * rtotal);	// Translate into dB re: stapedius velocity
		SignalBuf[i] = 1.0e-9 * rtotal;
	}
}

void TestDynamicVector(void)
{
	const int TestCount = 500;
	DoubleVector *dv = new DoubleVector();
	int i;
	double MyBuf[TestCount];

	cout << "TestDynamicVector: Writing data into empty vector" << endl;
	for (i = 0; i < TestCount; i++)
		dv->Add((double)i);

	cout << "TestDynamicVector: Beginning comparison.  Length: " << dv->Length() << endl;
	for (i = 0; i < TestCount; i++)
	{
		if ((*dv)[i] != (double)i)
			cout << "Discrepancy at index " << i << endl;
	}
	cout << "TestDynamicVector: Comparison complete" << endl;

	cout << "TestDynamicVector: Copying vector to user buffer" << endl;
	dv->CopyData(MyBuf, TestCount);
	cout << "TestDynamicVector: Copy complete.  Beginning comparison" << endl;
	for (i = 0; i < TestCount; i++)
	{
		if (MyBuf[i] != (double)i)
			cout << "Discrepancy at index " << i << endl;
	}
	cout << "TestDynamicVector: Comparison complete" << endl;

	delete dv;

	for (i = 0; i < TestCount; i++)
		MyBuf[i] = TestCount - i;

	cout << "TestDynamicVector: Creating vector from pre-initialized data" << endl;
	dv = new DoubleVector(MyBuf, TestCount);
	cout << "TestDynamicVector: Creation complete.  Beginning comparison" << endl;
	for (i = 0; i < TestCount; i++)
	{
		if ((*dv)[i] != (double)(TestCount - i))
			cout << "Discrepancy at index " << i << endl;
	}
	cout << "TestDynamicVector: Comparison complete" << endl;
	delete dv;
}
