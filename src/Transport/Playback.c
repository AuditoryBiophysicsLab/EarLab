/*
 * Data Playback Module
 */

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
INTERNAL int   StartFrame, EndFrame, NFrame;
INTERNAL int   DataFrame = 0;
INTERNAL int   Loop;
INTERNAL int   Iter = 0;

INTERNAL BYTE *Buf0[MAX_OUTPUT], *Buf1[MAX_OUTPUT];
INTERNAL int   Inbuf = 0;
INTERNAL int   Bufelems0, Bufelems1;
INTERNAL int   OutType[MAX_OUTPUT];

INTERNAL int   Done = 0;
INTERNAL int   DoneWait;
INTERNAL int   BufFilled, BufAvail;
INTERNAL int   ReaderId = 0;

INTERNAL void BufReader(void);
INTERNAL int  ReadMetaHead(int inarg, char type[], int dims[], int *maxsize);
INTERNAL int  ReadMeta(int frame, int inarg);
INTERNAL int  ReadData(int frame, int inarg, Stream *stream, int nitem);

Playback_SetName(char *instname) {
	if (instname) strcpy(Instname, instname);
	return(1);
}

Playback_ReadParameters(char *filename, char *section) {
	int   i, n;
	char *p, *q;
	char  strbuf[4000];

	if (Instname[0]) sprintf(Name, "%s.%s", Instname, MODULENAME);
	else             strcpy(Name, MODULENAME);

	printf("%s: processing parameter file %s:%s\n",
	       Name, filename?filename:"[NULL]", section);

	ReadParameterFile(filename);

	if (filename == NULL ) {
		if ( GetSectionParameterString("Model",   "ParameterFile", strbuf) ||
		     GetSectionParameterString(Instname, "ParameterFile", strbuf) )
		      filename = strbuf;
		else return(1);
		printf("Found section parameter ParameterFile: %s\n", strbuf);
	}


	/* Extract run name prefix from parameter file name base */
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
	char  filename[500], buf[500], dir[1000], *p;
	int   i, j, rc, elemsize, nelem, nbyte;
	int   dims[20], ndim, maxsize;
	char  dtype[100];

	Nin  = nin;
	Nout = nout;

	if (Verbose)
	   printf("%s.start: in=%d, out=%d\n", Name, Nin, Nout);

	if ( GetSectionParameterString(Instname, "PlaybackDirectory", dir) ||
	     GetSectionParameterString("Model",  "PlaybackDirectory", dir) ) {
		i = strlen(dir);
		if (dir[i-1] != '/') {
			dir[i]   = '/';
			dir[i+1] = '\0';
		}
	}
	else dir[0] = '\0';

	/* Get the experiment name to use as a prefix */
	if (!GetSectionParameterString(Instname, "PlaybackExperiment", Prefix) &
	    !GetSectionParameterString("Model", "PlaybackExperiment", Prefix) &&
	    !GetSectionParameterString("Model", "ExperimentName", Prefix) )
	     strcpy(Prefix, ExpName);

	for (i = 0; i < Nout; ++i) {
		OutType[i] = outtype[i];
		/*
		 * Construct and open data file
		 */
		sprintf(filename,
			"%s%s.%s.%d.out",
			dir, Prefix, Instname, i+1);

		Fout[i] = fopen(filename, "r");
		if (!Fout[i]) {
			fprintf(stderr,
				"%s.start: ****ERROR**** open failed for %s\n",
				Name, filename);
			fflush(stderr);
			return(0);
		}

		/*
		 * Construct and open corresponding meta file
		 */
		sprintf(filename,
			"%s%s.%s.%d.meta",
			dir, Prefix, Instname, i+1);
		Fmeta[i] = fopen(filename, "r");
		if (!Fout[i]) {
			fprintf(stderr,
				"%s.start: ****ERROR**** open failed for %s\n",
				Name, filename);
			fflush(stderr);
			return(0);
		}

		/*
		 * Read Metafile sizing information
		 */
		ndim = ReadMetaHead(i, dtype, dims, &maxsize);

     printf("%s.Start: outsize[%d] = %d, maxsize = %d\n", Name, i, outsize[i], maxsize);

		/* Allocate buffers */
		nelem = outsize[i];

		if (strmatch("wave*", dtype)) elemsize = sizeof(TMatrixN_T);
		else                          elemsize = sizeof(TSpike);

		nbyte = nelem * elemsize;

		Buf0[i] = (BYTE *)malloc(2*nbyte);
		if (!Buf0[i]) {
			fprintf(stderr, "%s: can't allocate %d bytes for output %d buffer\n",
				Name, nbyte, i);

			return(0);
		}
		Buf1[i] = Buf0[i] + nbyte;
	}

	/* Get start and ending frames (if given) */
	StartFrame = 1;
	EndFrame   = 0;

	GetSectionParameterInt(Instname, "StartFrame", &StartFrame) ||
	  GetSectionParameterInt("Model",  "StartFrame", &StartFrame);

	GetSectionParameterInt(Instname, "EndFrame", &EndFrame) ||
	  GetSectionParameterInt("Model",  "EndFrame", &EndFrame);

	NFrame = EndFrame - StartFrame + 1;

	/* Get looping flag */
	Loop = 0;

	GetSectionParameterInt(Instname, "Loop", &Loop) ||
	  GetSectionParameterInt("Model",  "Loop", &Loop);

	DataFrame = 0;

	Frame = 0;
	Done  = 0;
	BufAvail  = mp_allocsema();
	BufFilled = mp_allocsema();
	DoneWait  = mp_allocsema();

	mp_incsema(BufAvail);
	mp_incsema(BufAvail);


	ReaderId = mp_task(BufReader, 0, 0);
	return (1);
}

