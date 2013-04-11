/*
 * C Interface/wrapper for CPP modules
 */
#if !defined(MODULE)
#error MODULE not defined
#else

#include <stdio.h>
#include <unistd.h>
#include "earlabc.h"

#if !defined(MODULE_H)
#define H(x) x.h
#define MODULE_H H(MODULE)
#endif

#define STRING(x)  #x
#define XSTRING(x) STRING(x)

#define XCONCAT(a,b) a ## b
#define CONCAT(a,b) XCONCAT(a, b)

#define MODULENAME XSTRING(MODULE)

#define ModuleReadParameters CONCAT(MODULE,_ReadParameters)
#define ModuleAdvance        CONCAT(MODULE,_Advance)
#define ModuleStart          CONCAT(MODULE,_Start)
#define ModuleStop           CONCAT(MODULE,_Stop)
#define ModuleUnload         CONCAT(MODULE,_Unload)

typedef int ArgDesc[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions];

typedef struct {
	int    ndim;
	int    dimsize[10];
	float *data;
} MatrixN;

/*
 * Modules interface definitions
 */
extern int ModuleReadParameters(char *file, char *section);
extern int ModuleStart(int nin, ArgDesc insize, int nout, ArgDesc outsize);
extern int ModuleAdvance(void *in, void *out);
extern int ModuleStop(void);
extern int ModuleUnload(void);

main(int argc, char *argv[]) {
	char    *pgm, *inst, *pfile;
	ArgDesc ins, outs;
	int     nin, nout;
	MatrixN in, out;
	MatrixN *inv[10], *outv[10];
	int     i, nstep;

	pgm = argv[0];
	++argv; --argc;

	if (argc < 2) {
		printf("usage: %s instance parameter_file\n", pgm);
		exit(1);
	}
	inst = argv[0];
	++argv; --argc;

	pfile = argv[0];
	++argv; --argc;

	ModuleReadParameters(pfile, inst);

	nin = nout = 1;

	for (i = 0; i < 10; ++i) inv[i] = outv[i] = NULL;
	inv[0]  = &in;
	outv[0] = &out;


	ModuleStart(nin, ins, nout, outs);
	for (i = 0; i < nstep; ++i) {
		ModuleAdvance(inv, outv);
	}
	ModuleStop();
	ModuleUnload();
}
#endif
