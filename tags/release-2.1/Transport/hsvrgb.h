#ifndef _HSVRGB_H
#define _HSVRGB_H

typedef
struct {
	int r,g,b;
} COLORMAP;

int  make_colormap(char *cfile, COLORMAP map[], int n);
void rgb_to_hsv(float r, float g, float b, float *h, float *s, float *v);
void hsv_to_rgb(float h, float s, float v, float *r, float *g, float *b);
int  shsv_to_rgb(char *hue, float s, float v, float *r, float *g, float *b);
#endif
