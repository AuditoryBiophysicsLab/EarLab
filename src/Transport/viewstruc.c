/*
 * 3D MS viewer for physiological structures
 */
#undef WINAPP

#ifdef WINAPP
# define WINVER 0x0600
# include <windows.h>
# include <local/winapp.h>
# define main app_main
#endif

#include <stdio.h>
#include <local/std.h>

#ifdef WINAPP
# define WINGDI   1
# include <local/gd.h>
#endif

#ifdef XWINDOWS
# define XWINDOWS 1
# include <local/xw.h>
#endif

#ifdef OGL
# include <local/ogl.h>
# include <local/xwogl.h>
#endif

#include <daffie/event.h>
#include <ms/msproto.h>

#include "hsvrgb.h"
#ifdef WINAPP
# include "viewstruc.h"
#endif

typedef unsigned long long UTime;
typedef double DATA;

#define MAXCHANN   512
#define MAXDATA    5000*MAXCHANN
#define MAX_CMAP   1024

#define MINREDRAW  50
#define NORMREDRAW 100

#include "earlibc.h"
typedef TSpike     SPIKE_T;
typedef TMatrixN_T WAVE_T;

#define DOWNSAMP_PNT    1
#define DOWNSAMP_AVG    2
#define DOWNSAMP_ABS    3
#define DOWNSAMP_RMS    4
#define DOWNSAMP_MIN    5
#define DOWNSAMP_MAX    6
#define DOWNSAMP_PEAK   7

INTERNAL void   usage(char *pgm, int msg);
INTERNAL int    readspikefile(FILE *f,  SPIKE_T *data, int n);
INTERNAL int    readspikedatum(FILE *f, SPIKE_T *datum);
INTERNAL int    readwavedatum(FILE *f,  WAVE_T *datum);
INTERNAL int    readwavefile(FILE *f,   WAVE_T *data, int n);
INTERNAL int    analyzewave(DATA *data, int mode);
INTERNAL int    analyzespike(DATA *data, int mode);
INTERNAL void   setprobe(char *spec);
INTERNAL void   startui(void);
INTERNAL void   updateui(void);
INTERNAL void   make_rgb_ordering(void);
INTERNAL int    make_rgb(int r, int g, int b);
INTERNAL int    make_rgba(int r, int g, int b, int a);
INTERNAL int    downsample(int mode, int downsamp, int nchan, float *in, DATA *out);
INTERNAL int    isnumeric(char *s);

INTERNAL SPIKE_T *Spike_Data  = NULL;
INTERNAL WAVE_T  *Wave_Data = NULL;
INTERNAL DATA  *Data = NULL;
INTERNAL char  *Pgm;
INTERNAL int    Verbose = 1;
INTERNAL char  *XDisplay  = NULL;
INTERNAL int    Ndata     = 0;
INTERNAL int    NCell     = 0;          /* Number of cells              */
INTERNAL int    NChan     = 0;          /* Number of channels           */
INTERNAL int    SampPerFrame = 1;       /* Samples per frame            */
INTERNAL int    CellPerChan = 0;
INTERNAL int    Dim[4] = { 0,0,0,0 };
INTERNAL int    Redraw    = 0;          /* Display needs to be redrawn  */
INTERNAL int    Rerange   = 0;          /* Data range needs to be computed */
INTERNAL int    Done      = 0;
INTERNAL int    Double    = 0;          /* double buffering flag        */
INTERNAL int    FrontBuf;               /* front display buffer         */
INTERNAL int    BackBuf;                /* backing buffer               */
INTERNAL int    Delay     = 0;          /* display frame delay          */
INTERNAL int    PlotType  = 0;
INTERNAL double MinTime, MaxTime;       /* Max and min event time       */
INTERNAL float  FrameTime = 0.0;
INTERNAL int    CellMin, CellMax;
INTERNAL unsigned long long Lastdraw = 0;
INTERNAL int    Binary    = 0;          /* Binary float data              */
INTERNAL int    Bin64     = 0;          /* Double precision binary float  */
INTERNAL int    Autopan   = 0;          /* unused - for compatiblity only */
INTERNAL int    PanRight  = 0;          /* unused - for compatiblity only */
INTERNAL float  Time_X0   = 0.0;
INTERNAL float  Time_XN   = 0.0;
INTERNAL int    Data_X0   = 0;
INTERNAL int    Data_XN   = 0;
INTERNAL float  DataMin, DataMax;       /* Min and max data values        */
INTERNAL float  Domain_X0 = 0.0;
INTERNAL int    DownSamp  = 1;
INTERNAL int    WantData  = 0;
INTERNAL int    DataLock;
INTERNAL int    DataWait;
INTERNAL int    Frame     = 0;
INTERNAL int    Rendframe = 0;
INTERNAL FILE  *Dataf     = NULL;
INTERNAL char   Filename[2000];

#define EPSILON 1.0e-10

