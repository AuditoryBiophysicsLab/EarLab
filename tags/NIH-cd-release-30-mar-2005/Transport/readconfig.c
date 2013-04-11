
#include <stdio.h>
#include <local/std.h>

#include <daffie/event.h>
#include "efievent.h"
#include "agentd.h"
#include "cl.h"

#include "config.h"

#ifdef MAIN
main(int argc, char *argv[]) {
	MODULE_CONFIG modules[MAX_MODULES];
	int i , rc;

	if (argc < 2) {
		printf("usage: %s filename\n", argv[0]);
		exit(4);
	}
	rc = read_config(argv[1], modules);
	if (rc < 0) {
		config_error(stdout, "*ERROR - read_config: ");
		exit(rc);
	}
	printf("%s: read %d modules\n", argv[0], rc);

	print_config(stdout, modules, rc);
}
#endif

INTERNAL char ParamFile[2000]  = "";
INTERNAL int  Steps = 0;
INTERNAL char Errline[2000];
INTERNAL char Errmsg[500];

/*
 * Print the module configuration
 */
void
print_config(FILE *f, MODULE_CONFIG modules[], int nmods) {
	int i, j;

	for (i = 0; i < nmods; ++i) {
		fprintf(f, "%s %s %s: %d inputs",
			modules[i].modname,
			modules[i].instname,
			modules[i].host,
			modules[i].ninput);
		for (j = 0; j < modules[i].ninput; ++j) {
			fprintf(f, "%s%s", j?" ":" (", modules[i].insrc[j]);
		}
		if (modules[i].ninput) fprintf(f, ")");

		fprintf(f, ", %d outputs", modules[i].noutput);
		for (j = 0; j < modules[i].noutput; ++j) {
			int k;

			fprintf(f, "%s", j?" ":" (");
			for (k = 0; k < modules[i].out_dim[j]; ++k) {
				fprintf(f, "%s%d", k?" ":"[", modules[i].out_rank[j][k]);
			}
		}
		if (modules[i].noutput) fprintf(f, "])");
		fprintf(f, " %s \n", modules[i].paramf);

	}
}

void
config_error(FILE *outf, char *prefix) {
	fprintf(outf, "%s%s",   prefix?prefix:"", Errline);
	fprintf(outf, "%s%s\n", prefix?prefix:"", Errmsg);
}

/*
 * Read experiment configuration file
 */
