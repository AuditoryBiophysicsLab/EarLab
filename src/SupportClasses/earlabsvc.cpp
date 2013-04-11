/*
 * C Interface/wrapper for Earlib C++ Service
 */
#include <stdio.h>

#include <iostream>
using namespace std;

#include "Earlab.h"
#include "EarlabEnvironment.h"
#include "ModuleServices.h"

#include "earlabsvc.h"

extern "C" int
earlab_environment(EarlabEnv *envdata) {
	EarlabEnvironment *envp;
	char *indir, *outdir;

	indir  = envdata->InDir;
	outdir = envdata->OutDir;

	envp = new EarlabEnvironment();
	envp->SetInputDirectoryPath(indir);
	envp->SetOutputDirectoryPath(outdir);

	SetEnvironmentPointer(envp);
}


