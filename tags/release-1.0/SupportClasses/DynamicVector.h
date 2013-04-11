#ifndef __DYNAMIC_VECTOR_H_INCLUDED
#define __DYNAMIC_VECTOR_H_INCLUDED
#include "Complex.h"

template <class T> class DynamicVector
{
public:
	DynamicVector();
	DynamicVector(int InitialElementCount);
	DynamicVector(T *DataBuffer, int DataBufferLength);
	DynamicVector(int InitialElementCount, int GrowElementCount);
	DynamicVector(int GrowElementCount, T *DataBuffer, int DataBufferLength);
	~DynamicVector();

	T Get(int Index);
	void Set(int Index, T NewValue);
	void Add(T NewValue);

	int Length(void);

	inline T &operator[](int Index);

	void CopyData(T *DestinationBuffer, int ElementsToCopy);
private:
	void Initialize(int InitialElementCount, int GrowElementCount, T *DataBuffer);
	int curElements, maxElements, growElements, totalElements;
	bool localBufferManagement;
	DynamicVector *next;
	T *dataBuf;
};

typedef DynamicVector<int>				IntVector;
typedef DynamicVector<float>			FloatVector;
typedef DynamicVector<double>			DoubleVector;
#endif
