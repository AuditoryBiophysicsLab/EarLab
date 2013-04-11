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
#include "AGCAdvBionics.h"
#include "math.h"
//#include "DynamicVector.h"
#include "Downsampler.h"
#include "Earlab.h"
#include "HHChannelPopulation.h"
#include "MGCurrentBranch.h"
#include "Rothman93Compartment.h"
#include "Rothman93AHPCompartment.h"
#include "Brughera96Compartment.h"
#include "Cai98Compartment.h"
#include "Rothman03Compartment.h"
#include "Rothman03AHPCompartment.h"
#include "MGCompartment.h"
#include "ZhouLSOCompartment.h"
#include "RungeKuttaEquation.h"
#include "RungeKuttaSolver.h"
#include "EarlabDataStream.h"
#include "SpikeStream.h"
#include "stdlib.h"
#include "time.h"
#include "XmlMetadata.h"
#include "Random.h"
#include "FiniteImpulseResponse.h"

using namespace std;

void TestRandom(void);

void TestXmlMetadata(void);

//const int SignalLength = 1 << 20;
const int SignalLength = 10000;
//const EnumedResponseType ResponseTypeValue = TYPEZHOU2006;
const EnumedResponseType ResponseTypeValue = TYPE2;
const Enumed93ResponseType Response93TypeValue = TYPE_II;

int TestFilter(double *TestSignal, int SignalLen, double FilterFreq_Hz, double SampleRate_Hz, char *FreqFileName);
int TestAGC(double *TestSignal, int SignalLen, double EnvelopeCornerFrequency_Hz, double SampleRate_Hz, double AttackTau_sec, double ReleaseTau_sec, double Compression_dBRatio, char *FreqFileName);
// additional parameters for TestAGC: double EnvelopeCornerFrequency_Hz, double SampleRate_Hz, double AttackTau_sec, double ReleaseTau_sec, double Compression_dBRatio
void WriteOutput(double *InputSignalBuf, int InputSignalBufLen, char *BaseFileName, char *FreqFileName);
void GenerateImpulseSignal(double *SignalBuf, int SignalBufLen, double ImpulseAmplitude_dB);
void GenerateNoiseSignal(double *SignalBuf, int SignalBufLen, double NoiseAmplitude_dB);
void GenerateToneBurstSignal(double *SignalBuf, int SignalBufLen, double SampleRate_Hz, double ToneAmplitude_dB, double ToneFrequency_Hz);

double FilteredSignal[SignalLength];
double TestSignal[SignalLength];

//void TestDynamicVector(void);
void TestDownsampler(void);
void TestAutocorrelationGenerator(double SampleRate_Hz, double CenterFrequency_Hz, double HeadSize_uS);
int xcorr(double sig1[], double sig2[], int n, double lag[], int shiftmax);
double compute_lag(double sig1[], double sig2[], int n);

void TestBushyCell(int argc, char* argv[]);
void TestBushyCell_CurrentClamp(double CurrentClamp_Amperes);
void TestBushyCell_Spike(double EPSPStrength, double IPSPStrength);

void TestMGCell(int argc, char* argv[]);
void TestMGCell_CurrentClamp(double CurrentClamp_Amperes);
void TestMGCell_Spike(double EPSPStrength, double IPSPStrength);

void TestZhouLSOCell(int argc, char* argv[]);
void TestZhouLSOCell_CurrentClamp(double CurrentClamp_Amperes);
void TestZhouLSOCell_Spike(double EPSPStrength, double IPSPStrength);

void TestEarlabDataStream(void);
void TestSpikeStream(void);

void TestFIR(char *FileName);

int main(int argc, char* argv[])
{
	//TestFIR(argv[1]);
	//TestRandom();
	//TestSpikeStream();
	//TestXmlMetadata();
	//TestEarlabDataStream();
	//TestBushyCell(argc, argv);
	TestMGCell(argc, argv);
	//TestZhouLSOCell(argc, argv);
	//TestAutocorrelationGenerator(100000.0, 2345.6, 700.0);
	//return 0;
	//TestDownsampler();
	//TestDynamicVector();
	//GenerateNoiseSignal(TestSignal, SignalLength, 60.0);
	///GenerateToneBurstSignal(TestSignal, SignalLength, 48000.0, 60.0, 1000.0);
	//WriteOutput(TestSignal, SignalLength, "NoiseSignal", NULL); 
	///WriteOutput(TestSignal, SignalLength, "ToneBurstSignal", NULL); 
	//TestFilter(TestSignal, SignalLength,    20.0, 100000.0, "00020Hz");
	//TestFilter(TestSignal, SignalLength,   100.0, 100000.0, "00100Hz");
	//TestFilter(TestSignal, SignalLength,   500.0, 100000.0, "00500Hz");
	//TestFilter(TestSignal, SignalLength,  1000.0, 100000.0, "01000Hz");
	//TestFilter(TestSignal, SignalLength,  5000.0, 100000.0, "05000Hz");
	//TestFilter(TestSignal, SignalLength, 10000.0, 100000.0, "10000Hz");
	///TestAGC(TestSignal, SignalLength, 40.0, 48000.0, 0.018, 0.2, 10.0, "AGC1000Hz");
}

void TestFIR(char *FileName)
{
	FiniteImpulseResponse *FIR;
	float result;
	Random random;
	
	if (FileName == NULL)
		return;

	FIR = new FiniteImpulseResponse(FileName);
	for (int i = 0; i < 1000; i++)
		result = FIR->Filter((float)random.uniform());
}

