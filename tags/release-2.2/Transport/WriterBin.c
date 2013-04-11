/*
 * WriterBin module wrapper
 */

#define NIN  1
#define NOUT 0

#define MODULE WriterBin

#include "module.c"

#include <stdio.h>

typedef int RANK[MAX_INPUT][EFI_MAXDIM];

static FILE *fout = NULL;

WriterBin_SetName(char *instname) {
	return(1);
}

WriterBin_Start(int nin,  int intype[],  RANK inrank,
		int nout, int outtype[], RANK outrank,
		int outsize[]) {

	char filename[100];

	printf("%s: start in %d, out %d\n", MODULENAME, nin, nout);
	sprintf(filename, "%s.%d.out",
		(ExpName && ExpName[0])?ExpName:MODULENAME,
		getpid());

	fout = fopen(filename, "w");
	if (!fout) {
		fprintf(stderr,
			"%s.start: couldn't open output file \"%s\"\n",
			MODULENAME,
			filename);
		return(0);
	}
	return (1);
}

WriterBin_Stop(void) {
	if (fout) fclose(fout);
	fout = NULL;

	printf("%s: Stop\n", MODULENAME);

	return (1);
}

WriterBin_ReadParameters(char *filename, char *section) {
	printf("%s: processing parameter file %s:%s\n",
	       MODULENAME, filename, section);
	return (1);
}

WriterBin_Advance(TMatrixN_CLASS *in[], TMatrixN_CLASS *out[]) {
	TMatrixN_CLASS *in0;
	TMatrixN       *cm;
	MATRIXN_T      *data;
	int i, n;

	in0 = in[0];
	cm  = in0->mBuffer;
/****************
	printf("%s: advance in[0] at 0x%08x (0x%08x)\n", MODULENAME, in0, *in0);

	printf("%s: advance %d dimensions, %d elements\n",
		MODULENAME, in[0]->nDims, in[0]->nElements);
****************/

	printf("%s: advance with matrix %dx%dx%d\n",
		MODULENAME,
		cm->mDimensionRank[0],
		cm->mDimensionRank[1],
		cm->mDimensionRank[2]);


	data = cm->data;

	printf("%s: data at 0x%08x 0x%08x\n", MODULENAME, in0->mData, data);
	data = in0->mData;

	n = cm->mDimensionRank[0];
	if (cm->mDimensionRank[1]) n *= cm->mDimensionRank[1];
	if (cm->mDimensionRank[2]) n *= cm->mDimensionRank[2];
	if (cm->mDimensionRank[3]) n *= cm->mDimensionRank[3];

	if (fout)
	  fwrite(data, sizeof(MATRIXN_T), n, fout);
   /******
	for (i = 0; i < n; ++i) fprintf(fout, "%f\n", data[i]);
   ******/

	printf("%s: advanced\n", MODULENAME);

	return (1);
}


