main(int argc, char *argv[]) {
	char     *file;                         /* data file name       */
	char     *dump;                         /* output file spec     */
	int       ok, help;                     /* cmd line options     */
	int       isspike;                      /* data is spikes       */
	int       showmap;                      /* more cmd line opts   */
	int       probe;                        /* daffie probe         */
	int       i, j, k;                      /* loop counters        */
	int       dpf;                          /* datum per frame      */
	int       nitem;                        /* total data, items per channel */
	int       downmode;                     /* down sample mode     */
	int       xs, ys;                       /* screen size          */
	int       backxs, backys;               /* back buffer          */
	int       ix, iy;                       /* screen coords        */
	int       x0;                           /* panned data origin   */
	int       pan;                          /* pan value            */
	int       key;                          /* mouse key on press   */
	double    basescale, dscale;            /* data scaling         */
	double    xdisp, ydisp, zdisp;          /* Displacement         */
	int       dodisp;                       /* Do data displacement */
	double    xstretch, ystretch, zstretch; /* Stretch              */
	int       dostretch;                    /* Do data stretching   */
	double    xrot, yrot, zrot;             /* Rotation             */
	float     xscale, yscale, off;          /* data offset & scaling*/
	float     v;                            /* a datum              */
	char      title[300] = {'\0'}, *tp;     /* title bar            */
	char     *cmapfile = NULL;              /* color map file name  */
	COLORMAP  cmap[MAX_CMAP];               /* color map from above */
	char      *geom;                        /* Geometry file        */
	int       ncmap;                        /* # entriesin above    */
	int       cindx[256];                   /* allocated (X) color cells */
	int       crgb[256];                    /* corresponding rgb val */
	int       chindx[256];                  /* color index by channel*/
	int       plotx0, ploty0;               /* base of plot area     */
	UTime     now;
	char      optbuf[200];

	Pgm = argv[0];  if (streq(Pgm, "app_main")) Pgm = "viewstruc";
	++argv; --argc;

	/* Default window size */
	xs = 512;
	ys = 512;

	help     = 0;
	ok       = 1;
	isspike  = 0;
	showmap  = 0;
	DownSamp = 1;
	probe    = 0;
	geom     = NULL;
	dscale   = 0.1;
	basescale= -1.0;
	xrot     = 0.0;
	yrot     = 0.0;
	zrot     = 0.0;
	dodisp   = 0;
	dostretch= 0;
	downmode = DOWNSAMP_AVG;
	dump     = NULL;

	while (argc && *argv[0] == '-') {
		char *opt;

		opt = argv[0]+1;
		++argv; --argc;

		if (substr(opt, "help")) ++help;
		else if (substr(opt, "verbose"))     Verbose = 1;
		else if (substr(opt, "quiet"))       Verbose = 0;
		else if (substr(opt, "showmap"))     showmap = 1;
		else if (substr(opt, "probe"))       probe   = 1;
		else if (substr(opt, "ascii"))       Binary  = 0;
		else if (substr(opt, "binary"))      Binary  = 1;
		else if (substr(opt, "bin64"))       Bin64   = 1;
		else if (substr(opt, "autopan"))     Autopan = 1;
		else if (substr(opt, "spike"))       isspike = 1;
		else if (substr(opt, "waveform"))    isspike = 0;
		else if (substr(opt, "downsample")) {
			if (!argc || !isnumeric(argv[0])) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			DownSamp = atoi(argv[0]);
			isspike = 0;
			++argv; --argc;
		}
		else if (substr(opt, "mode")) {
			char *mspec;

			if (!argc) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}
			mspec = argv[0];
			++argv; --argc;
			if      (substr(mspec, "point"))    downmode = DOWNSAMP_PNT;
			else if (substr(mspec, "impulse"))  downmode = DOWNSAMP_PNT;
			else if (substr(mspec, "average"))  downmode = DOWNSAMP_AVG;
			else if (substr(mspec, "mean"))     downmode = DOWNSAMP_AVG;
			else if (substr(mspec, "absolute")) downmode = DOWNSAMP_ABS;
			else if (substr(mspec, "rms"))      downmode = DOWNSAMP_RMS;
			else if (substr(mspec, "minimum"))  downmode = DOWNSAMP_MIN;
			else if (substr(mspec, "maximum"))  downmode = DOWNSAMP_MAX;
			else if (substr(mspec, "peak"))     downmode = DOWNSAMP_PEAK;
			else {
				fprintf(stderr,
					"%s: unknown mode -%s\n",
					Pgm, mspec);
				ok = 0;
				continue;
			}
		}
		else if (substr(opt, "cells")) {
			if (!argc || !isdigit(*argv[0])) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			NCell = atoi(argv[0]);
			isspike = 1;
			++argv; --argc;
		}

		else if (substr(opt, "channel")) {
			int   x;
			char *s;

			if (!argc || !isdigit(*argv[0])) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}
			s = argv[0];
			++argv; --argc;

			x = 0;

			while (isdigit(*s)) x = 10*x + *s++ -'0';
			NChan = x;

		}

		else if (substr(opt, "dimensions")) {
			int   i = 0;
			int   x;
			char *s;

			if (!argc || !isdigit(*argv[0])) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}
			s = argv[0];
			++argv; --argc;

			i = 0;
			--s;
			do {
				x = 0; ++s;
				while (isdigit(*s)) x = 10*x + *s++ -'0';
				Dim[i++] = x;
				printf("dim %d = %d\n", i-1, x);
			} while (*s == '.' || *s == 'x');
		}

		/* TIME/FRAMETIME - frame time in milliseconds */
		else if (substr(opt, "time") || substr(opt, "frametime")) {
			if (!argc || !isnumeric(argv[0])) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			sscanf(argv[0], " %f", &FrameTime);
			++argv; --argc;
		}

		/* DATASCALE - scale object via data */
		else if (substr(opt, "datascale")) {
			float d;

			if (!argc || !isnumeric(argv[0])) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			sscanf(argv[0], " %f", &d);
			dscale = d;
			++argv; --argc;
		}

		/* BASESCALE - base scale for object */
		else if (substr(opt, "basescale")) {
			float d;

			if (!argc || !isnumeric(argv[0])) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			sscanf(argv[0], " %f", &d);
			basescale = d;
			++argv; --argc;
		}

		/* DISPLACEMENT - move object via data */
		else if (substr(opt, "displacement")) {
			float xd, yd, zd;

			if (argc < 3 ||
			    !isnumeric(argv[0]) ||
			    !isnumeric(argv[1]) ||
			    !isnumeric(argv[2]) ) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			sscanf(argv[0], " %f", &xd);
			sscanf(argv[1], " %f", &yd);
			sscanf(argv[2], " %f", &zd);
			xdisp = xd;
			ydisp = yd;
			zdisp = zd;

			dodisp = 1;
			argv += 3;
			argc -= 3;
		}

		/* STRETCH - stretch object via data */
		else if (substr(opt, "stretch")) {
			float xt, yt, zt;

			if (argc < 3 ||
			    !isnumeric(argv[0]) ||
			    !isnumeric(argv[1]) ||
			    !isnumeric(argv[2]) ) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			sscanf(argv[0], " %f", &xt);
			sscanf(argv[1], " %f", &yt);
			sscanf(argv[2], " %f", &zt);
			xstretch = xt;
			ystretch = yt;
			zstretch = zt;

			dostretch = 1;
			argv += 3;
			argc -= 3;
		}

		/* XROTATION - X rotation */
		else if (substr(opt, "xrotation")) {
			float angle;

			if (!argc || !isnumeric(argv[0])) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			sscanf(argv[0], " %f", &angle);
			xrot = derad(angle);
			++argv; --argc;
		}

		/* YROTATION - Y rotation */
		else if (substr(opt, "yrotation")) {
			float angle;

			if (!argc || !isnumeric(argv[0])) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			sscanf(argv[0], " %f", &angle);
			yrot = derad(angle);
			++argv; --argc;
		}

		/* ZROTATION - Z rotation */
		else if (substr(opt, "zrotation")) {
			float angle;

			if (!argc || !isnumeric(argv[0])) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			sscanf(argv[0], " %f", &angle);
			zrot = derad(angle);
			++argv; --argc;
		}


		/* TITLE -  Display Window Title */
		else if (substr(opt, "title")) {
			if (!argc) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			sprintf(title, "title=%s", argv[0]);
			++argv; --argc;
		}

		/* XDISPLAY - X Windows Display server/host */
		else if (substr(opt, "xdisplay")) {
			if (!argc) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			XDisplay = argv[0];
			++argv; --argc;
		}

		/* GEOMETRY - MS 3D Geometry file */
		else if (substr(opt, "geometry")) {
			if (!argc) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			geom = argv[0];
			++argv; --argc;
		}

		/* WINSIZE / SIZE  - specify window size */
		else if (substr(opt, "winsize") || substr(opt, "size")) {
			if (!argc || !sscanf(argv[0], " %d", &xs)) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}
			++argv; --argc;

			if (!argc || !sscanf(argv[0], " %d", &ys)) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}
			++argv; --argc;
		}

		/* FPS  - specify frames per second */
		else if (substr(opt, "fps")) {
			float fps;

			if (!argc || !sscanf(argv[0], " %f", &fps)) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}
			++argv; --argc;

			Delay = 1000.0/fps + 0.5;
			printf("Frame time %d\n", Delay);
		}

		else if (substr(opt, "map")) {
			if (!argc) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
			}
			else {
				cmapfile = argv[0];
				++argv; --argc;
			}
		}

		/* DUMP - dump geometry files */
		else if (substr(opt, "dump")) {
			if (!argc) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
			}
			else {
				dump = argv[0];
				++argv; --argc;
			}
		}

		else if (substr(opt, "double")) Double = 1;
		else if (streq(opt, "db"))      Double = 1;
		else {
			 fprintf(stderr, "%s: ignoring argument -%s\n", Pgm, opt);
			 ok = 0;
		}
	}

	if (!ok || help || !argc) {
		usage(Pgm, help);
		exit(!help);
	}

	/* Try to derive other values from args given */
	dpf = MAXDATA;
	if (isspike) {
		/* Try to derive other values from args given */
		if (Dim[0] && !NChan)       NChan       = Dim[0];
		if (Dim[1] && !CellPerChan) CellPerChan = Dim[1];

		if (NChan && CellPerChan && !NCell) NCell = NChan * CellPerChan;
		else if (NChan && NCell && !CellPerChan) CellPerChan = NCell/NChan;
		else if (NCell && CellPerChan && !NChan) NChan = NCell / CellPerChan;
	}
	else {
		if (Dim[0])           SampPerFrame = Dim[0];
		if (Dim[1] && !NChan) NChan = Dim[1];
		if (CellPerChan && !SampPerFrame) SampPerFrame = CellPerChan;

		dpf = NChan * SampPerFrame;
	}

