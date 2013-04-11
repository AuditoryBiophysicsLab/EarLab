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

#ifdef OGL
# include <local/ogl.h>
# include <local/xwogl.h>
#endif

#include <daffie/event.h>

#include "hsvrgb.h"
#ifdef WINAPP
# include "viewspike.h"
#endif

typedef unsigned long long UTime;

#define MAXCHANN   256
#define MAXDATA    5000*MAXCHANN
#define MAX_CMAP   1024

#define MINREDRAW  50
#define NORMREDRAW 100

typedef struct {
	int   cell;
	float time;
} SPIKEDATA;

INTERNAL void   usage(char *pgm, int msg);
INTERNAL int    readfile(FILE *f, int n);
INTERNAL void   setprobe(char *spec);
INTERNAL void   startui(void);
INTERNAL void   updateui(void);
INTERNAL void   make_rgb_ordering(void);
INTERNAL int    make_rgb(int r, int g, int b);
INTERNAL int    make_rgba(int r, int g, int b, int a);

INTERNAL char  *Pgm;
INTERNAL SPIKEDATA *Data  = NULL;
INTERNAL char  *XDisplay = NULL;
INTERNAL int    Ndata = 0;
INTERNAL int    NCell   = 0;
INTERNAL int    NChan   = 0;
INTERNAL int    CellPerChan = 0;
INTERNAL int    Redraw = 0;
INTERNAL int    Done = 0;
INTERNAL int    Double = 0;             /* double buffering flag        */
INTERNAL int    FrontBuf;               /* front display buffer         */
INTERNAL int    BackBuf;                /* backing buffer               */
INTERNAL int    Delay = 0;              /* display frame delay          */
INTERNAL int    PlotType = 0;
INTERNAL float  MinTime, MaxTime;
INTERNAL float  FrameTime = 0.0;
INTERNAL int    CellMin, CellMax;
INTERNAL unsigned long long Lastdraw = 0;
INTERNAL int    Binary    = 0;
INTERNAL int    Bin64     = 0;
INTERNAL float  Time_X0 = 0.0;
INTERNAL float  Time_XN = 0.0;
INTERNAL int    Data_X0 = 0;
INTERNAL int    Data_XN = 0;
INTERNAL float  Domain_X0 = 0.0;
INTERNAL int    Sample  = 1;
INTERNAL int    DataWait;
INTERNAL int    Frame = 0;
INTERNAL FILE  *Dataf = NULL;
INTERNAL char   Filename[2000];

#define EPSILON 1.0e-10

