/*
 * Writer module wrapper
 */

#define NIN  1
#define NOUT 0

#define MODULE Writer

#include "module.c"

#include <stdio.h>

#include "params.h"

static char  Name[200] = { '\0' };;
static FILE *Fout = NULL;
static char  Instname[200] = { '\0' };;
static int   WriteOutput    = 1;
static int   SkipFirstFrame = 0;
static int   Binary         = 0;

Writer_SetName(char *instname) {
	if (instname) strcpy(Instname, instname);
	return(1);
}

typedef int RANK[MAX_INPUT][EFI_MAXDIM];

Writer_Start(int nin,  int intype[],  RANK inrank,
	     int nout, int outtype[], RANK outrank,
	     int outsize[]) {

	char  filename[100];
	int   rc;
	PARAM param;

	if (Instname[0]) sprintf(Name, "%s.%s", Instname, MODULENAME);
	else             strcpy(Name, MODULENAME);

	printf("%s: start in %d, out %d\n", Name, nin, nout);

	if (GetSectionParameter(Instname, "ProduceOutputFile", &param))
	   WriteOutput = param.value.data.vint;

	if (!WriteOutput) {
		printf("%s: Supressing output\n", Name);
		Fout = NULL;
	}

	rc = GetSectionParameter(Instname, "OutputFileName", &param);
	if (rc) {
		sprintf(filename, param.value.data.vstring, getpid());
	}
	else {
		sprintf(filename, "%s.%d.out",
			(ExpName && ExpName[0])?ExpName:MODULENAME,
			getpid());
	}

	Fout = fopen(filename, "w");
	if (!Fout) {
		fprintf(stderr,
			"%s.start: couldn't open output file \"%s\"\n",
			Name, filename);
		return(0);
	}
	printf("%s: Opened output file %s\n", Name, filename);

	if (GetSectionParameter(Instname, "SkipFirstFrame", &param))
	   SkipFirstFrame = param.value.data.vint;

	if (GetSectionParameter(Instname, "BinaryOutput", &param))
	   Binary = param.value.data.vint;


	Frame = 0;
	return (1);
}

Writer_Stop(void) {
	if (Fout) fclose(Fout);
	Fout = NULL;

	printf("%s: Stopped\n", Name);

	return (1);
}

Writer_ReadParameters(char *filename, char *section) {
	if (Instname[0]) sprintf(Name, "%s.%s", Instname, MODULENAME);
	else             strcpy(Name, MODULENAME);

	printf("%s: processing parameter file %s:%s\n",
	       Name, filename, section);

	ReadParameterFile(filename);
	return (1);
}

Writer_Advance(Stream *inputs[], Stream *outputs[]) {
	Stream   *stream;
	int       i, n;
	char     *tname;

	stream = inputs[0];

	if (!Fout) {
		printf("%s.advance: sinking frame %d\n", Name, Frame);
		return(1);
	}

	if (SkipFirstFrame && Frame == 1) {
		printf("%s.advance: skipping frame 1\n", Name);
		return(1);
	}

printf("%s.advance: frame %d, checking stream type %d\n",
	Name, Frame, stream->datatype);

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
			fwrite(data, sizeof(TMatrixN_T), n, Fout);
		}
		else for (i = 0; i < n; ++i) fprintf(Fout, "%f\n", data[i]);

	  } break;

	  case EFI_STREAM_SPIKE: {
		Spike_CLASS *cs;
		TSpike      *data;

		tname = "spikes";

		cs   = stream->data.spike;
		data = cs->Spikes;

		n = cs->CurSpikes;

printf("%s.advance: frame %d writing %d spikes\n", Name, Frame, n);
		if (Binary && n) {
			fwrite(data, sizeof(TSpike), n, Fout);
		}
		else for (i = 0; i < n; ++i)
		   fprintf(Fout, "%d %d %f\n", Frame, data[i].cell, data[i].time);
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
	       Name, Fout?"wrote":"sank", n, tname, Frame);

	return (1);
}
