/*
 * C struct equivalent to a MatrixN
 */
#ifndef _CMATRIXN_H
#define _CMATRIXN_H

/*
 * Define underlying data type of MatrixN elements
 */
#define MATRIXN_T float

#if (MATRIXN_T == float)
# define MATRIXN_C Float
#else
# define MATRIXN_C Double
#endif

/* Macros for constructing names */
#ifndef XCONCAT
# define XCONCAT(a,b)  a ## b
# define CONCAT(a,b)   XCONCAT(a,b)
#endif

/* C and C++ names for this object */
#define TMatrixN_C    CONCAT(MATRIXN_T,  MatrixN)
#define TMatrixN_CPP  CONCAT(MATRIXN_C,  MatrixN)

#define MATRIXN_STRUCT CONCAT(MATRIXN_T,MatrixN)
#define MATRIXN_CLASS  CONCAT(MATRIXN_T,MatrixN_CLASS)


#define CMATRIXN_MAXDIM 10

typedef struct MATRIXN_STRUCT {
	unsigned  int length;
	unsigned  int mDimCount;
	unsigned  int mElementCount;
	unsigned  int mDimensionRank[CMATRIXN_MAXDIM];
	unsigned  int mDimensionStride[CMATRIXN_MAXDIM];
	MATRIXN_T *data;
} MATRIXN_STRUCT;

typedef struct MATRIXN_CLASS {
	char mErrStr[100];
    /******
	bool mDoBoundsChecking, mDoNotAllowGetBuff, mDestructorDontFreeMemory;
     ****** replaced by following line: ******/
	unsigned int    flags;
	unsigned int   mBufLen;
	unsigned int   nDims, nElements;

	/* Note: Include all necessary fields in transfer buffer, and write a co*/
	unsigned int  *mBufLenForTransport;
	unsigned int  *mDimCount;
	unsigned int  *mElementCount;
	unsigned int  *mDimensionRank;         /* Rank of each dimension   */
	unsigned int  *mDimensionStride;       /* Stride of each dimension */

	MATRIXN_STRUCT *mBuffer;
	MATRIXN_T      *mData;
} MATRIXN_CLASS;

/*
 * Typedefs for the default MatrixN-related  objects
 */
typedef MATRIXN_STRUCT TMatrixN;
typedef MATRIXN_T      TMatrixN_T;
typedef MATRIXN_CLASS  TMatrixN_CLASS;

#endif