void TestRandom(void)
{
	FILE *fp;
	Random random;

	fp = fopen("RandomUniform.txt", "w");
	if (fp == NULL)
	{
		printf("error opening output file 'RandomUniform.txt'\n");
		return;
	}
	for (int i = 0; i < 10000; i++)
		fprintf(fp, "%f\n", random.uniform());
	fclose(fp);

	fp = fopen("RandomGaussian.txt", "w");
	if (fp == NULL)
	{
		printf("error opening output file 'RandomGaussian.txt'\n");
		return;
	}
	for (int i = 0; i < 10000; i++)
		fprintf(fp, "%f\n", random.gaussian());
	fclose(fp);
}

void TestXmlMetadata(void)
{
	XmlMetadataFile theFile("TestXMLMetadata.metadata");
	theFile.Add(new XmlParameter("ParameterInt", 1));
	theFile.Add(new XmlParameter("ParameterFloat", 2.0f));
	theFile.Add(new XmlParameter("ParameterDouble", 3.0));
	theFile.Add(new XmlParameter("ParameterString", "Value1"));
	theFile.Add(new XmlDimension("Dimension1", 1, 0, 0));
	theFile.Add(new XmlDimension("Dimension2", 1, 0, 0));
	theFile.Add(new XmlDimension("Dimension3", 1, 0, 0));
	theFile.Add(new XmlDimension("Dimension4", 1, 0, 0));
	theFile.SetSource(new XmlSource("TestInstanceName", "TestModuleName_0.1.0.0"));
	theFile.Write();
}

#define SPIKE_CELLS 10
#define NUM_SAMPLES 10
#define SPIKE_COUNT (SPIKE_CELLS * NUM_SAMPLES)
#define SPIKE_SAMPLE_RATE_HZ 100000.0
#define SPIKE_TIME_STEP (2 / SPIKE_SAMPLE_RATE_HZ);
#define SPIKE_PROBABILITY 0.6

void TestEarlabDataStream(void)
{
	EarlabDataStream *Stream1, *Stream2;
	FloatMatrixN *Waves;
	SpikeStream *Spikes, *DelayedSpikes;
	SpikeDelayBuffer *DelayBuffer;
	Spike TheSpikes[SPIKE_COUNT];
	int i, j;

	Waves = new FloatMatrixN(1000, 256);
	Spikes = new SpikeStream(TheSpikes, SPIKE_COUNT, false, 100000);
	Stream1 = new EarlabWaveformStream(Waves);
	Stream2 = new EarlabSpikeStream(Spikes);
	DelayBuffer = new SpikeDelayBuffer(SPIKE_COUNT * 10, 0.00001, 0.01, 100000);
	for (i = 1; i < 30; i++)
		Spikes->Fire(i, .00001 * i);
	for (j = 0; j < 100; j++)
	{
		DelayedSpikes = DelayBuffer->Add(Spikes);
		if (DelayedSpikes->SpikeCount() > 0)
		{
			printf("Got %d spikes at frame %d\n", DelayedSpikes->SpikeCount(), j);
			for (i = 0; i < DelayedSpikes->SpikeCount(); i++)
				printf("  %d %05.5f\n", DelayedSpikes->GetSpike(i)->CellID, DelayedSpikes->GetSpike(i)->SpikeTime_Seconds);
		}
		else
			printf("No spikes at frame %d\n", j);
	}
	delete DelayBuffer;
	delete Stream2;
	delete Stream1;
	delete Spikes;
	delete Waves;
}

void TestSpikeStream(void)
{
	Spike Spikes[SPIKE_COUNT];
	SpikeStream Stream1(Spikes, SPIKE_COUNT, false, SPIKE_SAMPLE_RATE_HZ);
	SpikeStream Stream2(Spikes, SPIKE_COUNT, false, SPIKE_SAMPLE_RATE_HZ);
	//SpikeStream Stream3();
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
				Stream1.Fire(j);
				total++;
			}
		}
		cout << "Total spikes for sample " << i << ": " << total << endl;
		Stream1.Step();
	}
	Stream1.NewFrame();
	for (i = 0; i < NUM_SAMPLES; i++)
	{
		cout << "Number of spikes in odd  cells in frame " << i << ": " << Stream1.CountSpikes(odd_cells, SPIKE_CELLS / 2) << endl;
		cout << "Number of spikes in even cells in frame " << i << ": " << Stream1.CountSpikes(even_cells, SPIKE_CELLS / 2) << endl;
		Stream1.Step();
	}

	for (i = 0; i < NUM_SAMPLES; i++)
	{
		total = 0;
		for (j = 0; j < SPIKE_CELLS; j++)
		{
			result = (double)rand() / (double)RAND_MAX;
			if (result < SPIKE_PROBABILITY)
			{
				Stream2.Fire(j);
				total++;
			}
		}
		cout << "Total spikes for sample " << i << ": " << total << endl;
		Stream2.Step();
	}
	Stream2.NewFrame();
	for (i = 0; i < NUM_SAMPLES; i++)
	{
		cout << "Number of spikes in odd  cells in frame " << i << ": " << Stream2.CountSpikes(odd_cells, SPIKE_CELLS / 2) << endl;
		cout << "Number of spikes in even cells in frame " << i << ": " << Stream2.CountSpikes(even_cells, SPIKE_CELLS / 2) << endl;
		Stream2.Step();
	}

	//Stream3 = Stream1 + Stream2;
	for (i = 0; i < NUM_SAMPLES; i++)
	{
//		cout << "Number of spikes in odd  cells in frame " << i << ": " << Stream3.CountSpikes(odd_cells, SPIKE_CELLS / 2) << endl;
//		cout << "Number of spikes in even cells in frame " << i << ": " << Stream3.CountSpikes(even_cells, SPIKE_CELLS / 2) << endl;
//		Stream3.Step();
	}
}

#define BUSHY_CURRENT 130e-12

