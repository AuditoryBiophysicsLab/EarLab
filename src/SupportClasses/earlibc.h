#ifndef _EARLIBC_H
#define _EARLIBC_H

#include "CMatrixN.h"
#include "CSpike.h"

extern TMatrixN_CLASS *matrix1_alloc(int n0);
extern TMatrixN_CLASS *matrix2_alloc(int n0, int n1);
extern TMatrixN_CLASS *matrix3_alloc(int n0, int n1, int n2);
extern TMatrixN_CLASS *matrix4_alloc(int n0, int n1, int n2, int n3);

extern TMatrixN       *matrixn_get(TMatrixN_CLASS *M);
extern TMatrixN_CLASS *matrixn_inst(TMatrixN *);
extern TMatrixN_CLASS *matrixn_init(TMatrixN *cm, int ndim, int rank[]);
extern TMatrixN_CLASS *matrixn_const(unsigned int ndim, unsigned int rank[], TMatrixN_T *data);
extern int             matrixn_rank(TMatrixN_CLASS *m, int dim);
extern unsigned int   matrixn_sizeof(int ndim, int rank[]);
extern TMatrixN_T     *matrixn_getdata(TMatrixN_CLASS *M);

extern Spike_CLASS    *spike_const(unsigned int nspike, TSpike *data);

#endif
