#ifndef __FIR_RING_BUFFER_H_INCLUDED
#define __FIR_RING_BUFFER_H_INCLUDED

class FIR_RingBuffer
{
public:
	FIR_RingBuffer(int BufferLength);
	~FIR_RingBuffer();

	void AddSample(float NewSample);

	float &operator[](unsigned int index);
	float &operator[](int index);

private:
	unsigned int StartIndex, EndIndex, BufferLength;
	float *Buffer;
};

#endif