printf("NCell = %d, NChan = %d, CellPerChan = %d\n",
	NCell, NChan, CellPerChan);

	file = argv[0];
	++argv; --argc;

	/* Allocate storage for the data */
	Data = (DATA *)malloc(MAXCHANN * sizeof(DATA));
	if (!Data) {
		fprintf(stderr, "%s: not enough memory for %d datum\n", Pgm, MAXDATA);
		exit(1);
	}

	Spike_Data = (SPIKE_T *)malloc(MAXDATA * sizeof(SPIKE_T));
	if (!Spike_Data) {
		fprintf(stderr, "%s: not enough memory for %d spike datum\n", Pgm, MAXDATA);
		exit(1);
	}

	Wave_Data = (WAVE_T *)malloc(MAXDATA * sizeof(WAVE_T));
	if (!Wave_Data) {
		fprintf(stderr, "%s: not enough memory for %d wave datum\n", Pgm, MAXDATA);
		exit(1);
	}

	/* Initialize MP library and allocate a thread semaphore */
	mp_init();
	event_init();
	event_debug(0);

	DataLock = mp_alloclock();
	DataWait = mp_allocsema();

	/*
	 * Read in data file
	 */
	if (substr("daffie://", file) || substr("//", file)) probe = 1;

	if (probe) setprobe(file);
	else if (!(Dataf = fopen(file, "r"))) {
		fprintf(stderr, "%s: can't open file %s\n", Pgm, file);
		exit(1);
	}

	strcpy(Filename, file);

	if (!title[0]) {
		char *tp;

		if (probe) {
			tp = file + strlen("daffie://");
			while (*tp && *tp != '/') ++tp;
			if (*tp) ++tp;
			else     tp = file;
		}
		else if (tp = strrchr(file, '/')) ++tp;
		else tp = file;

		sprintf(title, "title=%s %s", Pgm, tp);
	}

	ms_init();

	/* Set device options */
	ms_devopt("X0", title);
	ms_devopt("X0", "fullcolor");
	ms_devopt("X0", "nokill");
	ms_devopt("X0", "double");
	if (XDisplay) {
		sprintf(optbuf, "display=%s", XDisplay);
		ms_devopt("X0", optbuf);
	}
	sprintf(optbuf, "size=%dx%d", xs, ys);
	ms_devopt("X0", optbuf);

	ms_define("CAM0", "device",   "X0");
	ms_define("CAM0", "render",   "scan");
	ms_define("CAM0", "fillview", "interior");

	/* Set basescale if not given on command line */
	if (basescale < 0.0)
	   if (dscale > 0.0) basescale = 2.0 * dscale;
	   else              basescale = 1.0;

	ms_bgnobj("STRUCT");
	  if (geom) {
		int rc;

		rc = ms_restore(geom);
		if (!rc) {
			fprintf(stderr, "%s: can't restore geometry file %s\n", Pgm, geom);
			exit(1);
		}
	}

	  else
	    for (i = 0; i < NChan; ++i) {
		char name[100];

		sprintf(name, "C%d", i);
		ms_bgnobj(NULL, NULL);
		 ms_bgnobj(name, "sphere");
		 ms_scale(".", basescale);
		 ms_endobj(name);
		 ms_move(".", (float)i - 0.5*NChan, 0.0, 0.0);
		 ms_putprp(".", "color", RGB((float)i/NChan, 0.0, 1.0-i/(float)NChan));
		ms_endobj(".");
	    }
	ms_endobj("STRUCT");

	/* Create color map */
	ncmap = make_colormap(cmapfile, cmap, MAX_CMAP);

	if (NChan) for (i = 0; i < NChan; ++i) {
		j = ((float) i)/(NChan-1) * 255.0;
		chindx[i] = cindx[j];
	}

	/* Initialize user interface */
	startui();

	x0 = 0;

	WantData  = 1;
	Rendframe = 0;
	while (!Done) {
		char  text[100];
		int   border = 4;
		int   th, tw, ta, td;   /* text metrics */

		updateui();

		/* Get/wait for more data */
		if (probe) mp_decsema(DataWait);
		else  {
			if (isspike) readspikefile(Dataf, Spike_Data, dpf);
			else         readwavefile (Dataf, Wave_Data,  dpf);
		}

		/*
		 * Set effective number of items to plot given number of
		 * channels and sampling size
		 */
		nitem  = Ndata;

		mp_lock(DataLock);
		if (isspike) analyzespike(Data, downmode);
		else         analyzewave(Data,  downmode);
		mp_unlock(DataLock);

		/* Update the data model based on recently acquired data */
		for (i = 0; i < NChan; ++i) {
			char  name[100];
			double v;

			sprintf(name, "C%d", i);

			v = Data[i];
			ms_reset(name);

			if (dscale > 0.0)
			   ms_scale(name, basescale + dscale * v);

			if (dostretch) {
				ms_stretch(name, 'x', 1.0+v*xstretch);
				ms_stretch(name, 'y', 1.0+v*ystretch);
				ms_stretch(name, 'z', 1.0+v*zstretch);
			}

			if (dodisp)
			   ms_move(name, v*xdisp, v*ydisp, v*zdisp);
		}

		if (dump) {
			char dumpfile[5000];

			sprintf(dumpfile, dump, Frame);
			ms_dump("STRUCT", dumpfile);
		}

		now = event_clock();
		if (now - Lastdraw < Delay) {
			int wait;

			wait = Delay - (now - Lastdraw);
			event_sleep(wait);
		}

		while (Rendframe < Frame) {
			if (abs(xrot) > 0.0001) ms_rotate("STRUCT", 'x', xrot);
			if (abs(yrot) > 0.0001) ms_rotate("STRUCT", 'y', yrot);
			if (abs(zrot) > 0.0001) ms_rotate("STRUCT", 'z', zrot);

			++Rendframe;
		}

		sprintf(optbuf, "%d", Frame);
		ms_define("CAM0", "label", optbuf);
		ms_render("STRUCT");
		if (!Verbose) {
			printf("%d     \r", Frame);
			fflush(stdout);
		}

		Lastdraw = event_clock();

	}
}

