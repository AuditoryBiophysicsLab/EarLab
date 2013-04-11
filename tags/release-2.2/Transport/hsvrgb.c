/*
 * Show Color Map
 */
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <local/std.h>
#include <local/xw.h>

#include "hsvrgb.h"

#ifdef MAIN
INTERNAL char *Pgm;

main(argc, argv)
 int   argc;
 char *argv[];
 {
	char *cmap;
	COLORMAP map[256];
	int  index[256];
	int  i, j, k;
	int  x, y, key;

	Pgm = argv[0];
	++argv; --argc;

	if (argc) {
		cmap = argv[0];
		++argv; --argc;
	}
	else cmap = NULL;

	make_colormap(cmap, map, 256);

	xw_size(512,32);
	if (cmap) xw_title(cmap);
	else      xw_title("DEFAULT (hsv spectrum)");
	xw_init();
	for (i = 0; i < 128; ++i) {
		j = 2*i;
		k = xw_alloccol(map[j].r, map[j].g, map[j].b);
		xw_color(k);
		index[i] = k;
		j = 4*i;
		xw_move(j, 0);
		xw_flbox(j+3, 31);
	}
	FOREVER {
		key = xw_xyin(&x, &y);
		if (key == 3) break;
		if (key == 2) { draw_map(index); continue; }

		x >>= 1;
		printf("%d: %d %d %d\n", x, map[x].r, map[x].g, map[x].b);
	}
	xw_fini();
}

/*
 * Draw the color map
 */
draw_map(int *index)
 {
	int i, j;

	for (i = 0; i < 128; ++i) {
		xw_color(index[i]);
		j = 4*i;
		xw_move(j, 0);
		xw_flbox(j+3, 31);
	}
}
#endif

/*
 * Create or read in color map
 */
int
make_colormap(char *cfile, COLORMAP map[], int n) {
	FILE *cf;
	int   i, j;
	float fr, fg, fb;
	float hue;
	int   r, g, b;
	int   rc;
	char  buf[512];

	/* Create color map if requested */
	if (cfile == NULL) for (i = 0; i < n; ++i) {

		hue = 240.0 * (float)i / (float)(n-1);
		hsv_to_rgb(hue, 1.0, 1.0, &fr, &fg, &fb);
		map[i].r = fr*255.0 + 0.5;
		map[i].g = fg*255.0 + 0.5;
		map[i].b = fb*255.0 + 0.5;
	}

	/* Read color map from user specified file */
	else  {
		cf = fopen(cfile, "r");
		if (cf == NULL) {
			fprintf(stderr,
				"make_colormap: can't open color map file %s\n",
				cfile);
			return(0);
		}
		j = 0;
		while (fgets(buf, 512, cf) != NULL) {
			rc = sscanf(buf, " %d %d %d %d", &i, &r, &g, &b);
			if (rc == 3) {
				b = g;
				g = r;
				r = i;
				i = j;
			}
			else if (rc != 4) break;
			if (i >= n) continue;
			map[i].r = r;
			map[i].g = g;
			map[i].b = b;
			++j;
		}
		if (!feof(cf)) {
			fprintf(stderr,
				"make_colormap: error reading color map file '%s' at line %d\n",
				cfile, j+1);
			exit(4);
		}
		fclose(cf);
		if (i < n) ++i;
	}
	return(i);
}

#define SETRGB(x,y,z) { *r = x; *g = y; *b = z;}
#define LIMIT(v,m,n) { if (v < m) v = m; if (v > n) v = n; }

/*
 * RGB to HSV color conversion
 */
struct ct {
   char *color;
   float value;
};

INTERNAL struct ct
colrtabl[] = {
	 "red",         0.0,
	 "orange",     30.0,
	 "yellow",     60.0,
	 "green",     120.0,
	 "cyan",      180.0,
	 "blue",      240.0,
	 "magenta",   300.0,
	 NULL,          0.0
};

INTERNAL int
gethue(char *hue, float *h) {
   int i;

   if (sscanf(hue, "%f", h) == 0) {
	for (i=0; colrtabl[i].color; i++)
	  if (strcmp(hue,colrtabl[i].color) == 0) {
		*h = colrtabl[i].value;
		return(1);
	  }
	return(0);
   }
   return(1);
}

