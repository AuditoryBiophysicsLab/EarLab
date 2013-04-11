/*
 * C Interface/wrapper for Earlib C++ Support classes
 */
#include <stdio.h>
/*
 * Include both C++ and C versions of MatrixN class/structures
 * N.B. TMatrixN_CPP is the C++ class
 *      TMatrixN_C   is the C equivalent of TMatrixN_CPP
 *      TMatrixN     is the C structure used for transport and initialization
 * This should be the only module that should be referencing both
 * TMatrixN_CPP and TMatrix_C so hopefully the confusion is
 * confined to this piece of code
 */

#include "MatrixN.h"
#include "CMatrixN.h"

extern "C" unsigned char *malloc(int);
extern "C" void exit(int);

extern "C" TMatrixN_CPP *
matrix1_alloc(int n) {
	unsigned char *mbuf;
	unsigned int  size;
	TMatrixN_CPP *mp;
	TMatrixN *cm;
	int            i, stride;

	size = sizeof(TMatrixN) + (n-1)*sizeof(MATRIXN_T);
	cm = (TMatrixN *)malloc(size);
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
	mp = new TMatrixN_CPP(mbuf, size);
	return(mp);
}

extern "C" TMatrixN_CPP *
matrix2_alloc(int n0, int n1) {
	unsigned char *mbuf;
	unsigned int  size;
	TMatrixN_CPP *mp;
	TMatrixN *cm;
	int            i, stride;

	size = sizeof(TMatrixN) + (n0*n1-1)*sizeof(MATRIXN_T);
	cm = (TMatrixN *)malloc(size);
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
	mp = new TMatrixN_CPP(mbuf, size);
	return(mp);
}

extern "C" TMatrixN_CPP *
matrix3_alloc(int n0, int n1, int n2) {
	unsigned char *mbuf;
	unsigned int  size;
	TMatrixN_CPP *mp;
	TMatrixN *cm;
	int            i, stride;

	size = sizeof(TMatrixN) + (n0*n1*n2-1)*sizeof(MATRIXN_T);
	cm = (TMatrixN *)malloc(size);
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
	mp = new TMatrixN_CPP(mbuf, size);
	return(mp);
}

extern "C" TMatrixN_CPP *
matrix4_alloc(int n0, int n1, int n2, int n3) {
	unsigned char *mbuf;
	unsigned int  size;
	TMatrixN_CPP *mp;
	TMatrixN *cm;
	int            i, stride;

	size = sizeof(TMatrixN) + (n0*n1*n2*n3-1)*sizeof(MATRIXN_T);
	cm = (TMatrixN *)malloc(size);
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
	mp = new TMatrixN_CPP(mbuf, size);
	return(mp);
}


extern "C" int
matrixn_rank(TMatrixN_CPP *M, unsigned int dim) {
	return(M->Rank(dim));
}


extern "C" MATRIXN_T *
matrixn_getdata(TMatrixN_CPP *M) {
	MATRIXN_T *data;
	unsigned int nelem;

	data = M->GetData(&nelem);
	return(data);
}

extern "C" TMatrixN *
matrixn_getbuf(TMatrixN_CPP *M) {
	unsigned int size;
	TMatrixN *cm;

	cm = (TMatrixN *) M->GetBuf(&size);
	/**
	  printf("getbuf size was %d\n", size);
	 **/
	return(cm);
}

extern "C" TMatrixN_CPP *
matrixn_inst(TMatrixN *cm) {
	TMatrixN_CPP *M;
	int i, n;
	unsigned int size;

	n = 1;
	for (i = 0; i < cm->mDimCount; ++i) {
		n *= cm->mDimensionRank[i];
	}
	size = sizeof(TMatrixN) + (n-1)*sizeof(MATRIXN_T);
	if (cm->length != size || cm->mElementCount != n) {
		fprintf(stderr,
			"matrixn_inst: initializer said %d elems in %d bytes, but computed %d elems in %d bytes\n",
			cm->mElementCount,  cm->length,
			n, size);
		exit(4);
	}
	M = new TMatrixN_CPP(cm, size);

	return(M);
}

extern "C" unsigned int
matrixn_sizeof(int ndim, int rank[]) {
	int i, nelem;
	unsigned int size;

	nelem  = 1;
	for (i = 0; i < ndim; ++i) {
		nelem *= rank[i];
	}
	size = sizeof(TMatrixN) + (nelem-1)*sizeof(MATRIXN_T);
	return(size);
}

extern "C" TMatrixN_CPP *
matrixn_init(TMatrixN *cm, int ndim, int rank[]) {
	TMatrixN_CPP *M;
	int i, nelem, stride;
	unsigned int size;

	cm->mDimCount = ndim;
	nelem         = 1;
	for (i = 0; i < ndim; ++i) {
		cm->mDimensionRank[i] = rank[i];
		nelem *= rank[i];
	}
	for ( ;i < CMATRIXN_MAXDIM; ++i) {
		cm->mDimensionRank[i] = 1;
	}
	size = sizeof(TMatrixN) + (nelem-1)*sizeof(MATRIXN_T);
	cm->length        = size;
	cm->mElementCount = nelem;

	stride = 1;
	for (i = ndim - 1; i >= 0; i--) {
		cm->mDimensionStride[i] = stride;
		stride *= cm->mDimensionRank[i];
	}

	M = new TMatrixN_CPP(cm, size);

	return(M);
}

extern "C" TMatrixN_CPP *
matrixn_const(unsigned int ndim, unsigned int rank[], MATRIXN_T *data) {
	TMatrixN_CPP *M;

	M = new TMatrixN_CPP(ndim, rank, data);

	return(M);
}

/*****************************************/
/* SPIKES                                */
/*****************************************/

#include "SpikeStream.h"
#include "CSpike.h"

extern "C" SpikeStream *
spike_const(unsigned int nspike, Spike *data) {
	SpikeStream *S;

	S = new SpikeStream(data, nspike, 1);

	return(S);
}
/*****************************************/
/* STREAMS                               */
/*****************************************/
#include "CEarlabDataStream.h"