INTERNAL int
analyzewave(DATA *data, int mode) {
	int    i, j, k;
	int    nsamples;
	double val;
	double avg;

	nsamples = Ndata/NChan;


	downsample(mode, nsamples, NChan, Wave_Data, data);

/*********************************

	for (i =  0; i < NChan; ++i) data[i] = 0.0;

	k = 0;
	avg = 1.0/(double)nsamples;
	for (i = 0; i < nsamples; ++i) {
		for (j = 0; j < NChan; ++j) {
			val = Wave_Data[k++];
			data[j] += avg*val;
		}
	}
*********************************/

	DataMin = DataMax = data[0];
	for (i = 0; i < NChan; ++i) {
		if (data[i] < DataMin) DataMin = data[i];
		if (data[i] > DataMax) DataMax = data[i];
	}

	return(i);
}

INTERNAL int
analyzespike(DATA *data, int mode) {
	int    i;
	int    cell, chan;
	double weight;

	for (i =  0; i < NChan; ++i) Data[i] = 0.0;

	weight = 1.0/(double)CellPerChan;
	for (i = 0; i < Ndata; ++i) {
		cell = Spike_Data[i].cell;
		chan = cell / CellPerChan;
		Data[chan] += weight;
	}

	DataMin = DataMax = data[0];
	for (i = 0; i < NChan; ++i) {
		if (data[i] < DataMin) DataMin = data[i];
		if (data[i] > DataMax) DataMax = data[i];
	}

	return(i);
}

