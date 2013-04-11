/*
 * C struct equivalent to a MatrixN
 */
#ifndef CMATRIXN_H
#define CMATRIXN_H

#define MATRIXN_T double

#define CMATRIXN_MAXDIM 10

typedef struct doubleMatrixN {
	unsigned  int length;
	unsigned  int mDimCount;
	unsigned  int mElementCount;
	unsigned  int mDimensionRank[CMATRIXN_MAXDIM];
	unsigned  int mDimensionStride[CMATRIXN_MAXDIM];
	MATRIXN_T *data;
} doubleMatrixN;

typedef struct {
	char mErrStr[100];
    /******
	bool mDoBoundsChecking, mDoNotAllowGetBuff, mDestructorDontFreeMemory;
     ****** replaced by following line: ******/
	unsigned int  flags;
	unsigned int mBufLen;
	unsigned int nDims, nElements;

	/* Note: Include all necessary fields in transfer buffer, and write a co*/
	unsigned int *mBufLenForTransport;
	unsigned int *mDimCount;
	unsigned int *mElementCount;
	unsigned int *mDimensionRank;          /* Rank of each dimension   */
	unsigned int *mDimensionStride;        /* Stride of each dimension */

	doubleMatrixN *mBuffer;
	MATRIXN_T     *mData;
} doubleMatrixN_CLASS;

/* Reminder to self: Come back and macroize these based on MATRIX_T */

typedef doubleMatrixN       TMatrixN;
typedef MATRIXN_T           TMatrixN_T;
typedef doubleMatrixN_CLASS TMatrixN_CLASS;










#endif
