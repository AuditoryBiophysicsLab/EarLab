/*
 * C Interface/wrapper for CPP modules
 */
#if !defined(MODULE)
#error MODULE not defined
#else

#include <stdio.h>
#include <unistd.h>
#include "earlabc.h"
#include "earlibc.h"
#include "CMatrixN.h"
#include "CSpike.h"
#include "CEarlabDataStream.h"

#include "earlabsvc.h"

#if !defined(MODULE_H)
#define H(x) x.h
#define MODULE_H H(MODULE)
#endif

#ifdef CONCAT
#undef CONCAT
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
extern int ModuleStart(int nin,  int intype[],  int inrank[],
		       int nout, int outtype[], int outrank[], int outsize[]);
extern int ModuleAdvance(void *in, void *out);
extern int ModuleStop(void);
extern int ModuleUnload(void);

main(int argc, char *argv[]) {
	char    *pgm, *inst, *pfile;
	EarlabEnv env;
	ArgDesc ins, outs;
	int     nin, nout;
	MatrixN in, out;
	MatrixN *inv[10], *outv[10];
	int     intype[10], outtype[10];
	int     inrank[10], outrank[10];
	int     outsize[10];
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

	env.InDir    = ".";
	env.OutDir   = ".";
	env.ExpName  = "ModTest";
	env.RunName  = "ModTest";
	env.UserName = "";
	earlab_environment(&env);

printf("Reading parameter file %s\n", pfile);
	ReadParameterFile(pfile);
	PrintParameterFile();
printf("Done reading parameter file %s\n", pfile);

	ModuleReadParameters(NULL, inst);

	for (i = 0; i < 10; ++i) intype[i] = outtype[i] = EFI_STREAM_WAVEFORM;
	inrank[0] = outrank[0] = 100;
	for (i = 1; i < 10; ++i) inrank[i] = outrank[i] = 0;

	nin  = 0;
	nout = 1;

	ModuleStart(nin, intype, inrank, nout, outtype, outrank, outsize);

	for (i = 0; i < 10; ++i) inv[i] = outv[i] = NULL;
	inv[0]  = &in;
	outv[0] = &out;

	return;

	for (i = 0; i < nstep; ++i) {
		ModuleAdvance(inv, outv);
	}
	ModuleStop();
	ModuleUnload();
}
#endif
