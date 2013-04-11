/*
 * Fake module
 */

#ifndef NIN
# define NIN  1
#endif
#ifndef NOUT
# define NOUT 1
#endif

#include "module.c"

INTERNAL int Nin, Nout;
INTERNAL int Rsize;
INTERNAL int Step = 0;

ModuleStart(int nin, int insize[], int nout, int outsize[]) {
	if (nout) Rsize = outsize[0];
	else      Rsize = insize[0];

	printf("%s: %d inputs, %d in elems, %d outputs, %d out elems\n",
		MODULENAME, nin, insize[0], nout, outsize[0]);

	if (nin != NIN || nout != NOUT) {
		fprintf(stderr, "%s: input (%d vs %d) or output (%d vs %d) mismatch\n",
			MODULENAME, nin, NIN, nout, NOUT);
		exit(4);
	}

	Step = 0;

	if (nin)  Nin = insize[0];
	else      Nin = 0;

	if (nout) Nout = outsize[0];
	else      Nout = 0;


	fflush(stderr); fflush(stdout);
	return (1);
}

ModuleStop(void) {
	printf("%s: stopping\n", MODULENAME);
	fflush(stderr); fflush(stdout);
	return (1);
}

ModuleReadParameters(char *filename, char *section) {
	printf("%s: processing parameter file %s:%s\n",
		MODULENAME, filename, section);
	fflush(stderr); fflush(stdout);
	return (1);
}

ModuleAdvance(TMatrixN_CLASS *in[], TMatrixN_CLASS *out[]) {
	TMatrixN_T *in0, *out0;
	static int irand(int a, int b);
	int   i;
	float v;

	++Step;

#if (NIN > 0)
	in0 = in[0]->mData;
	if (Step > 1) {
		v = in0[0];

		for (i = 1; i < Nin; ++i)
		  if (in0[i] != v) {
			printf("%s: BAD DATA input elem %d [%5.3f != %5.3f] at frame %d\n",
				MODULENAME, i, in0[i], v, Step);
			return(0);
		  }
	}
	else v = 0.0;
#else
	v = (float) Step;
	printf("%s: SOURCING = %5.3f at frame %d\n", MODULENAME, v, Step);
#endif

#if (NIN > 0 && NOUT > 0)
	printf("%s: PASSING = %5.3f at frame %d\n", MODULENAME, v, Step);
#endif

#if (NOUT > 0)
	out0 = out[0]->mData;
	for (i = 0; i < Nout; ++i) out0[i] = v;
#else
	printf("%s: SINKING = %5.3f at frame %d\n", MODULENAME, v, Step);
#endif

	event_sleep(irand(100, 2000));

	printf("%s: advance completed (%d elems)\n", MODULENAME, Rsize);
	fflush(stderr); fflush(stdout);
	return(Rsize);
}


#define CONS 0xffffff
INTERNAL int
irand(int a, int b) {
	int   val;
	float alpha;

	alpha = (random() & CONS) / (float)(CONS+1);
	val = (b - a + 1) * alpha + a;
	return(val);
}