int
read_config(char *config, MODULE_CONFIG *mods) {
	FILE *f;
	char buf[2000], *bp, *s;
	char dfltparam[2000];
	int  n = 0;
	int  notdone = 1, gotbuf = 0;
	int  model = 0;
	int  i, j, k;
	int  lineno = 0;

	if (!(f = fopen(config, "r"))) return(-1);

	/* build a default parameter file name based on config name */
	strcpy(dfltparam, config);
	s = dfltparam+strlen(dfltparam);
	while (*s != '.' && s != dfltparam) --s;
	if (*s == '.') strcpy(s+1, "parameters");
	else           strcat(dfltparam, ".parameters");

	n = -1;
	while (notdone) {
		if (!gotbuf) {
			if (!fgets(buf, sizeof(buf), f)) break;
			++lineno;
			bp = buf;
		}

		while (iswhite(*bp)) ++bp;

		gotbuf = 0;

		/* Deal with model/run/global section */
		if (substr("[Run]", bp) || substr("[Model]", bp)) {
			model = 1;
			continue;
		}
		if (model && strimatch("*ParameterFile*", bp)) {
			while (*bp && *bp != '"') ++bp;
			if (!*bp) {
				strcpy(Errmsg, "missing ParameterFile value");
				sprintf(Errline, "line %d: %s", lineno, buf);
				return(-2);
			}
			++bp;
			s =  ParamFile;
			while (*bp && *bp != '"') *s++ = *bp++;
			*s = '\0';
		}

		if (*bp != '[') continue;
		model = 0;

		/* Zero out module info */
		++n;
		memclr(&mods[n], sizeof(MODULE_CONFIG));

		s = mods[n].instname;
		while (*++bp && *bp != ']') *s++ = *bp;
		*s = '\0';
		strcpy(mods[n].host, "*");
		mods[n].ninput  = 0;
		mods[n].noutput = 0;

		while (notdone = (fgets(buf, sizeof(buf), f) != NULL)) {
			bp = buf;
			++lineno;

			while (iswhite(*bp)) ++bp;

			/* Break out on blank line */
			if (!*bp || *bp == '\n') break;

			/* See if we read too much - whoops */
			if (*bp == '[') {
				gotbuf = 1;  /* ok, reuse this buffer */
				break;
			}

			/* Look for ModuleDLL keyword */
			if (strimatch("*Module{DLL,Executable}*", bp)) {
				while (*bp && *bp != '"') ++bp;
				if (!*bp) {
					strcpy(Errmsg, "missing ModuleExecutable value");
					sprintf(Errline, "line %d: %s", lineno, buf);
					return(-2);
				}
				++bp;
				s =  mods[n].modname;
				while (*bp && *bp != '"') {
					if (substr(".dll", bp)) break;
					*s++ = *bp++;
				}
				*s = '\0';

				continue;
			}

			/* Look for PreferredHost keyword */
			if (strimatch("*Host*", bp)) {
				while (*bp && *bp != '"') ++bp;
				if (!*bp) {
					strcpy(Errmsg, "missing Host value");
					sprintf(Errline, "line %d: %s", lineno, buf);
					return(-2);
				}
				++bp;
				s =  mods[n].host;
				while (*bp && *bp != '"') {
					*s++ = *bp++;
				}
				*s = '\0';

				continue;
			}

			/* Look for Input.*.DataType keyword */
			if (strimatch("*Input.*.DataType*", bp)) {
				int in, type;

				while (!substr("Input.", bp)) ++bp;
				bp += 6;

				/* Get the input number */
				in = 0;
				while (isdigit(*bp)) in = 10*in + (*bp++-'0');

				/* Skip over ".DataType" */
				if (!substr(".DataType", bp)) {
					strcpy(Errmsg, "missing DataType value");
					sprintf(Errline, "line %d: %s", lineno, buf);
					return(-2);
				}
				bp += 9;

				/* Look for the type name */
				while (*bp && *bp != '"') ++bp;
				if (!*bp) {
					strcpy(Errmsg, "missing input DataType terminater");
					sprintf(Errline, "line %d: %s", lineno, buf);
					return(-2);
				}

				++bp;

				/* Match against know types */
				if (strimatch("Wave*", bp))
				   type = EFI_STREAM_WAVEFORM;
				else if (strimatch("Spike*", bp))
				   type = EFI_STREAM_SPIKE;
				else {
					strcpy(Errmsg, "unknown input DataType");
					sprintf(Errline, "line %d: %s", lineno, buf);
					return(-2);
				}
				mods[n].in_type[in-1] = type;

				/* Skip to matching quote (or eol) */
				while (*bp && *bp != '"') ++bp;
				if (*bp) ++bp;

				continue;
			}

			/* Look for Input keyword */
			if (strimatch("*Input.*", bp)) {
				int in;

				while (!substr("Input.", bp)) ++bp;
				bp += 6;

				/* Get the input number */
				in = 0;
				while (isdigit(*bp)) in = 10*in + (*bp++-'0');


				/* Look for the source name */
				while (*bp && *bp != '"') ++bp;
				if (!*bp) {
					strcpy(Errmsg, "missing input source");
					sprintf(Errline, "line %d: %s", lineno, buf);
					return(-2);
				}

				++bp;
				s =  mods[n].insrc[in-1];
				while (*bp && *bp != '"') {
					*s++ = *bp++;
				}
				*s = '\0';

				if (in > mods[n].ninput) mods[n].ninput = in;

				continue;
			}

			/* Look for Output.*.DataType keyword */
			if (strimatch("*Output.*.DataType*", bp)) {
				int out, type;

				while (!substr("Output.", bp)) ++bp;
				bp += 7;

				/* Get the output number */
				out = 0;
				while (isdigit(*bp)) out = 10*out + (*bp++-'0');

				/* Skip over ".DataType" */
				if (!substr(".DataType", bp)) {
					strcpy(Errmsg, "missing output DataType");
					sprintf(Errline, "line %d: %s", lineno, buf);
					return(-2);
				}
				bp += 9;

				/* Look for the type name */
				while (*bp && *bp != '"') ++bp;
				if (!*bp) {
					strcpy(Errmsg, "missing output DataType");
					sprintf(Errline, "line %d: %s", lineno, buf);
					return(-2);
				}

				++bp;

				/* Match against know types */
				if (strimatch("Wave*", bp))
				   type = EFI_STREAM_WAVEFORM;
				else if (strimatch("Spike*", bp))
				   type = EFI_STREAM_SPIKE;
				else {
					strcpy(Errmsg, "unknown output DataType");
					sprintf(Errline, "line %d: %s", lineno, buf);
					return(-2);
				}

				mods[n].out_type[out-1] = type;

				/* Skip to matching quote (or eol) */
				while (*bp && *bp != '"') ++bp;
				if (*bp) ++bp;

				continue;
			}

			/* Look for Output keyword */
			if (strimatch("*Output.*", bp)) {
				int out, dim;

				while (!substr("Output.", bp)) ++bp;
				bp += 7;

				/* Get the output number */
				out = 0;
				while (isdigit(*bp)) out = 10*out + (*bp++-'0');

				/* Look for the output rank vector */
				while (*bp && *bp != '[') ++bp;
				if (!*bp) {
					strcpy(Errmsg, "missing output size vector");
					sprintf(Errline, "line %d: %s", lineno, buf);
					return(-2);
				}

				++bp;
				dim = 0;
				while (*bp && *bp != ']') {
					int x;

					while (iswhite(*bp)) ++bp;
					if (!isdigit(*bp))
					  if (*bp != ']') return(-2);
					  else continue;

					x = 0;
					while(isdigit(*bp)) x = 10*x + *bp++-'0';

					mods[n].out_rank[out-1][dim++] = x;
				}
				mods[n].out_dim[out-1] = dim;
				if (out > mods[n].noutput) mods[n].noutput = out;

				continue;
			}

			/* Look for ParameterFile keyword */
			if (strimatch("*ParameterFile*", bp)) {
				while (*bp && *bp != '"') ++bp;
				if (!*bp) {
					strcpy(Errmsg, "missing ParameterFile value");
					sprintf(Errline, "line %d: %s", lineno, buf);
					return(-2);
				}
				++bp;
				s =  mods[n].paramf;
				while (*bp && *bp != '"') {
					*s++ = *bp++;
				}
				*s = '\0';

				continue;
			}
		}
		/* If no paramater file was specified, use the default */
		if (!mods[n].paramf[0])
		  if (ParamFile[0]) strcpy(mods[n].paramf, ParamFile);
		  else              strcpy(mods[n].paramf, dfltparam);
	}
	++n;

	/* Fake up the output dimension descriptors */
	for (i = 0; i < n; ++i)
	    for (j = 0; j < mods[i].noutput; ++j)
		for (k = 0; k < mods[i].out_dim[j]; ++k)
		    mods[i].out_desc[j][k] = EFI_NONE;

	/* Wire the module */
	if (!wire_config(n, mods)) return(0);

	return(n);
}

