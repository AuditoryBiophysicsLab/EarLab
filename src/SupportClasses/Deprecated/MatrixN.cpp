#include "MatrixN.h"
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include "EarlabException.h"

using namespace std;

<<<<<<< .mine
//template IntMatrixN;
//template FloatMatrixN;
//template DoubleMatrixN;
//template ComplexMatrixN;
=======
#ifdef WIN32
template IntMatrixN;
template FloatMatrixN;
template DoubleMatrixN;
template ComplexMatrixN;
>>>>>>> .r504
#endif

template <class T> MatrixN<T>::MatrixN()
{
	memset(this, 0, sizeof(MatrixN));
}

template <class T> MatrixN<T>::MatrixN(unsigned int dim0)
{
	unsigned int dim[10];

	memset(this, 0, sizeof(MatrixN));

	dim[0] = dim0;

	nDims = 1;
	Init(dim);
}

template <class T> MatrixN<T>::MatrixN(unsigned int dim0, unsigned int dim1)
{
	unsigned int dim[10];

	memset(this, 0, sizeof(MatrixN));

	dim[0] = dim0;	dim[1] = dim1;

	nDims = 2;
	Init(dim);
}

template <class T> MatrixN<T>::MatrixN(unsigned int dim0, unsigned int dim1, unsigned int dim2)
{
	unsigned int dim[10];

	memset(this, 0, sizeof(MatrixN));

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;

	nDims = 3;
	Init(dim);
}

template <class T> MatrixN<T>::MatrixN(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3)
{
	unsigned int dim[10];

	memset(this, 0, sizeof(MatrixN));

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;

	nDims = 4;
	Init(dim);
}

template <class T> MatrixN<T>::MatrixN(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3, unsigned int dim4)
{
	unsigned int dim[10];

	memset(this, 0, sizeof(MatrixN));

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;
	dim[4] = dim4;

	nDims = 5;
	Init(dim);
}

template <class T> MatrixN<T>::MatrixN(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3, unsigned int dim4, unsigned int dim5)
{
	unsigned int dim[10];

	memset(this, 0, sizeof(MatrixN));

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;
	dim[4] = dim4;	dim[5] = dim5;

	nDims = 6;
	Init(dim);
}

template <class T> MatrixN<T>::MatrixN(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3, unsigned int dim4, unsigned int dim5, unsigned int dim6)
{
	unsigned int dim[10];

	memset(this, 0, sizeof(MatrixN));

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;
	dim[4] = dim4;	dim[5] = dim5;
	dim[6] = dim6;

	nDims = 7;
	Init(dim);
}

template <class T> MatrixN<T>::MatrixN(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3, unsigned int dim4, unsigned int dim5, unsigned int dim6, unsigned int dim7)
{
	unsigned int dim[10];

	memset(this, 0, sizeof(MatrixN));

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;
	dim[4] = dim4;	dim[5] = dim5;
	dim[6] = dim6;	dim[7] = dim7;

	nDims = 8;
	Init(dim);
}

template <class T> MatrixN<T>::MatrixN(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3, unsigned int dim4, unsigned int dim5, unsigned int dim6, unsigned int dim7, unsigned int dim8)
{
	unsigned int dim[10];

	memset(this, 0, sizeof(MatrixN));

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;
	dim[4] = dim4;	dim[5] = dim5;
	dim[6] = dim6;	dim[7] = dim7;
	dim[8] = dim8;

	nDims = 9;
	Init(dim);
}

template <class T> MatrixN<T>::MatrixN(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3, unsigned int dim4, unsigned int dim5, unsigned int dim6, unsigned int dim7, unsigned int dim8, unsigned int dim9)
{
	unsigned int dim[10];

	memset(this, 0, sizeof(MatrixN));

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;
	dim[4] = dim4;	dim[5] = dim5;
	dim[6] = dim6;	dim[7] = dim7;
	dim[8] = dim8;	dim[9] = dim9;

	nDims = 10;
	Init(dim);
}