void TestBushyCell(int argc, char* argv[])
{
	double ExConductance, InConductance;
	double CurrentInjection;
	char *tmp;

	ExConductance = strtod(argv[1], &tmp);
	if (*tmp != '\0')
	{
		printf("Error on input\n");
		exit(1);
	}
	InConductance = strtod(argv[2], &tmp);
	if (*tmp != '\0')
	{
		printf("Error on input\n");
		exit(1);
	}
	printf("Calling TestBushyCell_Spike(%f, %f)\n", ExConductance, InConductance);
		fflush(stdout);
	TestBushyCell_Spike(ExConductance, InConductance);

	CurrentInjection = strtod(argv[3], &tmp);
	if (*tmp != '\0')
	{
		printf("Error on input\n");
		exit(1);
	}
	if (CurrentInjection != 0.0f)
	{
		printf("Calling TestBushyCell_CurrentClamp(%f)\n", CurrentInjection);
		fflush(stdout);
		TestBushyCell_CurrentClamp(CurrentInjection);
	}

	//TestBushyCell_Spike((18.2 * 18.2) / 19.5);
	//TestBushyCell_Spike(17);
	//TestSpikeStream();
	//return 0;
}

#define BUSHY_SPIKE_STEPS 20000
double BushyBuf[BUSHY_SPIKE_STEPS];
double K_slow[BUSHY_SPIKE_STEPS], K_fast[BUSHY_SPIKE_STEPS], Na_act[BUSHY_SPIKE_STEPS], Na_inact[BUSHY_SPIKE_STEPS];
double ExSynapse[BUSHY_SPIKE_STEPS];
double InSynapse[BUSHY_SPIKE_STEPS];
void TestBushyCell_Spike(double EPSPStrength, double IPSPStrength)
{
   int i;
   //Rothman93Compartment SpikeCompartment(100000.0, -60.0e-3, 38.0);    // 38 degrees C for spike tests
   Brughera96Compartment SpikeCompartment(100000.0, -60.0e-3, 38.0);    // 38 degrees C for spike tests
   //Cai98Compartment SpikeCompartment(100000.0, -60.0e-3, 38.0);    // 38 degrees C for spike tests
   // Rothman03Compartment SpikeCompartment(100000.0, -60.0e-3, 38.0, ResponseTypeValue);    // 38 degrees C for spike tests
   //Rothman03Compartment SpikeCompartment(100000.0, -64.0e-3, 32.0);    // 32 degrees C for spike test in Manis's email to Zhou
   //Rothman03Compartment SpikeCompartment(100000.0, -65.0e-3, 38.0);    // Initial Voltage -65.0e-3, 38 degrees C for Zhou spike tests
   //Rothman03AHPCompartment SpikeCompartment(100000.0, -65.0e-3, 38.0, ResponseTypeValue, 50.0e-3, 0.0e-9);    // 38 degrees C for spike tests
   //Rothman93Compartment SpikeCompartment(100000.0, -65.0e-3, 38.0, Response93TypeValue);    // 38 degrees C for spike tests
   HHSynapse Excitatory(100000.0, .0001, EPSPStrength); // the standard 0.1 ms used in Colburn's lab
   // HHSynapse Excitatory(100000.0, .00025, EPSPStrength); // Tau_rise 0.2ms Tau_fall 0.25-0.3ms suggested by reviewer for MSO neurons
   //HHSynapse Excitatory(100000.0, .00007, EPSPStrength); // Tau .00007 used to compare with Rothman and Manis 2003 at 38C (.0004 at 22C)
   //HHSynapse Excitatory(100000.0, .000077, 0.93*EPSPStrength); // area same as .00007 and 1, filter output same shape as t*exp(-t/T) in Matlab (both have same Ge thr) 
   HHSynapse Inhibitory(100000.0, .0001, IPSPStrength);
   HHSynapse *ExcitatorySynapses[2] = {&Excitatory, NULL};
   HHSynapse *InhibitorySynapses[2] = {&Inhibitory, NULL};

   Excitatory.Fire();
   Inhibitory.Fire();
   for (i = 0; i < BUSHY_SPIKE_STEPS; i++)
   {
       Excitatory.Step();
       Inhibitory.Step();
       ExSynapse[i] = Excitatory.GetConductance_Siemens();
       InSynapse[i] = Inhibitory.GetConductance_Siemens();
       SpikeCompartment.Step(ExcitatorySynapses, 1, InhibitorySynapses, 1);
       BushyBuf[i] = SpikeCompartment.GetVoltage_V();
       K_slow[i] = SpikeCompartment.GetActivations(1);
       K_fast[i] = SpikeCompartment.GetActivations(2);
       Na_act[i] = SpikeCompartment.GetActivations(3);
       Na_inact[i] = SpikeCompartment.GetActivations(4);
   }
   WriteOutput(BushyBuf, BUSHY_SPIKE_STEPS, "BushySpikeVoltage", NULL);
   WriteOutput(ExSynapse,  BUSHY_SPIKE_STEPS, "BushySpikeExSynapse", NULL);
   WriteOutput(InSynapse,  BUSHY_SPIKE_STEPS, "BushySpikeInSynapse", NULL);
   WriteOutput(K_slow,   BUSHY_SPIKE_STEPS, "BushySpikeKslow",   NULL);
   WriteOutput(K_fast,   BUSHY_SPIKE_STEPS, "BushySpikeKfast",   NULL);
   WriteOutput(Na_act,   BUSHY_SPIKE_STEPS, "BushySpikeNaAct",   NULL);
   WriteOutput(Na_inact, BUSHY_SPIKE_STEPS, "BushySpikeNaInact", NULL);
}