/*
 * Lookup a module by (instance) name
 */
int
find_module_byname(char *name, int nmods, MODULE_CONFIG *mods) {
	int i;

	for (i = 0; i < nmods; ++i)
	    if (streq(name, mods[i].instname)) return(i);
	return(-1);
}

/*
 * Lookup a module by clientid
 */
int
find_module_byclientid(int id, int nmods, MODULE_CONFIG *mods) {
	int i;

	for (i = 0; i < nmods; ++i)
	    if (mods[i].clientid == id) return(i);
	return(-1);
}

/*
 * Construct module wiring
 */
int
wire_config(int nmods, MODULE_CONFIG *mods) {
	MODULE_CONFIG *mod;
	int  i, j, k, indx;
	char name[CONFIG_NAMELEN], *s, *t;
	int  pos;
	int  tag = 1;

	for (i = 0; i < nmods; ++i) {
		mod = &mods[i];
	     /***
		printf("wire_config: module %s %d inputs\n", mod->instname, mod->ninput);
	     ***/

		for (j = 0; j < mod->ninput; ++j) {

			/* Split the source string into name and position */
			s = name;
			t = mod->insrc[j];
			while (*t && *t != '.') *s++ = *t++;
			if (*t != '.') {
				fprintf(stderr,
					"wire_config: bad configuration for source %s for module %s, input %d\n",
					mod->insrc[j], mod->instname, j+1);
				return(0);
			}
			*s = '\0';
			pos = atoi(++t)-1;

			/* Find the index of the source module */
			k = find_module_byname(name, nmods, mods);
			if (k < 0) {
				fprintf(stderr,
					"wire_config: could not locate source %s for module %s, input %d\n",
					name, mod->instname, j+1);
				return(0);
			}
			mods[i].in_src[j] = k;
			mods[i].in_pos[j] = pos;
			mods[i].in_tag[j] = tag;

			mods[k].out_dst[pos] = i;
			mods[k].out_pos[pos] = j+1;
			mods[k].out_tag[pos] = tag;

			printf("wire_config: %s[%d] -> %s[%d] (%s stream %d)\n",
				mods[k].instname, pos+1,
				mods[i].instname, j+1,
				StreamTypeName(mods[k].out_type[pos]),
				tag);

			++tag;

			/* For now simply copy the output descriptors to inputs */
			memcpy(mod->in_desc[j], mods[k].out_desc[pos],
			       mods[k].out_dim[pos]*sizeof(mods[k].out_desc[pos][0]));

			/*
			 * Set input argument properties from correspsonding
			 * output source
			 */

			/* Set number dimensions of input array */
			mod->in_dim[j] = mods[k].out_dim[pos];

			/* Set input arg type */
			mod->in_type[j] = mods[k].out_type[pos];

			/* Set input arg rank */
			memcpy(mod->in_rank[j], mods[k].out_rank[pos],
			       mods[k].out_dim[pos]*sizeof(int));


	printf("wire_config: setting %s %s input %d dim=%d: %d",
		mod->instname, StreamTypeName(mod->in_type[j]), j,
		mod->in_dim[j], mod->in_rank[j][0]);
	if (mod->in_dim[j] > 1) printf("x%d", mod->in_rank[j][1]);
	if (mod->in_dim[j] > 2) printf("x%d", mod->in_rank[j][2]);
	if (mod->in_dim[j] > 3) printf("x%d", mod->in_rank[j][3]);
	if (mod->in_dim[j] > 3) printf("... HUH?");
	printf("\n");


		}
	}
}