/*
 * Downsample/analyze data
 */
INTERNAL int
downsample(int mode, int downsamp, int nchan, float *in, DATA *out) {
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

	  case DOWNSAMP_PEAK:
		for (i = 0; i < nchan; ++i) out[i] = *in++;
		for (j = 0; j < downsamp-1; ++j)
		  for (i = 0; i < nchan; ++i) {
			v = *in++;
			if (abs(v) > out[i]) out[i] = abs(v);
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
readspikedatum(FILE *f, SPIKE_T *datum) {
	int rc;

	if (Binary || Bin64) {
		int    id;
		int    frame;
		float  ft;
		double dt;

		rc = fread(&frame, sizeof(int), 1, f);
		if (!rc) return(0);

		rc = fread(&id, sizeof(int), 1, f);
		if (!rc) return(0);

		datum->cell = id;

		if (Bin64) {
			rc = fread(&dt, sizeof(double), 1, f);
			datum->time = dt;
		}
		else {
			rc = fread(&ft, sizeof(float), 1, f);
			datum->time = ft;
		}
		return(frame);
	}
	else {
		int frame;

		rc = fscanf(f, " %d %d %f", &frame, &datum->cell, &datum->time);
		if (!rc) return(0);
		else return(frame);
	}
}

INTERNAL int
readspikefile(FILE *f, SPIKE_T *data, int n) {
	int   i;
	float mind, maxd;
	static SPIKE_T lastspike;
	static int     havespike = 0, frame;

	if (feof(f)) {
		Time_X0 = Time_XN = 0.0;
		havespike = 0;
		Frame     = 0;
		Rendframe = 0;
		rewind(f);
	}
	++Frame;

	if (!havespike) {
		frame = readspikedatum(f, &lastspike);
		havespike = 1;
	}

	if (Frame != frame) return(Ndata = 0);

	data[0] = lastspike;

	MinTime = MaxTime = data[0].time;
	for (i = 1; !feof(f) && i < n; ++i) {
		frame = readspikedatum(f, &lastspike);
		if (frame != Frame) break;

		data[i] = lastspike;

		if (data[i].time < MinTime) MinTime = data[i].time;
		if (data[i].time > MaxTime) MaxTime = data[i].time;
		if (data[i].cell > NCell)   NCell   = data[i].cell;
	}
	if (!feof(f)) havespike = 1;

	Ndata  = i;

	if (FrameTime > 0.0) {
		Time_X0 += FrameTime;
		Time_XN  = Time_X0 + FrameTime;
	}
	else {
		Time_X0 = MinTime;
		Time_XN = MaxTime;
	}

	if (Verbose)
	   printf("Read %d items (of %d) on frame %d - time %f-%f, %d cells\n",
		   Ndata, n, Frame, MinTime, MaxTime, NCell);

	return(Ndata);
}

INTERNAL int
readwavedatum(FILE *f, WAVE_T *datum) {
	int rc;

	if (Bin64) {
		double d;

		rc = fread(&d, sizeof(double), 1, f);
		*datum = d;
	}
	else if (Binary) {
		float d;

		rc = fread(&d, sizeof(float), 1, f);
		*datum = d;
	}
	else {
		float d;

		rc = fscanf(f, " %f", &d);
		*datum = d;
	}
	return(rc);
}

INTERNAL int
readwavefile(FILE *f, WAVE_T *data, int n) {
	int   i;
	float mind, maxd;

	++Frame;

	if (feof(f)) {
		Data_XN   = 0;
		rewind(f);
		Frame     = 0;
		Rendframe = 0;
	}

	if (!readwavedatum(f, &data[0])) {
		if (feof(f)) return(readwavefile(f, data, n));
		fprintf(stderr, "%s: couldn't read wave file %s\n", Pgm, Filename);
		exit(1);
	}
	mind = maxd = data[0];

	for (i = 1; !feof(f) && i < n; ++i) {
		if (!readwavedatum(f, &data[i])) break;
		if (data[i] < mind) mind = data[i];
		if (data[i] > maxd) maxd = data[i];
	}

	Ndata  = i;

	/* Check for errors in the data file */
	if (i != n && !feof(f)) {
		fprintf(stderr, "%s: error after %d items\n", Pgm, Ndata);
		if (!Binary) {
			char buf[128];

			buf[0] = '\0';
			fgets(buf, sizeof(buf)-1, f);
			fprintf(stderr, "%s: %s\n", Pgm, buf);
		}
		exit(4);
	}

	Data_X0  = Data_XN + 1;
	Data_XN  = Data_X0 + Ndata;

	DataMin = mind;
	DataMax = maxd;

	return(Ndata);
}

INTERNAL int
isnumeric(char *s) {
	if (*s == '-' || *s == '+') ++s;
	while (*s && *s != '.') if (!isdigit(*s)) return(0); else ++s;
	if (*s == '.') ++s;
	while (*s) if (!isdigit(*s)) return(0); else ++s;
	return(1);
}

#ifndef NOEFI
/*
 * Normal EFI build
 */
#include <daffie/event.h>
#include <daffie/eventdef.h>
#include <daffie/eclient.h>
#include "efievent.h"

INTERNAL void
probe_error(char *s, char *where) {
	fprintf(stderr,
		"%s: invalid probe \"%s\" - error at \"%s\"\n",
		Pgm, where, s);
	exit(4);
}

#include "agentd.h"
INTERNAL void
efi_init(void) {
	event_verbose(0);
	event_init();
#include "agentddef.h"
#include "efieventdef.h"
}

INTERNAL int
receive_spikedata(EVENT_EFI_SPIKEDATA *edata) {
	TSpike *d;
	int     i, j, nitem;
	static double mint = 0.0, maxt = 0.0;

	nitem = edata->indexN - edata->index0 + 1;
	if (Verbose)
	   printf("%s: received EFI %d spikes (%d bytes), frame %d, tag %d from %d\n",
		  Pgm, nitem, EventSize(*edata), edata->frame, edata->tag,
		  EventTo(*edata));

	j = Data_XN;

	for (i = 0; i < nitem; ++i) {
		d = &edata->data[i];
		if (d->time < mint)    mint  = d->time;
		if (d->time > maxt)    maxt  = d->time;
		if (d->cell > NCell)   NCell = d->cell;

		Spike_Data[j].cell = d->cell;
		Spike_Data[j].time = d->time;
		++j;
	}
	Data_XN = Ndata = j;

	if (Verbose)
	   printf("Received %d items ranging %f - %f\n", nitem, mint, maxt);

	Redraw = 1;

	/* If this is the last block of the frame, wake up main thread */
	if (edata->indexN == edata->elems-1) {
		Frame = edata->frame;
		if (FrameTime > 0.0) {
			Time_X0  = edata->frame * FrameTime;
			Time_XN  = Time_X0 + FrameTime;
		}
		else {
			Time_X0 = MinTime;
			Time_XN = MaxTime;
		}
		Data_X0 = Data_XN = 0;

		MinTime = mint; /* We really should semaphore here */
		MaxTime = maxt;

		if (Verbose)
		    printf("Frame %d complete: %d items ranging %f - %f\n",
			    Frame, Ndata, mint, maxt);
		if (WantData) mp_incsema(DataWait);

		/* Reset min/max time */
		mint = maxt = 0.0;
	}
	return(1);
}

INTERNAL float
getwavedatum(float data[], int xoff, int chan, int downsample) {
	int   i, j;
	float v;

	i = NChan * downsample * xoff + chan;
	j = i - Domain_X0 + Data_X0;
	while (j > MAXDATA) j -= MAXDATA;
	v = data[j];
	return (v);
}


INTERNAL int
receive_wavedata(EVENT_EFI_WAVEDATA *edata) {
	int        i, j;
	int        nitem;
	TMatrixN_T d;

	nitem = edata->indexN - edata->index0 + 1;

	if (Verbose)
	  printf("%s: received EFI %d samples (%d bytes), frame %d, tag %d from %d\n",
		 Pgm, nitem, EventSize(*edata), edata->frame, edata->tag,
		 EventFrom(*edata));

	j = Data_XN;
	for (i = 0; i < nitem; ++i) {
		d = edata->data[i];
		if (d < DataMin) DataMin = d;
		if (d > DataMax) DataMax = d;
		Wave_Data[j++] = d;

		/* Wrap data on overflow */
		if (j == MAXDATA) {
			Ndata   = MAXDATA;
			Data_X0 = 1;
			j       = 0;
		}
		if (Ndata == MAXDATA) {
			if (++Data_X0 == MAXDATA) Data_X0 = 0;
			Domain_X0++;
		}

	}
	Data_XN = j;
	if (j > Ndata) Ndata = j;

	if (Verbose)
	    printf("Data %d items ranging %f - %f\n", Ndata, DataMin, DataMax);

	Redraw = Rerange = 1;

	/* If this is the last block of the frame, wake up main thread */
	if (edata->indexN == edata->elems-1) {
		Frame = edata->frame;
		Data_X0 = Data_XN = 0;
		if (WantData) mp_incsema(DataWait);
	}
	return(1);
}

#define PREFIX "daffie://"

INTERNAL void
setprobe(char *spec) {
	char server[100];
	char instname[100];
	char *s, *t;
	int  param;
	int  id, nc;
	int  clientid = 0;
	ECLIENT_LIST  clients[MAX_CLIENT];
	EVENT_EFI_CTL eprobe;

	s = spec;

	if (substr(PREFIX, s)) s += strlen(PREFIX);
	if (substr("//", s))   s += 2;

	t = server;
	while (*s && *s != '/') *t++ = *s++;
	*t = '\0';

	if (*s++ != '/') probe_error(spec, s);
	if (*s == '/') ++s;

	t = instname;
	while (*s != '.') *t++ = *s++;
	*t = '\0';

	if (*s++ != '.') probe_error(spec, s);

	if (isdigit(instname[0])) clientid = atoi(instname);

	param = atoi(s);
	if (!param) probe_error(spec, s);

	printf("setprobe: setting probe daffie://%s/%s.%d\n",
		server, instname, param);

	efi_init();
	event_select_type(0, ET_MIN, ET_MAX);

	id = event_join(server, &nc);
	if (!id) {
		fprintf(stderr, "%s: couldn't join server \"%s\"\n", Pgm, server);
		exit(4);
	}
	printf("%s: joined %s as client id %d (%d clients)\n", Pgm, server, id, nc);

	event_register("earlab", "control", Pgm);

	event_select_type(0, ET_MIN, ET_MAX);
	event_callback(ET_EFI_WAVEDATA,  receive_wavedata);
	event_callback(ET_EFI_SPIKEDATA, receive_spikedata);
	event_select_type(1, ET_EFI_WAVEDATA, ET_EFI_WAVEDATA);
	event_select_type(1, ET_EFI_SPIKEDATA, ET_EFI_SPIKEDATA);
	event_receive_enable(0);
	event_flush(0);

	eclient_monitor();
	if (!clientid)
	   nc = eclient_match(clients, "*", "earlab", "*", instname);
	else
	   nc = eclient_match_id(clients, clientid);

	if (nc < 1) {
		fprintf(stderr, "%s: couldn't find match for %s\n",
			Pgm, instname);
		event_leave();
		exit(1);
	}
	if (nc > 1) {
		fprintf(stderr, "%s: %s matched multiple (%d) clients\n",
			Pgm, instname, nc);
		event_leave();
		exit(1);
	}

	eprobe.type     = EFI_CTL_PROBE;
	eprobe.clientid = id;
	eprobe.tag      = getpid();
	eprobe.param    = param - 1;
	eprobe.flag     = 1;
	eprobe.value    = 0;

	SEND(clients[0].id, EFI_CTL, eprobe);

}

#else

/*
 * Standalone version
*/
INTERNAL void
setprobe(char *spec) {
	fprintf(stderr, "%s: Not DAFFIE/EFI enable\n", Pgm);
	exit(4);
}
#endif

#ifdef XWINDOWS
INTERNAL void
startui(void) {
	xw_track_mode(TRACK_NOBLOCK);
	xw_tbinit();
}

INTERNAL void
updateui(void) {
	int key, x, y;
	return;

	while (!(key = xw_tbxyin(&x, &y)));

	if (key == 2) Done = 1;

	if (key > 0) Redraw = 1;
}
#else
#ifndef WINAPP

INTERNAL void
startui(void) {
}

INTERNAL void
updateui(void) {
}

#endif
#endif

#ifdef WINAPP
INTERNAL void
startui(void) {
}

INTERNAL void
updateui(void) {
}

void do_redraw() {
	Redraw = 1;
	mp_incsema(DataWait);
}

void do_cleanup() {
	Done = 1;
}

void do_mouse_down(int key, int x, int y) {
	if (key == 2) Done = 1;

	if (key) Redraw = 1;
}

void do_key_down(char keycode) {
     /****
	printf("Key press %d\n", keycode);
      ****/
}

void do_key_up(char keycode) {
     /****
	printf("Key release %d\n", keycode);
      ****/
}

INTERNAL void open_file(void);

do_menu(int item) {
	int  xitem;
	char text[1000];

	if (SAMP_MENU_1 <= item && item <= SAMP_MENU_N) {
		CheckMenuItem(MainMenu, SAMP_MENU_0+DownSamp,
			      MF_BYCOMMAND|MF_UNCHECKED);

		CheckMenuItem(MainMenu, item,
			      MF_BYCOMMAND|MF_CHECKED);

		DownSamp = item-SAMP_MENU_0;
		Redraw = 1;
		return(1);
	}

	switch (item) {
	  case FILE_MENU_OPEN:
		open_file();
		return(1);

	  case CHAN_MENU_NCHAN:
		sprintf(text, "%d", NCell);
		DialogSetText(text, CHAN_MENU_TEXTBOX);
		DialogBegin("ChanDlg");
		return(1);

	  case CHAN_MENU_TEXTBOX:
		return(0);

	  case CHAN_MENU_SET:
		DialogGetText(text, CHAN_MENU_TEXTBOX);
		DialogEnd();
		printf("Set channels to %s\n", text);
		NCell = atoi(text);
		Redraw = 1;
		return(1);

	  case CHAN_MENU_CANCEL:
		DialogEnd();
		printf("cancelled channel dialog\n");
		return(1);

	  case PLOT_MENU_SETUP: /* used for setting menu items */
		break;

	  default:
	      printf("Didn't recognize menu pick %d\n", item);
	}

	return(1);
}

INTERNAL char FileFilter[] = {
    'D', 'A', 'T', 'A', '\0',   '*', '.', 'd', 'a', 't', '\0',
    'T', 'E', 'X', 'T', '\0',   '*', '.', 't', 'x', 't', '\0',
    'A', 'N', 'Y',      '\0',   '*', '\0',
    '\0', '\0'
};

INTERNAL void
open_file(void) {
	OPENFILENAME filename;
	char path[1024], file[256];
	int  rc;
	FILE *f;

	path[0] = '\0';
	memset(&filename, 0, sizeof(filename));
	filename.lStructSize        = sizeof(OPENFILENAME);
	filename.hwndOwner          = MainWin;
	filename.lpstrFilter        = FileFilter;
	filename.lpstrCustomFilter  = NULL;
	filename.nFilterIndex       = 1;
	filename.lpstrFile          = path;
	filename.nMaxFile           = sizeof(path)-1;
	filename.lpstrFileTitle     = file;
	filename.nMaxFileTitle      = sizeof(file)-1;
	filename.lpstrInitialDir    = NULL;
	filename.Flags              = OFN_FILEMUSTEXIST;

	rc = GetOpenFileName(&filename);
	if (!rc) {
		MessageBox(MainWin, "cancelled", "quiter", MB_OK);
		return;
	}
	cygwin_conv_to_full_posix_path(path, Filename);

	printf("Opening %s\n", Filename);
	Dataf = fopen(Filename, "r");
	if (!Dataf) {
		fprintf(stderr, "%s: open failed for %s\n", Filename);
	}

	Ndata = 0;

	Redraw = 1;
}
#endif


#define PRT(opt,str) \
	printf(msg?"\t%s%s%s\n":"%s ", opt, (strlen(opt)<8)?"\t\t":"\t", str)

INTERNAL void
usage(char *pgm, int msg) {
	if (msg) printf("usage: %s [options] datafile\n", pgm);
	else     printf("usage: %s ", pgm);
	PRT("[-help]",          "display this message");
	PRT("[-spike]",         "input data are spikes");
	PRT("[-waveform]",      "input data are waveform samples");
	PRT("[-downsample n]",  "downsample input by n");
	PRT("[-frametime ms]",  "specify frame time in milliseconds");
	PRT("[-datscale f]",    "specify object data scaling factor");
	PRT("[-displacement x y z]", "specify object displacment vector");
	PRT("[-xrotation angle","specify display frame x rotation");
	PRT("[-yrotation angle","specify display frame y rotation");
	PRT("[-zrotation angle","specify display frame z rotation");
	PRT("[-map file]",      "read color map for file");
	PRT("[-channel n[.i]]", "select channel i of n");
	PRT("[-autopan]",       "auto pan on incoming data");
	PRT("[-probe]",         "use \"datafile\" as probe name");
	PRT("[-ascii]",         "data file is ascii [default]");
	PRT("[-binary]",        "data file is binary floats");
	PRT("[-bin64]",         "data file is binary doubles");
	PRT("[-downsample n]",  "downsample by n");
	PRT("[-title string]",  "specify plot window title");
	PRT("[-range lo hi]",   "specify data range");
	PRT("[-db]",            "double buffer graphics output");
	PRT("[-double]",        "same as -double");
	PRT("[-title string]",  "specify window title");
	PRT("[-winsize x y]",   "desired display indow size");
	PRT("[-geom msfile]",   "specify geometry data file");
	PRT("[-fps n]",         "desired frame per second");
	PRT("datafile",         "data file");
	printf("\n");
}

