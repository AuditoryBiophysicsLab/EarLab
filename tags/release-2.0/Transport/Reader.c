/*
 * Reader module wrapper
 */

#define NIN  0
#define NOUT 1

#define MODULE Reader

#include "module.c"

INTERNAL char  Filename[EFI_FILENAME_SIZE];
INTERNAL FILE *File;
INTERNAL int   Nsample;
INTERNAL char  Instname[200] = { '\0' };;


Reader_SetName(char *instname) {
	if (instname) strcpy(Instname, instname);
	return(1);
}

typedef int RANK[MAX_INPUT][EFI_MAXDIM];

Reader_Start(int nin,  int intype[],  RANK inrank,
	     int nout, int outtype[], RANK outrank,
	     int outsize[]) {

	File = fopen(Filename, "r");
	if (!File) {
		printf("Reader_Start: open failed on %s\n", Filename);
		return(0);
	}
	/* Fudge this for now */
	Nsample = OutRank[0][0];
	outsize[0] = Nsample;

	return (File!=NULL);
}

Reader_Stop(void) {
	if (!File) return(0);

	fclose(File);

	File = NULL;

	return (1);
}

Reader_ReadParameters(char *filename) {
	printf("Reader parameter file %s\n", filename);

	/* Fudge this for now */
	strcpy(Filename, filename);
}

Reader_Advance(TMatrixN_CLASS *in[], TMatrixN_CLASS *out[]) {
	TMatrixN_CLASS *M;
	MATRIXN_T      *data;
	float           f;
	int             i, rc;

printf("reader: advancing\n");
	if (!File || feof(File)) return(0);

	M    = out[0];
	data = matrixn_getdata(M);

	for (i = 0; i < Nsample; ++i) {
		rc = fscanf(File, " %f", &f);
		*data++ = f;
		if (!rc) break;
	}

	data = matrixn_getdata(M);
printf("reader: read %d items (%f %f %f %f ...)\n", i, data[0], data[1], data[2], data[3]);

	return(i);
}


















