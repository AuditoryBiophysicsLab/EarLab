/*
 * Data Playback Module
 */

#define NIN  1
#define NOUT 0

#define MODULE Playback

#include "module.c"

#include <stdio.h>
#include <strings.h>

#include "params.h"

typedef int RANK[MAX_INPUT][EFI_MAXDIM];

INTERNAL char  Name[200]     = { '\0' };
INTERNAL char  Instname[200] = { '\0' };
INTERNAL char  Expname[200]  = { '\0' };
INTERNAL char  Prefix[200];
INTERNAL FILE *Fout[MAX_OUTPUT];
INTERNAL FILE *Fmeta[MAX_OUTPUT];
INTERNAL int   Nin, Nout;

Playback_SetName(char *instname) {
	if (instname) strcpy(Instname, instname);
	return(1);
}

Playback_ReadParameters(char *filename, char *section) {
	int   i, n;
	char *p, *q;

	if (Instname[0]) sprintf(Name, "%s.%s", Instname, MODULENAME);
	else             strcpy(Name, MODULENAME);

	printf("%s: processing parameter file %s:%s\n",
	       Name, filename, section);

	ReadParameterFile(filename);

	/* Extract prefix */
	if (p = strrchr(filename, '/')) ++p; /* tail of file path */
	else p = filename;

	/* Start at end of string and scan backwards for a '.' */
	n = strlen(p);
	q = p + n;
	while (--q > p) if (*q == '.') break;
	if (q != p) n = q - p;  /* number of chars to copy */

	for (i = 0; i < n; ++i) Prefix[i] = *p++;
	Prefix[i] = '\0';

	printf("%s.ReadParameters: set prefix to %s\n",
	       Name, Prefix);

	return (1);
}

Playback_Start(int nin,  int intype[],  RANK inrank,
	       int nout, int outtype[], RANK outrank,
	       int outsize[]) {
	char  filename[500], buf[500], *p;
	int   i, rc;
	PARAM param;

	Nin  = nin;
	Nout = nout;
	printf("%s: start in %d, out %d\n", Name, Nin, Nout);

	if (GetSectionParameter("Model", "ExperimentName", &param))
	   strcpy(Prefix, param.value.data.vstring);
/**** HACK HACK HACK until i figure out the right way to do this ******/
	else strcpy(Prefix, ExpName);

	for (i = 0; i < Nout; ++i) {
		/*
		 * Construct and open data file
		 */
		sprintf(filename,
			"%s.%s.%d.out",
			Prefix, Instname, i+1);

		Fout[i] = fopen(filename, "r");
		if (!Fout[i]) {
			fprintf(stderr,
				"%s.start: ****ERROR**** open failed for %s\n",
				Name, filename);
			fflush(stderr);
			return(0);
		}

		/*
		 * Construct and open correspodning meta
		 */
		sprintf(filename,
			"%s.%s.%d.meta",
			Prefix, Instname, i+1);
		Fmeta[i] = fopen(filename, "r");
		if (!Fout[i]) {
			fprintf(stderr,
				"%s.start: ****ERROR**** open failed for %s\n",
				Name, filename);
			fflush(stderr);
			return(0);
		}
		/*
		 * Read sizing information
		 */
		fgets(buf, sizeof(buf), Fmeta[i]);
		printf("%s: Output %d %s", Name, i+1, buf);
		p = buf;
		while (*p && *p != ']') ++p;
		if (!*p) {
			fprintf(stderr,
				"%s.start: ****ERROR**** couldn't read dimension from %s\n",
				Name, filename);
			return(0);
		}
		++p;
		sscanf(p, " %d", &outsize[i]);
	}

	Frame = 0;
	return (1);
}

Playback_Stop(void) {
	int i;

	for (i = 0; i < Nout; ++i) {
		fclose(Fout[i]);
		fclose(Fmeta[i]);
	}

	printf("%s: Stopped\n", Name);

	return (1);
}

Playback_Advance(Stream *input[], Stream *output[]) {
	Stream   *stream;
	int       i, n;
	int       frame, nitem = 1;
	char     *tname;
	char      buf[100];

	printf("%s.advance: frame %d\n", Name, Frame);

	for (i = 0; i < Nout; ++i) {
		fgets(buf, sizeof(buf), Fmeta[i]);
		sscanf(buf, " %d %d", &frame, &nitem);
		if (frame != Frame) {
			fprintf(stderr,
				"%s: ****ERROR**** Metafile frame number (%d) mismatch on frame %d\n",
				Name, frame, Frame);
		}

		stream = output[i];

		switch (stream->datatype) {
		  case EFI_STREAM_WAVEFORM: {
			TMatrixN_CLASS *cm;
			TMatrixN_T     *data;

			tname = "wave samples";

			cm   = stream->data.waveform;
			data = cm->mData;

			fread(data, sizeof(TMatrixN_T), nitem, Fout[i]);

		  } break;

		  case EFI_STREAM_SPIKE: {
			Spike_CLASS *cs;
			TSpike     *data;

			tname = "spikes";

			cs   = stream->data.spike;
			data = cs->Spikes;

			fread(data, sizeof(TSpike), nitem, Fout[i]);

			cs->CurSpikes = nitem;
		  } break;

		  case EFI_STREAM_NONE:
			fprintf(stderr, "%s: ****ERROR**** NO DATA STREAM", Name);
			return(0);

		  default:
			fprintf(stderr,
				"%s: ****ERROR**** Invalid stream type %d",
				Name, stream->datatype);
			return(0);
		}
		printf("%s.advance: frame %d, output.%d %d %s\n",
			Name, Frame, i, nitem, tname);

	}

	if (nitem == 0) return(1);
	else            return(nitem);
}







