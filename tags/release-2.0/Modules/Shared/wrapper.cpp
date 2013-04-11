/*
 * C Iterface/wrapper for CPP modules
 */

#if !defined(MODULE)
#error MODULE not defined
#else

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
#define ModuleSetName        CONCAT(MODULE,_SetName)

#define MODULE_ADVANCE CONCAT(MODULE,_Advance)

#include XSTRING(MODULE_H)

static MODULE MODULE;

typedef int ArgDims[EarlabMaxIOStreamCount][EarlabMaxIOStreamDimensions];

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "Earlab.h"
#include "EarlabException.h"
#include "MatrixN.h"

using namespace std;

extern "C" int
ModuleReadParameters(char *file, char *section) {
	int rc;

	cout << MODULENAME <<": parameter file" << file << ":" << section << endl;

	try {

		if (!section) rc = MODULE.ReadParameters(file);
		else          rc = MODULE.ReadParameters(file, section);
	}
	catch (EarlabException e) {
		cout  << "ReadParameters"<<MODULENAME <<": Error - " << e.GetExceptionString() << endl;
		return(0);
	}
	catch(const char *msg) {
		cout << "ReadParameters"<<MODULENAME <<": Error - " << msg << endl;
		return(0);
	}
	return(rc);
}

extern "C" int
ModuleSetName(char *instname) {
	try {
		MODULE.SetModuleName(instname);
	}
	catch (EarlabException e) {
		cout  << "SetModuleName"<<MODULENAME <<": Error - " << e.GetExceptionString() << endl;
		return(0);
	}
	catch(const char *msg) {
		cout << "SetModuleName"<<MODULENAME <<": Error - " << msg << endl;
		return(0);
	}
	return(1);
}

extern "C" int
ModuleStart(int nin,  EarlabDataStreamType intype[], ArgDims insize,
	    int nout, EarlabDataStreamType outtype[], ArgDims outsize,
	    unsigned long outelems[]
	    ) {
	int rc;

	cout << MODULENAME <<": start - ninput=" << nin << ", nouput=" << nout << endl;
	try {
		rc = MODULE.Start(nin, intype, insize, nout, outtype, outsize, outelems);
	}
	catch (EarlabException e) {
		cout  << "Start"<<MODULENAME <<": Error - " << e.GetExceptionString() << endl;
		return(0);
	}
	catch(const char *msg) {
		cout << "Start"<<MODULENAME <<": Error - " << msg << endl;
		return(0);
	}
	return(rc);
}

extern "C" int
ModuleAdvance(EarlabDataStream *in[], EarlabDataStream*out[]) {
	int rc;

	cout << MODULENAME <<": advance" << endl;
	try {
		rc = MODULE.Advance(in, out);
	}
	catch (EarlabException e) {
		cout  << "Advance"<<MODULENAME <<": Error - " << e.GetExceptionString() << endl;
		return(0);
	}
	catch(const char *msg) {
		cout << "Advance"<<MODULENAME <<": Error - " << msg << endl;
		return(0);
	}

	return(rc);
}

extern "C" int
ModuleStop(void) {
	int rc;

	try {
		rc = MODULE.Stop();
	}
	catch (EarlabException e) {
		cout  << "Stop"<<MODULENAME <<": Error - " << e.GetExceptionString() << endl;
		return(0);
	}
	catch(const char *msg) {
		cout << "Stop"<<MODULENAME <<": Error - " << msg << endl;
		return(0);
	}

	return(rc);
}

extern "C" int
ModuleUnload(void) {
	int rc;

	cout << MODULENAME <<": unload" << endl;
	try {
		rc = MODULE.Unload();
	}
	catch (EarlabException e) {
		cout  << "Unload"<<MODULENAME <<": Error - " << e.GetExceptionString() << endl;
		return(0);
	}
	catch(const char *msg) {
		cout << "Unload"<<MODULENAME <<": Error - " << msg << endl;
		return(0);
	}

	return(rc);
}

#endif
