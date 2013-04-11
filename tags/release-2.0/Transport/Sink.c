/*
 * Sink module wrapper
 */

#define NIN  1
#define NOUT 0

#define MODULE Sink

#include "module.c"

#include <stdio.h>

static FILE *fout = NULL;
static char  Instname[200] = MODULENAME;

Sink_SetName(char *instname) {
	strcpy(Instname, instname);
	return(1);
}

typedef int RANK[MAX_INPUT][EFI_MAXDIM];

Sink_Start(int nin,  int intype[],  RANK inrank,
	   int nout, int outtype[], RANK outrank,
	   int outsize[]) {

	int  i, j;
	char filename[100];

	printf("%s: start in %d, out %d\n", Instname, nin, nout);
	for (i = 0; i < nin; ++i) {
		printf("input %d: [ ", i);
		for (j = 0; inrank[i][j]; ++j) printf("%d ", inrank[i][j]);
		printf("]\n");
	}
	for (i = 0; i < nout; ++i) outsize[i] = 0;

   /******
	sprintf(filename, "%s.%d.out",
		(Expname && Expname[0])?Expname:Instname,
		getpid());

	fout = fopen(filename, "w");
	if (!fout) {
		fprintf(stderr,
			"%s.start: couldn't open output file \"%s\"\n",
			Instname,
			filename);
		return(0);
	}
     ******/
	return (1);
}

Sink_Stop(void) {
	if (fout) fclose(fout);
	fout = NULL;

	printf("%s: Stop\n", Instname);

	return (1);
}

Sink_ReadParameters(char *filename, char *section) {
	printf("%s: processing parameter file %s:%s\n",
	       Instname, filename, section);
	return (1);
}

Sink_Advance(Stream *in[], Stream *out[]) {
	Stream        *in0;
	TMatrixN       *cm;
	MATRIXN_T      *data;
	int            i, n;

/****************
	in0 = in[0];
	cm  = in0->mBuffer;
	printf("%s: advance in[0] at 0x%08x (0x%08x)\n", Instname, in0, *in0);

	printf("%s: advance %d dimensions, %d elements\n",
		Instname, in[0]->nDims, in[0]->nElements);

	printf("%s: advance with matrix %dx%dx%d\n",
		Instname,
		cm->mDimensionRank[0],
		cm->mDimensionRank[1],
		cm->mDimensionRank[2]);


	data = cm->data;

	printf("%s: data at 0x%08x 0x%08x\n", Instname, in0->mData, data);
	data = in0->mData;

	n = cm->mDimensionRank[0];
	if (cm->mDimensionRank[1]) n *= cm->mDimensionRank[1];
	if (cm->mDimensionRank[2]) n *= cm->mDimensionRank[2];
	if (cm->mDimensionRank[3]) n *= cm->mDimensionRank[3];

	if (fout) for (i = 0; i < n; ++i) fprintf(fout, "%f\n", data[i]);
****************/

	printf("%s: advanced\n", Instname);

	return (1);
}


















