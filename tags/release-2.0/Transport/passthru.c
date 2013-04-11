/*
 * Pass through  module
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

ModuleStart(int ninargs, int insize[], int noutargs, int outsize[]) {
	if (noutargs) Rsize = outsize[0];
	else          Rsize = 1;

	printf("%s: %d inputs, %d in elems, %d outputs, %d out elems\n",
		MODULENAME, ninargs, insize[0], noutargs, outsize[0]);

	if (ninargs != NIN || noutargs != NOUT) {
		fprintf(stderr, "%s: input (%d vs %d) or output (%d vs %d) mismatch\n",
			MODULENAME, ninargs, NIN, noutargs, NOUT);
		exit(4);
	}

	Step = 0;

	if (ninargs)  Nin = insize[0];
	else          Nin = 0;

	if (noutargs) Nout = outsize[0];
	else          Nout = 0;


	fflush(stderr); fflush(stdout);
	return (1);
}

ModuleStop() {
	printf("%s: stopping\n", MODULENAME);
	fflush(stderr); fflush(stdout);
	return (1);
}

ModuleGetParameters(char *filename) {
	printf("%s: processing parameter file %s\n", MODULENAME, filename);
	fflush(stderr); fflush(stdout);
	return (1);
}

PROTOTYPE(ModuleAdvance) {
	int   i, j;
	float *in, *out;

	++Step;


	for (i = 0, out = out0; i < Nout; )
	  for (j = 0, in = in0; i < Nout && j < Nin; )
	    out[i++] = in[j++];

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

