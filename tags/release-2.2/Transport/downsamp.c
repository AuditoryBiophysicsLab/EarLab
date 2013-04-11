/*
 * Data file downsampler
 */
#include <stdio.h>
#include <fcntl.h>
#include <math.h>

#include <local/std.h>
#include <string.h>

INTERNAL int  Blip = 1;

INTERNAL int  isnumeric(char *s);
INTERNAL void usage(char *pgm, int msg);
INTERNAL int  downsample(int mode, int downsamp, int nchan, float *in, float *out);

#define DOWNSAMP_PNT    1
#define DOWNSAMP_AVG    2
#define DOWNSAMP_ABS    3
#define DOWNSAMP_RMS    4
#define DOWNSAMP_MIN    5
#define DOWNSAMP_MAX    6

main(int argc, char *argv[]) {
	char *pgm;
	char *infile, *outfile;
	FD    infd, outfd;
	int   dsamp;
	int   nchan;
	int   help;
	int   ok;
	int   datumsize = sizeof(float);
	int   rc, eof;
	int   bin64;
	int   first, last;
	int   insamp, outsamp;
	int   rflag, wflag;
	int   inbyte, outbyte;  /* input/output bytes per downsample   */
	int   insize, outsize;  /* input/output buffer sizes           */
	int   iNblock, oNblock; /* number of samples blocks per buffer */
	int   iblock, oblock;   /* current sample block                */
	int   mode;
	BYTE  *inbuf,  *outbuf;
	BYTE  *indata, *outdata;

	if (pgm = strrchr(argv[0], '/')) ++pgm;
	else pgm = argv[0];
	++argv; --argc;

	nchan = 1;
	help  = 0;
	ok    = 1;
	first = 0;
	last  = 0;
	bin64 = 0;
	mode  = DOWNSAMP_PNT;
	while (argc && argv[0][0] == '-') {
		char *opt;

		opt = argv[0]+1;
		++argv; --argc;

		if (substr(opt, "mode")) {
			char *mspec;

			if (!argc) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					pgm, opt);
				ok = 0;
				continue;
			}
			mspec = argv[0];
			++argv; --argc;
			if      (substr(mspec, "point"))    mode = DOWNSAMP_PNT;
			else if (substr(mspec, "impulse"))  mode = DOWNSAMP_PNT;
			else if (substr(mspec, "average"))  mode = DOWNSAMP_AVG;
			else if (substr(mspec, "mean"))     mode = DOWNSAMP_AVG;
			else if (substr(mspec, "absolute")) mode = DOWNSAMP_ABS;
			else if (substr(mspec, "rms"))      mode = DOWNSAMP_RMS;
			else if (substr(mspec, "minimum"))  mode = DOWNSAMP_MIN;
			else if (substr(mspec, "maximum"))  mode = DOWNSAMP_MAX;
			else {
				fprintf(stderr,
					"%s: unknown mode -%s\n",
					pgm, mspec);
				ok = 0;
				continue;
			}
		}
		else if (substr(opt, "blip"))   Blip = 1;
		else if (substr(opt, "noblip")) Blip = 0;
		else if (substr(opt, "bin64")) {
			bin64 = 1;
			datumsize = sizeof(double);
		}
		else if (substr(opt, "channels")) {
			if (!argc || !isdigit(*argv[0])) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					pgm, opt);
				ok = 0;
				continue;
			}

			nchan = atoi(argv[0]);
			++argv; --argc;
		}
		else if (substr(opt, "select")) {
			if (!argc || !isdigit(*argv[0])) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					pgm, opt);
				ok = 0;
				continue;
			}
			first = atoi(argv[0]) - 1;
			++argv; --argc;

			if (argc && isnumeric(argv[0])) {
				last = atoi(argv[0]);
				++argv; --argc;
			}
		}
		else {
			fprintf(stderr, "%s: unknown option -%s\n", pgm, opt);
			ok = 0;
		}
	}

	if (!ok || help || argc < 3) {
		usage(pgm, help);
		exit(!ok);
	}

	infile  = argv[0];
	outfile = argv[1];
	dsamp   = atoi(argv[2]);

	if (dsamp < 1) {
		fprintf(stderr, "%s: downsample rate (%d) must be >= 1\n",
			pgm, dsamp);
		exit(1);
	}

	inbyte  = dsamp * datumsize * nchan;
	iNblock = (5*1024*1024)/inbyte;
	insize  = iNblock * inbyte;
	inbuf   = (BYTE *)malloc(insize);
	if (!inbuf) {
		fprintf(stderr,
			"%s: input data buffer (%d bytes) allocation failed\n",
			pgm, insize);
		exit(1);
	}

	outbyte = datumsize * nchan;
	oNblock = (4*1024*1024)/outbyte;
	outsize = oNblock * outbyte;
	outbuf  = (BYTE *)malloc(outsize);
	if (!outbuf) {
		fprintf(stderr,
			"%s: output data buffer (%d bytes) allocation failed\n",
			pgm, outsize);
		exit(1);
	}

	printf("%s: reading %d x %d Byte input buffer, %d x %d Byte output buffer\n",
	       pgm, iNblock, inbyte, oNblock, outbyte);

	rflag = O_RDONLY;
	wflag = O_WRONLY | O_CREAT;
	if (*outfile == '+') {
		wflag |= O_APPEND;
		++outfile;
	}
	else wflag |= O_TRUNC;

	/* Other flags if available */