INTERNAL void
BufReader(void) {
	int    i, ibuf = 0;
	int    frame, nitem;
	int    type, elemsize;
	int    eof;
	char  *tname;
	char   inp[100];
	BYTE  *buf;

	eof = 0;
	while (!Done) {
		mp_decsema(BufAvail);

		if (Done) break;

		++DataFrame; /* increment frame number */

		for (i = 0; i < Nout; ++i) {
			nitem = ReadMeta(DataFrame, i);
			if (nitem == 0) { /* EOF */
				eof = 1;
				break;
			}

			if (ibuf) { buf = Buf1[i]; Bufelems1 = nitem; }
			else      { buf = Buf0[i]; Bufelems0 = nitem; }

			type = OutType[i];
			switch (type) {
			  case EFI_STREAM_WAVEFORM:
				tname    = "wave samples";
				elemsize = sizeof(TMatrixN_T);
				break;

			  case EFI_STREAM_SPIKE:
				tname    = "spikes";
				elemsize = sizeof(TMatrixN_T);
				break;

			  case EFI_STREAM_NONE:
				fprintf(stderr, "%s: ****ERROR**** NO DATA STREAM", Name);
				exit(1);

			  default:
				fprintf(stderr,
					"%s: ****ERROR**** Invalid stream type %d",
					Name, type);
				exit(1);
			}
			if (nitem) read(fileno(Fout[i]), buf, nitem*elemsize);

			if (Verbose || Debug)
			   printf("%s.readbuf: frame %d (%d), output.%d %d %s (buf %d)\n",
				   Name, DataFrame, Frame, i, nitem, tname, ibuf);
		}
		ibuf = !ibuf;

		mp_incsema(BufFilled);

		for (i = 0; !eof && i < Noutput; ++i) {
			eof = feof(Fmeta[i]) || feof(Fout[i]);
		}

		if (Loop && (eof || (EndFrame && DataFrame >= EndFrame))) {
			char type[100];
			int  dims[20], ndim, size;

			for (i = 0; i < Noutput; ++i) {
				rewind(Fmeta[i]);
				ndim = ReadMetaHead(i, type, dims, &size);

				rewind(Fout[i]);
			}
		    /** if (Verbose) **/
			   printf("%s: Rewinding data files at %d\n",
				   Name, DataFrame);

			DataFrame = 0;
			eof       = 0;
			++Iter;
		}
		else if (eof) break;
	}

	mp_incsema(DoneWait);
}

Playback_Stop(void) {
	int i;

	for (i = 0; i < Nout; ++i) {
		fclose(Fout[i]);
		fclose(Fmeta[i]);
	}
	Done = 1;
	mp_incsema(BufAvail);
	mp_incsema(BufAvail);

	mp_decsema(DoneWait);

	mp_freesema(BufFilled);
	mp_freesema(BufAvail);
	mp_freesema(DoneWait);
	ReaderId = 0;

	printf("%s: Stopped\n", Name);

	return (1);
}

