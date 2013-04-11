/*
 * Wave reader/player/viewer module
 */
#ifdef OGL
# include <local/ogl.h>
# include <local/xwogl.h>
# ifdef WINDOWS
#  include <local/gd.h>
#  define main app_main
#  define WINAPP
# endif
#else
# include <local/xw.h>
#endif

#define NIN  0
#define NOUT 1

#ifndef MODULE
# define MODULE WaveReader
#endif

#include <stdio.h>
#include <strings.h>

#include <local/std.h>
#include <local/audiolib.h>

#include "module.c"
#include "params.h"

typedef int RANK[MAX_INPUT][EFI_MAXDIM];

INTERNAL char      Prtname[200]  = "";
INTERNAL char      Instname[200] = "";
INTERNAL char      Filename[200];
INTERNAL FD        Wavefd;
INTERNAL HWAVEOUT  Audiofd;
INTERNAL WAVEHEAD  Wave;
INTERNAL int       SentDone = 0;
INTERNAL int       Freebuf  = 0;
INTERNAL int       DoneWait;
INTERNAL BYTE     *Buf1, *Buf2;
INTERNAL int       Bufsize;
INTERNAL int       Chunk;
INTERNAL int       Ibuf, Ichunk;
INTERNAL int       BufAvail;
INTERNAL int       Outsamp;
INTERNAL int       View = 1;
INTERNAL int       Play = 1;
INTERNAL char      XDisplay[200] = "";
INTERNAL int       Loop = 0;
INTERNAL int       Eof;
INTERNAL int       Mix  = 1;
INTERNAL int       DownSample = 4;
INTERNAL int       Nin, Nout;
INTERNAL float     SPL = 1.0;

INTERNAL int  AudioSent(int tag);
INTERNAL void WaveShow(int bufno);

ModuleSetName(char *instname) {
	if (instname) strcpy(Instname, instname);
#ifdef WINAPP
	gd_title("WaveReader");
	gd_init();
	gd_erase();
#endif
	return(1);
}

ModuleReadParameters(char *filename, char *section) {
	int   i, n;
	char *p, *q;

	if (Instname[0]) sprintf(Prtname, "%s.%s", Instname, MODULENAME);
	else {
		strcpy(Prtname,  MODULENAME);
		strcpy(Instname, MODULENAME);
	}

	printf("%s: processing parameter file %s:%s\n",
	       Prtname, filename?filename:"[NONE]", section);

	ReadParameterFile(filename);

	return (1);
}