/* ========================================================================= */

/****************************************************************************
 * Functions Exported to the MODEL layer                                    *
 ****************************************************************************/

#define CONTROL_LAYER

INTERNAL MODULE_CONFIG ModuleConf[MAX_MODULES];
INTERNAL int           Nmodule;

EXPORT int
define_module(char *name, char *module,
	      int ninput,  int indim[],  int *intype,
	      int noutput, int outdim[], int *outtype) {

	int i, j;

	i = Nmodule++;

	strcpy(ModuleConf[i].instname,   name);
	strcpy(ModuleConf[i].modname, module);
	ModuleConf[i].ninput  = ninput;

	for (j = 0; j < ninput; ++j)
	    ModuleConf[i].in_dim[j] = indim[j];

	for (j = 0; j < ninput; ++j) {
		int k;

		for (k = 0; k < indim[j]; ++k)
		    ModuleConf[i].in_desc[j][k] = *intype++;
	}

	ModuleConf[i].noutput = noutput;

	for (j = 0; j < noutput; ++j)
	    ModuleConf[i].out_dim[j] = outdim[j];

	for (j = 0; j < noutput; ++j) {
		int k;

		for (k = 0; k < outdim[j]; ++k)
		    ModuleConf[i].out_desc[j][k] = *outtype++;
	}

  printf("Defined module %s (%d): %d inputs, %d outputs\n",
	  ModuleConf[i].instname, i, ModuleConf[i].ninput, ModuleConf[i].noutput);

	return(i);
}

/*
 * Specify module i/o connections
 */
EXPORT void
wire_module(int source, int output, int sink, int input) {
	--output; --input;
  printf("Wiring %s[%d] ->  %s[%d]\n",
	 ModuleConf[source].instname, output,
	 ModuleConf[sink].instname, input);

	ModuleConf[sink].in_src[input]  = source;
	ModuleConf[sink].in_pos[input]     = output;

	ModuleConf[source].out_dst[output] = sink;
	ModuleConf[source].out_pos[output] = input;
}

/*
 * Lookup a module's id
 */
EXPORT int
lookup_module(char *name) {
	int i;

	i = find_module_byname(name, Nmodule, ModuleConf);
	if (i < 0) {
		fprintf(stderr, "lookup_module: couldn't find %s\n", name);
		exit(1);
		return(-1);   /* keep compiler happy */
	}
	return(i);
}

/*
 * Lookup a module's id given the DAFFIE client id
 */
EXPORT int
lookup_module_client(int clientid) {
	int i;

	i = find_module_byclientid(clientid, Nmodule, ModuleConf);
	if (i < 0) {
		fprintf(stderr, "lookup_module_clientid: couldn't find id %d\n", clientid);
		return(-1);
	}
	return(i);

}

/*
 * Runtime configure a module input arg
 */
EXPORT int
config_module_input(int id, int arg, int type, int ndim, int rank[]) {
	int i;

	if (id < 0 || id >= Nmodule) {
		fprintf(stderr, "config_module_input: bad module id %d\n", id);
		return(0);
	}
	if (arg < 1 || arg > ModuleConf[id].ninput) {
		fprintf(stderr, "config_module_input: module %s, bad arg index %d (%d)\n",
			ModuleConf[id].instname, arg, ModuleConf[id].ninput);
		return(0);
	}
	--arg;

	ModuleConf[id].in_type[arg] = type;

	for (i = 0; i < ndim; ++i) {
		ModuleConf[id].in_rank[arg][i]  = rank[i];
	}


	return(1);
}

/*
 * Runtime configure a module output
 */
EXPORT int
config_module_output(int id, int arg, int type, int ndim, int rank[]) {
	int i;

	if (id < 0 || id >= Nmodule) {
		fprintf(stderr, "config_module_output: bad module id %d\n", id);
		return(0);
	}
	if (arg < 1 || arg > ModuleConf[id].noutput) {
		fprintf(stderr, "config_module_output: module %s, bad arg index %d (%d)\n",
			ModuleConf[id].instname, arg, ModuleConf[id].ninput);
		return(0);
	}
	--arg;

	ModuleConf[id].out_type[arg] = type;

	for (i = 0; i < ndim; ++i) {
		ModuleConf[id].out_rank[arg][i]  = rank[i];
	}

	return(1);
}

