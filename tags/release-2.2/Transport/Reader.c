/*
 * Reader module wrapper
 */

#define NIN  0
#define NOUT 1

#define MODULE Reader

#include "module.c"

INTERNAL char  Name[200]     = { '\0' };;
INTERNAL char  Instname[200] = { '\0' };;
INTERNAL char  Filename[EFI_FILENAME_SIZE];
INTERNAL FILE *File;
INTERNAL int   Nsample;
INTERNAL int   Binary  = 0;
INTERNAL float SampleRate;

Reader_SetName(char *instname) {
	if (instname) strcpy(Instname, instname);
	return(1);
}

typedef int RANK[MAX_INPUT][EFI_MAXDIM];

Reader_Start(int nin,  int intype[],  RANK inrank,
	     int nout, int outtype[], RANK outrank,
	     int outsize[]) {
	char wd[2000];

	if (Instname[0]) sprintf(Name, "%s.%s", Instname, MODULENAME);
	else             strcpy(Name, MODULENAME);

	getcwd(wd, sizeof(wd));

	GetSectionParameterString(Instname, "InputFileName", Filename);
	GetSectionParameterBool(Instname,   "BinaryInput",   &Binary);
	GetSectionParameterFloat(Instname,  "SampleRate",    &SampleRate);

	printf("%s_start: Input file name \"%s\"\n", Name, Filename);

	File = fopen(Filename, "r");
	if (!File) {
		printf("%s_Start: open failed on %s in %s\n", Name, Filename, wd);
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
	printf("%s: parameter file %s\n", Name, filename);

	/* Fudge this for now */
	if (filename) strcpy(Filename, filename);
	else Filename[0] = '\0';
}

Reader_Advance(Stream *in[], Stream *out[]) {
	Stream         *stream;
	char           *tname;
	float           f;
	int             i, n, rc;

printf("%s_advance: advancing\n", Name);
	if (!File || feof(File)) return(0);

	n  = Nsample;

	stream = out[0];

	switch (stream->datatype) {
	  case EFI_STREAM_WAVEFORM: {
		MATRIXN_CLASS *cm;
		TMatrixN_T    *data;

		tname = "wave samples";

		cm   = stream->data.waveform;
		data = cm->mData;

		n = cm->mDimensionRank[0];
		if (cm->mDimensionRank[1]) n *= cm->mDimensionRank[1];
		if (cm->mDimensionRank[2]) n *= cm->mDimensionRank[2];
		if (cm->mDimensionRank[3]) n *= cm->mDimensionRank[3];

		if (Binary) {
			fread(data, sizeof(TMatrixN_T), n, File);
		}
		else for (i = 0; i < n; ++i) {
			fscanf(File, " %f", &f);
			data[i] = f;
		}

	  } break;

	  case EFI_STREAM_SPIKE: {
		Spike_CLASS *cs;
		TSpike      *data;

		tname = "spikes";

		cs   = stream->data.spike;
		data = cs->Spikes;

		n = cs->CurSpikes;

		if (Binary && n) {
			fread(data, sizeof(TSpike), n, File);
		}
		else for (i = 0; i < n; ++i) {
			fscanf(File, " %d %d %f\n", Frame, data[i].cell, &f);
			data[i].time = f;
		}
	  } break;

	  case EFI_STREAM_NONE:
		fprintf(stderr, "%s: NO DATA STREAM", Name);
		break;

	  default:
		fprintf(stderr,
			"%s: *******INVALID STREAM TYPE %d",
			Name, stream->datatype);
		break;
	}

	printf("%s: advanced - %s %d %s on frame %d\n",
	       Name, "read", n, tname, Frame);

	return(i);
}