#define BUSHY_CLAMP_STEPS 20000
double BushyClampBuf[BUSHY_CLAMP_STEPS];
double K_slow_Clamp[BUSHY_CLAMP_STEPS], K_fast_Clamp[BUSHY_CLAMP_STEPS], Na_act_Clamp[BUSHY_CLAMP_STEPS], Na_inact_Clamp[BUSHY_CLAMP_STEPS];
void TestBushyCell_CurrentClamp(double CurrentClamp_Amperes)
{
	int i;

	//Rothman93Compartment ClampCompartment(100000.0, -60.0e-3, 22.0);	// 22 degrees C for current clamp test
	Brughera96Compartment ClampCompartment(100000.0, -60.0e-3, 22.0);	// 22 degrees C for current clamp test
	//Cai98Compartment ClampCompartment(100000.0, -60.0e-3, 22.0);	// 22 degrees C for current clamp test
	//Rothman03Compartment ClampCompartment(100000.0, -64.0e-3, 22.0);	// 22 degrees C for current clamp test
	//Rothman03AHPCompartment ClampCompartment(100000.0, -65.0e-3, 22.0);	// 22 degrees C for current clamp test
	//Rothman03AHPCompartment ClampCompartment(100000.0, -65.0e-3, 22.0, ResponseTypeValue, 50.0e-3, 0.0e-9);	// 22 degrees C for current clamp test
	//Rothman93Compartment ClampCompartment(100000.0, -65.0e-3, 22.0, Response93TypeValue);	// 22 degrees C for current clamp test


	//MGCompartment ClampCompartment(100000.0, 0.01, 2.0, 0.0075);  //double SampleRate_Hz, double Tau_m_Seconds, double b_K, double V_th_Volts

	//For Zhou
	//Rothman03Compartment ClampCompartment(100000.0, -65.0e-3, 38.0);	// 38 degrees C for current clamp test
	//Rothman03Compartment ClampCompartment(100000.0, -65.0e-3, 22.0);	// 22 degrees C for current clamp test

	for (i = 0; i < BUSHY_CLAMP_STEPS; i++)
	{
		if (i == 1000)
			ClampCompartment.SetCurrentInjection_A(CurrentClamp_Amperes);
		if (i == (11000))
			ClampCompartment.SetCurrentInjection_A(0.0);

		ClampCompartment.Step(NULL, 0, NULL, 0);
		BushyClampBuf[i] = ClampCompartment.GetVoltage_V();
		K_slow_Clamp[i] = ClampCompartment.GetActivations(1);
		K_fast_Clamp[i] = ClampCompartment.GetActivations(2);
		Na_act_Clamp[i] = ClampCompartment.GetActivations(3);
		Na_inact_Clamp[i] = ClampCompartment.GetActivations(4);
	}
	WriteOutput(BushyClampBuf, BUSHY_CLAMP_STEPS, "BushyClampPositive_Voltage", NULL);
	WriteOutput(Na_act_Clamp,   BUSHY_CLAMP_STEPS, "BushyClampPositive_NaAct",   NULL);
	WriteOutput(Na_inact_Clamp, BUSHY_CLAMP_STEPS, "BushyClampPositive_NaInact", NULL);
	WriteOutput(K_slow_Clamp,   BUSHY_CLAMP_STEPS, "BushyClampPositive_Kslow",   NULL);
	WriteOutput(K_fast_Clamp,   BUSHY_CLAMP_STEPS, "BushyClampPositive_Kfast",   NULL);
	fflush(stdout);

	for (i = 0; i < BUSHY_CLAMP_STEPS; i++)
	{
		if (i == 1000)
			ClampCompartment.SetCurrentInjection_A(-1.0 * CurrentClamp_Amperes);
		if (i == (11000))
			ClampCompartment.SetCurrentInjection_A(0.0);

		ClampCompartment.Step(NULL, 0, NULL, 0);
		BushyClampBuf[i] = ClampCompartment.GetVoltage_V();
		K_slow_Clamp[i] = ClampCompartment.GetActivations(1);
		K_fast_Clamp[i] = ClampCompartment.GetActivations(2);
		Na_act_Clamp[i] = ClampCompartment.GetActivations(3);
		Na_inact_Clamp[i] = ClampCompartment.GetActivations(4);
	}
	WriteOutput(BushyClampBuf, BUSHY_CLAMP_STEPS, "BushyClampNegative_Voltage", NULL);
	WriteOutput(Na_act_Clamp,   BUSHY_CLAMP_STEPS, "BushyClampNegative_NaAct",   NULL);
	WriteOutput(Na_inact_Clamp, BUSHY_CLAMP_STEPS, "BushyClampNegative_NaInact", NULL);
	WriteOutput(K_slow_Clamp,   BUSHY_CLAMP_STEPS, "BushyClampNegative_Kslow",   NULL);
	WriteOutput(K_fast_Clamp,   BUSHY_CLAMP_STEPS, "BushyClampNegative_Kfast",   NULL);
	fflush(stdout);
}

//MacGregor neuromime SupportClasses
#define MG_CURRENT 130e-12

void TestMGCell(int argc, char* argv[])
{
	double ExConductance, InConductance;
	double CurrentInjection;
	char *tmp;

	ExConductance = strtod(argv[1], &tmp);
	if (*tmp != '\0')
	{
		printf("Error on input\n");
		exit(1);
	}
	InConductance = strtod(argv[2], &tmp);
	if (*tmp != '\0')
	{
		printf("Error on input\n");
		exit(1);
	}
	printf("Calling TestMGCell_Spike(%f, %f)\n", ExConductance, InConductance);
		fflush(stdout);
	TestMGCell_Spike(ExConductance, InConductance);

	CurrentInjection = strtod(argv[3], &tmp);
	if (*tmp != '\0')
	{
		printf("Error on input\n");
		exit(1);
	}
	if (CurrentInjection != 0.0f)
	{
		printf("Calling TestMGCell_CurrentClamp(%f)\n", CurrentInjection);
		fflush(stdout);
		TestMGCell_CurrentClamp(CurrentInjection);
	}
}

