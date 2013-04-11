/*
 * Control Program - run a model after modules have been loaded
 */
#include <stdio.h>

#include <local/std.h>
#include <daffie/event.h>
#include <daffie/eclient.h>

#define CONTROL_LAYER 1

#include "efievent.h"
#include "config.h"

#include "tl.h"
#include "cl.h"

/***********
#include "model.h"
***********/

INTERNAL void efi_init(void);
INTERNAL void usage(char *pgm, int verbose);

EXPORT   int  TransportInit(char *server, char *expname);
EXPORT   int  TransportFini(void);
EXPORT   int  DefineModel(char *configfile);
EXPORT   int  LoadAllModules(void);
EXPORT   int  StartAllModules(void);
EXPORT   int  SetAllParameters(void);
EXPORT   int  StopAllModules(int exitcode);
EXPORT   int  UnloadAllModules(int exitcode);
EXPORT   int  RunModel(int nstep);
EXPORT   int  AdvanceAllModules(int frame);
EXPORT   int  WaitAllModules(int frame);
EXPORT   int  CheckStatus(void);

/* Internal variables */
#define PATHLEN   2500
#define SYS_OK    0

INTERNAL char         *Pgm = "";
INTERNAL char          Prefix[100] = {'\0'};
INTERNAL int           Debug = 0;