template <class T> MatrixN<T>::MatrixN(unsigned int ndim, unsigned int dim[], T *data)
{
	memset(this, 0, sizeof(MatrixN));

	nDims = ndim;
	Init(ndim, dim, data);
}

template <class T> MatrixN<T>::MatrixN(void *Buf, unsigned int BufLen)
{
	unsigned char *CurPtr;
	T **datap;

	memset(this, 0, sizeof(MatrixN));

	mBuffer = (unsigned char *)Buf;
	mBufLen = BufLen;
	CurPtr = mBuffer;

	mBufLenForTransport = (unsigned int *)CurPtr;
	CurPtr += sizeof(unsigned int);

	mDimCount = (unsigned int *)CurPtr;
	nDims = *mDimCount;
	CurPtr += sizeof(unsigned int);

	mElementCount = (unsigned int *)CurPtr;
	nElements = *mElementCount;
	CurPtr += sizeof(unsigned int);

	mDimensionRank = (unsigned int *)CurPtr;
	CurPtr += 10 * sizeof(unsigned int);

	mDimensionStride = (unsigned int *)CurPtr;
	CurPtr += 10 * sizeof(unsigned int);

	datap = (T **)CurPtr;
	CurPtr += sizeof(T *);

	mData = (T *)CurPtr;
	*datap = mData;

	mDestructorDontFreeMemory = true;
}

template <class T> void MatrixN<T>::Init(unsigned int dim0)
{
	unsigned int dim[10];

	dim[0] = dim0;

	nDims = 1;
	Init(dim);
}

template <class T> void MatrixN<T>::Init(unsigned int dim0, unsigned int dim1)
{
	unsigned int dim[10];

	dim[0] = dim0;	dim[1] = dim1;

	nDims = 2;
	Init(dim);
}

template <class T> void MatrixN<T>::Init(unsigned int dim0, unsigned int dim1, unsigned int dim2)
{
	unsigned int dim[10];

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;

	nDims = 3;
	Init(dim);
}

template <class T> void MatrixN<T>::Init(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3)
{
	unsigned int dim[10];

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;

	nDims = 4;
	Init(dim);
}

template <class T> void MatrixN<T>::Init(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3, unsigned int dim4)
{
	unsigned int dim[10];

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;
	dim[4] = dim4;

	nDims = 5;
	Init(dim);
}

template <class T> void MatrixN<T>::Init(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3, unsigned int dim4, unsigned int dim5)
{
	unsigned int dim[10];

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;
	dim[4] = dim4;	dim[5] = dim5;

	nDims = 6;
	Init(dim);
}

template <class T> void MatrixN<T>::Init(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3, unsigned int dim4, unsigned int dim5, unsigned int dim6)
{
	unsigned int dim[10];

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;
	dim[4] = dim4;	dim[5] = dim5;
	dim[6] = dim6;

	nDims = 7;
	Init(dim);
}

template <class T> void MatrixN<T>::Init(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3, unsigned int dim4, unsigned int dim5, unsigned int dim6, unsigned int dim7)
{
	unsigned int dim[10];

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;
	dim[4] = dim4;	dim[5] = dim5;
	dim[6] = dim6;	dim[7] = dim7;

	nDims = 8;
	Init(dim);
}

template <class T> void MatrixN<T>::Init(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3, unsigned int dim4, unsigned int dim5, unsigned int dim6, unsigned int dim7, unsigned int dim8)
{
	unsigned int dim[10];

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;
	dim[4] = dim4;	dim[5] = dim5;
	dim[6] = dim6;	dim[7] = dim7;
	dim[8] = dim8;

	nDims = 9;
	Init(dim);
}

template <class T> void MatrixN<T>::Init(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3, unsigned int dim4, unsigned int dim5, unsigned int dim6, unsigned int dim7, unsigned int dim8, unsigned int dim9)
{
	unsigned int dim[10];

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;
	dim[4] = dim4;	dim[5] = dim5;
	dim[6] = dim6;	dim[7] = dim7;
	dim[8] = dim8;	dim[9] = dim9;

	nDims = 10;
	Init(dim);
}