#ifdef O_BINARY
	rflag |= O_BINARY;
	wflag |= O_BINARY;
#endif

#ifdef O_LARGEFILE
fprintf(stderr, "%s: using LARGEFILE i/o\n", pgm);
	rflag |= O_LARGEFILE;
	wflag |= O_LARGEFILE;
#endif

	infd = open(infile, rflag, 0666);
	if (infd < 0) {
		fprintf(stderr, "%s: can't open input file \"%s\"\n",
			pgm, infile);
		exit(1);
	}

	outfd = open(outfile, wflag, 0666);
	if (outfd < 0) {
		fprintf(stderr, "%s: can't open output file \"%s\"\n",
			pgm, outfile);
		exit(1);
	}

	eof    = 0;
	insamp = 0;

	/* Read iNblock downsamples (dsamp x nchan) of input data */
	rc = read(infd, inbuf, insize);
	if (rc < 0) {                       /* Error */
		fprintf(stderr,
			"%s: error (%d) reading %d Bytes of input data at sample %d\n",
			pgm, rc, insize, insamp+1);
		perror(pgm);
		exit(1);
	}
	if (rc == 0) { /* EOF */
		fprintf(stderr, "%s: error no input data\n", pgm);
		exit(1);
	}
	if (rc < insize) {  /* EOF w/ partial read */
		eof = 1;
		/* Calculate number of chunks actually read */
		iNblock = rc / inbyte;
	}
	iblock = 0;
	oblock = 0;

	for (insamp = outsamp = 0; !last || insamp < last; insamp += dsamp) {
		/* Read more data if neccessary */
		if (iblock == iNblock) {
			if (eof) break; /* Already at EOF - done */
			rc = read(infd, inbuf, insize);
			if (rc < 0) {       /* Error */
				fprintf(stderr,
					"%s: error (%d) reading input data sample %d\n",
					 pgm, rc, insamp+1);
				perror(pgm);
				exit(1);
			}
			else if (rc == 0) { /* Normal EOF */
				eof = 1;
				break;
			}
			else if (rc < insize) {  /* EOF w/ partial read */
				eof = 1;
				/* Calculate number of chunks actually read */
				iNblock = rc / inbyte;
			}
			iblock  = 0;
		}
/*****************
  printf("processing insample %d at block %d, outsample %d at block %d\n",
	 insamp, iblock, outsamp, oblock);
*****************/

		if (insamp < first) continue;

		indata  = inbuf  + iblock * inbyte;
		outdata = outbuf + oblock * outbyte;

		downsample(mode, dsamp, nchan, (float *)indata, (float *)outdata);
	    /***
		downsample64(mode, dsamp, nchan, (double *)indata, (double *)outdata);
	    ***/

		iblock++;
		oblock++;

		if (oblock == oNblock) {
			int nout;

			nout = oNblock*outbyte;
			rc = write(outfd, outbuf, oNblock*outbyte);
			if (rc != nout) {
				fprintf(stderr,
					"%s: error writing out data sample %d\n",
					pgm, outsamp+1);
				exit(1);
			}
			oblock = 0;
		}
		outsamp++;

		if (Blip) {
			printf("%8d - %8d (%d)\r", insamp, outsamp, outbyte);
			fflush(stdout);
		}
	}
	if (Blip) printf("\n");


	/* Write any remaining output samples */
	if (oblock) {
		int nout;

		nout = oblock*outbyte;
		rc = write(outfd, outbuf, nout);
			if (rc != nout) {
			fprintf(stderr,
				"%s: error writing out data sample %d\n",
				pgm, outsamp);
			exit(1);
		}
	}

	printf("%s: read %d samples%sin %d channels, wrote %d samples\n",
		pgm, insamp, eof?" (eof) ":" ", nchan, outsamp);

	close(infd);
	close(outfd);

	return 0;
}