#define MG_SPIKE_STEPS 10000
double MGBuf[MG_SPIKE_STEPS];
double g_K[MG_SPIKE_STEPS];
double MGExSynapse[MG_SPIKE_STEPS];
double MGExSynapseNSA[MG_SPIKE_STEPS];
double MGInSynapse1[MG_SPIKE_STEPS];
double MGInSynapse2[MG_SPIKE_STEPS];
double MGExCurrent[MG_SPIKE_STEPS];
double MGExCurrentNSA[MG_SPIKE_STEPS];
double MGInCurrent1[MG_SPIKE_STEPS];
double MGInCurrent2[MG_SPIKE_STEPS];

void TestMGCell_Spike(double EPSPStrength, double IPSPStrength)
{
   int i;
   double SampleRate_Hz = 100000.0;
   double Tau_m_Seconds = 0.0003;
   double b_K = 2.0;
   double V_th_Volts = 0.0075;
   double Tau_K_Seconds = 0.003;
   double E_K_Volts = -0.010;
   double E_ex_Volts = 0.070;
   double E_in_Volts = -0.010;
   double g_leak_S = 10.0e-9;

   MGCompartment MGSpikeCompartment(SampleRate_Hz, Tau_m_Seconds, b_K, V_th_Volts, Tau_K_Seconds, E_K_Volts, E_ex_Volts, E_in_Volts);
   //MGCompartment MGSpikeCompartment(100000.0, 0.0003, 2.0, 0.0075, 0.003, -0.010, 0.070, -0.010);
   // MGCompartment(double SampleRate_Hz, double Tau_m_Seconds, double b_K, double V_th_Volts, 
   // double Tau_K_Seconds, double E_K_Volts, double E_ex_Volts, double E_in_Volts)
   // MGCompartment MGSpikeCompartment(100000.0, 0.01, 2.0, 0.0075, .001, -0.010, 0.070, -0.010);


   // The current equation in Hancock and Voigt (1999) has been divided 
   // by the membrane leakage conductance so that Tau_m_Seconds (= C_m/G) can be used.
   // InjectedCurrent_Amps must be also divided by G (assumed to be 10e-9 Siemens for testing).
   // Conductances are unitless because they are divided by the leakage conductance.
   // In MGSynapse there is no 1.0e-9 conversion from nS to S (as in the HHSynapse) because MacGregor conductances are normalized 
	// by dividing by leakage G (assumed 10e-9 in setting AuxCurrentInput_A).
	// Also the (E / Tau) factor is not present because the conductance profile is a simple step
	// followed by exponential decay.

   MGSynapseFirstOrder Excitatory(100000.0, .0002, EPSPStrength, &MGSpikeCompartment);
   //MGSynapseFirstOrder ExcitatoryNSA(100000.0, .01, 0.0, &MGSpikeCompartment);
   MGSynapseFirstOrder ExcitatoryNSA(100000.0, .01, EPSPStrength/10.0, &MGSpikeCompartment);
   MGSynapseFirstOrder Inhibitory1(100000.0, .0002, 5.0*IPSPStrength, &MGSpikeCompartment);
   MGSynapseFirstOrder Inhibitory2(100000.0, .002, IPSPStrength, &MGSpikeCompartment);
   MGSynapseFirstOrder *ExcitatorySynapses[2] = {&Excitatory, NULL};
   MGSynapseFirstOrder *ExcitatorySynapsesNSA[2] = {&ExcitatoryNSA, NULL};
   MGSynapseFirstOrder *InhibitorySynapses1[2] = {&Inhibitory1, NULL};
   MGSynapseFirstOrder *InhibitorySynapses2[2] = {&Inhibitory2, NULL};

   for (i = 0; i < MG_SPIKE_STEPS; i++)
   {
	   if (i == 1000)
	   {
		   Excitatory.Fire();
	   }
	   if (i == 3000)
	   {
		   ExcitatoryNSA.Fire();
		   Inhibitory1.Fire();
		   Inhibitory2.Fire();
	   }

       Excitatory.Step();
       ExcitatoryNSA.Step();
       Inhibitory1.Step();
       Inhibitory2.Step();
       MGExSynapse[i] = Excitatory.GetConductance_Siemens();
       MGExSynapseNSA[i] = ExcitatoryNSA.GetConductance_Siemens();
       MGInSynapse1[i] = Inhibitory1.GetConductance_Siemens();
       MGInSynapse2[i] = Inhibitory2.GetConductance_Siemens();
       //MGSpikeCompartment.Step(ExcitatorySynapses, 1, InhibitorySynapses, 1);
       MGSpikeCompartment.Step(ExcitatorySynapses, 1, ExcitatorySynapsesNSA, 1, InhibitorySynapses1, 1, InhibitorySynapses2, 1);
       MGBuf[i] = MGSpikeCompartment.GetMembraneVoltage_V();
       g_K[i] = MGSpikeCompartment.GetSolutionY(1);
	  // For currents in amps, multiply by g_leak_S
	   MGExCurrent[i] = g_leak_S * Excitatory.GetConductance_Siemens() * (E_ex_Volts - MGSpikeCompartment.GetMembraneVoltage_V());
	   MGExCurrentNSA[i] = g_leak_S * ExcitatoryNSA.GetConductance_Siemens() * (E_ex_Volts - MGSpikeCompartment.GetMembraneVoltage_V());
	   MGInCurrent1[i] = g_leak_S * Inhibitory1.GetConductance_Siemens() * (E_in_Volts - MGSpikeCompartment.GetMembraneVoltage_V());
	   MGInCurrent2[i] = g_leak_S * Inhibitory2.GetConductance_Siemens() * (E_in_Volts - MGSpikeCompartment.GetMembraneVoltage_V()); 
   }
   WriteOutput(MGBuf, MG_SPIKE_STEPS, "MGSpikeVoltage", NULL);
   WriteOutput(g_K,   MG_SPIKE_STEPS, "MGSpike_g_K",   NULL);
   WriteOutput(MGExSynapse,  MG_SPIKE_STEPS, "MGSpikeMGExSynapse", NULL);
   WriteOutput(MGExSynapseNSA,  MG_SPIKE_STEPS, "MGSpikeMGExSynapseNSA", NULL);
   WriteOutput(MGInSynapse1,  MG_SPIKE_STEPS, "MGSpikeMGInSynapse1", NULL);
   WriteOutput(MGInSynapse2,  MG_SPIKE_STEPS, "MGSpikeMGInSynapse2", NULL);
   WriteOutput(MGExCurrent,  MG_SPIKE_STEPS, "MGSpikeMGExCurrent", NULL);
   WriteOutput(MGExCurrentNSA,  MG_SPIKE_STEPS, "MGSpikeMGExCurrentNSA", NULL);
   WriteOutput(MGInCurrent1,  MG_SPIKE_STEPS, "MGSpikeMGInCurrent1", NULL);
   WriteOutput(MGInCurrent2,  MG_SPIKE_STEPS, "MGSpikeMGInCurrent2", NULL);
   fflush(stdout);
}