template <class T> void MatrixN<T>::Init(unsigned int dim[])
{
	unsigned int curdim;
	unsigned int cursize, TotalSize;
	unsigned char *CurPtr;
	T             **datap;
	int i;

	if (nDims <= 0)
		throw EarlabException("Cannot declare a matrix with zero dimensions");

	if (nDims > 10)
		throw EarlabException("Cannot declare a matrix with more than 10 dimensions.  See Dave Anderson...");

	nElements = 1;
	for (curdim = 0; curdim < nDims; curdim++)
	{
		if (dim[curdim] == 0)
			throw EarlabException("MatrixN: Cannot declare a dimension size of zero");
		nElements *= dim[curdim];
	}

	Cleanup();

	TotalSize = 0;
	TotalSize += sizeof(unsigned int);	// Space for Buffer Length
	TotalSize += sizeof(unsigned int);	// Space for Dimension count
	TotalSize += sizeof(unsigned int);	// Space for Element count
	TotalSize += 10 * sizeof(unsigned int);	// Space for mDimensionRank array
	TotalSize += 10 * sizeof(unsigned int);	// Space for mDimensionStride array
	TotalSize += sizeof(T *);                // Space for data ptr

	TotalSize += sizeof(T) * nElements;	// Space for payload data

	mBuffer = new unsigned char [TotalSize];
	mBufLen = TotalSize;
	memset(mBuffer, 0, TotalSize);
	CurPtr = mBuffer;

	mBufLenForTransport = (unsigned int *)CurPtr;
	*mBufLenForTransport = mBufLen;
	CurPtr += sizeof(unsigned int);

	mDimCount = (unsigned int *)CurPtr;
	*mDimCount = nDims;
	CurPtr += sizeof(unsigned int);

	mElementCount = (unsigned int *)CurPtr;
	*mElementCount = nElements;
	CurPtr += sizeof(unsigned int);

	mDimensionRank = (unsigned int *)CurPtr;
	CurPtr += 10 * sizeof(unsigned int);

	mDimensionStride = (unsigned int *)CurPtr;
	CurPtr += 10 * sizeof(unsigned int);

	datap = (T **) CurPtr;
	CurPtr += sizeof(T *);

	mData = (T *)CurPtr;
	*datap = mData;

	for (curdim = 0; curdim < nDims; curdim++)
		mDimensionRank[curdim] = dim[curdim];

	cursize = 1;
	for (i = nDims - 1; i >= 0; i--)
	{
		mDimensionStride[i] = cursize;
		cursize *= mDimensionRank[i];
	}
}

template <class T> void MatrixN<T>::Init(unsigned int ndim, unsigned int dim[], T *data)
{
	unsigned int curdim;
	unsigned int cursize, TotalSize;
	unsigned char *CurPtr;
	T **datap;

	int i;

	if (ndim <= 0)
		throw EarlabException("Cannot declare a matrix with zero dimensions");

	if (ndim > 10)
		throw EarlabException("Cannot declare a matrix with more than 10 dimensions.  See Dave Anderson...");
	cout <<"MatrixN: initializing "<<ndim<<" dimensions"<<endl;
	nDims     = ndim;
	cout <<"MatrixN: set Ndims = "<<nDims<<endl;
	nElements = 1;
	for (curdim = 0; curdim < nDims; curdim++)
	{
	cout <<"MatrixN: dim " << curdim << ", rank " << dim[curdim] <<endl;
		if (dim[curdim] == 0)
			throw EarlabException("MatrixN: Cannot declare a dimension size of zero");
		nElements *= dim[curdim];
	}
	cout <<"MatrixN: initializing "<<nElements<<" elements"<<endl;

	Cleanup();

	TotalSize = 0;
	TotalSize += sizeof(unsigned int);     // Space for Buffer Length
	TotalSize += sizeof(unsigned int);     // Space for Dimension count
	TotalSize += sizeof(unsigned int);     // Space for Element count
	TotalSize += 10 * sizeof(unsigned int);        // Space for mDimensionRank array
	TotalSize += 10 * sizeof(unsigned int);        // Space for mDimensionStride array
	TotalSize += sizeof(T *);               // Space for data pointer

	mBuffer = new unsigned char [TotalSize];
	mBufLen = TotalSize;
	memset(mBuffer, 0, TotalSize);
	CurPtr = mBuffer;

	mBufLenForTransport = (unsigned int *)CurPtr;
	*mBufLenForTransport = mBufLen;
	CurPtr += sizeof(unsigned int);

	mDimCount = (unsigned int *)CurPtr;
	*mDimCount = nDims;
	CurPtr += sizeof(unsigned int);

	mElementCount = (unsigned int *)CurPtr;
	*mElementCount = nElements;
	CurPtr += sizeof(unsigned int);

	mDimensionRank = (unsigned int *)CurPtr;
	CurPtr += 10 * sizeof(unsigned int);

	mDimensionStride = (unsigned int *)CurPtr;
	CurPtr += 10 * sizeof(unsigned int);

	for (curdim = 0; curdim < nDims; curdim++)
		mDimensionRank[curdim] = dim[curdim];

	cursize = 1;
	for (i = nDims - 1; i >= 0; i--)
	{
		mDimensionStride[i] = cursize;
		cursize *= mDimensionRank[i];
	}

	datap = (T **) CurPtr;
	*datap = data;
	CurPtr += sizeof(T *);

	mData = data;
}

