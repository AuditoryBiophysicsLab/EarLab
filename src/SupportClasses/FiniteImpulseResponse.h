#ifndef __FINITE_IMPULSE_RESPONSE_H_INCLUDED
#define __FINITE_IMPULSE_RESPONSE_H_INCLUDED

class FiniteImpulseResponse
{
public:
	FiniteImpulseResponse(char *FileName);
	FiniteImpulseResponse(float *ImpulseResponse, unsigned int ImpulseResponseLength_Samples, float SampleRate_Hz);
	~FiniteImpulseResponse();

	float Filter(float Sample);

	float GetSampleRate_Hz(void);
private:
	void InitializeRingBuffer(void);
	float *RingBuffer;
	float *ImpulseResponse;
	unsigned int ImpulseResponseLength_Samples;
	float SampleRate_Hz;
	unsigned int StartIndex, EndIndex;	// Indices for the ring buffer
};

#endif
