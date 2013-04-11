#include "Earlab.h"
#include "EarlabException.h"
#include "FiniteImpulseResponse.h"
#include "stdio.h"
#include "memory.h"
#include "ModuleServices.h"
#include "FPException.h"
#include <math.h>

using namespace std;

FiniteImpulseResponse::FiniteImpulseResponse(char *FileName)
{
	FILE *fp;
	float tmp;

	if ((FileName == NULL) || (strlen(FileName) == 0))
		throw EarlabException("FiniteImpulseResponse::Constructor: NULL or empty Impulse Response File name specified");

	fp = OpenInputFile(FileName, "r");
	if (fp == NULL)
		throw EarlabException("FiniteImpulseResponse::Constructor: Error opening specified impulse response file \"%s\"", FileName);
	
	fscanf(fp, "%f", &SampleRate_Hz);
	ImpulseResponseLength_Samples = 0;
	while (true)
	{
		fscanf(fp, "%f", &tmp);
		if (feof(fp))
			break;
		ImpulseResponseLength_Samples++;
	}
	fseek(fp, 0, SEEK_SET);
	ImpulseResponse = new float [ImpulseResponseLength_Samples];
	fscanf(fp, "%f", &SampleRate_Hz);
	for (int i = 0; i < (int)ImpulseResponseLength_Samples; i++)
		fscanf(fp, "%f", &(ImpulseResponse[i]));
	fclose(fp);

	InitializeRingBuffer();
}

FiniteImpulseResponse::FiniteImpulseResponse(float *ImpulseResponse, unsigned int ImpulseResponseLength_Samples, float SampleRate_Hz)
{
	this->ImpulseResponseLength_Samples = ImpulseResponseLength_Samples;
	this->SampleRate_Hz = SampleRate_Hz;
	this->ImpulseResponse = new float [ImpulseResponseLength_Samples];
	memcpy(this->ImpulseResponse, ImpulseResponse, sizeof(float) * ImpulseResponseLength_Samples);

	InitializeRingBuffer();
}

void FiniteImpulseResponse::InitializeRingBuffer(void)
{
	RingBuffer = new float [ImpulseResponseLength_Samples];
	memset(RingBuffer, 0, sizeof(float) * ImpulseResponseLength_Samples);
	// Set the start and end indices of the ring buffer
	StartIndex = 0;
	EndIndex = ImpulseResponseLength_Samples - 1;
}

FiniteImpulseResponse::~FiniteImpulseResponse()
{
	delete [] ImpulseResponse;
	delete [] RingBuffer;
}

float FiniteImpulseResponse::Filter(float Sample)
{
	double CurResult = 0;

	// Add a sample to the end of the ring buffer

	// First, move the Start and End indices appropriately.
	StartIndex++;
	StartIndex %= ImpulseResponseLength_Samples;
	EndIndex++;
	EndIndex %= ImpulseResponseLength_Samples;
	// Insert the new sample at EndIndex
	RingBuffer[EndIndex] = Sample;

	for (int i = 0; i < (int)ImpulseResponseLength_Samples; i++)
		CurResult += RingBuffer[(StartIndex + i) % ImpulseResponseLength_Samples] * ImpulseResponse[(int)ImpulseResponseLength_Samples - i];
	if (IS_ALMOST_DENORMAL(CurResult))
		CurResult = 0;
	return (float)CurResult;
}

float FiniteImpulseResponse::GetSampleRate_Hz(void)
{
	return SampleRate_Hz;
}