template <class T> unsigned int MatrixN<T>::Rank(unsigned int Dimension)
{
	if (Dimension < nDims)
		return mDimensionRank[Dimension];
	else
		throw EarlabException("MatrixN: Cannot get rank for requested dimension - matrix does not have that many dimensions");
}

template <class T> MatrixN<T>::~MatrixN()
{
	Cleanup();
}

template <class T> void MatrixN<T>::Cleanup(void)
{
	if ((mBuffer != NULL) && (mDestructorDontFreeMemory == false))
		delete [] mBuffer;
}

template <class T> void MatrixN<T>::EnableBoundsChecking(bool EnableFlag)
{
	mDoBoundsChecking = EnableFlag;
}

template <class T> T &MatrixN<T>::GetData(unsigned int dim[], int NumDimensions)
{
	int i;
	unsigned int CurOffset;

	if (mDoBoundsChecking == true)
	{
		if (nDims != NumDimensions)
			throw EarlabException("MatrixN: This matrix requires the use of %d dimension(s), but %d was provided", nDims, NumDimensions);
		for (i = 0; i < NumDimensions; i++)
			if (dim[i] >= mDimensionRank[i])
				throw EarlabException("MatrixN: Array bounds exceeded.  Dimension: %d, Max Index: %d, Requested Index: %d", i, mDimensionRank[i], dim[i]);
	}
	if (NumDimensions < 0)
		throw EarlabException("MatrixN: Negative dimensional arrays are not supported - please try another universe");
	if (NumDimensions == 0)
		throw EarlabException("MatrixN: Zero dimensional arrays are not supported - why not use a scalar?");
	CurOffset = 0;
	for (i = 0; i < NumDimensions; i++)
		CurOffset += mDimensionStride[i] * dim[i];
	return mData[CurOffset];
}

// One-dimensional accessor function
template <class T> T &MatrixN<T>::Data(unsigned int dim0)
{
	unsigned int dim[1];

	dim[0] = dim0;

	return GetData(dim, 1);
}

// Two-dimensional accessor function
template <class T> T &MatrixN<T>::Data(unsigned int dim0, unsigned int dim1)
{
	unsigned int dim[2];

	dim[0] = dim0;	dim[1] = dim1;

	return GetData(dim, 2);
}

// Three-dimensional accessor function
template <class T> T &MatrixN<T>::Data(unsigned int dim0, unsigned int dim1, unsigned int dim2)
{
	unsigned int dim[3];

	dim[0] = dim0;	dim[1] = dim1;	dim[2] = dim2;

	return GetData(dim, 3);
}

// Four-dimensional accessor function
template <class T> T &MatrixN<T>::Data(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3)
{
	unsigned int dim[4];

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;

	return GetData(dim, 4);
}

