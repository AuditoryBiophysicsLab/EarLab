#include "DynamicVector.h"
#include "stdio.h"
#include "memory.h"
#include "EarlabException.h"

#ifdef WIN32
template IntVector;
template FloatVector;
template DoubleVector;
#endif

#define INITIAL_ELEMENT_DEFAULT 50
#define GROW_ELEMENT_DEFAULT 50

template <class T> DynamicVector<T>::DynamicVector()
{
	Initialize(INITIAL_ELEMENT_DEFAULT, GROW_ELEMENT_DEFAULT, NULL);
}

template <class T> DynamicVector<T>::DynamicVector(int InitialElementCount)
{
	Initialize(InitialElementCount, GROW_ELEMENT_DEFAULT, NULL);
}

template <class T> DynamicVector<T>::DynamicVector(T *DataBuffer, int DataBufferLength)
{
	Initialize(DataBufferLength, GROW_ELEMENT_DEFAULT, DataBuffer);
}

template <class T> DynamicVector<T>::DynamicVector(int InitialElementCount, int GrowElementCount)
{
	Initialize(InitialElementCount, GrowElementCount, NULL);
}

template <class T> DynamicVector<T>::DynamicVector(int GrowElementCount, T *DataBuffer, int DataBufferLength)
{
	Initialize(DataBufferLength, GrowElementCount, DataBuffer);
}

template <class T> DynamicVector<T>::~DynamicVector()
{
	if ((dataBuf != NULL) && localBufferManagement)
		delete [] dataBuf;
	if (next != NULL)
		delete next;
}

template <class T> void DynamicVector<T>::Initialize(int InitialElementCount, int GrowElementCount, T *DataBuffer)
{
	memset(this, 0, sizeof(DynamicVector<T>));
	if (DataBuffer != NULL)
	{
		dataBuf = DataBuffer;
		localBufferManagement = false;
		totalElements = curElements = maxElements = InitialElementCount;
	}
	else
	{
		dataBuf = new T[InitialElementCount];
		memset(dataBuf, 0, sizeof(T) * InitialElementCount);
		localBufferManagement = true;
		maxElements = InitialElementCount;
	}
	growElements = GrowElementCount;
}

template <class T> T DynamicVector<T>::Get(int Index)
{
	return (*this)[Index];
}

template <class T> void DynamicVector<T>::Set(int Index, T NewValue)
{
	(*this)[Index] = NewValue;
}

template <class T> T &DynamicVector<T>::operator[](int Index)
{
	if ((Index >= 0) && (Index < maxElements) && (Index < curElements))
		return dataBuf[Index];

	if (Index < 0)
		throw EarlabException("DynamicVector<T>: Negative index requested");

	if (Index >= maxElements)
	{
		if (next != NULL)
			return (*next)[Index - maxElements];
		else
			throw EarlabException("DynamicVector<T>: Attempted access past end of vector");
	}
	throw EarlabException("DynamicVector<T>: Attempted access past end of vector");
}

template <class T> void DynamicVector<T>::Add(T NewValue)
{
	if (dataBuf == NULL)
		throw EarlabException("DynamicVector<T>: Data buffer is NULL on Add()");
	if (curElements < maxElements)
		dataBuf[curElements++] = NewValue;
	else
	{
		if (next == NULL)
			next = new DynamicVector<T>(growElements, growElements);
		next->Add(NewValue);
	}
	totalElements++;
}

template <class T> int DynamicVector<T>::Length(void)
{
	return totalElements;
}

template <class T> void DynamicVector<T>::CopyData(T *DestinationBuffer, int ElementsToCopy)
{
	int i, CopyCount;

	CopyCount = ElementsToCopy < Length() ? ElementsToCopy : Length();
	for (i = 0; i < CopyCount; i++)
		DestinationBuffer[i] = (*this)[i];
}
