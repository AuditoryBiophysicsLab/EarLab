#ifndef MATRIX_N_H_INCLUDED
#define MATRIX_N_H_INCLUDED
#include <stdarg.h>
#include "Complex.h"

typedef struct _CmplxStruct 
{
    double real; 
    double imag;
} ComplexStruct;

template <class T> class MatrixN
{
public:
	MatrixN();

	MatrixN(unsigned long dim0);
	MatrixN(unsigned long dim0, unsigned long dim1);
	MatrixN(unsigned long dim0, unsigned long dim1, unsigned long dim2);
	MatrixN(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3);
	MatrixN(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3, unsigned long dim4);
	MatrixN(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3, unsigned long dim4, unsigned long dim5);
	MatrixN(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3, unsigned long dim4, unsigned long dim5, unsigned long dim6);
	MatrixN(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3, unsigned long dim4, unsigned long dim5, unsigned long dim6, unsigned long dim7);
	MatrixN(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3, unsigned long dim4, unsigned long dim5, unsigned long dim6, unsigned long dim7, unsigned long dim8);
	MatrixN(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3, unsigned long dim4, unsigned long dim5, unsigned long dim6, unsigned long dim7, unsigned long dim8, unsigned long dim9);

	MatrixN(unsigned long ndim, unsigned long rank[], T *data);
	MatrixN(void *Buf, unsigned long BufLen);

	~MatrixN();
	// Note: Glenn will pass a pointer to a data block to a new constructor, which must build a MatrixN correctly from it.
	// The destructor in this case cannot free the data block when called, so handle that correctly.

	void Init(unsigned long dim0);
	void Init(unsigned long dim0, unsigned long dim1);
	void Init(unsigned long dim0, unsigned long dim1, unsigned long dim2);
	void Init(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3);
	void Init(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3, unsigned long dim4);
	void Init(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3, unsigned long dim4, unsigned long dim5);
	void Init(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3, unsigned long dim4, unsigned long dim5, unsigned long dim6);
	void Init(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3, unsigned long dim4, unsigned long dim5, unsigned long dim6, unsigned long dim7);
	void Init(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3, unsigned long dim4, unsigned long dim5, unsigned long dim6, unsigned long dim7, unsigned long dim8);
	void Init(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3, unsigned long dim4, unsigned long dim5, unsigned long dim6, unsigned long dim7, unsigned long dim8, unsigned long dim9);

	void Init(unsigned long ndim, unsigned long rank[], T *data);

	T &Data(unsigned long dim0);
	T &Data(unsigned long dim0, unsigned long dim1);
	T &Data(unsigned long dim0, unsigned long dim1, unsigned long dim2);
	T &Data(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3);
	T &Data(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3, unsigned long dim4);
	T &Data(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3, unsigned long dim4, unsigned long dim5);
	T &Data(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3, unsigned long dim4, unsigned long dim5, unsigned long dim6);
	T &Data(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3, unsigned long dim4, unsigned long dim5, unsigned long dim6, unsigned long dim7);
	T &Data(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3, unsigned long dim4, unsigned long dim5, unsigned long dim6, unsigned long dim7, unsigned long dim8);
	T &Data(unsigned long dim0, unsigned long dim1, unsigned long dim2, unsigned long dim3, unsigned long dim4, unsigned long dim5, unsigned long dim6, unsigned long dim7, unsigned long dim8, unsigned long dim9);

	void EnableBoundsChecking(bool EnableFlag);

	unsigned long Rank(unsigned long Dimension);

	const void *GetBuf(unsigned long *BufLen);
	T *GetData(unsigned long *BufLen);
	const T *SetData(T *data);
	MatrixN<T> &operator=(MatrixN<T> &rhs);

private:
	void Init(unsigned long dim[]);
	void Cleanup(void);
	inline T &GetData(unsigned long dim[], int NumDimensions);

	char mErrStr[100];
	bool mDoBoundsChecking, mDoNotAllowGetBuff, mDestructorDontFreeMemory;
	unsigned long mBufLen;
	unsigned long nDims, nElements;

	// Note: Include all necessary fields in transfer buffer, and write a constructor to take a transfer buffer and produce a real MatrixN
	unsigned long *mBufLenForTransport;
	unsigned long *mDimCount;
	unsigned long *mElementCount;
	unsigned long *mDimensionRank;		// Rank of each dimension
	unsigned long *mDimensionStride;	// Stride of each dimension

	unsigned char *mBuffer;
	T *mData;
};

typedef MatrixN<int>     IntMatrixN;
typedef MatrixN<float>   FloatMatrixN;
typedef MatrixN<double>  DoubleMatrixN;
typedef MatrixN<DoubleComplex> ComplexMatrixN;

#endif // MATRIX_N_H_INCLUDED
