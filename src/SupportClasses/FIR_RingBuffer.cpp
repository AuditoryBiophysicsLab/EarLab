#include "FIR_RingBuffer.h"
#include "Earlab.h"
#include "EarlabException.h"
#include "memory.h"

FIR_RingBuffer::FIR_RingBuffer(int BufferLength)
{
	// Allocate the new buffer
	Buffer = new float[BufferLength];
	// Zero out all the elements
	memset(Buffer, 0, sizeof(float) * BufferLength);
	// Set the start and end indices
	StartIndex = 0;
	EndIndex = BufferLength - 1;
	this->BufferLength = BufferLength;
}

FIR_RingBuffer::~FIR_RingBuffer()
{
	// Free the buffer
	delete [] Buffer;
}

void FIR_RingBuffer::AddSample(float NewSample)
{
	// Add a sample to the end of the ring buffer

	// First, move the Start and End indices appropriately.
	StartIndex++;
	StartIndex %= BufferLength;
	EndIndex++;
	EndIndex %= BufferLength;
	// Insert the new sample at EndIndex
	Buffer[EndIndex] = NewSample;
}

float &FIR_RingBuffer::operator[](unsigned int index)
{
	if (index >= BufferLength)
		throw EarlabException("FIR_RingBuffer::operator[]: Array index out of bounds.  You asked for index [%d], but array only contains %d elements", index, BufferLength);
	index += StartIndex;
	index %= BufferLength;
	return Buffer[index];
}

float &FIR_RingBuffer::operator[](int index)
{
	if (index < 0)
		throw EarlabException("FIR_RingBuffer::operator[]: Negative array indices not supported.  You asked for index [%d].", index);
	if (index >= (int)BufferLength)
		throw EarlabException("FIR_RingBuffer::operator[]: Array index out of bounds.  You asked for index [%d], but array only contains %d elements", index, BufferLength);
	index += StartIndex;
	index %= BufferLength;
	return Buffer[index];
}