ModuleStart(int nin,  int intype[],  RANK inrank,
		 int nout, int outtype[], RANK outrank,
		 int outsize[]) {
	char  *p;
	int   i, rc;
	int   filesize;
	char  str[100];

	/*
	 * Basic testing on input/output counts and types
	 */
	for (i = 0; i < nout; ++i) {
		if (outtype[i] != EFI_STREAM_WAVEFORM) {
			fprintf(stderr,
				"%s: ****ERROR**** only supports waveforms\n",
				Prtname);
			return(0);
		}
		outsize[i] = outrank[i][0];
	}

	rc = GetSectionParameterString(Instname, "MonoMix", str);
	if (!rc) Mix = 1;
	else if (strimatch("Left",  str)) Mix = 1;
	else if (strimatch("Right", str)) Mix = 2;
	else if (strimatch("Both",  str)) Mix = 3;

	Play = View = Loop = 0;
	Chunk = 0;

	GetSectionParameterBool(Instname,   "Play", &Play);
	GetSectionParameterBool(Instname,   "View", &View);
	GetSectionParameterBool(Instname,   "Loop", &Loop);
	GetSectionParameterInt(Instname,    "ViewDownSample", &DownSample);

	GetSectionParameterInt(Instname,    "Chunks", &Chunk) ||
	GetSectionParameterInt(Instname,    "FramesPerBuffer", &Chunk);

	GetSectionParameterString(Instname, "Display", XDisplay);
	GetSectionParameterFloat(Instname,  "SoundPressureLevel", &SPL);
	GetSectionParameterBool(Instname,   "Verbose", &Verbose);

	printf("%s: play %s, view %s, loop %s, spl = %f\n",
		Prtname,
		Play?"on":"off",
		View?"on":"off",
		Loop?"on":"off",
		SPL);
	fflush(stdout);

	/*
	 * Get file name and open
	 */
	rc = GetSectionParameterString(Instname, "InputFileName", Filename);
	if (!rc) {
		fprintf(stderr,
			"%s: ****ERROR**** no input file specified\n",
			Prtname);
		return(0);
	}

	Wavefd = wave_read_open(Filename, &Wave, &filesize);
	if (!Wavefd) {
		fprintf(stderr,
			"%s: ****ERROR**** open failed for input file %s\n",
			Prtname, Filename);
		return(0);
	}
	Eof = 0;

	printf("%s: %s encoded %d channel%s@ %dHz, %d bytes/sec, %d bytes/sample, %d bit\n",
		Prtname,
		Wave.AudioFormat==1?"PCM":"*unknown*",
		Wave.NumChannels,
		Wave.NumChannels==1?" ":"s ",
		Wave.SampleRate,
		Wave.ByteRate,
		Wave.BlockAlign,
		Wave.BitsPerSample);

	/*
	 * Determine chunk size and allocate buffers
	 */
	Outsamp = outrank[0][0];               /* output samples per frame */

	if (!Chunk)
	   Chunk = (DownSample*1100)/Outsamp;  /* frame chunks per buffer  */

	Bufsize = Chunk * Outsamp * Wave.BlockAlign;

	Buf1 = (BYTE *)malloc(Bufsize);
	Buf2 = (BYTE *)malloc(Bufsize);
	if (!Buf1 || !Buf2) {
		fprintf(stderr,
			"%s: couldn't allocate buffers of %d bytes\n",
			Prtname, Bufsize);
		return(0);
	}
	printf("%s: allocated %d byte buffer for %d frames per buffer\n",
		Prtname, Bufsize, Chunk);

	mp_init();

	/* Allocate a semaphore for data available */
	BufAvail = mp_allocsema();

	/* Read the first two buffers */
	WaveRead(1);
	WaveRead(2);

	mp_incsema(BufAvail);
	mp_incsema(BufAvail);

	Ibuf = 1; Ichunk = 0;

	if (Play) {
		 audio_init();
		 Audiofd = audio_out_open("*",
					  Wave.NumChannels,
					  Wave.SampleRate,
					  Wave.BitsPerSample);
		if (!Audiofd) {
			fprintf(stderr,
				"%s: ****ERROR**** couldn't open audio output\n",
				Prtname);
			return(0);
		}
		audio_verbose(0);
		event_sleep(500);
	   /*   audio_tone(Audiofd, 200, 500); */
		audio_out_callback(Audiofd, AudioSent);

		WavePlay(1);
		WavePlay(2);
	}

	if (View) WaveShow(Ibuf);

	Nin  = nin;
	Nout = nout;
	printf("%s: start in %d, out %d\n", Prtname, Nin, Nout);

	return (1);
}

WaveRead(int bufno) {
	int   n;
	BYTE *buf;
	int   filesize;

	buf = (bufno==1)?Buf1:Buf2;

	if (Eof) n = 0;
	else     n = wave_read(Wavefd, &Wave, buf, Bufsize);
	if (n < Bufsize) {
		while (n < Bufsize) buf[n++] = 0;
		if (!Loop) Eof = 1;
		else {
			wave_read_close(Wavefd, &Wave);

			Wavefd = wave_read_open(Filename, &Wave, &filesize);
		}
	}
	return(n);
}

ModuleStop(void) {
	int i;

	printf("%s: Stopped\n", Prtname);

	return (1);
}

