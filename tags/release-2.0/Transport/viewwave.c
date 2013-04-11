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
#else
# define XWINDOWS 1
# include <local/xw.h>
#endif

#include <daffie/event.h>

#include "hsvrgb.h"
#ifdef WINAPP
# include "viewwave.h"
#endif

typedef unsigned long long UTime;


#define MAXCHANN   256
#define MAXDATA    5000*MAXCHANN
#define MAX_CMAP   1024

#define MINREDRAW  50
#define NORMREDRAW 100

#define T_LINE     0x0001
#define T_SCATTER  0x0002
#define T_COLORMAP 0x0004
#define T_STACKED  0x0100

#define T_TYPEMASK 0x00ff
#define T_OPTMASK  0xff00

INTERNAL void   usage(char *pgm, int msg);
INTERNAL int    readfile(FILE *f, int n);
INTERNAL float  datum(float data[], int xoff, int chan, int sample);
INTERNAL void   setprobe(char *spec);
INTERNAL void   startui(void);
INTERNAL void   updateui(void);
INTERNAL void   make_rgb_ordering(void);
INTERNAL int    make_rgb(int r, int g, int b);
INTERNAL int    make_rgba(int r, int g, int b, int a);

INTERNAL char  *Pgm;
INTERNAL float *Data  = NULL;
INTERNAL char  *Display = NULL;
INTERNAL int    Ndata = 0;
INTERNAL int    Nchan = 1;
INTERNAL int    Dim[4] = { 0,0,0,0 };
INTERNAL int    Redraw = 0;
INTERNAL int    Done = 0;
INTERNAL int    Double = 0;             /* double buffering flag        */
INTERNAL int    FrontBuf;               /* front display buffer         */
INTERNAL int    BackBuf;                /* backing buffer               */
INTERNAL int    Delay = 0;              /* display frame delay          */
INTERNAL int    PlotType = T_LINE;
INTERNAL int    PanLeft  = 0;
INTERNAL int    PanRight = 0;
INTERNAL int    KeepTrucking = 0;
INTERNAL float  DataMin, DataMax;
INTERNAL float  RangeMin = -1.0, RangeMax = 1.0;
INTERNAL float  Pan = 0.5;
INTERNAL int    Autorange = 1;
INTERNAL int    Autopan   = 0;
INTERNAL UTime  Lastdraw = 0;
INTERNAL int    Binary    = 0;
INTERNAL int    Bin64     = 0;
INTERNAL int    Data_X0   = 0;
INTERNAL int    Data_XN   = 0;
INTERNAL int    Domain_X0 = 0;
INTERNAL float  RangeInc  = 1.0e-5;
INTERNAL int    Rerange = 1;
INTERNAL int    Sample  = 1;
INTERNAL int    DataWait;
INTERNAL int    Frame = 0;
INTERNAL FILE  *Dataf = NULL;
INTERNAL char   Filename[2000];

#define EPSILON 1.0e-10

