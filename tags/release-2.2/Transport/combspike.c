/*
 * Combine spike frames/change spike sample time
 */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include <local/std.h>

#include "CSpike.h"

INTERNAL int  Blip = 1;

INTERNAL int  isnumeric(char *s);
INTERNAL void usage(char *pgm, int msg);
INTERNAL int  ReadMetaHead(FILE *f, char type[], int dims[], int *maxsize);
INTERNAL int  ReadMeta(int frame, FILE *f);

main(int argc, char *argv[]) {
	char   *pgm;
	char   *infile, *inmeta, *outfile, *outmeta;
	FD      infd, outfd;
	FILE   *inmf, *outmf;
	int     comb;
	float   intime;
	double  curtime;

	int     datumsize = sizeof(TSpike);
	int     ok, rc;
	int     help;
	int     bin64;
	int     mode = 0;
	int     first, last;

	int     rflag, wflag;

	int     bufsize;        /* input/output buffer size        */
	BYTE   *buf;
	TSpike *data;

	char    type[100];
	int     dim[20], ndim, maxsize;
	int     i, j, nitem;
	int     iframe, oframe;

	if (pgm = strrchr(argv[0], '/')) ++pgm;
	else pgm = argv[0];
	++argv; --argc;

	help  = 0;
	ok    = 1;
	first = 0;
	last  = 0;
	bin64 = 0;
	mode  = 1;
	while (argc && argv[0][0] == '-') {
		char *opt;

		opt = argv[0]+1;
		++argv; --argc;

		if (substr(opt, "mode")) {     /* NOT USED HERE */
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
			if      (substr(mspec, "combine"))  mode = 1;
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

	if (!ok || help || argc < 6) {
		usage(pgm, help);
		exit(!ok);
	}

	infile   = argv[0];
	inmeta   = argv[1];
	outfile  = argv[2];
	outmeta  = argv[3];
	sscanf(argv[4], " %f", &intime);
	comb     = atoi(argv[5]);

	if (comb < 1) {
		fprintf(stderr, "%s: downsample rate (%d) must be >= 1\n",
			pgm, comb);
		exit(1);
	}


	/*
	 * Open the data files using read/write for large file support
	 */
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

	/*
	 * Open metafiles using simpler stream i/o
	 */
	inmf  = fopen(inmeta, "r");
	if (!inmf) {
		fprintf(stderr, "%s: can't open input metafile \"%s\"\n",
			pgm, inmeta);
		exit(1);
	}
	outmf = fopen(outmeta, "w");
	if (!outmf) {
		fprintf(stderr, "%s: can't open input metafile \"%s\"\n",
			pgm, outmeta);
		exit(1);
	}

	/*
	 * Read Metafile Header
	 */

	ndim = ReadMetaHead(inmf, type, dim, &maxsize);
	if (ndim < 1) {
		fprintf(stderr, "%s: can't parse input metafile \"%s\"\n",
			pgm, inmeta);
		exit(1);
	}
printf("input metafile %s: %s, ndim= %d, max = %d\n", inmeta, type, ndim, maxsize);

	if (strcasecmp("spike", type)!=0) {
		fprintf(stderr, "%s: input file must contain spike data\n",
			pgm);
		exit(1);
	}

	/* Construct a plausible output header */

	fprintf(outmf, "spike [");
	for (i = 0; i < ndim; ++i) fprintf(outmf, " %d", dim[i]);
	fprintf(outmf, " ] %d\n", maxsize*comb);

	/*
	 * Allocate i/o buffers
	 */
	bufsize = comb * maxsize * datumsize;
	buf     = (BYTE *)malloc(bufsize);
	if (!buf) {
		fprintf(stderr,
			"%s: data buffer (%d bytes) allocation failed\n",
			pgm, bufsize);
		exit(1);
	}

	iframe  = 1;
	oframe  = 1;
	while (!feof(inmf)) {
		int outcount = 0;

		data = (TSpike *)buf;
		for (i = 0; i < comb; ++i) {
			nitem = ReadMeta(iframe++, inmf);
			if (nitem < 0) break; /* eof */
			if (nitem == 0) continue;
			read(infd, data, nitem*datumsize);

			outcount += nitem;
			if (i == 0) {
				data += nitem;
				continue;
			}
			curtime = i * intime;
			for (j = 0; j < nitem; ++j)
			    data[j].time += curtime;

			data += nitem;
		}
		if (i == 0) break; /* EOF on first input frame */

		if (Blip) {
			printf("%8d - %8d (%6d)\r", iframe, oframe, outcount);
			fflush(stdout);
		}

		data = (TSpike *)buf;
		fprintf(outmf, "%d %d\n", oframe++, outcount);
		if (outcount) write(outfd, data, outcount*datumsize);
	}
	if (Blip) printf("\n");

	fclose(inmf);
	fclose(outmf);

	close(infd);
	close(outfd);

	return 0;
}

INTERNAL int
ReadMetaHead(FILE *f, char type[], int dims[], int *maxsize) {
	int   ndim;
	char *p, *q, buf[500];

	p = fgets(buf, sizeof(buf), f);
	if (!p) {
		fprintf(stderr,
			"ReadMetaHead: ****ERROR**** couldn't read dimension from Metafile\n");
		exit(4);
	}

	/* Extract file type name */
	p = buf; q = type;
	while (*p  && iswhite(*p)) ++p;
	while (*p &&  !iswhite(*p)) *q++ = *p++;
	*q = '\0';

	if (!*p) {
		fprintf(stderr,
			"ReadMetaHead: ****ERROR**** bad Metafile header type: %s at \"%s\"\n",
			 buf, p);
		exit(4);
	}

	/* Extract dimensions */
	while (*p  && iswhite(*p)) ++p;
	if (!*p || *p != '[') {
		fprintf(stderr,
			"ReadMetaHead: ****ERROR**** bad Metafile header dims: %s at \"%s\"\n",
			 buf, p);
		exit(4);
	}
	++p;

	ndim = 0;
	while (*p && *p != ']') {
		int x;

		while (*p && iswhite(*p)) ++p;
		if (!*p || !isdigit(*p)) {
			fprintf(stderr,
				"ReadMetaHead: ****ERROR**** bad Metafile header:  %s at \"%s\"\n",
				 buf, p);
			exit(4);
		}

		x = 0;
		while (*p && isdigit(*p)) x = 10*x + *p++ - '0';
		dims[ndim++] = x;

		while (*p && iswhite(*p)) ++p;
	}

	if (!*p || *p != ']') {
		fprintf(stderr,
			"ReadMetaHead: ****ERROR**** bad Metafile header dims:  %s at \"%s\"\n",
			 buf, p);
		exit(4);
	}
	++p;

	sscanf(p, " %d", maxsize);

	return(ndim);
}

INTERNAL int
ReadMeta(int frame, FILE *f) {
	char buf[100], *sp;
	int  mframe, nitem;

	sp = fgets(buf, sizeof(buf), f);
	if (!sp) return(-1);
	sscanf(buf, " %d %d", &mframe, &nitem);
	if (mframe != frame) {
		fprintf(stderr,
			"ReadMeta: ****ERROR**** Metafile frame number (%d) mismatch on frame %d\n",
			mframe, frame);
		exit(4);
	       /*** no return ***/ return(-2);
	}

	return(nitem);
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
	if (msg) printf("usage: %s [options] infile inmeta outfile outmeta intime combine\n", pgm);
	else     printf("usage: %s ", pgm);
	PRT("[-help]",          "display this message");
	PRT("infile",           "input data file");
	PRT("inmeta",           "input meta file");
	PRT("outfile",          "output data file");
	PRT("outmeta",          "output meta file");
	PRT("time",             "input frame time");
	PRT("combine",          "number of frames to combine");
	printf("\n");
}