#define MG_CLAMP_STEPS 50000
double MGClampBuf[MG_CLAMP_STEPS];
double g_K_Clamp[MG_CLAMP_STEPS];
void TestMGCell_CurrentClamp(double CurrentClamp_Amperes)
{
	int i;

	MGCompartment ClampCompartment(100000.0, 0.01, 2.0, 0.0075, .001, -0.010, 0.070, -0.010);  //double SampleRate_Hz, double Tau_m_Seconds, double b_K, double V_th_Volts

	for (i = 0; i < MG_CLAMP_STEPS; i++)
	{
		if (i == 5000)
			ClampCompartment.SetCurrentInjection_A(CurrentClamp_Amperes);
		if (i == (25000))
			ClampCompartment.SetCurrentInjection_A(0.0);

		ClampCompartment.Step(NULL, 0, NULL, 0, NULL, 0, NULL, 0);
		MGClampBuf[i] = ClampCompartment.GetMembraneVoltage_V();
		g_K_Clamp[i] = ClampCompartment.GetSolutionY(1);
	}
	WriteOutput(MGClampBuf, MG_CLAMP_STEPS, "MGClampPositive_Voltage", NULL);
	WriteOutput(g_K_Clamp,   MG_CLAMP_STEPS, "MGClampPositive_g_K_Clamp",   NULL);
	fflush(stdout);

	for (i = 0; i < MG_CLAMP_STEPS; i++)
	{
		if (i == 5000)
			ClampCompartment.SetCurrentInjection_A(-1.0 * CurrentClamp_Amperes);
		if (i == (25000))
			ClampCompartment.SetCurrentInjection_A(0.0);

		ClampCompartment.Step(NULL, 0, NULL, 0, NULL, 0, NULL, 0);
		MGClampBuf[i] = ClampCompartment.GetMembraneVoltage_V();
		g_K_Clamp[i] = ClampCompartment.GetSolutionY(1);
	}
	WriteOutput(MGClampBuf, MG_CLAMP_STEPS, "MGClampNegative_Voltage", NULL);
	WriteOutput(g_K_Clamp,   MG_CLAMP_STEPS, "MGClampNegative_g_K_Clamp",   NULL);
	fflush(stdout);
}

// ZhouLSO SupportClasses
#define ZhouLSO_CURRENT 130e-12

void TestZhouLSOCell(int argc, char* argv[])
{
	double ExConductance, InConductance;
	double CurrentInjection;
	char *tmp;

	ExConductance = strtod(argv[1], &tmp);
	if (*tmp != '\0')
	{
		printf("Error on input\n");
		exit(1);
	}
	InConductance = strtod(argv[2], &tmp);
	if (*tmp != '\0')
	{
		printf("Error on input\n");
		exit(1);
	}
	printf("Calling TestZhouLSOCell_Spike(%f, %f)\n", ExConductance, InConductance);
		fflush(stdout);
	TestZhouLSOCell_Spike(ExConductance, InConductance);

	CurrentInjection = strtod(argv[3], &tmp);
	if (*tmp != '\0')
	{
		printf("Error on input\n");
		exit(1);
	}
	if (CurrentInjection != 0.0f)
	{
		printf("Calling TestZhouLSOCell_CurrentClamp(%f)\n", CurrentInjection);
		fflush(stdout);
		TestZhouLSOCell_CurrentClamp(CurrentInjection);
	}
}