INTERNAL int
downsample(int mode, int downsamp, int nchan, float *in, float *out) {
	int i, j, n;
	int datumsize = sizeof(float);
	double v, factor;

	switch (mode) {
	  case DOWNSAMP_PNT:
		memcpy(out, in, nchan*datumsize);
		break;

	  case DOWNSAMP_AVG:
		for (i = 0; i < nchan; ++i) out[i] = *in++;
		for (j = 0; j < downsamp-1; ++j)
		  for (i = 0; i < nchan; ++i) out[i] += *in++;
		factor = 1.0/(double)downsamp;
		for (i = 0; i < nchan; ++i) out[i] *= factor;
		break;

	  case DOWNSAMP_ABS:
		for (i = 0; i < nchan; ++i) {
			v     = *in++;
			out[i] = abs(v);
		}
		for (j = 0; j < downsamp-1; ++j)
		  for (i = 0; i < nchan; ++i) {
			v     = *in++;
			out[i] += abs(v);
		}
		factor = 1.0/(double)downsamp;
		for (i = 0; i < nchan; ++i) out[i] *= factor;
		break;

	  case DOWNSAMP_MIN:
		for (i = 0; i < nchan; ++i) out[i] = *in++;
		for (j = 0; j < downsamp-1; ++j)
		  for (i = 0; i < nchan; ++i) {
			v = *in++;
			if (v < out[i]) out[i] = v;
		}
		break;

	  case DOWNSAMP_MAX:
		for (i = 0; i < nchan; ++i) out[i] = *in++;
		for (j = 0; j < downsamp-1; ++j)
		  for (i = 0; i < nchan; ++i) {
			v = *in++;
			if (v > out[i]) out[i] = v;
		}
		break;

	  case DOWNSAMP_RMS:
		for (i = 0; i < nchan; ++i) {
			v     = *in++;
			out[i] = v*v;
		}
		for (j = 0; j < downsamp-1; ++j)
		  for (i = 0; i < nchan; ++i) {
			v       = *in++;
			out[i] += v*v;
		}
		factor = 1.0/(double)downsamp;
		for (i = 0; i < nchan; ++i)  out[i] = sqrt(factor*out[i]);
		break;

	  default:
		fprintf(stderr, "downsamp: unknown mode %d\n", mode);
		exit(4);
	}
	return(1);
}


INTERNAL int
isnumeric(char *s) {
	if (*s == '-' || *s == '+') ++s;
	while (*s && *s != '.') if (!isdigit(*s)) return(0); else ++s;
	if (*s == '.') ++s;
	while (*s) if (!isdigit(*s)) return(0); else ++s;
	return(1);
}

#define PRT(opt,str) \
	printf(msg?"\t%s%s%s\n":"%s ", opt, (strlen(opt)<8)?"\t\t":"\t", str)

INTERNAL void
usage(char *pgm, int msg) {
	if (msg) printf("usage: %s [options] infile outfile dsample\n", pgm);
	else     printf("usage: %s ", pgm);
	PRT("[-help]",          "display this message");
	PRT("[-channel n]",     "select channel i of n");
	PRT("infile",           "input data file");
	PRT("outfile",          "output data file");
	PRT("downsample",       "downsample size");
	printf("\n");
}