#ifdef RUNMODEL
main(int argc, char *argv[]) {
	int   ok;
	char *server;
	char *config;
	char *name;
	int   help;
	int   done;
	int   nstep = 1;
	int   unload;

	if (Pgm = strrchr(argv[0], '/')) ++Pgm;
	else Pgm = argv[0];
	++argv; --argc;
	sprintf(Prefix, "%s: ", Pgm);

	ok    = 1;
	help  = 0;
	Debug = 0;
	name  = NULL;
	unload= 1;
	while (argc && *argv[0] == '-') {
		char *opt;

		opt = argv[0] + 1;
		++argv; --argc;

		if      (streq(opt, "help"))   ++help;
		else if (substr(opt, "debug")) ++Debug;
		else if (substr(opt, "unload"))   unload = 1;
		else if (substr(opt, "nounload")) unload = 0;
		else if (substr(opt, "keep"))     unload = 0;
		else if (substr(opt, "steps")) {
			if (argc < 1) {
				fprintf(stderr, "%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				break;
			}
			nstep = atoi(argv[0]);
			++argv; --argc;
		}
		else if (substr(opt, "name") || substr(opt, "experiment")) {
			if (argc < 1) {
				fprintf(stderr, "%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				break;
			}
			name = argv[0];
			++argv; --argc;
		}
		else {
			fprintf(stderr, "%s: ignoring argument -%s\n", Pgm, opt);
			help = 1;
			ok   = 0;
		}
	}
	if (!ok || argc < 2 || help) {
		usage(Pgm, help);
		exit(!help);
	}

	server = argv[0];
	++argv; --argc;

	config = argv[0];
	++argv; --argc;

	if (!DefineModel(config)) {
		fprintf(stderr,
			"%s: couldn't load model file \"%s\"\n",
			Pgm, config);
		exit(4);
	}

	TransportInit(server, name?name:config);

	/* Locate and configure all modules */
	LoadAllModules();

	SetAllParameters();
	WaitAllModules(0);
	CheckStatus();

	StartAllModules();
	WaitAllModules(0);
	CheckStatus();

	ConfigureAllModules();
	WaitAllModules(0);
	CheckStatus();

	ok = RunModel(nstep);
	if (!ok) {
		TransportFini();
		printf("%s: Exiting on run failure\n", Pgm);
		exit(1);
	}

	StopAllModules(EFI_STATUS_OK);
	WaitAllModules(nstep+1);
	CheckStatus();

	if (unload) {
		UnloadAllModules(EFI_STATUS_OK);
		WaitAllModules(nstep+1);
		CheckStatus();
	}

	TransportFini();

	printf("%s: Done and exiting\n", Pgm);

	exit(0);
}

/*
 * Help message
 */
#define PRT(opt,str) \
	printf(msg?"\t%s%s%s\n":"%s ", opt, (strlen(opt)<8)?"\t\t":"\t", str)

INTERNAL void
usage(char *pgm, int msg) {
	if (msg) printf("usage: %s [options] server\n", pgm);
	else     printf("usage: %s ", pgm);
	PRT("[-help]",          "display this message");
	PRT("[-debug]",         "turning on debugging mode");
	PRT("[-steps n]",       "specify number of steps/frames");
	PRT("[-unload|-nounload","(don't) unload modules at end of run");
	PRT("server",           "event server name");
	PRT("config",           "model configuration file");
	if (!msg) printf("\n");
}
#endif


/****************************************************************************/

INTERNAL MODULE_CONFIG ModuleConf[MAX_MODULES];
INTERNAL int           Nmodule = 0;
INTERNAL int           ModuleWait;
INTERNAL int           Error = 0, ErrorStatus, ErrorDetail, ErrorClient;
INTERNAL int           Frame = 0;
INTERNAL int           Stopped = 0;
INTERNAL int           Started = 0;

INTERNAL int  start_module(MODULE_CONFIG *conf);
INTERNAL int  started(EVENT_EFI_START *estart);
INTERNAL void configure_modules(void);
INTERNAL int  recv_status(EVENT_EFI_STATUS *estatus);
INTERNAL int  client_exit(EVENT_LEAVE *eleave);

/*
 * Initialize the Transport layer
 */
EXPORT int
TransportInit(char *server, char *name) {
	int id, nc;

	/* Initialize DAFFIE EFI */
	event_verbose(0);
	event_tunnel_enable(0);

	efi_init();

	id = event_join(server, &nc);
	if (!id) {
		fprintf(stderr, "%sjoin failed for server %s\n", Prefix, server);
		exit(4);
	}
	printf("%sjoined %s as client id %d (%d clients)\n",
		Prefix, server, id, nc);

	event_register("control", "runmodel", name);

	ModuleWait = mp_allocsema();

	event_select_type(0, 0, ET_MAX);

	event_callback(ET_EFI_STATUS, recv_status);
	event_callback(ET_LEAVE, client_exit);
	event_callback(ET_EFI_START, started);

	event_select_type(1, ET_EFI_STATUS, ET_EFI_STATUS);
	event_select_type(1, ET_EFI_START,  ET_EFI_START);
	event_select_type(1, ET_LEAVE, ET_LEAVE);

	event_receive_enable(0);
	event_flush(1);

	return(1);
}

/*
 * Finalize Transport layer
 */
EXPORT int
TransportFini(void) {
	event_leave();
	return(1);
}

/*
 * Define Model
 */
DefineModel(char *config) {
	Nmodule = read_config(config, ModuleConf);
	if (Nmodule < 0) config_error(stderr, "DefineModel: ");

	return(Nmodule > 0);
}

/*
 * Load/find modules
 */
EXPORT int
LoadAllModules(void) {
	ECLIENT_LIST clients[MAX_CLIENT];
	int i, k = 0, n;

	/* Use Daffie services to find modules */
	eclient_monitor();

	for (i = 0; i < Nmodule; ++i) {
		printf("%sSearching for %s.%s\n", Prefix, ModuleConf[i].modname, ModuleConf[i].instname);
		while (!eclient_match(clients, "*", "earlab", ModuleConf[i].modname, ModuleConf[i].instname, "*")) {
			if (++k == 50) {
				printf("."); fflush(stdout);
				k = 0;
			}
			event_sleep(250);
		}
		printf("%sfound %s: %s %d (%s %s)\n", Prefix,
			ModuleConf[i].instname,
			clients[0].host,
			clients[0].id,
			clients[0].species,
			clients[0].instance);

		ModuleConf[i].clientid = clients[0].id;
		strcpy(ModuleConf[i].host, clients[0].host);
	}


	return(1);
}

EXPORT int
ConfigureAllModules(void) {
	/* Send configuration to all modules */
	Error = 0;

	configure_modules();
}

EXPORT int
StartAllModules(void) {
	int i;

	printf("%sStarting all modules\n", Prefix);

	Error = Stopped = Started = 0;

	for (i = 0; i < Nmodule; ++i) {
		printf("%sStarting %s as %s\n", Prefix,
			ModuleConf[i].modname, ModuleConf[i].instname);
		start_module(&ModuleConf[i]);
	}

	return(TL_OK);
}


EXPORT int
SetAllParameters(void) {
	int i;

	Error = 0;

	for (i = 0; i < Nmodule; ++i) {
		printf("%sSending parameter file %s to %s\n", Prefix,
			ModuleConf[i].paramf, ModuleConf[i].instname);
		set_parameters_module(i, ModuleConf[i].paramf, NULL);
	}
	return(TL_OK);
}

EXPORT int
StopAllModules(int exitcode) {
	EVENT_EFI_STOP estop;

	Error = 0;

	printf("%sStopping all modules - simulation %s after %d frames\n", Prefix,
	       (exitcode==EFI_STATUS_OK)?"completed normally":"abort", Frame-1);

	estop.rc = exitcode;

	event_send(EVENT_BCAST_NOTME, ET_EFI_STOP,
		   (EVENT *)&estop, sizeof(estop));

	Stopped  = 1;

	return(TL_OK);
}

EXPORT int
UnloadAllModules(int exitcode) {
	EVENT_EFI_UNLOAD eunload;

	Error = 0;

	printf("%sUnloading all modules - simulation %s after %d frames\n", Prefix,
	       (exitcode==EFI_STATUS_OK)?"completed normally":"abort", Frame-1);

	eunload.exitcode = exitcode;

	event_callback(ET_LEAVE, NULL);

	event_send(EVENT_BCAST_NOTME, ET_EFI_UNLOAD,
		   (EVENT *)&eunload, sizeof(eunload));

	return(TL_OK);
}

EXPORT int
RunModel(int nstep) {
	EVENT_EFI_STATUS estat;

	for (Frame = 1; Frame <= nstep; ++Frame) {
		Error = 0;

		AdvanceAllModules(Frame);
		WaitAllModules(Frame);
		CheckStatus();

		if (Error) break;

		/* Send a "frame complete" status message */
		estat.type   = EFI_STATUS_FRAME;
		estat.status = Frame;
		estat.detail = nstep;

		event_send(EVENT_BCAST_NOTME, ET_EFI_STATUS,
			   (EVENT *)&estat, sizeof(estat));
	}
	if (!Error) return(TL_OK);

	/* On Error stop all modules */
	StopAllModules(EFI_STATUS_ERROR);

	return(TL_FAIL);
}


EXPORT int
AdvanceAllModules(int frame) {
	EVENT_EFI_ADVANCE eadv;

	printf("%sAdvancing all modules - frame %d\n", Prefix, frame);

	Error = 0;

	eadv.frame = frame;
	event_send(EVENT_BCAST_NOTME, ET_EFI_ADVANCE,
		   (EVENT *)&eadv, sizeof(eadv));

	return(TL_OK);
}


EXPORT int
WaitAllModules(int frame) {
	int i;

	printf("%sWaiting all modules - frame %d\n", Prefix, frame);

	for (i = 0; i < Nmodule; ++i) {
		if (Error) break;
		mp_decsema(ModuleWait);
	}

	return(Error?TL_FAIL:TL_OK);
}

EXPORT int
WaitModuleReady(void) {

	printf("%sWaiting module ready\n", Prefix);
	mp_decsema(ModuleWait);

	return(TL_OK);
}

EXPORT int
CheckStatus(void) {
	int   i;
	char *errmsg, errbuf[100];
	char modname[200];

	if (!Error) return(TL_OK);

	if (ErrorStatus > 0 && ErrorStatus <= EFI_STATUS_MAXSTATUS) {
		errmsg = StatusStatusMsg[ErrorStatus];
	}
	else sprintf(errmsg = errbuf, "unknown error %d", ErrorStatus);

	for (i = 0; i < Nmodule; ++i)
	  if (ModuleConf[i].clientid == ErrorClient) {
		sprintf(modname, "%s@%s (%d)",
			ModuleConf[i].instname,
			ModuleConf[i].host,
			ErrorClient);
		break;
	}

	if (i == Nmodule)
	  sprintf(modname, "Unknown client id %d", ErrorClient);

	fprintf(stderr,
		"CheckStatus: Error \"%s\" occurred in module %s\n",
		errmsg, modname);

	if (!Stopped) StopAllModules(EFI_STATUS_ERROR);
	UnloadAllModules(EFI_STATUS_ERROR);

	TransportFini();

	exit(1);
}

/****************************************************************************/
/****************************************************************************/

/*************/
/* INTERNALS */
/*************/

/*
 * DAFFIE definitions for EFI events
 */
#include "eventdef.h"
#include "agentd.h"
INTERNAL void
efi_init(void) {
	event_init();
#include "efieventdef.h"
#include "agentddef.h"
}

INTERNAL int
recv_status(EVENT_EFI_STATUS *estatus) {
	char *tmsg, tbuf[30];
	char *smsg, sbuf[30];

	if (0 < estatus->type && estatus->type <= EFI_STATUS_MAXTYPE)
	   tmsg = StatusTypeMsg[estatus->type];
	else
	   sprintf(tmsg=tbuf,"BAD (type=%d)", estatus->type);

	/* Check specifically for FRAME messages 'cause status is invalid */
	if (estatus->type == EFI_STATUS_FRAME)
	   sprintf(smsg=sbuf,"Frame %d =%d", estatus->status);
	else if (0 < estatus->status && estatus->status <= EFI_STATUS_MAXSTATUS)
	   smsg = StatusStatusMsg[estatus->status];
	else
	   sprintf(smsg=sbuf,"BAD (status=%d)", estatus->status);

	if (Debug)
	   printf("%sClient %d responded with %s %s (%d) at frame %d", Prefix,
		  estatus->head.from, tmsg, smsg, estatus->status, estatus->frame);

	if (estatus->status == EFI_STATUS_READY) {
		mp_incsema(ModuleWait);
		if (Debug) printf(" - joined barrier\n");
	}
	else if (estatus->status == EFI_STATUS_ERROR) {
		if (Debug) printf(" -- ERROR\n");
		printf("recv_status: Client %d generated a fatal error (%d) of type \"%s\" on frame %d\n",
			estatus->head.from, estatus->detail,
			smsg, estatus->frame);
		ErrorStatus = estatus->status;
		ErrorDetail = estatus->detail;
		ErrorClient = estatus->head.from;
		Error = 1;

		mp_incsema(ModuleWait);
	}
	else {
		if (Debug) printf(" - ignored\n");
	}

	return(1);
}

INTERNAL int
client_exit(EVENT_LEAVE *eleave) {
	int i;

	for (i = 0; i < Nmodule; ++i) {
		if (eleave->id == ModuleConf[i].clientid) {
			printf("%sclient %s@%s (%d) exitted!!\n",
				Prefix[0]?Prefix:"Control - ",
				ModuleConf[i].host,
				ModuleConf[i].instname,
				ModuleConf[i].clientid);
			Error = 1;
			ErrorClient = ModuleConf[i].clientid;
			ErrorStatus = EFI_STATUS_DIED;
			ErrorDetail = eleave->detail;
			mp_incsema(ModuleWait);
			return(1);
		}
	}
	return(1);
}

/****************************************************************************/
/****************************************************************************/

INTERNAL char *
typename(int type) {
	switch(type) {
	  case EFI_STREAM_WAVEFORM:     return("waveform");
	  case EFI_STREAM_SPIKE:        return("spike");
	  case EFI_STREAM_RESERVED:     return("reserved");
	  case EFI_STREAM_NONE:         return("none");
	  default:                      return("*INVALID*");
	}
}

INTERNAL int
start_module(MODULE_CONFIG *conf) {
	int             i, j, ninput, noutput;
	EVENT_EFI_START estart;

	estart.ninput = ninput = conf->ninput;
	for (i = 0; i < ninput; ++i)   estart.input_type[i] = conf->in_type[i];
	for (     ; i < MAX_ARGS; ++i) estart.input_type[i] = EFI_STREAM_NONE;

	for (i = 0; i < ninput; ++i)
	  for (j = 0; j < MAX_DIM; ++j)
	    estart.input_rank[i][j] = conf->in_rank[i][j];

	estart.noutput = noutput = conf->noutput;
	for (i = 0; i < noutput; ++i)  estart.output_type[i] = conf->out_type[i];
	for (     ; i < MAX_ARGS; ++i) estart.output_type[i] = EFI_STREAM_NONE;

	for (i = 0; i < noutput; ++i)
	  for (j = 0; j < MAX_DIM; ++j)
	    estart.output_rank[i][j] = conf->out_rank[i][j];

for (i = 0; i < ninput; ++i) {
	printf("%sstarting %s.input.%d %s %d [ ",
		Prefix,
		conf->instname, i+1,
		StreamTypeName(conf->in_type[i]),
		conf->in_size[i]);

	for (j = 0; j < MAX_DIM; ++j)
	    if (conf->in_rank[i][j]) printf("%d ", conf->in_rank[i][j]);

	printf(" ]\n");
}
for (i = 0; i < noutput; ++i) {
	printf("%sstarting %s.output.%d %s %d [ ",
		Prefix,
		conf->instname, i+1,
		StreamTypeName(conf->out_type[i]),
		conf->out_size[i]);

	for (j = 0; j < MAX_DIM; ++j)
	    if (conf->out_rank[i][j]) printf("%d ", conf->out_rank[i][j]);

	printf(" ]\n");
}

	event_send(conf->clientid, ET_EFI_START, (EVENT *)&estart, sizeof(estart));

	return(TL_OK);
}

/*
 * Update configuration based on returned values from START
 */
INTERNAL int
started(EVENT_EFI_START *estart) {
	MODULE_CONFIG *conf;
	int            ninput, noutput;
	int            fromid;
	int            i, j, dest, k;

	++Started;

	fromid = EventFrom(*estart);
	i = find_module_byclientid(fromid, Nmodule, ModuleConf);
	if (i < 0) {
		fprintf(stderr,
			"%sStarted: Couldn't find module corresponding to cleint id %d\n",
			Prefix,
			fromid);
		++Error;
		ErrorStatus = EFI_STATUS_ERROR;
		ErrorClient = fromid;
		return(1);
	}
	conf = &ModuleConf[i];

	ninput  = estart->ninput;
	noutput = estart->noutput;

	/***** SHOULD REALLY CHECK ALL THE VALUES HERE *****/
printf("%sModule started %s: %d inputs, %d output\n",
	Prefix, conf->instname, ninput, noutput);

	/*
	 * Propogate output properties to the inputs to which they are connected
	 */
	for (i = 0; i < noutput; ++i) {
		for (j = 0; j < MAX_DIM; ++j)
		    conf->out_rank[i][j] = estart->output_rank[i][j];

		conf->out_type[i] = estart->output_type[i];

		conf->out_size[i] = estart->output_size[i];
		dest = conf->out_dst[i]; /* Destination of this output */
		k    = conf->out_pos[i]; /* Input arg at destination */
		if (dest < 0) continue;  /* This output not connected  */
		--k;

		ModuleConf[dest].in_size[k] = conf->out_size[i];
		ModuleConf[dest].in_type[k] = conf->out_type[i];

printf("%sModule started %s.%d -> %s.%d (%s size %d [ ",
	Prefix,
	conf->instname, i+1,
	ModuleConf[dest].instname, k+1,
	StreamTypeName(ModuleConf[dest].in_type[k]),
	ModuleConf[dest].in_size[k]);

		for (j = 0; j < MAX_DIM; ++j) {
			ModuleConf[dest].in_rank[k][j] = conf->out_rank[i][j];
if (ModuleConf[dest].in_rank[k][j])
   printf("%d ", ModuleConf[dest].in_rank[k][j]);
		}
printf(" ]\n");

	}

	return(1);
}

/*
 * Send configuration message to each running module
 */
INTERNAL void
configure_modules(void) {
	int i, j, k, arg, n;
	int nelem, desc, f;
	EVENT_EFI_CONFIG econf;
	int from[MAX_MODULES*MAX_ARGS];

	memclr(&econf, sizeof(econf));

	for (i = 0; i < Nmodule; ++i) {
		n = econf.ninput = ModuleConf[i].ninput;
		for (j = 0; j < n; ++j) {
			desc = 0; f = 1;

       printf("%sconfiguring %s input %d (of %d), %s w/ %d elems, %d dims [ ",
	       Prefix,
	       ModuleConf[i].instname, j+1,
	       ModuleConf[i].ninput,
	       typename(ModuleConf[i].in_type[j]),
	       ModuleConf[i].in_size[j],
	       ModuleConf[i].in_dim[j]);

			for (k = 0; k < ModuleConf[i].in_dim[j]; ++k) {
				econf.input_rank[j][k] = ModuleConf[i].in_rank[j][k];

       printf("%c%d", k?'x':' ', econf.input_rank[j][k]);

				/********************************************
				 * encode descriptor vector as a decimal number
				 * where each digit represents an element
				 * i.e. the low order digit represents the
				 * type of the scalar values, the 10s digit
				 * represents the 2nd dimension, i.e. the
				 * aggregation of scalars (e.g. by time, by
				 * channel, etc), etc.
				 ********************************************/
				desc += f * ModuleConf[i].in_desc[j][k];
				f *= 10;
			}

	printf(" ]\n");
			/* Fill in unused dimensions with 1s */
			while (k < MAX_DIM) econf.input_rank[j][k++] = 1;

			econf.input_type[j] = ModuleConf[i].in_type[j];
			econf.input_size[j] = ModuleConf[i].in_size[j];
			econf.input_desc[j] = desc; /* above encoding */

			econf.input_tag[j]  = ModuleConf[i].in_tag[j];
			econf.input_from[j] = from[ModuleConf[i].in_tag[j]];

		}

		n = econf.noutput = ModuleConf[i].noutput;
		for (j = 0; j < n; ++j) {
			econf.output_desc[j] = ModuleConf[i].out_desc[j][ModuleConf[i].out_dim[j]-1];
			desc = 0; f = 1;
       printf("%sconfiguring %s output %d (of %d), %s w/ %d elems, %d dims [ ",
	       Prefix,
	       ModuleConf[i].instname, j+1,
	       ModuleConf[i].noutput,
	       typename(ModuleConf[i].out_type[j]),
	       ModuleConf[i].out_size[j],
	       ModuleConf[i].out_dim[j]);
			for (k = 0; k < ModuleConf[i].out_dim[j]; ++k) {
				econf.output_rank[j][k] = ModuleConf[i].out_rank[j][k];

       printf("%c%d", k?'x':' ', econf.output_rank[j][k]);
				/* Encode desc - see above */
				desc += f * ModuleConf[i].out_desc[j][k];
				f *= 10;
			}

			while (k < MAX_DIM) econf.output_rank[j][k++] = 1;

			econf.output_type[j] = ModuleConf[i].out_type[j];
			econf.output_size[j] = ModuleConf[i].out_size[j];
			econf.output_desc[j] = desc; /* encoded above */

			econf.output_tag[j] = ModuleConf[i].out_tag[j];
			econf.output_to[j]  = ModuleConf[ModuleConf[i].out_dst[j]].clientid;
		}
	printf(" ]\n");

		SEND(ModuleConf[i].clientid, EFI_CONFIG, econf);
	}

}

/*
 * Send parameters to model
 */
EXPORT int
set_parameters_module(int id, char *file, char *section) {
	EVENT_EFI_PARAMETER_FILE epf;

	strcpy(epf.filename, file);
	strcpy(epf.section, section?section:ModuleConf[id].instname);

	SEND(ModuleConf[id].clientid, EFI_PARAMETER_FILE, epf);

	return(1);
}