Playback_Advance(Stream *input[], Stream *output[]) {
	Stream   *stream;
	int       i, n;
	int       nitem, itemsize;
	char     *tname;
	BYTE     *buf;

	if (Verbose || Debug) printf("%s.advance: frame %d\n", Name, Frame);

	mp_decsema(BufFilled);
	for (i = 0; i < Nout; ++i) {
		if (Inbuf == 1) {
			buf = Buf1[i];
			nitem = Bufelems1;
		}
		else {
			buf   = Buf0[i];
			nitem = Bufelems0;
		}

		stream = output[i];

		switch (stream->datatype) {
		  case EFI_STREAM_WAVEFORM: {
			TMatrixN_CLASS *cm;
			TMatrixN_T     *data;

			tname = "wave samples";

			cm   = stream->data.waveform;
			data = cm->mData;

			memcpy(data, buf, nitem*sizeof(TMatrixN_T));

		  } break;

		  case EFI_STREAM_SPIKE: {
			Spike_CLASS *cs;
			TSpike     *data;

			tname = "spikes";

			cs   = stream->data.spike;
			data = cs->Spikes;

			memcpy(data, buf, nitem*sizeof(TSpike));

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
		if (Verbose || Debug)
		   printf("%s.advance: frame %d, output.%d %d %s (buf %d)\n",
			   Name, Frame, i, nitem, tname, Inbuf);
	}
	Inbuf = !Inbuf;

	mp_incsema(BufAvail);

	if (nitem == 0) return(1);
	else            return(nitem);
}

INTERNAL int
ReadMetaHead(int inarg, char type[], int dims[], int *maxsize) {
	int   ndim;
	char *p, *q, buf[500];

	p = fgets(buf, sizeof(buf), Fmeta[inarg]);
	if (!p) {
		fprintf(stderr,
			"%s.start: ****ERROR**** couldn't read dimension from Metafile %d\n",
			 Name, inarg);
		exit(4);
	}

	/* Extract file type name */
	p = buf; q = type;
	while (*p  && iswhite(*p)) ++p;
	while (*p &&  !iswhite(*p)) *q++ = *p++;
	*q = '\0';

	if (!*p) {
		fprintf(stderr,
			"%s.start: ****ERROR**** bad Metafile %d header type: %s at \"%s\"\n",
			 Name, inarg, buf, p);
		exit(4);
	}

	/* Extract dimensions */
	while (*p  && iswhite(*p)) ++p;
	if (!*p || *p != '[') {
		fprintf(stderr,
			"%s.start: ****ERROR**** bad Metafile %d header dims: %s at \"%s\"\n",
			 Name, inarg, buf, p);
		exit(4);
	}
	++p;

	ndim = 0;
	while (*p && *p != ']') {
		int x;

		while (*p && iswhite(*p)) ++p;
		if (!*p || !isdigit(*p)) {
			fprintf(stderr,
				"%s.start: ****ERROR**** bad Metafile %d header dims: %s at \"%s\"\n",
				 Name, inarg, buf, p);
			exit(4);
		}

		/* Get next dimension */
		x = 0;
		while (*p && isdigit(*p)) x = 10*x + *p++ - '0';
		dims[ndim++] = x;

		while (*p && iswhite(*p)) ++p;
	}

	if (!*p || *p != ']') {
		fprintf(stderr,
			"%s.start: ****ERROR**** bad Metafile %d header dims: %s at \"%s\"\n",
			 Name, inarg, buf, p);
		exit(4);
	}
	++p;

	sscanf(p, " %d", maxsize);

	return(ndim);
}

INTERNAL int
ReadMeta(int frame, int inarg) {
	char buf[100], *sp;
	int  mframe, nitem;

	sp = fgets(buf, sizeof(buf), Fmeta[inarg]);
	if (!sp) return(0);
	sscanf(buf, " %d %d", &mframe, &nitem);
	if (mframe != frame) {
		fprintf(stderr,
			"%s: ****ERROR**** Metafile frame number (%d) mismatch on frame %d\n",
			Name, mframe, frame);
		exit(4);
	       /*** no return ***/ return(0);
	}

	return(nitem);
}

INTERNAL int
ReadData(int frame, int inarg, Stream *stream, int nitem) {
	char *tname;
	int   rc;

	if (!nitem) return(1);

	switch (stream->datatype) {
	  case EFI_STREAM_WAVEFORM: {
		TMatrixN_CLASS *cm;
		TMatrixN_T             *data;

		tname = "wave samples";

		cm   = stream->data.waveform;
		data = cm->mData;

		rc = fread(data, sizeof(TMatrixN_T), nitem, Fout[inarg]);

	  } break;

	  case EFI_STREAM_SPIKE: {
		Spike_CLASS *cs;
		TSpike      *data;

		tname = "spikes";

		cs   = stream->data.spike;
		data = cs->Spikes;

		rc = fread(data, sizeof(TSpike), nitem, Fout[inarg]);

		cs->CurSpikes = nitem;
	  } break;

	  case EFI_STREAM_NONE:
		fprintf(stderr,
			"%s: ****ERROR**** NO DATA STREAM for arg %d",
			Name, inarg);
		return(0);

	  default:
		fprintf(stderr,
			"%s: ****ERROR**** Invalid stream type %d for arg %d",
			Name, stream->datatype, inarg);
		return(0);
	}

	if (rc != nitem)
	   if (feof(Fout[inarg])) return(0);
	   else {
		fprintf(stderr,
			"%s: ****ERROR**** EOF on input %d frame %d",
			Name, inarg, DataFrame);
		exit(4);
	   }

	return(nitem);
}