main(int argc, char *argv[]) {
	char     *file;                         /* data file name       */
	int       ok, help, chann, nchan;       /* cmd line options     */
	int       type;                         /* cmd line options     */
	int       showmap;                      /* more cmd line opts   */
	int       probe;                        /* daffie probe         */
	int       i, j, k;                      /* loop counters        */
	int       nitem;                        /* total data, items per channel */
	int       xs, ys;                       /* screen size          */
	int       backxs, backys;               /* backing window       */
	int       ix, iy;                       /* screen coords        */
	int       x0;                           /* panned data origin   */
	int       pan;                          /* pan value            */
	int       key;                          /* mouse key on press   */
	int       imgxs, imgys, *img;           /* image data           */
	double    range, dscale;                /* data range/scale     */
	float     xscale, yscale, off;          /* data offset & scaling*/
	float     v;                            /* a datum              */
	int       select[MAXCHANN], nselect;    /* select channels      */
	char      title[300] = {'\0'};          /* title bar            */
	char     *cmapfile = NULL;              /* color map file name  */
	COLORMAP  cmap[MAX_CMAP];               /* color map from above */
	int       ncmap;                        /* # entriesin above    */
	int       cindx[256];                   /* allocated (X) color cells */
	int       crgb[256];                    /* corresponding rgb val */
	int       chindx[256];                  /* color index by channel*/
	int       plotx0, ploty0;               /* base of plot area     */
	UTime     now;

	Pgm = argv[0];  if (streq(Pgm, "app_main")) Pgm = "viewdata";
	++argv; --argc;

	/* Set default window size */
	xs = 1000;
	ys = 512;

	/* Set default option values */
	chann = 0;
	nchan = 1;
	help  = 0;
	ok    = 1;
	type  = 0;
	showmap = 0;
	Sample  = 1;
	probe   = 0;
	while (argc && *argv[0] == '-') {
		char *opt;

		opt = argv[0]+1;
		++argv; --argc;

		if (substr(opt, "help")) ++help;
		else if (substr(opt, "scatter"))     type |= T_SCATTER;
		else if (substr(opt, "line"))        type |= T_LINE;
		else if (substr(opt, "colormapped")) type |= T_COLORMAP;
		else if (substr(opt, "stacked"))     type |= T_STACKED;
		else if (substr(opt, "showmap"))     showmap = 1;
		else if (substr(opt, "probe"))       probe   = 1;
		else if (substr(opt, "ascii"))       Binary  = 0;
		else if (substr(opt, "binary"))      Binary  = 1;
		else if (substr(opt, "bin64"))       Bin64   = 1;
		else if (substr(opt, "autopan"))     Autopan = 1;
		else if (substr(opt, "channel")) {
			char *s;
			int           x;

			if (!argc || !isdigit(*argv[0])) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			s = argv[0];
			++argv; --argc;

			nchan = chann = 0;
			while (*s && isdigit(*s))
			   nchan = 10*nchan + *s++ - '0';
			if (*s) ++s;
			while (*s && isdigit(*s))
			  chann = 10*chann + *s++ - '0';

			if (!nchan) {
				fprintf(stderr,
					"%s: invalid argument to -%s\n",
					Pgm, opt);
				ok = 0;
			}
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

			i = 0;
			do {
				x = 0;
				while (isdigit(*s)) x = 10*x + *s++ -'0';
				Dim[i++] = x;
				++s;
			} while (*s == '.' || *s == 'x');

			++argv; --argc;
		}

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
		else if (substr(opt, "pan")) {
			if (!argc || !isdigit(*argv[0])) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			Pan = 0.01 * atoi(argv[0]);
			++argv; --argc;
		}

		else if (substr(opt, "sample")) {
			if (!argc || !isdigit(*argv[0])) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			Sample = atoi(argv[0]);
			++argv; --argc;
		}
		else if (substr(opt, "title")) {
			if (!argc) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			strcpy(title, argv[0]);
			++argv; --argc;
		}

		else if (substr(opt, "display")) {
			if (!argc) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			Display = argv[0];
			++argv; --argc;
		}

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

		else if (substr(opt, "range")) {
			if (argc < 2) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}
			ok &= sscanf(argv[0], " %f", &RangeMin);
			ok &= sscanf(argv[1], " %f", &RangeMax);
			if (!ok) {
				fprintf(stderr,
					"%s: invalid arguments to -%s\n",
					Pgm, opt);
			}
			Autorange = Rerange = 0;
			argv += 2;
			argc -= 2;
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

	/* See if -dimensions was given if -channel was not */
	if (!Nchan && Dim[1]) Nchan = Dim[1];

	file = argv[0];
	++argv; --argc;

	/* Allocate storage for the data */
	Data = (float *)malloc(MAXDATA * sizeof(float));
	if (!Data) {
		fprintf(stderr, "%s: not enough memory for %d datum\n", Pgm, MAXDATA);
		exit(1);
	}

	/* Initialize MP library and allocate a thread semaphore */
	mp_init();

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

	/* Set default range */
	range = RangeMax - RangeMin;
	dscale = 1.0/range;

	strcpy(Filename, file);


	Nchan = nchan;

	if (Nchan > 1) printf("%s: display channel %d of %d\n", Pgm, chann, Nchan);

	PlotType = type;

	if (!title[0]) sprintf(title, "%s %s", Pgm, file);
	xw_title(title);

	/* Initialize display */
#ifdef XWINDOWS
	if (Display) xw_server(Display);
#endif
	xw_size(xs, ys);

	if (!xw_init()) {
		fprintf(stderr, "%s: can't open display %s\n",
			Pgm, Display?Display:"[default]");
		exit(1);
	}
	for (i = 0; i < 8; ++i) xw_setcol(i, (i&1)?255:0, (i&2)?255:0, (i&4)?255:0);

	/* Initializing RGB byte ordering table */
	make_rgb_ordering();

	xw_color(7);

#ifndef WINAPP
	/* Create back buffer */
	if (Double) {
		int xs, ys;

		xw_qsize(&xs, &ys);
		BackBuf  = xw_unused();
		FrontBuf = xw_select(BackBuf);
		xw_pixmap(1);
		xw_size(xs, ys);
		xw_init();
		for (i = 0; i < 8; ++i)
		    xw_setcol(i, (i&1)?255:0, (i&2)?255:0, (i&4)?255:0);

		xw_select(FrontBuf);
	}
#endif

	/* Create color map */
	ncmap = make_colormap(cmapfile, cmap, MAX_CMAP);

	for (i = 0; i < 128; ++i) {
		int k, ci, rgb;

		j  = ncmap-1 - ((float)i)/127.0 * (ncmap-1);
		ci = xw_alloccol(cmap[j].r, cmap[j].g, cmap[j].b);
	     /****
		printf("Allocated color cell %d for index %d (%d %d %d)\n",
			k, i<<1, cmap[j].r, cmap[j].g, cmap[j].b);
	     ****/
#ifndef WINAPP
		if (Double) {
			xw_select(BackBuf);
			xw_setcol(ci, cmap[j].r, cmap[j].g, cmap[j].b);
			xw_select(FrontBuf);
		}
#endif
		k = i<<1;

		cindx[k]   = ci;
		cindx[k+1] = ci;

	     /*****
		rgb = (cmap[j].r<<16) | (cmap[j].g<<8) | cmap[j].b;
	      *****/
		rgb = make_rgb(cmap[j].r, cmap[j].g, cmap[j].b);

		crgb[k]   = rgb;
		crgb[k+1] = rgb;
	}

	if (Nchan) for (i = 0; i < Nchan; ++i) {
		j = ((float) i)/(Nchan-1) * 255.0;
	     /****
		printf("Setting channel %d to cmap entry %d, color cell %d\n", i, j, cindx[j]);
	     ****/
		chindx[i] = cindx[j];
	}

	if (Nchan && !chann) for (i = 0; i < Nchan; ++i) select[i] = 1;
	else for (i = 0; i < Nchan; ++i) select[i] = (i == chann-1);

	if (showmap) {
		int xs, ys, box0, box1;
		float bs;

		xw_qsize(&xs, &ys);
		bs = (float)xs / (float)Nchan;

		for (i = 0; i < Nchan; ++i) {
			printf("Channel %d - color index %d\n", i, chindx[i]);
			xw_color(chindx[i]);
			box0 = i*bs;
			box1 = (i+1)*bs - 1;
			xw_move(box0,  0);
			xw_flbox(box1, ys-1);
		}
		printf("Click in window to continue...\n");
		xw_xyin(&ix, &iy);
	}

	/* Initialize user interface */
	startui();

	x0 = 0;

	img = NULL;
	while (!Done) {
		char  text[100];
		int   border = 4;
		int   th, tw, ta, td;   /* text metrics */

		updateui();

		/* Wait for more data */
		if (probe) mp_decsema(DataWait);

		/* Verify/allocate an in-core image buffer for the display */
		xw_cont();
		xw_qsize(&xs, &ys);
		if (!img || imgxs != xs || imgys != ys) {
			if (img) free(img);
			img = (int *)malloc(xs*ys*sizeof(int));
			if (!img) {
				fprintf(stderr,
					"Not enough memory for %dx%d image buffer\n",
					xs, ys);
				exit(4);
			}
			imgxs = xs;
			imgys = ys;
			memset(img, 0, xs*ys*sizeof(int));
		}

#ifdef XWINDOWS
		if (backxs != xs || backys != ys && Double) {
			xw_select(BackBuf);
			xw_fini();

			BackBuf  = xw_unused();
			xw_select(BackBuf);
			if (Display) xw_server(Display);
			xw_pixmap(1);
			xw_size(xs, ys);
			xw_init();

			/* Set color map for back buffer */
			for (i = 0; i < 8; ++i)
			    xw_setcol(i, (i&1)?255:0, (i&2)?255:0, (i&4)?255:0);
			for (i = 0; i < 128; ++i) {
				int ci, k;

				j = ncmap-1 - ((float)i)/127.0 * (ncmap-1);
				k = i<<1;
				xw_setcol(cindx[k], cmap[j].r, cmap[j].g, cmap[j
			}
			backxs = xs; backys = ys;
			xw_select(FrontBuf);
		}
#endif

		if (!probe) {
			readfile(Dataf, Nchan*xs);

			Rerange = 1;

			printf("%s: read %d items (min = %f, max = %f)\n",
			       Pgm, Ndata, DataMin, DataMax);
		}

		/*
		 * Set effective number of items to plot given number of
		 * channels and sampling size
		 */
		nitem  = Ndata/(Nchan*Sample);

#ifndef WINAPP
		/* Use double buffer on X Windows displays */
		if (Double) xw_select(BackBuf);
#endif

		/* Compute data range if necessary */
		if (Autorange && Rerange) {
			if (DataMin < RangeMin) {
				float delta = RangeMin - DataMin;

				while (RangeInc < delta) {
					if (9.0*RangeInc >= delta)
					  while (RangeInc < delta) RangeInc += RangeInc;
					else RangeInc *= 10.0;
				}
				RangeMin -= RangeInc;
				printf("Set Range min to %f\n", RangeMin);
			}
			if (DataMax > RangeMax) {
				float delta = DataMax - RangeMax;

				while (RangeInc < delta) {
					if (9.0*RangeInc >= delta)
					  while (RangeInc < delta) RangeInc += RangeInc;
					else RangeInc *= 10.0;
				}

				RangeMax += RangeInc;
				printf("Set Range max to %f\n", RangeMax);
			}
			Rerange = 0;
		}
		range  = RangeMax-RangeMin;
		if (range > EPSILON) dscale = 1.0/range;
		else                 dscale = 1.0;
		off    = -RangeMin;

		yscale = ys*dscale;

		/*
		 * Redraw the display
		 */
		xw_erase();
		xw_color(7);
		xw_move(0, 0);
		xw_color(7);
		xw_move(0, 0);

		plotx0 = 0;
		ploty0 = 1;

		nselect = 0;
		for (k = 0; k < Nchan; ++k) if (select[k]) ++nselect;
		if (!nselect) {
			printf("nothing selected!!!");
			break;
		}

		if (PlotType & T_STACKED) yscale /= Nchan;

		if (PlotType & T_COLORMAP) {
			if (!img) for (i = 0; i < min(xs, nitem); ++i) {
				float yband;
				int    iy;

				 yband = (float)(ys - ploty0) / (float)Nchan;

				ix = plotx0 + i;
				xw_move(ix, ploty0);

				for (k = 0; k < Nchan; ++k) {
					int ci;

				     /******
					v  = Data[Nchan*(x0 + i*Sample) + k];
				      *****/
					v = datum(Data, x0+i, k, Sample);

					ci = (v+off)*dscale * 255.0 + 0.5;
					xw_color(cindx[ci]);

					iy = (k+1) * yband;

					xw_draw(ix, iy);
				}
			}
			else {
			  for (i = 0; i < min(xs, nitem/Sample); ++i) {
				float yband;
				int   iy0, iy1;
				int  *pix;

				yband = (float)(ys - ploty0) / (float)Nchan;

				ix  = plotx0 + i;
				iy0 = ploty0;

				pix = img + imgxs * (imgys-1) + ix;

				for (k = 0; k < Nchan; ++k) {
					int ci, rgbcol;

				     /*******
					v  = Data[Nchan*(x0 + i*Sample) + k];
				      *******/
					v = datum(Data, x0+i, k, Sample);

					ci = (v+off)*dscale * 255.0 + 0.5;
					rgbcol = crgb[ci];
					iy1 = (k+1) * yband;

					while (iy0++ <= iy1) {
						*pix = rgbcol;
						pix -= xs;
					}
					--iy0;
				}
			  }
			  /* Fill remainder of image with black */
			  for (ix = i; ix < xs; ++ix)
			    for (iy = ploty0; iy < ys; ++iy)
				img[imgxs*iy + ix] = 0;

			  /* Send above created image to the display */
			  xw_showimage(img, i, ys, 0, ys-1, 0);
			}
		}
		else for (k = 0; k < Nchan; ++k) {
			if (!select[k]) continue;

			if (nselect > 1) {
			     /***
				printf("plotting chan %d color %d\n", k, chindx[k]);
			      ***/
				xw_color(chindx[k]);
			}
			else xw_color(7);

			if (PlotType & T_STACKED) ploty0 = k*(float)ys/(float)Nchan;

			for (i = 0; i < min(xs, nitem/Sample); i += Sample) {
				v  = datum(Data, x0+i, k, Sample);
				ix = plotx0 + i;
				iy = ploty0 + yscale*(v+off);

				if (PlotType & T_SCATTER || !i) xw_move(ix, iy);

				xw_draw(ix, iy);
			}
		}

		if (!(PlotType & T_STACKED)) {
			iy = yscale*off;
			xw_color(1);
			xw_move(plotx0,  1+iy);
			xw_draw(xs,      1+iy);
		}
		xw_color(7);
		sprintf(text, "%d", Domain_X0/nchan + x0);
		xw_qtext(text, &tw, &th, &ta, &td);

		xw_move(border, border);
		xw_text(text);
		xw_move(border, ys - border - th);
		xw_text(text);

		sprintf(text, "%d", Domain_X0/nchan + x0 + xs);
		xw_qtext(text, &tw, &th, &ta, &td);

		xw_move(xs-tw-border, border);
		xw_text(text);
		xw_move(xs-tw-border, ys - border - th);
		xw_text(text);

		now = event_clock();
		if (now - Lastdraw < Delay) {
			int wait;

			wait = Delay - (now - Lastdraw);
			event_sleep(wait);
		}
#ifdef XWINDOWS
		if (Double) {
			int xs, ys;

			xw_qsize(&xs, &ys);
			xw_select(FrontBuf);
			xw_copyfrom(BackBuf, 0, 0, xs, ys, 0, 0);
			xw_flush();
		}
#else
		/* HACK ALERT - workaround for GDI flushing */
		xw_color(0);
		xw_qtext("X", &tw, &th, &ta, &td);
		xw_move(0, border + th);
		xw_text(" ");
#endif
		xw_term();

		Lastdraw = event_clock();
	}
#ifndef WINAPP
	xw_erase();
	xw_flush();
	sleep(1);
	xw_fini();
#endif
}

INTERNAL float
datum(float data[], int xoff, int chan, int sample) {
	int   i, j;
	float v;

	i = Nchan * sample * xoff + chan;
	j = i - Domain_X0 + Data_X0;
	while (j > MAXDATA) j -= MAXDATA;
	v = data[j];
	return (v);
}

INTERNAL int
readdatum(FILE *f, float *datum) {
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
readfile(FILE *f, int n) {
	int   i;
	float mind, maxd;

	if (feof(f)) {
		Data_XN = 0;
		rewind(f);
	}

	if (!readdatum(f, &Data[0])) {
		fprintf(stderr, "%s: couldn't read data file %s\n", Pgm, Filename);
		exit(1);
	}
	mind = maxd = Data[0];
	for (i = 1; !feof(f) && i < n; ++i) {
		if (!readdatum(f, &Data[i])) break;
		if (Data[i] < mind) mind = Data[i];
		if (Data[i] > maxd) maxd = Data[i];
	}

	Ndata  = i;

	/* Check for errors in the data file */
	if (i != n && !feof(f)) {
		char buf[128];

		buf[0] = '\0';
		fgets(buf, 127, f);
		fprintf(stderr, "%s: error after %d items\n", Pgm, Ndata);
		if (!Binary) fprintf(stderr, "%s: %s\n", Pgm, buf);
		exit(4);
	}

	Data_X0  = Data_XN + 1;
	Data_XN  = Data_X0 + Ndata;

	Domain_X0 = Data_X0;

	DataMin = mind;
	DataMax = maxd;

	return(Ndata);
}

#ifndef NOEFI
#include <daffie/event.h>
#include <daffie/eventdef.h>
#include <daffie/eclient.h>
#include "efievent.h"

INTERNAL void
probe_error(s) {
	fprintf(stderr, "%s: invalid probe \"%s\"\n", Pgm, s);
	exit(4);
}

INTERNAL void
efi_init(void) {
	event_verbose(0);
	event_init();
#include "agentddef.h"
#include "efieventdef.h"
}

INTERNAL int
receive_spikedata(EVENT_EFI_SPIKEDATA *edata) {
	/* Ignore for now */

}

INTERNAL int
receive_wavedata(EVENT_EFI_WAVEDATA *edata) {
	int        i, j;
	int        nitem;
	TMatrixN_T d;

	nitem = edata->indexN - edata->index0 + 1;
	printf("%s: received EFI %d datum, frame %d, tag %d from %d\n",
	       Pgm, nitem, edata->frame, edata->tag,
	       EventFrom(edata));

	j = Data_XN;
	for (i = 0; i < nitem; ++i) {
		d = edata->data[i];
		if (d < DataMin) DataMin = d;
		if (d > DataMax) DataMax = d;
		Data[j++] = d;
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
	printf("Data %d items ranging %f - %f\n", Ndata, DataMin, DataMax);
	Redraw = Rerange = 1;
	if (Autopan) PanRight = 1;

	/* If this is the last block of the frame, wake up main thread */
	if (edata->indexN == edata->elems-1) {
		++Frame;
		Data_X0 = Data_XN = 0;
		mp_incsema(DataWait);
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

	if (*s++ != '/') probe_error(spec);
	if (*s == '/') ++s;

	t = instname;
	while (*s != '.') *t++ = *s++;
	*t = '\0';

	if (*s++ != '.') probe_error(spec);

	if (isdigit(instname[0])) clientid = atoi(instname);

	param = atoi(s);
	if (!param) probe_error(spec);

	printf("setprobe: setting probe daffie://%s/%s.%d\n",
		server, instname, param);

	efi_init();
	event_select_type(0, 0, ET_MAX);

	id = event_join(server, &nc);
	if (!id) {
		fprintf(stderr, "%s: couldn't join server \"%s\"\n", Pgm, server);
		exit(4);
	}
	printf("%s: joined %s as client id %d (%d clients)\n", Pgm, server, id, nc);

	event_register("earlab", "control", Pgm);

	event_select_type(0, 0, ET_MAX);
	event_select_type(1, ET_EFI_WAVEDATA,  ET_EFI_WAVEDATA);
	event_select_type(1, ET_EFI_SPIKEDATA, ET_EFI_SPIKEDATA);
	event_callback(ET_EFI_WAVEDATA,  receive_wavedata);
	event_callback(ET_EFI_SPIKEDATA, receive_spikedata);
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

	while (!(key = xw_tbxyin(&x, &y)));

	PanLeft = PanRight = 0;
	if      (key == 1) PanLeft  = 1;
	else if (key == 3) PanRight = 1;
	else if (key == 2) Done = 1;

	if (key > 0) Redraw = 1;
}
#else
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
	if      (key == 1) PanLeft  = 1;
	else if (key == 3) PanRight = 1;
	else if (key == 2) Done = 1;

	if (key) Redraw = 1;
}

void do_key_down(char keycode) {
     /****
	printf("Key press %d\n", keycode);
      ****/
	if      (keycode == 39) PanRight = 1;
	else if (keycode == 37) PanLeft  = 1;
	KeepTrucking = 1;
}

void do_key_up(char keycode) {
     /****
	printf("Key release %d\n", keycode);
      ****/
	PanLeft = PanRight = KeepTrucking = 0;
}


INTERNAL void open_file(void);

do_menu(int item) {
	int  xitem;
	char text[1000];

	if (SAMP_MENU_1 <= item && item <= SAMP_MENU_N) {
		CheckMenuItem(MainMenu, SAMP_MENU_0+Sample,
			      MF_BYCOMMAND|MF_UNCHECKED);

		CheckMenuItem(MainMenu, item,
			      MF_BYCOMMAND|MF_CHECKED);

		Sample = item-SAMP_MENU_0;
		Redraw = 1;
		return(1);
	}

	switch (item) {
	  case FILE_MENU_OPEN:
		open_file();
		return(1);

	  case CHAN_MENU_NCHAN:
		sprintf(text, "%d", Nchan);
		DialogSetText(text, CHAN_MENU_TEXTBOX);
		DialogBegin("ChanDlg");
		return(1);

	  case CHAN_MENU_TEXTBOX:
		return(0);

	  case CHAN_MENU_SET:
		DialogGetText(text, CHAN_MENU_TEXTBOX);
		DialogEnd();
		printf("Set channels to %s\n", text);
		Nchan = atoi(text);
		Redraw = 1;
		return(1);

	  case CHAN_MENU_CANCEL:
		DialogEnd();
		printf("cancelled channel dialog\n");
		return(1);

	  case PLOT_MENU_SETUP: /* used for setting menu items */
		break;

	  case PLOT_MENU_LINE:
		PlotType &= ~0xff;
		PlotType |= T_LINE;
		Redraw   = 1;
		break;

	  case PLOT_MENU_SCATTER:
		PlotType &= ~0xff;
		PlotType |= T_SCATTER;
		Redraw   = 1;
		break;

	  case PLOT_MENU_COLMAP:
		PlotType &= ~0xff;
		PlotType |= T_COLORMAP;
		Redraw   = 1;
		break;

	  case PLOT_MENU_STACKED:
		if (PlotType & T_STACKED) PlotType &= ~T_STACKED;
		else                      PlotType |=  T_STACKED;
		Redraw   = 1;
		break;

	  default:
	      printf("Didn't recognize menu pick %d\n", item);
	}

	CheckMenuItem(MainMenu, PLOT_MENU_LINE,
		      MF_BYCOMMAND|(PlotType&T_LINE?MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem(MainMenu, PLOT_MENU_SCATTER,
		      MF_BYCOMMAND|(PlotType&T_SCATTER?MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem(MainMenu, PLOT_MENU_COLMAP,
		      MF_BYCOMMAND|(PlotType&T_COLORMAP?MF_CHECKED:MF_UNCHECKED));

	CheckMenuItem(MainMenu, PLOT_MENU_STACKED,
		      MF_BYCOMMAND|(PlotType&T_STACKED?MF_CHECKED:MF_UNCHECKED));

	if (PlotType & T_COLORMAP)
	 EnableMenuItem(MainMenu, PLOT_MENU_STACKED, MF_GRAYED);
	else
	 EnableMenuItem(MainMenu, PLOT_MENU_STACKED, MF_ENABLED);

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

	Rerange = Redraw = 1;
}
#endif


#define PRT(opt,str) \
	printf(msg?"\t%s%s%s\n":"%s ", opt, (strlen(opt)<8)?"\t\t":"\t", str)

INTERNAL void
usage(char *pgm, int msg) {
	if (msg) printf("usage: %s [options] datafile\n", pgm);
	else     printf("usage: %s ", pgm);
	PRT("[-help]",          "display this message");
	PRT("[-scatter]",       "scatter plot");
	PRT("[-line]",          "line plot");
	PRT("[-colormapped]",   "color mapped image plot");
	PRT("[-map file]",      "read color map for file");
	PRT("[-channel n[.i]]", "select channel i of n");
	PRT("[-stacked]",       "stack channel plots");
	PRT("[-autopan]",       "auto pan on incoming data");
	PRT("[-pan percent]",   "pan by given percentage [default 50]");
	PRT("[-probe]",         "use \"datafile\" as probe name");
	PRT("[-ascii]",         "data file is ascii [default]");
	PRT("[-binary]",        "data file is binary floats");
	PRT("[-sample n]",      "downsample by n");
	PRT("[-title string]",  "specify plot window title");
	PRT("[-range lo hi]",   "specify data range");
	PRT("[-db]",            "double buffer graphics output");
	PRT("[-double]",        "same as -double");
	PRT("datafile",         "data file");
	printf("\n");
}

/*
 * Deal with various display and host byte orderings
 */
INTERNAL int R_Shift, G_Shift, B_Shift, A_Shift;

INTERNAL void
make_rgb_ordering(void) {
	int  byte_0, byte_1, byte_2, byte_3;
	BYTE *p;
	int  order;

	/* Determine machine byte order */
	order = 1;
	p     = (BYTE *)&order;
	if (*p == 1) {  /* Little-endian */
		byte_0 = 0;
		byte_1 = 8;
		byte_2 = 16;
		byte_3 = 24;
	}
	else {          /* Big-endian    */
		byte_0 = 24;
		byte_1 = 16;
		byte_2 = 8;
		byte_3 = 0;
	}

	order = xw_qformat();

	switch (order) {
	  case XW_PIXEL_RGBA:
		R_Shift = byte_0;
		G_Shift = byte_1;
		B_Shift = byte_2;
		A_Shift = byte_3;
		break;

	  case XW_PIXEL_BGRA:
		B_Shift = byte_0;
		G_Shift = byte_1;
		R_Shift = byte_2;
		A_Shift = byte_3;
		break;

	  case XW_PIXEL_ARGB:
		A_Shift = byte_0;
		R_Shift = byte_1;
		G_Shift = byte_2;
		B_Shift = byte_3;
		break;

	  case XW_PIXEL_ABGR:
		A_Shift = byte_0;
		B_Shift = byte_1;
		G_Shift = byte_2;
		R_Shift = byte_3;
		break;
	}
}

INTERNAL int
make_rgb(int r, int g, int b) {
	int rgb;

	rgb = ((r&0xff)<<R_Shift) | ((g&0xff)<<G_Shift) | ((b&0xff)<<B_Shift);

	return(rgb);
}

INTERNAL int
make_rgba(int r, int g, int b, int a) {
	int rgba;

	rgba = ((r&0xff)<<R_Shift) | ((g&0xff)<<G_Shift) |
	       ((b&0xff)<<B_Shift) | ((a&0xff)<<A_Shift);

	return(rgba);
}


#ifndef WINAPP
int
xw_showimage(void *pix, int xs, int ys, int x0, int y0, int opts) {
	int  i, xsize;
	int *p;

	p = (int *)pix;
	for (i = 0; i < ys; ++i) {
		xw_move(x0, y0-i);
		xw_wrras((BYTE *)p, xs);
		p += xs;
	}
}

#endif