// Five-dimensional accessor function
template <class T> T &MatrixN<T>::Data(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3, unsigned int dim4)
{
	unsigned int dim[5];

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;
	dim[4] = dim4;

	return GetData(dim, 5);
}

// Six-dimensional accessor function
template <class T> T &MatrixN<T>::Data(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3, unsigned int dim4, unsigned int dim5)
{
	unsigned int dim[6];

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;
	dim[4] = dim4;	dim[5] = dim5;

	return GetData(dim, 6);
}

// Seven-dimensional accessor function
template <class T> T &MatrixN<T>::Data(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3, unsigned int dim4, unsigned int dim5, unsigned int dim6)
{
	unsigned int dim[7];

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;
	dim[4] = dim4;	dim[5] = dim5;
	dim[6] = dim6;

	return GetData(dim, 7);
}

// Eight-dimensional accessor function
template <class T> T &MatrixN<T>::Data(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3, unsigned int dim4, unsigned int dim5, unsigned int dim6, unsigned int dim7)
{
	unsigned int dim[8];

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;
	dim[4] = dim4;	dim[5] = dim5;
	dim[6] = dim6;	dim[7] = dim7;

	return GetData(dim, 8);
}

// Nine-dimensional accessor function
template <class T> T &MatrixN<T>::Data(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3, unsigned int dim4, unsigned int dim5, unsigned int dim6, unsigned int dim7, unsigned int dim8)
{
	unsigned int dim[9];

	dim[0] = dim0;	dim[1] = dim1;	dim[2] = dim2;
	dim[3] = dim3;	dim[4] = dim4;	dim[5] = dim5;
	dim[6] = dim6;	dim[7] = dim7;	dim[8] = dim8;

	return GetData(dim, 9);
}

// Ten-dimensional accessor function
template <class T> T &MatrixN<T>::Data(unsigned int dim0, unsigned int dim1, unsigned int dim2, unsigned int dim3, unsigned int dim4, unsigned int dim5, unsigned int dim6, unsigned int dim7, unsigned int dim8, unsigned int dim9)
{
	unsigned int dim[10];

	dim[0] = dim0;	dim[1] = dim1;
	dim[2] = dim2;	dim[3] = dim3;
	dim[4] = dim4;	dim[5] = dim5;
	dim[6] = dim6;	dim[7] = dim7;
	dim[8] = dim8;	dim[9] = dim9;

	return GetData(dim, 10);
}

template <class T> T *MatrixN<T>::GetData(unsigned int *BufLen)
{
	if (mDoNotAllowGetBuff)
		return NULL;
	else
	{
		*BufLen = nElements;
		return mData;
	}
}

template <class T> const T *MatrixN<T>::SetData(T *data)
{
	T *old;

	if (mDoNotAllowGetBuff)
		return NULL;
	else
	{
		old   = mData;
		mData = (T *)data;
		return old;
	}
}

template <class T> const void *MatrixN<T>::GetBuf(unsigned int *BufLen)
{
	if (mDoNotAllowGetBuff)
		return NULL;
	else
	{
		*BufLen = mBufLen;
		return (const T *)mBuffer;
	}
}

template <class T> MatrixN<T> &MatrixN<T>::operator=(MatrixN<T> &rhs)
{
	unsigned int i;

	if (rhs.nDims != nDims)
		throw EarlabException("MatrixN: Cannot copy from one array to another if arrays are not the same size.  Source dims: %d, dest dims: %d",
		rhs.nDims, nDims);
	for (i = 0; i < nDims; i++)
	{
		if (rhs.mDimensionRank[i] != mDimensionRank[i])
			throw EarlabException("MatrixN: Cannot copy from one array to another if arrays are not the same size.  Dimension %d, source size: %d, dest size: %d", 
			i, rhs.mDimensionRank[i], mDimensionRank[i]);
	}

	if (rhs.mData == NULL)
		throw EarlabException("MatrixN: Source matrix data pointer is NULL");
	if (mData == NULL)
		throw EarlabException("MatrixN: Destination matrix data pointer is NULL");
	memcpy(mData, rhs.mData, nElements * sizeof(T));
	return *this;
}