main(int argc, char *argv[]) {
	char     *file;                         /* data file name       */
	int       ok, help;                     /* cmd line options     */
	int       type;                         /* cmd line options     */
	int       showmap;                      /* more cmd line opts   */
	int       probe;                        /* daffie probe         */
	int       i, j, k;                      /* loop counters        */
	int       nitem;                        /* total data, items per channel */
	int       xs, ys;                       /* screen size          */
	int       backxs, backys;               /* back buffer          */
	int       ix, iy;                       /* screen coords        */
	int       x0;                           /* panned data origin   */
	int       pan;                          /* pan value            */
	int       key;                          /* mouse key on press   */
	int       imgxs, imgys, *img;           /* image data           */
	double    range, dscale;                /* data range/scale     */
	float     xscale, yscale, off;          /* data offset & scaling*/
	float     v;                            /* a datum              */
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

	/* Default window size */
	xs = 1000;
	ys = 512;

	help    = 0;
	ok      = 1;
	type    = 0;
	showmap = 0;
	Sample  = 1;
	probe   = 0;
	while (argc && *argv[0] == '-') {
		char *opt;

		opt = argv[0]+1;
		++argv; --argc;

		if (substr(opt, "help")) ++help;
		else if (substr(opt, "showmap"))     showmap = 1;
		else if (substr(opt, "probe"))       probe   = 1;
		else if (substr(opt, "ascii"))       Binary  = 0;
		else if (substr(opt, "binary"))      Binary  = 1;
		else if (substr(opt, "bin64"))       Bin64   = 1;
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
		else if (substr(opt, "cells")) {
			if (!argc || !isdigit(*argv[0])) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			NCell = atoi(argv[0]);
			++argv; --argc;
		}
		else if (substr(opt, "channel") || substr(opt, "dimensions")) {
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
			x = 0;
			while (isdigit(*s)) x = 10*x + *s++ -'0';
			NChan = x;

			if (*s == '.' || *s == 'x') {
				++s;

				x = 0;
				while (isdigit(*s)) x = 10*x + *s++ -'0';
				CellPerChan = x;
			}
			++argv; --argc;
		}
		else if (substr(opt, "time") || substr(opt, "frametime")) {
			if (!argc || !isdigit(*argv[0])) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				continue;
			}

			sscanf(argv[0], " %f", &FrameTime);
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

			XDisplay = argv[0];
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
	if      (NChan && CellPerChan && !NCell) NCell = NChan * CellPerChan;
	else if (NChan && NCell & !CellPerChan)  CellPerChan = NCell/NChan;
	else if (NCell && CellPerChan && !NChan) NChan = NCell / CellPerChan;

	file = argv[0];
	++argv; --argc;

	/* Allocate storage for the data */
	Data = (SPIKEDATA *)malloc(MAXDATA * sizeof(SPIKEDATA));
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

	strcpy(Filename, file);

	PlotType = type;

	if (!title[0]) sprintf(title, "%s %s", Pgm, file);
	xw_title(title);

	/* Initialize display */
	xw_size(xs, ys);
	if (XDisplay) xw_server(XDisplay);
	if (!xw_init()) {
		fprintf(stderr, "%s: can't open display %s\n",
			Pgm, XDisplay?XDisplay:"[default]");
		exit(1);
	}
	for (i = 0; i < 8; ++i) xw_setcol(i, (i&1)?255:0, (i&2)?255:0, (i&4)?255:0);

	/* Initializing RGB byte ordering table */
	make_rgb_ordering();

	xw_color(7);

#ifdef XWINDOWS
	/* Create back buffer */
	if (Double) {
		xw_qsize(&backxs, &backys);
		BackBuf  = xw_unused();
		FrontBuf = xw_select(BackBuf);
		if (XDisplay) xw_server(XDisplay);
		xw_pixmap(1);
		xw_size(backxs, backys);
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
#ifdef XWINDOWS
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

	if (NChan) for (i = 0; i < NChan; ++i) {
		j = ((float) i)/(NChan-1) * 255.0;
		chindx[i] = cindx[j];
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
			if (XDisplay) xw_server(XDisplay);
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
				xw_setcol(cindx[k], cmap[j].r, cmap[j].g, cmap[j].b);
			}
			backxs = xs; backys = ys;
			xw_select(FrontBuf);
		}
#endif

		if (!probe) {
			readfile(Dataf, xs*NChan);
		}

		/*
		 * Set effective number of items to plot given number of
		 * channels and sampling size
		 */
		nitem  = Ndata;

#ifdef XWINDOWS
		/* Use double buffer on X Windows displays */
		if (Double) xw_select(BackBuf);
#endif

		/*
		 * Redraw the display
		 */
		xw_erase();


		plotx0 = 0;
		ploty0 = 1;

		yscale = (float)ys / (float)NCell;
		xscale = (float)xs / (MaxTime - MinTime);

		for (i = 0; i < nitem; ++i) {
			int   cell;
			float time;
			int   ix, iy;

			cell = Data[i].cell;
			time = Data[i].time;

			ix = xscale * time + 0.5;
			iy = yscale * cell + 0.5;

			if (ix > xs || iy > ys) continue;

			if (!NChan) xw_color(7);
			else        xw_color(chindx[cell/CellPerChan]);

			xw_move(ix, iy);
			xw_flbox(ix+3, iy+2);
		}

		xw_color(7);
		if (NChan) for (i = 1; i < NChan; ++i) {
			int iy;

			iy = i*(float)ys/NChan + 0.5;
			xw_move(0, iy);
			xw_draw(xs-1, iy);
		}
		xw_color(7);
		sprintf(text, "%5.3f", Time_X0);
		xw_qtext(text, &tw, &th, &ta, &td);

		xw_move(border, border);
		xw_text(text);
		xw_move(border, ys - border - th);
		xw_text(text);

		sprintf(text, "%5.3f", Time_XN);
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
#ifdef XWINDOWS
	xw_erase();
	xw_flush();
	sleep(1);
	xw_fini();
#endif
}

INTERNAL int
readdatum(FILE *f, SPIKEDATA *datum) {
	int rc;

	if (Binary || Bin64) {
		int    id;
		float  ft;
		double dt;

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
		return(rc);
	}
	else {
		int frame;

		rc = fscanf(f, " %d %d %f", &frame, &datum->cell, &datum->time);
		if (!rc) return(0);
		else return(frame);
	}
}

INTERNAL int
readfile(FILE *f, int n) {
	int   i;
	float mind, maxd;
	static SPIKEDATA lastspike;
	static int       havespike = 0, frame;

	if (feof(f)) {
		Time_X0 = Time_XN = 0.0;
		havespike = 0;
		Frame     = 0;
		rewind(f);
	}
	++Frame;

	if (!havespike) {
		frame = readdatum(f, &lastspike);
		havespike = 1;
	}
	if (Frame != frame) return(0);

	Data[0] = lastspike;

	for (i = 1; !feof(f) && i < n; ++i) {
		frame = readdatum(f, &lastspike);
		if (frame != Frame) break;

		Data[i] = lastspike;

		if (Data[i].time < MinTime) MinTime = Data[i].time;
		if (Data[i].time > MaxTime) MaxTime = Data[i].time;
		if (Data[i].cell > NCell)   NCell   = Data[i].cell;
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

	printf("Read %d items on frame %d - time %f-%f, %d cells\n",
		Ndata, Frame, MinTime, MaxTime, NCell);

	return(Ndata);
}

#ifndef NOEFI
#include <daffie/event.h>
#include <daffie/eventdef.h>
#include <daffie/eclient.h>
#include "efievent.h"

INTERNAL void
probe_error(char *s) {
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
	TSpike *d;
	int     i, j, nitem;

	nitem = edata->indexN - edata->index0 + 1;
	printf("%s: received EFI %d spikes, frame %d, tag %d from %d\n",
	       Pgm, nitem, edata->frame, edata->tag,
	       EventFrom(edata));

	j = Data_XN;
	for (i = 0; i < nitem; ++i) {
		d = &edata->data[i];
		if (d->time < MinTime) MinTime = d->time;
		if (d->time > MaxTime) MaxTime = d->time;
		if (d->cell > NCell)   NCell   = d->cell;

		Data[j].cell = d->cell;
		Data[j].time = d->time;
		++j;
	}
	Data_XN = Ndata = j;

	printf("Data %d items ranging %f - %f\n", Ndata, MinTime, MaxTime);
	Redraw = 1;

	/* If this is the last block of the frame, wake up main thread */
	if (edata->indexN == edata->elems-1) {
		++Frame;
		if (FrameTime > 0.0) {
			Time_X0  = edata->frame * FrameTime;
			Time_XN  = Time_X0 + FrameTime;
		}
		else {
			Time_X0 = MinTime;
			Time_XN = MaxTime;
		}
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
	event_select_type(1, ET_EFI_SPIKEDATA, ET_EFI_SPIKEDATA);
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
	return;

	while (!(key = xw_tbxyin(&x, &y)));

	if (key == 2) Done = 1;

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
	PRT("[-bin64]",         "data file is binary doubles");
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


#ifdef XWINDOWS
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