#define ZhouLSO_SPIKE_STEPS 10000
double ZhouLSOBuf[ZhouLSO_SPIKE_STEPS];
//double g_AHP[ZhouLSO_SPIKE_STEPS];
double ZhouLSOExSynapse[ZhouLSO_SPIKE_STEPS];
double ZhouLSOInSynapse[ZhouLSO_SPIKE_STEPS];
void TestZhouLSOCell_Spike(double EPSPStrength, double IPSPStrength)
{
   int i;
   ZhouLSOCompartment ZhouLSOSpikeCompartment(100000.0, 31.4e-12, 0.0314e-6, -0.065, 10.0e-6, -0.065,
	   0.002, 0.02e-6, -0.065, 0.020, -0.050, -0.065,
	   0.0, -0.070, (EPSPStrength*1e-9), (IPSPStrength*1e-9),
	   0.0001, 0.001, 0.0001, 0.001); 
   // double SampleRate_Hz, double C_m_Farads, double G_L_Siemens, double E_L_Volts, double G_abs_increment_Siemens, double E_abs_Volts,
   // double t_abs_seconds, double G_AHP_increment_Siemens, double E_AHP_Volts, double Tau_AHP_Seconds, double V_th_Volts, double V_rest_Volts
	// double E_e_Volts, double E_i_Volts,	double G_e_Siemens, double G_i_Siemens,	
	// double Tau_e_rise_Seconds, double Tau_e_fall_Seconds, double Tau_i_rise_Seconds, double Tau_i_fall_Seconds);

   // ZLSynapse1stOrderDiffc(double SampleRate_Hz, double Tau_rise_seconds, double Tau_fall_seconds, double EPSP_strength_Siemens, ZhouLSOCompartment *theCompartment);
   ZLSynapse1stOrderDiffc Excitatory(100000.0, 0.0001, 0.001, (EPSPStrength*1e-9), &ZhouLSOSpikeCompartment);
   ZLSynapse1stOrderDiffc Inhibitory(100000.0, 0.0001, 0.001, (IPSPStrength*1e-9), &ZhouLSOSpikeCompartment);
   ZLSynapse1stOrderDiffc *ExcitatorySynapses[2] = {&Excitatory, NULL};
   ZLSynapse1stOrderDiffc *InhibitorySynapses[2] = {&Inhibitory, NULL};

   for (i = 0; i < ZhouLSO_SPIKE_STEPS; i++)
   {
	   if (i == 0)
	   {
		   Excitatory.Fire();
		   Inhibitory.Fire();
	   }
       Excitatory.Step();
       Inhibitory.Step();
       ZhouLSOExSynapse[i] = Excitatory.GetConductance_Siemens();
       ZhouLSOInSynapse[i] = Inhibitory.GetConductance_Siemens();
       ZhouLSOSpikeCompartment.Step(ExcitatorySynapses, 1, InhibitorySynapses, 1);
       ZhouLSOBuf[i] = ZhouLSOSpikeCompartment.GetMembraneVoltage_V();
       //g_AHP[i] = ZhouLSOSpikeCompartment.GetSolutionY(1);
   }
   WriteOutput(ZhouLSOBuf, ZhouLSO_SPIKE_STEPS, "ZhouLSOSpikeVoltage", NULL);
   WriteOutput(ZhouLSOExSynapse,  ZhouLSO_SPIKE_STEPS, "ZhouLSOSpikeExSynapse", NULL);
   WriteOutput(ZhouLSOInSynapse,  ZhouLSO_SPIKE_STEPS, "ZhouLSOSpikeInSynapse", NULL);
   //WriteOutput(g_AHP,   ZhouLSO_SPIKE_STEPS, "ZhouLSOSpike_g_AHP",   NULL);
}