ModuleAdvance(Stream *input[], Stream *output[]) {
	Stream   *stream;
	int       i, k, n;
	int       frame, nitem = 1;
	int       bytesper;
	char     *tname;
	BYTE     *bp;
	char      buf[100];
	int       hi, lo;
	short     samp1, samp2;
	float     SampToFloat = SPL/0x7fff;
	float     out1, out2;
	TMatrixN_CLASS *Out1, *Out2;
	TMatrixN_T *data1, *data2;

	if (Verbose) printf("%s.advance: frame %d\n", Prtname, Frame);

	/* Get pointer to output data buffer(s) */
	Out1  = output[0]->data.waveform;
	data1 = Out1->mData;

	if (Nout > 1) {
		Out2  = output[1]->data.waveform;
		data2 = Out1->mData;
	}

	/* Get pointer to current wave buffer */
	bp = (Ibuf==1)?Buf1:Buf2;

	/* Offset pointer to current chunk (Chunk blocks in each buffer) */
	bytesper = Wave.BlockAlign;        /* bytes per sample */
	k = Ichunk * Outsamp * bytesper;
	bp += k;

	/* Copy data from wave buffer to output buffers */
	for (i = 0; i < Outsamp; ++i) {
		lo = *bp++;
		hi = *bp++;
		samp1 = (hi<<8) | lo;
		out1  = SampToFloat * samp1;

		if (Wave.NumChannels == 2) {
			lo = *bp++;
			hi = *bp++;
			samp2 = (hi<<8) | lo;
			out2  = SampToFloat * samp2;
		}
		else out2 = 0.0;

		if (Nout == 1) switch (Mix) {
		  case 0:
		  case 1:
			*data1++ = out1; break;
		  case 2:
			*data1++ = out2; break;
		  case 3:
			*data1++ = out1 + out2; break;
		}
		else {
			*data1++ = out1;
			*data2++ = out2;
		}

	}

	/* See if we are done with this buffer */
	if (++Ichunk == Chunk) {
		if (Verbose)
		   printf("%s.advance: waiting for buffer\n");

		/* Wait for a buffer to become available */
		if (Play) mp_decsema(BufAvail);
		WaveRead(Ibuf);

		if (Verbose)
		   printf("%s.advance: playing/displaying new buffer\n", Prtname);

		/* Queue the buffer up for playing */
		if (Play) WavePlay(Ibuf);

		Ibuf = (Ibuf==1)?2:1;   /* Swap buffers */

		/* N.B. We are always displaying one buffer behind */
		if (View) WaveShow(Ibuf);

		Ichunk = 0;
	}

	return (Outsamp);
}

WavePlay(int bufno) {
	BYTE *buf;

	if (Verbose)
	   printf("%s.WavePlay: sending buffer %d\n", Prtname, bufno);

	buf = (bufno==1)?Buf1:Buf2;
	audio_out(Audiofd, buf, Bufsize, bufno);
}

AudioSent(int bufno) {
	mp_incsema(BufAvail);
}


INTERNAL void
WaveShow(int bufno) {
	int   nchan, n;
	BYTE *buf;
	static int init = 0;
	int   xs, ys;
	short lo, hi, samp;
	int   x, y;
	int   y0, mid, height;
	int   nleft;
	int   bytesper, skip;
	int   nsamp;
	float norm = 1.0/0x7fff;
	double rx, dx;
	BYTE  *bp;

	buf      = (bufno==1)?Buf1:Buf2;
	n        = Bufsize;
	nchan    = Wave.NumChannels;
	bytesper = Wave.BlockAlign;
	skip     = DownSample;

	nsamp    = Bufsize/(bytesper*DownSample);

	if (!init) {
		init = 1;
		if (XDisplay[0]) xw_server(XDisplay);
		xw_size(nsamp, 256*nchan);
		xw_init();
	}
	xw_cont();
	xw_qsize(&xs, &ys);
	xw_erase();

xw_rgbcol(255, 255, 255);
xw_move(0,    ys-1);
xw_draw(xs-1, ys-1);

	mid = ys/2;
	if (nchan == 1) {
		y0     = mid;
		height = mid-2;
	}
	else {
		height = mid/2 - 4;
		y0     = mid + mid/2;
	}

	dx = (double)(xs-1) / (double)(nsamp-1);

	/*
	 * Draw left or mono channel
	 */
	if (nchan == 1) xw_rgbcol(255, 255, 255);
	else            xw_rgbcol(0,   255, 0);
	xw_move(0, y0);

	norm *= height;

	bp       = buf;
	nleft    = n;

	rx = 0.5;
	for (x = 0; nleft > bytesper && x < xs; x = (int)rx) {
		lo = bp[0];
		hi = bp[1];
		samp = (hi<<8) | lo;

		y = y0 + norm * samp + 0.5;
		xw_draw(x, y);

		bp    += skip * bytesper;
		nleft -= skip * bytesper;

		rx += dx;
	}

	if (nchan == 1) {
		xw_flush();
		return;
	}

	/*
	 * Right channel
	 */
	xw_rgbcol(255, 255, 255);
	xw_move(0, mid); xw_draw(xs-1, mid);

	bp       = buf + 2;
	nleft    = n;
	y0       = mid/2;

	xw_rgbcol(255, 0, 0);
	xw_move(0, y0);

	rx = 0.5;
	for (x = 0; nleft > bytesper && x < xs; x = (int)rx) {
		lo = bp[0];
		hi = bp[1];
		samp = (hi<<8) | lo;

		y = y0 + norm * samp + 0.5;
		xw_draw(x, y);

		bp    += skip * bytesper;
		nleft -= skip * bytesper;

		rx += dx;
	}

	xw_term();
}

