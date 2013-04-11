/*
 * C Interface/wrapper for Earlib C++ Support classes
 */
#include <stdio.h>
/*
 * Include both C++ and C versions of MatrixN class/structures
 * N.B. DoubleMatrixN is the C++ class
 *      doubleMatrixN is the C structure used for transport and initialization
 *      doubleMatrixN_CLASS is the C equivalent of DoubleMatrixN
 * This should be the only module that should be referencing both
 * DoubleMatrixN and doubleMatrixN so hopefully the confusion is
 * confined to this piece of code
 */

#include "MatrixN.h"
#include "CMatrixN.h"

extern "C" unsigned char *malloc(int);
extern "C" void exit(int);

extern "C" DoubleMatrixN *
matrix1_alloc(int n) {
	unsigned char *mbuf;
	unsigned long  size;
	DoubleMatrixN *mp;
	doubleMatrixN *cm;
	int            i, stride;

	size = sizeof(doubleMatrixN) + (n-1)*sizeof(MATRIXN_T);
	cm = (doubleMatrixN *)malloc(size);
	cm->length = size;
	cm->mDimCount     = 1;
	cm->mElementCount = n;
	cm->mDimensionRank[0] = n;
	for (i = 1; i < CMATRIXN_MAXDIM; ++i) cm->mDimensionRank[i] = 1;

	stride = 1;
	for (i = CMATRIXN_MAXDIM - 1; i >= 0; i--)
	{
		cm->mDimensionStride[i] = stride;
		stride *= cm->mDimensionRank[i];
	}

	mbuf = (unsigned char *)cm;
	mp = new DoubleMatrixN(mbuf, size);
	return(mp);
}

extern "C" DoubleMatrixN *
matrix2_alloc(int n0, int n1) {
	unsigned char *mbuf;
	unsigned long  size;
	DoubleMatrixN *mp;
	doubleMatrixN *cm;
	int            i, stride;

	size = sizeof(doubleMatrixN) + (n0*n1-1)*sizeof(MATRIXN_T);
	cm = (doubleMatrixN *)malloc(size);
	cm->length = size;
	cm->mDimCount     = 2;
	cm->mElementCount = n0*n1;
	cm->mDimensionRank[0] = n0;
	cm->mDimensionRank[1] = n1;
	for (i = 2; i < CMATRIXN_MAXDIM; ++i) cm->mDimensionRank[i] = 1;

	stride = 1;
	for (i = CMATRIXN_MAXDIM - 1; i >= 0; i--)
	{
		cm->mDimensionStride[i] = stride;
		stride *= cm->mDimensionRank[i];
	}

	mbuf = (unsigned char *)cm;
	mp = new DoubleMatrixN(mbuf, size);
	return(mp);
}

extern "C" DoubleMatrixN *
matrix3_alloc(int n0, int n1, int n2) {
	unsigned char *mbuf;
	unsigned long  size;
	DoubleMatrixN *mp;
	doubleMatrixN *cm;
	int            i, stride;

	size = sizeof(doubleMatrixN) + (n0*n1*n2-1)*sizeof(MATRIXN_T);
	cm = (doubleMatrixN *)malloc(size);
	cm->length = size;
	cm->mDimCount     = 3;
	cm->mElementCount = n0*n1*n2;
	cm->mDimensionRank[0] = n0;
	cm->mDimensionRank[1] = n1;
	cm->mDimensionRank[2] = n2;
	for (i = 3; i < CMATRIXN_MAXDIM; ++i) cm->mDimensionRank[i] = 1;

	stride = 1;
	for (i = CMATRIXN_MAXDIM - 1; i >= 0; i--)
	{
		cm->mDimensionStride[i] = stride;
		stride *= cm->mDimensionRank[i];
	}

	mbuf = (unsigned char *)cm;
	mp = new DoubleMatrixN(mbuf, size);
	return(mp);
}

