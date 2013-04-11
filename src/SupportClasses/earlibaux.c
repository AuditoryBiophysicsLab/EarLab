/*
 * Auxillary C routines
 */
#include <stdio.h>

/*****************************************/
/* MatrixN                               */
/*****************************************/
#include "CMatrixN.h"

void
matrixn_printdims(FILE *f, TMatrixN_CLASS *M) {
	int i, n;

	n = M->nDims;
	fprintf(f, "[");
	for (i = 0; i < n; ++i)
	  fprintf(f, "%d%c", M->mBuffer->mDimensionRank[i], (i!=n-1)?'x':']');
}

/*****************************************/
/* STREAMS                               */
/*****************************************/
#include "CEarlabDataStream.h"

char *
StreamTypeName(int type) {
	switch(type) {
	  case EFI_STREAM_WAVEFORM:     return("waveform");
	  case EFI_STREAM_SPIKE:        return("spike");
	  case EFI_STREAM_RESERVED:     return("reserved");
	  case EFI_STREAM_NONE:         return("none");
	  default:                      return("*INVALID*");
	}
}