void
hsv_to_rgb(float h, float s, float v, float *r, float *g, float *b) {
	int i;
	float f, p, q, t;

	/* Limit Saturation and Value */
	LIMIT(s, 0.0, 1.0);
	LIMIT(v, 0.0, 1.0);

	/* Value == 0 return black */
	if (v == 0.0) {
		SETRGB(0.0,0.0,0.0);
		return;
	}

	/* Saturation == 0 return grey */
	if (s == 0.0) {
	    *r = *g = *b = v;
	    return;
	}

	/* Wrap Hue value */
	while (h >= 360.0) h -= 360.0;
	while (h <  0.0  ) h += 360.0;

	h /= 60.0;
	i = h;  f = h - i;
	p = v * (1.0 - s);
	q = v * (1.0 - (s * f));
	t = v * (1.0 - (s * (1.0 - f)));
	switch(i) {
	   case 0: SETRGB(v,t,p); break;
	   case 1: SETRGB(q,v,p); break;
	   case 2: SETRGB(p,v,t); break;
	   case 3: SETRGB(p,q,v); break;
	   case 4: SETRGB(t,p,v); break;
	   case 5: SETRGB(v,p,q); break;
	   default: printf("hsv_to_rgb: switch error!! sector=%d\n",i);
	}
}

/*
 * Converted named hue to rgb
 */
int
shsv_to_rgb(char *hue, float s, float v, float *r, float *g, float *b) {
	int i;
	float h;
	float f, p, q, t;

	/* Limit Saturation and Value */
	LIMIT(s, 0.0, 1.0);
	LIMIT(v, 0.0, 1.0);

	/* Value == 0 return black */
	if (v == 0.0) {
		SETRGB(0.0,0.0,0.0);
		return(1);
	}

	/* Saturation == 0 return grey */
	if (s == 0.0) {
	    *r = *g = *b = v;
	    return(1);
	}

	/* Get Hue in degrees */
	if (!gethue(hue, &h)) return(0);

	/* Wrap Hue value */
	while (h >= 360.0) h -= 360.0;
	while (h <  0.0  ) h += 360.0;

	h /= 60.0;
	i = h;  f = h - i;

	p = v * (1.0 - s);
	q = v * (1.0 - (s * f));
	t = v * (1.0 - (s * (1.0 - f)));
	switch(i) {
	   case 0: SETRGB(v,t,p); break;
	   case 1: SETRGB(q,v,p); break;
	   case 2: SETRGB(p,v,t); break;
	   case 3: SETRGB(p,q,v); break;
	   case 4: SETRGB(t,p,v); break;
	   case 5: SETRGB(v,p,q); break;
	   default: printf("switch error: %d\n",i);
		return(0);
	}
	return(1);
}

/*
 * RGB to HSV color conversion
 */
#define RED 0
#define GRN 1
#define BLU 2

void
rgb_to_hsv(float r, float g, float b, float *h, float *s, float *v) {
     int   col;
     float col_max, col_min, col_dif, r_dist, g_dist, b_dist;
     float hue, sat, val;

     if (r >= g && r >= b) { col_max = r; col = RED; }
     if (g >= r && g >= b) { col_max = g; col = GRN; }
     if (b >= g && b >= r) { col_max = b; col = BLU; }

     if (r <= g && r <= b)   col_min = r;
     if (g <= r && g <= b)   col_min = g;
     if (b <= g && b <= r)   col_min = b;

     col_dif = col_max - col_min;
     val = col_max;

     if (col_max == 0.0) sat = 0.0;
     else sat = col_dif / col_max;

     if (sat != 0.0) {
	   r_dist = (col_max - r)/col_dif;
	   g_dist = (col_max - g)/col_dif;
	   b_dist = (col_max - b)/col_dif;

	   switch(col) {
	      case RED: hue =       b_dist - g_dist; break;
	      case GRN: hue = 2.0 + r_dist - b_dist; break;
	      case BLU: hue = 4.0 + g_dist - r_dist; break;
	   }

	   hue *= 60.0;
	   if (hue < 0.0) hue += 360.0;
     }
     else hue = 0.0;

     *h = hue;
     *s = sat;
     *v = val;
}