extern "C" DoubleMatrixN *
matrix4_alloc(int n0, int n1, int n2, int n3) {
	unsigned char *mbuf;
	unsigned long  size;
	DoubleMatrixN *mp;
	doubleMatrixN *cm;
	int            i, stride;

	size = sizeof(doubleMatrixN) + (n0*n1*n2*n3-1)*sizeof(MATRIXN_T);
	cm = (doubleMatrixN *)malloc(size);
	cm->length = size;
	cm->mDimCount     = 4;
	cm->mElementCount = n0*n1*n2*n3;
	cm->mDimensionRank[0] = n0;
	cm->mDimensionRank[1] = n1;
	cm->mDimensionRank[2] = n2;
	cm->mDimensionRank[3] = n3;
	for (i = 4; i < CMATRIXN_MAXDIM; ++i) cm->mDimensionRank[i] = 1;

	stride = 1;
	for (i = CMATRIXN_MAXDIM - 1; i >= 0; i--)
	{
		cm->mDimensionStride[i] = stride;
		stride *= cm->mDimensionRank[i];
	}

	mbuf = (unsigned char *)cm;
	mp = new DoubleMatrixN(mbuf, size);
	return(mp);
}


extern "C" int
matrixn_rank(DoubleMatrixN *M, unsigned long dim) {
	return(M->Rank(dim));
}


extern "C" MATRIXN_T *
matrixn_getdata(DoubleMatrixN *M) {
	MATRIXN_T *data;
	unsigned long nelem;

	data = M->GetData(&nelem);
	return(data);
}

extern "C" doubleMatrixN *
matrixn_getbuf(DoubleMatrixN *M) {
	unsigned long size;
	doubleMatrixN *cm;

	cm = (doubleMatrixN *) M->GetBuf(&size);
	/**
	  printf("getbuf size was %d\n", size);
	 **/
	return(cm);
}

extern "C" DoubleMatrixN *
matrixn_inst(doubleMatrixN *cm) {
	DoubleMatrixN *M;
	int i, n;
	unsigned long size;

	n = 1;
	for (i = 0; i < cm->mDimCount; ++i) {
		n *= cm->mDimensionRank[i];
	}
	size = sizeof(doubleMatrixN) + (n-1)*sizeof(MATRIXN_T);
	if (cm->length != size || cm->mElementCount != n) {
		fprintf(stderr,
			"matrixn_inst: initializer said %d elems in %d bytes, but computed %d elems in %d bytes\n",
			cm->mElementCount,  cm->length,
			n, size);
		exit(4);
	}
	M = new DoubleMatrixN(cm, size);

	return(M);
}

extern "C" unsigned long
matrixn_sizeof(int ndim, int rank[]) {
	int i, nelem;
	unsigned long size;

	nelem  = 1;
	for (i = 0; i < ndim; ++i) {
		nelem *= rank[i];
	}
	size = sizeof(doubleMatrixN) + (nelem-1)*sizeof(MATRIXN_T);
	return(size);
}

extern "C" DoubleMatrixN *
matrixn_init(doubleMatrixN *cm, int ndim, int rank[]) {
	DoubleMatrixN *M;
	int i, nelem, stride;
	unsigned long size;

	cm->mDimCount = ndim;
	nelem         = 1;
	for (i = 0; i < ndim; ++i) {
		cm->mDimensionRank[i] = rank[i];
		nelem *= rank[i];
	}
	for ( ;i < CMATRIXN_MAXDIM; ++i) {
		cm->mDimensionRank[i] = 1;
	}
	size = sizeof(doubleMatrixN) + (nelem-1)*sizeof(MATRIXN_T);
	cm->length        = size;
	cm->mElementCount = nelem;

	stride = 1;
	for (i = ndim - 1; i >= 0; i--) {
		cm->mDimensionStride[i] = stride;
		stride *= cm->mDimensionRank[i];
	}

	M = new DoubleMatrixN(cm, size);

	return(M);
}

extern "C" DoubleMatrixN *
matrixn_const(unsigned long ndim, unsigned long rank[], MATRIXN_T *data) {
	DoubleMatrixN *M;

	M = new DoubleMatrixN(ndim, rank, data);

	return(M);
}





























