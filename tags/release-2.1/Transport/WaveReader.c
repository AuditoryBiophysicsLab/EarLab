/*
 * Wave reader/player/viewer module
 */

#define NIN  0
#define NOUT 1

#define MODULE WaveReader

#include <stdio.h>
#include <strings.h>

#ifdef win32
# define WINDOWS
# include <windows.h>
# define WINGDI 1
# ifndef WIN32
#  define WIN32
# endif
# define WINAPP 1
#else
# define XWINDOWS
#endif

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
INTERNAL int       Show = 1;
INTERNAL int       Play = 1;
INTERNAL char      Display[200] = "";
INTERNAL int       Loop = 0;
INTERNAL int       Eof;
INTERNAL int       Mix  = 1;
INTERNAL int       DownSample = 4;
INTERNAL int       Nin, Nout;
INTERNAL float     SPL = 1.0;

INTERNAL int  AudioSent(int tag);
INTERNAL void WaveShow(int bufno);

WaveReader_SetName(char *instname) {
	if (instname) strcpy(Instname, instname);
	return(1);
}

WaveReader_ReadParameters(char *filename, char *section) {
	int   i, n;
	char *p, *q;

	if (Instname[0]) sprintf(Prtname, "%s.%s", Instname, MODULENAME);
	else {
		strcpy(Prtname,  MODULENAME);
		strcpy(Instname, MODULENAME);
	}

	printf("%s: processing parameter file %s:%s\n",
	       Prtname, filename, section);

	ReadParameterFile(filename);

	return (1);
}

WaveReader_Start(int nin,  int intype[],  RANK inrank,
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

	GetSectionParameterString(Instname, "MonoMix", str);
	if      (strimatch("Left", str))  Mix = 1;
	else if (strimatch("Right", str)) Mix = 2;
	else if (strimatch("Both", str))  Mix = 3;

	Play = Show = Loop = 0;

	GetSectionParameterBool(Instname, "Play", &Play);
	GetSectionParameterBool(Instname, "View", &Show);
	GetSectionParameterBool(Instname, "Loop", &Loop);
	GetSectionParameterInt(Instname,  "ShowDownsample", &DownSample);

	GetSectionParameterString(Instname, "Display", Display);

	GetSectionParameterFloat(Instname, "SoundPressureLevel", &SPL);

	printf("%s: play %s, view %s, loop %s, spl = %f\n",
		Prtname,
		Play?"on":"off",
		Show?"on":"off",
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
	Outsamp = outrank[0][0];            /* output samples per frame */

	Chunk = (DownSample*1100)/Outsamp;  /* frame chunks per buffer  */

	Bufsize = Chunk * Outsamp * Wave.NumChannels * Wave.BlockAlign;

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

	BufAvail = mp_allocsema();

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

		mp_incsema(BufAvail);
		WaveRead(1);
		WavePlay(1);

		mp_incsema(BufAvail);
		WaveRead(2);
		WavePlay(2);
	}
	Ibuf = 1; Ichunk = 0;
	if (Show) WaveShow(Ibuf);

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

	n = wave_read(Wavefd, &Wave, buf, Bufsize);
	if (n < Bufsize && n) {
		while (n < Bufsize) buf[n++] = 0;
		if (!Loop) Eof = 1;
		else {
			wave_read_close(Wavefd, &Wave);

			Wavefd = wave_read_open(Filename, &Wave, &filesize);
		}
	}
	return(n);
}

WaveReader_Stop(void) {
	int i;

	printf("%s: Stopped\n", Prtname);

	return (1);
}

WaveReader_Advance(Stream *input[], Stream *output[]) {
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

	printf("%s.advance: frame %d\n", Prtname, Frame);

	Out1  = output[0]->data.waveform;
	data1 = Out1->mData;

	if (Nout > 1) {
		Out2  = output[1]->data.waveform;
		data2 = Out1->mData;
	}

	/* Copy data from wave buffer to output arg */
	bp = (Ibuf==1)?Buf1:Buf2;

	bytesper = Wave.NumChannels * Wave.BlockAlign;  /* bytes per sample */
	k = Ichunk * Outsamp * bytesper;
	bp += k;

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
    printf("%s.advance: waiting for buffer\n");
		if (Play) mp_decsema(BufAvail);
		WaveRead(Ibuf);
    printf("%s.advance: playing/displaying new buffer\n", Prtname);
		if (Play) WavePlay(Ibuf);
		Ibuf = (Ibuf==1)?2:1;   /* Swap buffers */
		Ichunk = 0;
		if (Show) WaveShow(Ibuf);
	}

	return (Outsamp);
}

WavePlay(int bufno) {
	BYTE *buf;

    printf("%s.WavePlay: sending buffer %d\n", Prtname, bufno);
	buf = (bufno==1)?Buf1:Buf2;
	audio_out(Audiofd, buf, Bufsize, bufno);
}

AudioSent(int bufno) {
	mp_incsema(BufAvail);
}


#include <local/xw.h>

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
	float norm = 1.0/0x7fff;
	BYTE  *bp;

	buf      = (bufno==1)?Buf1:Buf2;
	n        = Bufsize;
	nchan    = Wave.NumChannels;
	bytesper = nchan * Wave.BlockAlign;
	skip     = DownSample;

	if (!init) {
		init = 1;
		if (Display[0]) xw_server(Display);
		xw_size(Bufsize/(bytesper*DownSample)+5, 256*nchan);
		xw_init();
	}
	xw_cont();
	xw_qsize(&xs, &ys);
	mid = ys/2;
	if (nchan == 1) {
		y0     = mid;
		height = mid-2;
	}
	else {
		height = mid/2 - 4;
		y0     = mid + mid/2;
	}
	xw_erase();

	/*
	 * Draw left or mono channel
	 */
	if (nchan == 1) xw_rgbcol(255, 255, 255);
	else            xw_rgbcol(0,   255, 0);
	xw_move(0, y0);

	norm *= height;

	bp       = buf;
	nleft    = n;

	for (x = 0; nleft > bytesper && x < xs; ++x) {
		lo = bp[0];
		hi = bp[1];
		samp = (hi<<8) | lo;

		y = y0 + norm * samp + 0.5;
		xw_draw(x, y);

		bp    += skip * bytesper;
		nleft -= skip * bytesper;
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

	for (x = 0; nleft > bytesper && x < xs; ++x) {
		lo = bp[0];
		hi = bp[1];
		samp = (hi<<8) | lo;

		y = y0 + norm * samp + 0.5;
		xw_draw(x, y);

		bp    += skip * bytesper;
		nleft -= skip * bytesper;
	}

	xw_term();
}

