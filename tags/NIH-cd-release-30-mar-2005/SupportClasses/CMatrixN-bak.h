/*
 * C struct equivalent to a MatrixN
 */
#ifndef CMATRIXN_H
#define CMATRIXN_H

#define MATRIXN_T double

#define CMATRIXN_MAXDIM 10

typedef struct doubleMatrixN {
	unsigned  long length;
	unsigned  long mDimCount;
	unsigned  long mElementCount;
	unsigned  long mDimensionRank[CMATRIXN_MAXDIM];
	unsigned  long mDimensionStride[CMATRIXN_MAXDIM];
	MATRIXN_T *data;
} doubleMatrixN;

typedef struct {
	char mErrStr[100];
    /******
	bool mDoBoundsChecking, mDoNotAllowGetBuff, mDestructorDontFreeMemory;
     ****** replaced by following line: ******/
	unsigned int  flags;
	unsigned long mBufLen;
	unsigned long nDims, nElements;

	/* Note: Include all necessary fields in transfer buffer, and write a co*/
	unsigned long *mBufLenForTransport;
	unsigned long *mDimCount;
	unsigned long *mElementCount;
	unsigned long *mDimensionRank;          /* Rank of each dimension   */
	unsigned long *mDimensionStride;        /* Stride of each dimension */

	doubleMatrixN *mBuffer;
	MATRIXN_T     *mData;
} doubleMatrixN_CLASS;

/* Reminder to self: Come back and macroize these based on MATRIX_T */

typedef doubleMatrixN       TMatrixN;
typedef MATRIXN_T           TMatrixN_T;
typedef doubleMatrixN_CLASS TMatrixN_CLASS;










#endif