#define ZhouLSO_CLAMP_STEPS 50000
double ZhouLSOClampBuf[ZhouLSO_CLAMP_STEPS];
//double g_AHP_Clamp[ZhouLSO_CLAMP_STEPS];
void TestZhouLSOCell_CurrentClamp(double CurrentClamp_Amperes)
{
	int i;
	double InputCurrent_Amps = 0.0;

	ZhouLSOCompartment ClampCompartment(100000.0, 31.4e-12, 0.0314e-6, -0.065, 10.0e-6, -0.065, 0.002, 0.02e-6, -0.065, 0.020, -0.050, -0.065); 
	// double SampleRate_Hz, double C_m_Farads, double G_L_Siemens, double E_L_Volts, double G_abs_increment_Siemens, double E_abs_Volts,
	// double t_abs_seconds, double G_AHP_increment_Siemens, double E_AHP_Volts, double Tau_AHP_Seconds, double V_th_Volts, double V_rest_Volts

   //ZhouLSOCompartment ClampCompartment(100000.0, 31.4e-12, 0.0314e-6, -0.065, 10.0e-6, -0.065,
	 //  0.002, 0.02e-6, -0.065, 0.020, -0.050, -0.065,
	 //  0.0, -0.070, 0.0, 0.0,
	 //  0.0001, 0.001, 0.0001, 0.001); 
   // double SampleRate_Hz, double C_m_Farads, double G_L_Siemens, double E_L_Volts, double G_abs_increment_Siemens, double E_abs_Volts,
   // double t_abs_seconds, double G_AHP_increment_Siemens, double E_AHP_Volts, double Tau_AHP_Seconds, double V_th_Volts, double V_rest_Volts
	// double E_e_Volts, double E_i_Volts,	double G_e_Siemens, double G_i_Siemens,	
	// double Tau_e_rise_Seconds, double Tau_e_fall_Seconds, double Tau_i_rise_Seconds, double Tau_i_fall_Seconds);

	for (i = 0; i < ZhouLSO_CLAMP_STEPS; i++)
	{
		if (i == 5000)
			InputCurrent_Amps = CurrentClamp_Amperes;
		if (i == (25000))
			InputCurrent_Amps = 0.0;

		//bool ZhouLSOCompartment::Step(float InputCurrent_Amps)
		ClampCompartment.Step(InputCurrent_Amps);
		ZhouLSOClampBuf[i] = ClampCompartment.GetMembraneVoltage_V();
		//g_AHP_Clamp[i] = ClampCompartment.GetSolutionY(1);
	}
	WriteOutput(ZhouLSOClampBuf, ZhouLSO_CLAMP_STEPS, "ZhouLSOClampPositive_Voltage", NULL);
	//WriteOutput(g_AHP_Clamp,   ZhouLSO_CLAMP_STEPS, "ZhouLSOClampPositive_g_AHP_Clamp",   NULL);
	fflush(stdout);

	for (i = 0; i < ZhouLSO_CLAMP_STEPS; i++)
	{
		if (i == 5000)
			InputCurrent_Amps = -CurrentClamp_Amperes;
		if (i == (25000))
			InputCurrent_Amps = 0.0;

		//bool ZhouLSOCompartment::Step(float InputCurrent_Amps)
		ClampCompartment.Step(InputCurrent_Amps);

		ZhouLSOClampBuf[i] = ClampCompartment.GetMembraneVoltage_V();
		//g_AHP_Clamp[i] = ClampCompartment.GetSolutionY(1);
	}
	WriteOutput(ZhouLSOClampBuf, ZhouLSO_CLAMP_STEPS, "ZhouLSOClampNegative_Voltage", NULL);
	//WriteOutput(g_AHP_Clamp,   ZhouLSO_CLAMP_STEPS, "ZhouLSOClampNegative_g_AHP_Clamp",   NULL);
	fflush(stdout);
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

int xcorr(double sig1[], double sig2[], int n, double lag[], int shiftmax)
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

double compute_lag(double sig1[], double sig2[], int n)
{
    int i;
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
	float *TestSignal, r, rmax;
	Downsampler *ds = new Downsampler(0, SignalLength, 100000, 8192, "TestDownsampler", NULL, "Test");
	int i, j;

	ds->SetMaxLevel(32);
	ds->SetMinimumSampleCountToKeep(400);
	ds->SetType(DownsampleTypeAverage);
	TestSignal = new float[SignalLength];
	rmax = (double)RAND_MAX;

	for (j = 0; j < NumReps; j++)
	{
		for (i = 0; i < SignalLength; i++)
		{
			r = (float)rand();
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

int TestAGC(double *TestSignal, int SignalLen, double EnvelopeCornerFrequency_Hz, double SampleRate_Hz, double AttackTau_sec, double ReleaseTau_sec, double Compression_dBRatio, char *FreqFileName)
{
	
	AGCAdvBionics AGC;
	int i, j;

	AGC.Init(EnvelopeCornerFrequency_Hz, SampleRate_Hz, AttackTau_sec, ReleaseTau_sec, Compression_dBRatio);
	for (i = 0; i < SignalLen; i++)
	{
		FilteredSignal[i] = AGC.Filter(TestSignal[i]);
		if ((i == 1000) || (i == 4000))
			j = 1;
		else 
			j = 0;
	}
	WriteOutput(FilteredSignal, SignalLen, "Output", FreqFileName); 

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

void GenerateToneBurstSignal(double *SignalBuf, int SignalBufLen, double SampleRate_Hz, double ToneAmplitude_dB, double ToneFrequency_Hz)
{
	int i, j, k;
	int ToneOnsetIndex, ToneOffsetIndex, ToneDurationNsamples;
	double SamplePeriod_sec, BaseFrequency_Hz; 
	double Multiplier, Remainder, AdjustedToneFrequency_Hz;
	int TruncatedMultiplier, RoundUp, RoundedMultiplier;

	if (SignalBuf == NULL)
		return;

	if (SignalBufLen <= 0)
		return;

	// Check for exceeding the nyquist rate
	if (ToneFrequency_Hz > (SampleRate_Hz/2.0))
		return;


	//ToneOnsetIndex = SignalBufLen/10; // remember, indices start at 0 in cpp
	//ToneDurationNsamples = SignalBufLen/2;  // There may be problems if SignalBufLen is odd
	ToneOnsetIndex = 1000; // remember, indices start at 0 in cpp
	ToneDurationNsamples = 4000;  // There may be problems if SignalBufLen is odd
	ToneOffsetIndex = ToneOnsetIndex + ToneDurationNsamples;
	SamplePeriod_sec = 1.0/SampleRate_Hz;

	// Adjust the tone frequency for an integer number of periods:

	// The BaseFrequency is also the lowest frequency that will have an integer number of cycles.  
	// All multiples of this frequency will also have an integer number of cycles.
	BaseFrequency_Hz = SampleRate_Hz/ToneDurationNsamples;

	// Multiplier variable to determine nearest integer cycle frequency to requested frequency.
	Multiplier = ToneFrequency_Hz/BaseFrequency_Hz;
	TruncatedMultiplier = (int) Multiplier;
	Remainder = Multiplier - TruncatedMultiplier;
	if (Remainder >= 0.5)
		RoundUp = 1;
	else RoundUp = 0;
	RoundedMultiplier = TruncatedMultiplier + RoundUp;
	//if (multiplier <= 0)
	//	multiplier = 1;
	AdjustedToneFrequency_Hz = RoundedMultiplier * BaseFrequency_Hz;
	// *** TEST WITHOUT ADJUSTMENT
	//AdjustedToneFrequency_Hz = ToneFrequency_Hz;

	// Check for exceeding the nyquist rate
	if (AdjustedToneFrequency_Hz > (SampleRate_Hz/2.0))
		return;

	for (i = 0; i < ToneOnsetIndex; i++)
		SignalBuf[i] = 0.0;

	for (j = i; j < ToneOffsetIndex; j++)
	{
//		SignalBuf[j] = ((20.0 * pow(10.0, ToneAmplitude_dB / 20.0)) * 2.0e-9 * sin(2*PI*AdjustedToneFrequency_Hz*SamplePeriod_sec*(j-ToneOnsetIndex)));
		SignalBuf[j] = 100.0 * sin(2*PI*AdjustedToneFrequency_Hz*SamplePeriod_sec*(j-ToneOnsetIndex));
	}

	for (k = j; k < SignalBufLen; k++)
		SignalBuf[j] = 0.0;
}

#if 0
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
#endif