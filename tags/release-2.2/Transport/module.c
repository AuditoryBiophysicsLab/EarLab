/*
 * DAFFIE transport interface to Earlab computational module
 */
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <local/std.h>

#include "earlibc.h"
#include "CMatrixN.h"
#include "CSpike.h"
#include "CEarlabDataStream.h"

#include "earlabsvc.h"

#include "event.h"

#include "eventdef.h"
#include "efievent.h"

#ifndef MODULE
# define MODULE  xxx
#endif

#ifndef DEBUG
# define DEBUG 0        /* Default debug level */
#endif

#ifdef USE_GETWD
# define GetWD(s,n) getwd(s)
#else
# define GetWD(s,n) getcwd(s, n)
#endif

/*
 * Macros to generate actual module names
 * N.B. this works with GCC but may not work with other CPPs
 */
#ifndef XSTRING
# define XSTRING(x) #x
# define STRING(x) XSTRING(x)
#endif

#ifndef XCONCAT
# define XCONCAT(a,b) a ## b
# define CONCAT(a,b) XCONCAT(a, b)
#endif

#define MODULENAME STRING(MODULE)

#define ModuleSetName        CONCAT(MODULE,_SetName)
#define ModuleReadParameters CONCAT(MODULE,_ReadParameters)
#define ModuleAdvance        CONCAT(MODULE,_Advance)
#define ModuleStart          CONCAT(MODULE,_Start)
#define ModuleStop           CONCAT(MODULE,_Stop)

/* This are defined in sys/limits on some systems */
#ifdef MAX_INPUT
# undef MAX_INPUT
#endif
#ifdef MAX_OUTPUT
# undef MAX_OUTPUT
#endif

#define MAX_INPUT   10          /* Maximum number of inputs  */
#define MAX_OUTPUT  10          /* Maximum number of output  */
#define MAX_STREAM  500         /* Maximum number of streams */

typedef unsigned long      ULong;
typedef unsigned long long UTIME;

INTERNAL void efi_init(void);
INTERNAL void usage(char *pgm, int msg);
INTERNAL void try_advance(void);
INTERNAL void send_status(int what, int rc, int frame);
INTERNAL void send_data(int to, EVENT_EFI_DATA *out);
INTERNAL void dump_data(FILE *f, EVENT_EFI_DATA *obuf);
INTERNAL void dump_meta(FILE *f, EVENT_EFI_DATA *obuf);

INTERNAL void stream_zero(Stream *stream);
INTERNAL void show_desc(FILE *f, Stream *stream);
INTERNAL void show_data(FILE *f, Stream *stream, int n);
INTERNAL void tunnel_ready(int flag, int clientid);

INTERNAL int SetState(int what);
INTERNAL int ClearState(int what);
INTERNAL int TestState(int what);

INTERNAL int  do_start(EVENT_EFI_START *estart);
INTERNAL int  do_stop(EVENT_EFI_STOP *estop);
INTERNAL int  do_config(EVENT_EFI_CONFIG *econf);
INTERNAL void do_advance(void);
INTERNAL void do_param_file(EVENT_EFI_PARAMETER_FILE *eparam);
INTERNAL void do_param_set(EVENT_EFI_PARAMETER_SET *eparam);
INTERNAL void do_ctl(EVENT_EFI_CTL *ectl);
INTERNAL int  do_probe(int to, int enable, int param, int tag);

INTERNAL int  do_wavedata(EVENT_EFI_WAVEDATA *edata);
INTERNAL int  do_spikedata(EVENT_EFI_SPIKEDATA *edata);

/*
 * Structure for data stream buffers in buffer pool
 */
INTERNAL int    InTag[MAX_STREAM];        /* Input tag to arg index mapping  */
INTERNAL int    OutDest[MAX_OUTPUT];      /* Output destination (client id)  */

INTERNAL int    Ninput;                   /* Number of required inputs       */
INTERNAL int    Noutput;                  /* Number of outputs produced      */
INTERNAL int    HaveInput;                /* Have required inputs            */
INTERNAL int    Use;                      /* Next buffer set to use          */
INTERNAL int    In0, In1;                 /* Number of inputs store in bufs  */

/* Descriptors for the input and output arguments for the module */
INTERNAL int    InType[MAX_INPUT];             /* Input  arg stream type     */
INTERNAL ULong  InDim[MAX_INPUT];              /* Input  arg dimensions      */
INTERNAL ULong  InRank[MAX_INPUT][EFI_MAXDIM]; /* Input  arg rank vectors    */
INTERNAL ULong  InSize[MAX_INPUT];             /* Max # elems of input  args */

INTERNAL int    OutType[MAX_INPUT];            /* Output arg stream type     */
INTERNAL ULong  OutDim[MAX_OUTPUT];            /* Output arg dimensions      */
INTERNAL ULong  OutRank[MAX_INPUT][EFI_MAXDIM];/* Output arg rank vector     */
INTERNAL ULong  OutSize[MAX_OUTPUT];           /* Max # elems of output args */

#define MAX_IOS (2*MAX_INPUT+MAX_OUTPUT)

/* The array of Stream strcutures (whose address are) passed to Advance */
INTERNAL Stream         InArg0[MAX_INPUT];      /* C++ input  arg vector     */
INTERNAL Stream         InArg1[MAX_INPUT];      /* C++ input  arg vector     */
INTERNAL Stream         OutArg[MAX_OUTPUT];     /* C++ output arg vector     */

/* Storage for the incoming and outgoing events */
INTERNAL EVENT_EFI_DATA *Ibuf0[MAX_INPUT];      /* Event input buffer set 0  */
INTERNAL EVENT_EFI_DATA *Ibuf1[MAX_INPUT];      /* Event input buffer set 1  */
INTERNAL EVENT_EFI_DATA *Obuf[MAX_OUTPUT];      /* Event output buffer       */
INTERNAL int             Fbuf[MAX_INPUT];       /* Input buffer to be filled */

/*
 * Module states
 */

/* Transport layer states */
#define STATE_CONNECTED         0x00001 /* Connected to server          */
#define STATE_CONFIGURED        0x00002 /* Received i/o configuration   */
#define STATE_GETPARAMFILE      0x00004 /* Paramaters file requested    */
#define STATE_HAVEPARAMFILE     0x00008 /* Module parameters set        */
#define STATE_HAVEINPUTS        0x00010 /* Have inputs for next frame   */
#define STATE_RUNNING           0x00020 /* Module running (advance)     */
#define STATE_READY             0x00040 /* Ready for next frame         */
#define STATE_ADVANCE           0x00080 /* Received ADVANCE request     */
#define STATE_STOPPED           0x00100 /* Module is stopped            */
#define STATE_BROKEN            0x00200 /* Lost connection to server    */
#define STATE_ERROR             0x00400 /* Fatal error occurred         */

/* Module EFI states */
#define STATE_EFI_START         0x01000 /* Module started               */
#define STATE_EFI_STOP          0x02000 /* Module stopped               */
#define STATE_EFI_GETPARAM      0x04000 /* Paramters loaded             */
#define STATE_EFI_ADVANCE       0x08000 /* Module running               */
#define STATE_EFI_ERROR         0x10000 /* Module raised error          */

typedef volatile int VOLINT;

INTERNAL int        ControlId;          /* Client ID of control         */
INTERNAL VOLINT     State   = 0;        /* Module state                 */
INTERNAL VOLINT     Advance = 0;        /* Advance received             */
INTERNAL int        Frame   = 0;        /* Current frame number         */
INTERNAL int        Debug   = DEBUG;    /* Module debugging flag        */
INTERNAL int        Verbose = 0;        /* Verbosity flag               */
INTERNAL int        Unload  = 0;        /* Stop forces unload & exit    */
INTERNAL int        Tunnel  = 0;        /* Use tunnels for data         */
INTERNAL int        Ntunnel = 0;        /* Number of tunnels requested  */
INTERNAL int        TunnelReady = 0;    /* Number of tunnels established*/
INTERNAL int        Dump    = 0;        /* Dump output flag             */
INTERNAL FILE      *Dumpf[MAX_OUTPUT];  /* File ptrs. for dump files    */
INTERNAL FILE      *Metaf[MAX_OUTPUT];  /* Same for dump meta files     */
INTERNAL char      *Pgm;                /* Program name                 */
INTERNAL char       Rootdir[4000];      /* Shared file directory        */
INTERNAL char       Section[EFI_NAMESIZE];/* File section name          */
INTERNAL FILE      *Logf;               /* Log file descriptor          */
INTERNAL char      *ExpName;            /* Experiment name              */
INTERNAL char      *InstName;           /* Module instance name         */
INTERNAL char       ModName[200];       /* Module registered name       */
INTERNAL char       MyName[200];        /* Identifying/print name       */
INTERNAL UTIME      StartTime, RefTime;

#define MAX_PROBE   20
typedef struct {
	int     to;
	int     tag;
	int     arg;
} PROBE;
INTERNAL PROBE      Probe[MAX_PROBE];
INTERNAL int        Nprobe = 0;
INTERNAL int        Probelock;

INTERNAL EarlabEnv  Environ;

main(int argc, char *argv[]) {
	int   ok;
	char *server;
	int   id, nc;
	int   help, fsdebug, dafdebug;
	int   rc, done;
	int   i, thisid;
	EVENT e;
	char  wd[1000], me[200];

	/* Get program name - strip of leading path */
	if (Pgm = strrchr(argv[0], '/')) ++Pgm;
	else Pgm = argv[0];
	++argv; --argc;

	/* Set defaults then process command line args */
	strcpy(ModName, MODULENAME);
	wd[0]    = '\0';
	ok       = 1;
	help     = 0;
	fsdebug  = dafdebug = 0;
	InstName = MODULENAME;
	ExpName  = "";
	Logf     = stderr;
	while (argc && *argv[0] == '-') {
		char *opt;

		opt = argv[0] + 1;
		++argv; --argc;

		if      (streq(opt,  "help"))     ++help;
		else if (substr(opt, "debug"))    ++Debug;
		else if (substr(opt, "Debug"))    ++dafdebug;
		else if (substr(opt, "verbose"))  ++Verbose;
		else if (substr(opt, "quiet"))    Verbose = 0;
		else if (substr(opt, "fsdebug"))  ++fsdebug;
		else if (substr(opt, "unload"))   Unload = 1;
		else if (substr(opt, "nounload")) Unload = 0;
		else if (substr(opt, "tunnel"))   Tunnel = 1;
		else if (substr(opt, "notunnel")) Tunnel = 0;
		else if (substr(opt, "nodump"))   Dump   = 0;
		else if (substr(opt, "dump"))     Dump   = 1;
		else if (substr(opt, "capture"))  Dump   = 1; /* == dump */
		else if (substr(opt, "instance") || streq(opt, "name")) {
			if (argc < 1) {
				fprintf(stderr, "%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				break;
			}
			InstName = argv[0];
			++argv; --argc;
		}
		else if (substr(opt, "module") || streq(opt, "modname")) {
			if (argc < 1) {
				fprintf(stderr, "%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				break;
			}
			strcpy(ModName, argv[0]);
			++argv; --argc;
		}
		else if (substr(opt, "experiment")) {
			if (argc < 1) {
				fprintf(stderr, "%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				break;
			}
			ExpName = argv[0];
			++argv; --argc;
		}
		else if (substr(opt, "dir") || streq(opt, "cd")) {
			if (argc < 1) {
				fprintf(stderr, "%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				break;
			}
			strcpy(wd, argv[0]);
			++argv; --argc;
		}
		else if (substr(opt, "log")) {
			char *filename;

			if (argc < 1) {
				fprintf(stderr, "%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
				break;
			}
			filename = argv[0];
			++argv; --argc;
			if      (streq(filename, "-stdout")) Logf = stdout;
			else if (streq(filename, "-stderr")) Logf = stderr;
			else if (!(Logf = fopen(filename, "w"))) {
				fprintf(stderr,
					"%s: can't open log file %s\n",
					filename);
				Logf = stderr;
				ok = 0;
			}
		}
		else {
			fprintf(stderr, "%s: ignoring argument -%s\n", Pgm, opt);
			help = 1;
			ok   = 0;
		}
	}

	/* Check for errors and correct number of args */
	if (!ok || argc == 0 || help) {
		usage(Pgm, help);
		exit(!help);
	}

	/* Get event server name */
	server = argv[0];
	++argv; --argc;

	/* Initialize data structures */
	for (i = 0; i < MAX_INPUT; ++i) {
		Ibuf0[i] = Ibuf1[i] = Obuf[i] = NULL;
		Fbuf[i] = 0;
	}
	In0 = In1 = 0;
	Use = 0;

	mp_init();

	/* DAFFIE/EFI initialization - temporary until integrated */
	efi_init();

	/* Join the event server */
	event_debug(dafdebug);

	id = event_join(server, &nc);
	if (!id) {
		fprintf(stderr, "%s: join failed for server %s\n", Pgm, server);
		exit(4);
	}

	/* Enable/disable tunnels */
	event_tunnel_enable(Tunnel);

	/* Initialize streaming file/data services */
	efs_init();
	efs_debug(fsdebug);

	/* Specify root directory for file services */
	if (!wd[0]) {
		GetWD(wd, sizeof(wd));
		fprintf(stderr,
			"%s: warning - no file service directory specified, using %s\n",
			Pgm, wd);
	}
	if (!efs_chdir_receive(wd)) {
		fprintf(stderr,
			"%s: could not establish file service directory %s, chdir_receive failed\n",
			Pgm, wd);
		exit(1);
	}
	strcpy(Rootdir, wd);
	chdir(Rootdir);

	/* Create Earlab environment */
	Environ.InDir    = ".";
	Environ.OutDir   = ".";
	Environ.ExpName  = ExpName;
	Environ.RunName  = ExpName;
	Environ.UserName = "";
	earlab_environment(&Environ);

	/* Select the events we wish to recieve */
	event_select_type(0, ET_USER_MIN, ET_USER_MAX);
	event_select_type(1, ET_EFI_MIN,  ET_EFI_MAX);

	printf("\n============================ %s\n", ModName);
	printf("%s: %s.%s joined %s as client id %d (%d clients)\n",
	       Pgm, ModName, InstName, server, id, nc);
	gethostname(me, sizeof(me));
	printf("%s: %s.%s loaded on host %s\n", Pgm, ModName, InstName, me);

	SetState(STATE_CONNECTED);

	/* Allocate a lock for Probe structure */
	Probelock = mp_alloclock();

	/* Register ourselves */
	event_register("earlab", ModName, InstName);

	/* Set Module's Instance Name */
	ModuleSetName(InstName);

	/* Construct module name to be used in print message */
	if (streq(ModName, InstName)) strcpy(MyName, ModName);
	else sprintf(MyName, "%s.%s", InstName, ModName);

	/* Establish a callback for DATA messages */

	/* this is done as a callback so as to be architecturelly
	 * equivalent to an EFS data transfer
	 */
	event_callback(ET_EFI_WAVEDATA,  do_wavedata);
	event_callback(ET_EFI_SPIKEDATA, do_spikedata);

	/* Initialize parameter space */
	ReadParameterFile(NULL);

	/* Main loop - wait for events and process */
	done = 0;
	while (!done) {
		/* Force flush of stdout/stderr for redirection */
		fflush(stdout); fflush(stderr);

		/* Do error checks */
		if (State & (STATE_BROKEN|STATE_ERROR))
		   break;

		HaveInput = (Ninput == (Use?In1:In0));

		if (Advance && HaveInput) do_advance();

		event_wait_timeout(50);
		if (!event_receive(&e)) continue;

		switch(e.ev_head.type) {
		  case ET_EFI_CONFIG:
			rc = do_config((EVENT_EFI_CONFIG *)&e);
			break;

		  case ET_EFI_PARAMETER_FILE:
			do_param_file((EVENT_EFI_PARAMETER_FILE *)&e);
			break;

		  case ET_EFI_PARAMETER_SET:
			do_param_set((EVENT_EFI_PARAMETER_SET *)&e);
			break;

		  case ET_EFI_START:
			rc = do_start((EVENT_EFI_START *)&e);
			if (!rc) {
				printf("%s: start failed!\n", MyName);
				done = 1;
			}
			break;

		  case ET_EFI_ADVANCE:
			++Advance;

			send_status(EFI_STATUS_ADVANCE,
				    EFI_STATUS_WAITDATA,
				    ((EVENT_EFI_ADVANCE *)&e)->frame);

			if (Verbose || Debug)
			   printf("%s: Got advance for frame %d, Advance=%d, HaveInput=%d, Use=%d\n",
				   MyName,
				   ((EVENT_EFI_ADVANCE *)&e)->frame,
				   Advance,
				   HaveInput, Use);


			break;

		  case ET_EFI_STOP:
			do_stop((EVENT_EFI_STOP *)&e);
			done = Unload;
			break;

		  case ET_EFI_UNLOAD:
			done = 1;
			send_status(EFI_STATUS_UNLOAD, EFI_STATUS_OK, Advance);
			break;

		  case ET_EFI_STATUS:
			/* Quietly ignore STATUS messages */
			break;

		  case ET_EFI_CTL:
			do_ctl((EVENT_EFI_CTL *)&e);
			break;

		  case ET_LEAVE:
			/* See if this client was receiving probes data */
			thisid = ((EVENT_LEAVE *)&e)->id;

			for (i = 0; i < Nprobe; ++i)
			  if (Probe[i].to == thisid)  break;

			if (i < Nprobe)
			   /* remove all probes to this client */
			   do_probe(thisid, 0, -1, 0);

			break;

		  default:
			if (Debug > 1)
			   printf("%s: Ignoring event %s (%d) from %d\n",
				   MyName,
				   event_lookup_name(e.ev_head.type),
				   e.ev_head.type, e.ev_head.from);
		}
	}
	printf("%s: Done and exiting\n", MyName);

	event_flush_sync();
	event_leave();

	return(0);

}

/*
 * DAFFIE runtime definitions for EFI events
 */
#include "agentd.h"

INTERNAL void
efi_init(void) {
	event_verbose(Verbose);
	event_init();
#include "efieventdef.h"
#include "agentddef.h"
}

/*
 * Help funtion
 */
#define PRT(opt,str) \
	printf(msg?"\t%s%s%s\n":"%s ", opt, (strlen(opt)<8)?"\t\t":"\t", str)

INTERNAL void
usage(char *pgm, int msg) {
	if (msg) printf("usage: %s [options] server\n", pgm);
	else     printf("usage: %s ", pgm);
	PRT("[-help]",          "display this message");
	PRT("[-debug]",         "turning on debugging");
	PRT("[-verbose]",       "verbose messages");
	PRT("[-quiet]",         "no verbose messages");
	PRT("[-fsdebug]",       "turning on file service debugging");
	PRT("[-unload]",        "unload and exit on 'stop'");
	PRT("[-nounload]",      "leave module loaded on 'stop'");
	PRT("[-tunnel]",        "use tunnels for data transfers");
	PRT("[-notunnel]",      "don't use tunnels for data transfers");
	PRT("[-instance name]", "specify instance name");
	PRT("[-experiment name]","specify experiment name");
	PRT("[-dir path]",      "specify shared filesystem root");
	PRT("[-cd path]",       "synonym for -dir");
	PRT("[-log {file|-stderr|-stdout}]", "write log to file");
	PRT("[-memory mb]",     "specify memory buffer size");

	PRT("server",           "event server name");
	if (!msg) printf("\n");
}

/*
 * Exported to the module
 */
LogInfo(char *msg) {
	printf("%s\n", msg);
	return(1);
}

/*
 * Ancillary stuff for manipulating state
 */
INTERNAL int
SetState(int what) {
	int was = State;

	if (Debug > 1)
	   printf("%s.SetState: Adding 0x%06x to 0x%06x\n", MyName, what, State);
	State |= what;

	return(was);
}

INTERNAL int
ClearState(int what) {
	int was = State;

	if (Debug > 1)
	   printf("%s.ClearState: Clearing 0x%06x from 0x%06x\n", MyName, what, State);

	State &= ~what;

	return(was);
}

INTERNAL int
TestState(int what) {
	int is;

	if (Debug > 1)
	   printf("%s.TestState: Testing for 0x%06x in 0x%06x\n", MyName, what, State);

	is = State&what;
	return(is != 0);
}

/*
 * Process a GetParameterFile message
 *   download the file and pass to module
 */
INTERNAL void download_complete(int reqid, int clientd, int rc, char *name, int size, int tag);

INTERNAL void
do_param_file(EVENT_EFI_PARAMETER_FILE *eparam) {
	char *file, *section;
	int   rc;

	file    = eparam->filename;
	section = eparam->section;

	if (Debug) printf("%s.do_param_file: file \"%s\"\n", MyName, file);

	if (!*file) {
		rc = ModuleReadParameters(NULL, *section?section:InstName);
		send_status(EFI_STATUS_PARAM, rc?EFI_STATUS_READY:EFI_STATUS_ERROR, 0);
		return;
	}

	if (Debug)
	   printf("%s.do_param_file: requesting download %s\n", MyName, file);

	strcpy(Section, section);

	rc = efs_download_cond_fs(file, file, download_complete, 0);
	if (!rc) {
		int errno;

		errno = efs_error();
		printf("%s.do_param_file: download failure %d for %s - %s\n",
			MyName, errno, file, efs_errormsg(errno));

		send_status(EFI_STATUS_PARAM, EFI_STATUS_ERROR, 0);

		SetState(STATE_ERROR);
	}

	SetState(STATE_GETPARAMFILE);
	return;
}

/*
 * Callback function when download completes
 */
INTERNAL void
download_complete(int reqid, int clientid, int rc, char *name, int size, int tag) {
	char path[4000];

	if (rc != EFS_RC_OK) {
		printf("%s.do_param_complete: Error %d processing request for %s!!\n",
			MyName, rc, name);
		exit(2);
	}
	printf("%s.do_param_complete: File %s download complete, %d bytes\n",
		MyName, name, size);

	/***
	  sprintf(path, "%s/%s", Rootdir, name);
	***/
	strcpy(path, name);

	rc = ModuleReadParameters(path, Section[0]?Section:NULL);

	send_status(EFI_STATUS_PARAM, rc?EFI_STATUS_READY:EFI_STATUS_ERROR, 0);

	SetState(STATE_HAVEPARAMFILE|STATE_EFI_GETPARAM);
	return;
}

/*
 * Set a parameter
 */
INTERNAL void
do_param_set(EVENT_EFI_PARAMETER_SET *eparam) {
	int   rc;

	if (Debug) printf("%s.do_param_set: %s\n", MyName, eparam->paramspec);

	rc = ParseParameter(NULL, eparam->paramspec);

	if (!rc) {
		send_status(EFI_STATUS_PARAM, EFI_STATUS_ERROR, 0);

		SetState(STATE_ERROR);
	}

	return;
}
/*
 * Process a module configuration message
 */
INTERNAL int
do_config(EVENT_EFI_CONFIG *econf) {
	int    i, k;
	int    x = 1, endian = ! *((BYTE *)&x);
	int    nmatrix, nspike, nelem, type, ninput, noutput;
	int    nbyte, esize;
	unsigned long ndim;
	BYTE  *bp, *buf;

	/* Stash away senders id */
	ControlId = econf->head.from;

	/*
	 * Count number of elements and allocate storage
	 */

	/* Input arrays */
	nmatrix  = 0;
	nspike   = 0;
	ninput = econf->ninput;
	for (i = 0; i < ninput; ++i) {
		/* Determine dimensionality of input array */
		for (ndim = EFI_MAXDIM;
		     econf->input_rank[i][ndim-1] <= 1 && ndim > 1;
		     --ndim);

		printf("%s.config: configuring input %d: %s tag %d, rank %d",
			MyName, i+1,
			StreamTypeName(econf->input_type[i]),
			econf->input_tag[i],
			econf->input_rank[i][0]);
		if (ndim > 1) printf("x%d", econf->input_rank[i][1]);
		if (ndim > 2) printf("x%d", econf->input_rank[i][2]);
		if (ndim > 3) printf("x%d", econf->input_rank[i][3]);
		if (ndim > 4) printf("x%d", econf->input_rank[i][4]);
		if (ndim > 5) printf("... (%d huh?)", ndim);
		printf(", size %d\n", econf->input_size[i]);

		InTag[econf->input_tag[i]] = i;

		InDim[i]  = ndim;
		InSize[i] = nelem = econf->input_size[i];
		InType[i] = type  = econf->input_type[i];

		for (k = 0; k < ndim; ++k) InRank[i][k] = econf->input_rank[i][k];
		for (;k < EFI_MAXDIM; ++k) InRank[i][k] = 0; /* that's they way he wants it */

		if      (type == EFI_STREAM_WAVEFORM) nmatrix += nelem;
		else if (type == EFI_STREAM_SPIKE)    nspike  += nelem;
		else {
			printf("%s.config: unknown input arg %d type %d\n",
				MyName, i+1, econf->input_type[i]);
			send_status(EFI_STATUS_CONFIG, EFI_STATUS_ERROR, 0);
			return(0);
		}

	}
	nmatrix <<= 1;    /* Times 2 for double buffering */
	nspike  <<= 1;    /* Times 2 for double buffering */

	/* Output arrays */
	noutput = econf->noutput;
	for (i = 0; i < noutput; ++i) {
		for (ndim = EFI_MAXDIM;
		     econf->output_rank[i][ndim-1] <= 1 && ndim > 1;
		     --ndim);
		printf("%s.config: configuring output %d: %s tag %d, rank %d",
			MyName, i+1,
			StreamTypeName(econf->output_type[i]),
			econf->output_tag[i],
			econf->output_rank[i][0]);
		if (ndim > 1) printf("x%d", econf->output_rank[i][1]);
		if (ndim > 2) printf("x%d", econf->output_rank[i][2]);
		if (ndim > 3) printf("x%d", econf->output_rank[i][3]);
		if (ndim > 4) printf("x%d", econf->output_rank[i][4]);
		if (ndim > 5) printf("... (%d huh?)", ndim);
		printf(", size %d\n", econf->output_size[i]);

	if (econf->output_size[i] < 0) {
		printf("Output.%d size < 0!!! Aborting\n");
		abort();
	}

		OutDest[i] = econf->output_to[i];

		/* If tunneling is enable, request tunnel to destination */
		if (Tunnel) {
			++Ntunnel;
			event_tunnel_request(OutDest[i], tunnel_ready);
		}

		OutDim[i]  = ndim;
		OutSize[i] = nelem = econf->output_size[i];
		OutType[i] = type  = econf->output_type[i];

		for (k = 0; k < ndim; ++k) OutRank[i][k] = econf->output_rank[i][k];
		for (;k < EFI_MAXDIM; ++k) OutRank[i][k] = 0; /* that's they way he wants it */

		if      (type == EFI_STREAM_WAVEFORM) nmatrix += nelem;
		else if (type == EFI_STREAM_SPIKE)    nspike  += nelem;
		else {
			printf("%s.config: unknown output arg %d type %d\n",
				MyName, i+1, econf->output_type);
			send_status(EFI_STATUS_CONFIG, EFI_STATUS_ERROR, 0);
			return(0);
		}
	}

if (Verbose || Debug)
printf("%s.config: %d inputs, %d outputs, %d total args, %d matrix elements, %d spike elements\n",
	MyName, ninput, noutput, ninput+noutput, nmatrix, nspike);

	esize = sizeof(EVENT_EFI_DATA) - 4;
	nbyte = (2*ninput + noutput)*esize + nmatrix*sizeof(MATRIXN_T) + nspike*sizeof(TSpike);

if (Debug)
printf("sizeof spike is %d - %d spikes = %d\n",
	sizeof(TSpike), nspike, nspike*sizeof(TSpike));

if (Debug)
printf("%s.config: allocating %d bytes for 2 x %d inputs, %d outputs\n", MyName, nbyte, ninput, noutput);

	bp = buf = (BYTE *)malloc(nbyte);

	/* Set pointers to input data events for bank 0 */
	for (i = 0; i < ninput; ++i) {
		int j, elemsize;
		EVENT_EFI_DATA      *edata;
		EVENT_EFI_WAVEDATA  *ewave;
		EVENT_EFI_SPIKEDATA *espike;

		Ibuf0[i] = edata = (EVENT_EFI_DATA *)bp;

		/* Instantiate the C++ data class for this input arg */
		switch (econf->input_type[i]) {
		  case EFI_STREAM_WAVEFORM:
			edata->elems = InSize[i];
			edata->type  = EFI_STREAM_WAVEFORM;

			ewave = (EVENT_EFI_WAVEDATA  *)edata;

			InArg0[i].datatype      = EFI_STREAM_WAVEFORM;
			InArg0[i].data.waveform = matrixn_const(InDim[i], InRank[i], ewave->data);
			elemsize = sizeof(MATRIXN_T);
    if (Debug)
     printf("%s.conf: Constructed bank 0, input %d, MatrixN w/ %d dim %dx%dx%d\n",
	    MyName, i, *(InArg0[i].data.waveform->mDimCount), InRank[i][0], InRank[i][1], InRank[i][2]);

			break;

		  case EFI_STREAM_SPIKE:
			edata->elems = 0;
			edata->type  = EFI_STREAM_SPIKE;

			espike = (EVENT_EFI_SPIKEDATA  *)edata;

			InArg0[i].datatype      = EFI_STREAM_SPIKE;
			InArg0[i].data.spike    = spike_const(InSize[i], espike->data);
			elemsize = sizeof(TSpike);
    if (Debug)
     printf("%s.conf: Constructed bank 0, input %d, Spike max %d\n",
	    MyName, i, InArg0[i].data.spike->MaxSpikes);
			break;

		  default: /* Should never happen since we already checked it */
			printf("%s: Fatal error - Input %d of unknown type %d\n",
				MyName, i, econf->input_type[i]);
			abort();
		}
		edata->tag    = econf->input_tag[i];
		edata->frame  = 1;
		edata->endian = endian;
		edata->index0 = 0;
		edata->indexN = edata->elems - 1;

		bp += esize + InSize[i]*elemsize;
	}


	/* Set pointers to input data events for bank 1 */
	for (i = 0; i < ninput; ++i) {
		int j, elemsize;
		EVENT_EFI_DATA      *edata;
		EVENT_EFI_WAVEDATA  *ewave;
		EVENT_EFI_SPIKEDATA *espike;

		Ibuf1[i] = edata = (EVENT_EFI_DATA *)bp;

		/* Instantiate the C++ class for this input arg */
		switch (econf->input_type[i]) {
		  case EFI_STREAM_WAVEFORM:
			edata->elems = InSize[i];
			edata->type  = EFI_STREAM_WAVEFORM;

			ewave = (EVENT_EFI_WAVEDATA  *)edata;

			InArg1[i].datatype      = EFI_STREAM_WAVEFORM;
			InArg1[i].data.waveform = matrixn_const(InDim[i], InRank[i], ewave->data);
			elemsize = sizeof(MATRIXN_T);
    if (Debug)
     printf("%s.conf: Constructed bank 1, input %d, MatrixN w/ %d dim %dx%dx%d\n",
	    MyName, i, *(InArg1[i].data.waveform->mDimCount), InRank[i][0], InRank[i][1], InRank[i][2]);

			break;

		  case EFI_STREAM_SPIKE:
			edata->elems = 0;
			edata->type  = EFI_STREAM_SPIKE;

			espike = (EVENT_EFI_SPIKEDATA  *)edata;

			InArg1[i].datatype      = EFI_STREAM_SPIKE;
			InArg1[i].data.spike    = spike_const(InSize[i], espike->data);
			elemsize = sizeof(TSpike);
    if (Debug)
     printf("%s.conf: Constructed bank 1, input %d, Spike max %d\n",
	    MyName, i, InArg1[i].data.spike->MaxSpikes);
			break;

		  default: /* Should never happen since we already checked it */
			printf("%s: Fatal error - Input %d of unknown type %d\n",
				MyName, i, econf->input_type[i]);
			abort();
		}

		edata->tag    = econf->input_tag[i];
		edata->frame  = 1;
		edata->endian = endian;
		edata->index0 = 0;
		edata->indexN = edata->elems - 1;

		bp += esize + InSize[i]*elemsize;
	}

	/* Set pointers to output data events */
	for (i = 0; i < noutput; ++i) {
		int j, type, elemsize;
		EVENT_EFI_DATA      *edata;
		EVENT_EFI_WAVEDATA  *ewave;
		EVENT_EFI_SPIKEDATA *espike;

		Obuf[i] = edata = (EVENT_EFI_DATA *)bp;

		/* Instantiate the C++ data class for this output arg */
		switch (econf->output_type[i]) {
		  case EFI_STREAM_WAVEFORM:
			edata->elems = OutSize[i];

			ewave = (EVENT_EFI_WAVEDATA  *)edata;

			OutArg[i].datatype      = EFI_STREAM_WAVEFORM;
			OutArg[i].data.waveform = matrixn_const(OutDim[i], OutRank[i], ewave->data);

			type     = EFI_STREAM_WAVEFORM;
			elemsize = sizeof(MATRIXN_T);
    if (Debug)
     printf("%s.conf: Constructed output %d, MatrixN w/ %d dim %dx%dx%d\n",
	    MyName, i, *(OutArg[i].data.waveform->mDimCount), OutRank[i][0], OutRank[i][1], OutRank[i][2]);

			break;

		  case EFI_STREAM_SPIKE:
			edata->elems = 0;

			espike = (EVENT_EFI_SPIKEDATA  *)edata;

			OutArg[i].datatype      = EFI_STREAM_SPIKE;
			OutArg[i].data.spike    = spike_const(OutSize[i], espike->data);

			type     = EFI_STREAM_SPIKE;
			elemsize = sizeof(TSpike);
    if (Debug)
     printf("%s.conf: Constructed ouput %d, Spike max %d\n",
	    MyName, i, OutArg[i].data.spike->MaxSpikes);
			break;

		  default: /* Should never happen since we already checked it */
			printf("%s: Fatal error - output %d of unknown type %d\n",
				MyName, i, econf->output_type[i]);
			abort();
		}

		Obuf[i]->endian= endian;
		Obuf[i]->type  = type;
		Obuf[i]->tag   = econf->output_tag[i];
		printf("%s: Setting output %d, tag %d to addr 0x%x\n",
			MyName, i, Obuf[i]->tag, OutArg[i].data);

		bp += esize + OutSize[i]*elemsize;
	}

	if (bp != buf+nbyte)
	   printf("%s.conf: bp = %x, should be %x\n", MyName, bp, buf+nbyte);

	/* Zero input bank 0 arrays for 1st time step */
	for (i = 0; i < ninput; ++i) stream_zero(&InArg0[i]);

	/* Zero output arrys */
	for (i = 0; i < noutput; ++i) stream_zero(&OutArg[i]);

	Ninput  = ninput;
	Noutput = noutput;

	/*
	 * Open output files if Dump option is enabled
	 */
	if (Dump) for (i = 0; i < Noutput; ++i) {
		char basename[500], filename[500];
		char oname[20];
		int  fd, wflag;

		/*
		 * Construct file name and open for writing
		 */
		basename[0] = '\0';
		if (*ExpName && ExpName[0]) {
			strcat(basename, ExpName);
			strcat(basename, ".");
		}
		if (InstName) strcat(basename, InstName);
		else          strcat(basename, ModName);
		sprintf(oname, ".%d.out", i+1);

		strcpy(filename, basename);
		strcat(filename, oname);

		/* Use Open to attempt large files on 32 bit systems */
		wflag = O_WRONLY | O_CREAT | O_TRUNC;
#ifdef O_LARGEFILE
		wflag |= O_LARGEFILE;
#endif
		fd = open(filename, wflag, 0666);
		if (fd < 0) Dumpf[i] = NULL;
		else        Dumpf[i] = fdopen(fd, "w");

		/*
		 * Construct metafile name and open for writing
		 */
		sprintf(oname, ".%d.meta", i+1);
		strcpy(filename, basename);
		strcat(filename, oname);

		Metaf[i] = fopen(filename, "w");
		if (!Dumpf[i] || !Metaf[i]) {
			fprintf(stderr,
				"%s: can't open file \"%s\" for %s output %d\n",
				MyName, filename,
				Dumpf[i]?"metafile":"data",
				i+1);
			if (Dumpf[i]) fclose(Dumpf[i]);

			send_status(EFI_STATUS_CONFIG, EFI_STATUS_ERROR, 0);
			while (--i >= 0) {
				fclose(Dumpf[i]);
				fclose(Metaf[i]);
			}
			return(0);
		}
		fprintf(Metaf[i], "%s [", StreamTypeName(OutType[i]));
		for (k = 0; OutRank[i][k]; ++k) {
			fprintf(Metaf[i], " %d", OutRank[i][k]);
		}
		fprintf(Metaf[i], " ] %d\n", OutSize[i]);
	}

	SetState(STATE_CONFIGURED);

	send_status(EFI_STATUS_CONFIG, EFI_STATUS_OK, 0);

	return(1);
}

INTERNAL void
stream_zero(Stream *stream) {
	switch (stream->datatype) {
	  case EFI_STREAM_WAVEFORM: {
		int        nelem = stream->data.waveform->nElements;
		MATRIXN_T *data  = stream->data.waveform->mData;
		int        k;

		for (k = 0; k < nelem; ++k) data[k] = 0.0;
	  } break;

	  case EFI_STREAM_SPIKE: {
		Spike_CLASS *spikec = stream->data.spike;
		spikec->CurSpikes = 0;

	  } break;

	  default:
		printf("stream_zero: Unknown stream type %d\n", stream->datatype);
	}
}

/*
 * Callback from tunnel_request
 */
INTERNAL void
tunnel_ready(int flag, int clientid) {
	if (Verbose)
	   printf("tunnel_ready: tunnel to %d %s\n",
		   clientid, flag?"ready":"failed!");
	if (!flag) return;

	++TunnelReady;

	/* Use tunnel for data events */
	event_tunnel_select(clientid, 1, ET_EFI_DATA_LO, ET_EFI_DATA_HI);
}

INTERNAL int
do_start(EVENT_EFI_START *estart) {
	int  i, j, rc;
	int  ndim, nelem;

	Ninput  = estart->ninput;
	Noutput = estart->noutput;

printf("%s.start: Ninput = %d, Noutput = %d\n", MyName, Ninput, Noutput);

	Use   = 0;
	In0   = Ninput;
	Frame = 0;

	for (i = 0; i < Ninput; ++i) Fbuf[i] = 1;

	for (i = 0; i < Ninput; ++i) {
		/* Determine dimensionality of input array */
		for (ndim = EFI_MAXDIM;
		     estart->input_rank[i][ndim-1] <= 1 && ndim > 1;
		     --ndim);

		/* Compute total number of elements*/
		nelem = 1;
		for (j = 0; j < ndim; ++j)
		   nelem *= estart->input_rank[i][j];
		InSize[i] = nelem;

		for (j = 0; j < ndim; ++j)
		   InRank[i][j] = estart->input_rank[i][j];
		for (     ; j < EFI_MAXDIM; ++j)
		   InRank[i][j] = 0; /* zero unused dimensions for Class lib */

		InType[i] = estart->input_type[i];
printf("%s.pre-start: input.%d %s %d elems\n", MyName, i+1, StreamTypeName(InType[i]), InSize[i]);
	}

	/* Output arrays */
	for (i = 0; i < Noutput; ++i) {
		/* Determine deminsionality */
		for (ndim = EFI_MAXDIM;
		     estart->output_rank[i][ndim-1] <= 1 && ndim > 1;
		     --ndim);

		/* Compute total number of elements*/
		nelem = 1;
		for (j = 0; j < ndim; ++j)
		   nelem *= estart->output_rank[i][j];
		OutSize[i] = nelem;

		for (j = 0; j < ndim; ++j)
		   OutRank[i][j] = estart->output_rank[i][j];
		for (     ; j < EFI_MAXDIM; ++j)
		   OutRank[i][j] = 0; /* zero unused dimensions for Class lib */

		OutType[i] = estart->output_type[i];
printf("%s.pre-start: output.%d %s %d elems\n", MyName, i+1, StreamTypeName(OutType[i]), OutSize[i]);
	}

	rc = ModuleStart(Ninput, InType, InRank, Noutput, OutType, OutRank, OutSize);

	/*
	 * Copy output size as set by module.start and send back
	 */
	for (i = 0; i < Noutput; ++i) {
		estart->output_size[i] = OutSize[i];

printf("%s.start: output.%d %s size = %d [ ",
	MyName, i+1, StreamTypeName(OutType[i]), OutSize[i]);
for (j = 0; j < EFI_MAXDIM && estart->output_rank[i][j]; ++j)
   printf("%d ", estart->output_rank[i][j]);
printf("]\n");

	}
	event_send(EventFrom(*estart), ET_EFI_START, (EVENT *)estart, sizeof(EVENT_EFI_START));

	printf("%s: Started module (%s)\n", MyName, rc==1?"ok":"failed");

	ClearState(STATE_STOPPED);
	SetState(STATE_EFI_START|STATE_HAVEINPUTS|(rc?STATE_READY:STATE_ERROR));

	/* Send back a status message */
	send_status(EFI_STATUS_START, rc?EFI_STATUS_READY:EFI_STATUS_ERROR, 0);

	return(1);
}

INTERNAL void
send_status(int type, int status, int frame) {
	char *tmsg, tbuf[30];
	char *smsg, sbuf[30];
	EVENT_EFI_STATUS estatus;

	estatus.type   = type;
	estatus.status = status;
	estatus.detail = 0;
	estatus.frame  = frame;
	estatus.timestamp = event_clock();

	/* Format up a nice message */
	if (0 < estatus.type && estatus.type <= EFI_STATUS_MAXTYPE)
	   tmsg = StatusTypeMsg[estatus.type];
	else
	   sprintf(tmsg=tbuf,"BAD (%d)", estatus.type);

	if (0 < estatus.status && estatus.status <= EFI_STATUS_MAXSTATUS)
	   smsg = StatusStatusMsg[estatus.status];
	else
	   sprintf(smsg=sbuf,"BAD (%d)", estatus.type);

	if (Verbose || Debug) {
	   printf("%s: Sending status %s %s (%d) for frame %d\n",
		  MyName, tmsg, smsg, estatus.status, estatus.frame);
	   fflush(stdout); fflush(stderr);
	}

	/* Send status */
	SEND(EVENT_BCAST_NOTME, EFI_STATUS, estatus);

}

INTERNAL int
do_stop(EVENT_EFI_STOP *estop) {
	int i;

	printf("%s.stop: at Frame %d\n", MyName, Frame);

	/* Stop the module */
	ModuleStop();

	send_status(EFI_STATUS_STOP, EFI_STATUS_OK, Frame);

	/* Clear state and reset globals */
	ClearState(-1 & ~(STATE_CONNECTED|STATE_EFI_STOP));
	SetState(STATE_STOPPED|STATE_EFI_STOP);

	printf("%s.stop: at Frame %d - freeing buffers\n", MyName, Frame);

	/* Free buffers */
	if (Ibuf0[0]) free(Ibuf0[0]);
	else if (Obuf[0]) free(Obuf[0]);

	for (i = 0; i < Ninput; ++i) {
		Ibuf0[i]  = NULL;
		Ibuf1[i]  = NULL;
		InArg0[i].datatype = EFI_STREAM_NONE;
		InArg1[i].datatype = EFI_STREAM_NONE;
		InArg0[i].data.ptr = NULL;
		InArg1[i].data.ptr = NULL;
		InSize[i] = 0;
		Fbuf[i]   = 0;
	}
	for (i = 0; i < Noutput; ++i) {
		Obuf[i] = NULL;
		OutArg[i].datatype = EFI_STREAM_NONE;
		OutArg[i].data.ptr = NULL;
		OutSize[i] = 0;

		if (Dump) {
			fclose(Dumpf[i]);
			fclose(Metaf[i]);
		}
	}
	Ninput = Noutput = 0;
	In0 = In1 = 0;
	HaveInput = 0;

	printf("%s.stop: at Frame %d - complete\n", MyName, Frame);

	return(1);
}

INTERNAL int
do_wavedata(EVENT_EFI_WAVEDATA *edata) {
	int   from;
	int   inarg;
	int   indx;
	int   count;
	BYTE *inbuf;

	from  = edata->head.from;
	inarg = InTag[edata->tag];

	indx = Fbuf[inarg];

   if (Debug)
      printf("%s.rcvdata: Rcv input stream %d from %d for input %d, for frame %d, using buffer %d at 0x%08x (%d bytes)\n",
	      MyName, edata->tag, from, inarg, edata->frame,
	      indx, indx?Ibuf1[inarg]:Ibuf0[inarg],
	      EventSize(edata));

	/* Ignore data from the ultimate frame, i.e. after STOP */
	if (TestState(STATE_STOPPED)) {
		if (Debug)
		  printf("%s.rcvdata: ignoring input in stopped state at frame %d\n",
			  MyName, Frame);
		return(1);
	}

   if (Debug) {
	MATRIXN_T *data;

	data = edata->data;
	 printf("%s.rcvdata: input data %f %f %f %f ...\n",
		MyName, data[0], data[1], data[2], data[3]);
   }
	inbuf = (BYTE *)(indx?Ibuf1:Ibuf0)[inarg];

	/* Sanity check on buffer pointer */
	/* This was added while debugging the "last frame" problem and  *
	 * it makes some sense to keep this as a prophylatic for        *
	 * future bugs                                                  */
	if (!inbuf) {
		 printf("%s: *ERRROR* do_wavedata frame %d - NULL buffer input.%d buf set %d !!!\n",
			MyName, Frame, inarg, indx);
		 return(0);
	}

	if (edata->index0 == 0) {
		memcpy(inbuf, edata, edata->head.size);
	}
	else if (edata->index0 < edata->indexN) {
		int offset, bytes;

		offset = sizeof(EVENT_EFI_DATA) + (edata->index0-1)*sizeof(MATRIXN_T);
		bytes  = (edata->indexN - edata->index0 + 1) * sizeof(MATRIXN_T);
		memcpy(inbuf+offset, edata->data, bytes);

	}
   if (Debug)
	printf("%s.rcvdata: Added elems %d-%d of %d to buf %d\n",
		MyName, edata->index0, edata->indexN, edata->elems, indx);

	if (edata->indexN+1 < edata->elems) return(1);

	if (indx == 0) count = ++In0;
	else           count = ++In1;

   if (Debug)
      printf("%s.rcvdata: Have %d of %d streams in buf set %d\n", MyName, indx?In1:In0, Ninput, indx);

	/* Swap buffers */
	Fbuf[inarg] = !Fbuf[inarg];

	if (Use == indx && count == Ninput) {
		event_wait_cancel();
	}
	return(1);

}

INTERNAL int
do_spikedata(EVENT_EFI_SPIKEDATA *edata) {
	int   from;
	int   inarg;
	int   indx;
	int   count;
	int   nelem;
	BYTE *inbuf;

	from  = edata->head.from;       /* Sender                        */
	inarg = InTag[edata->tag];      /* Index of this input paramater */

	indx = Fbuf[inarg];             /* index fo buffer set being filled */

   if (Debug)
      printf("%s.rcvdata: Rcv input stream %d from %d for input %d, for frame %d, using buffer %d at 0x%08x (%d bytes)\n",
	      MyName, edata->tag, from, inarg, edata->frame,
	      indx, indx?Ibuf1[inarg]:Ibuf0[inarg],
	      EventSize(edata));

	/* Ignore data from the ultimate frame, i.e. after STOP */
	if (TestState(STATE_STOPPED)) {
		if (Debug)
		  printf("%s.rcvdata: ignoring input in stopped state at frame %d\n",
			  MyName, Frame);
		return(1);
	}

   if (Debug) {
	 TSpike *data;

	 data = edata->data;
	 printf("%s.rcvdata: input data %d:%f %d:%f %d:%f ...\n",
		MyName,
		data[0].cell, data[0].time,
		data[1].cell, data[1].time,
		data[2].cell, data[2].time);
   }
	inbuf = (BYTE *)(indx?Ibuf1:Ibuf0)[inarg];      /* -> arg buffer */

	/* Sanity check on buffer pointer */
	/* This was added while debugging the "last frame" problem and  *
	 * it makes some sense to keep this as a prophylatic for        *
	 * future bugs                                                  */
	if (!inbuf) {
		 printf("%s: *ERRROR* do_spikedata frame %d - NULL buffer input.%d buf set %d !!!\n",
			MyName, Frame, inarg, indx);
		 return(0);
	}

	/* Copy in the additional elements from this transfer */
	if (edata->index0 == 0) {       /* copy header, as well */
		memcpy(inbuf, edata, edata->head.size);
	}
	else if (edata->index0 <= edata->indexN) { /* copy just the data */
		int offset, bytes;

		offset = sizeof(EVENT_EFI_DATA) + (edata->index0-1)*sizeof(TSpike);
		bytes  = (edata->indexN - edata->index0 + 1) * sizeof(TSpike);
		memcpy(inbuf+offset, edata->data, bytes);

	}

   if (Debug)
	printf("%s.rcvdata: Added spikes %d-%d of %d to buf %d\n",
		MyName, edata->index0, edata->indexN, edata->elems, indx);

	/* Check to see if this is the last transfer for this arugment   */
	if (edata->indexN+1 < edata->elems) return(1);  /* if not return */

	/* Increment the number of complete arguments in this buffer set */
	if (indx == 0) count = ++In0;
	else           count = ++In1;

   if (Debug)
      printf("%s.rcvdata: Have %d of %d streams in buf set %d\n", MyName, indx?In1:In0, Ninput, indx);

	/* Swap buffers for this argument */
	Fbuf[inarg] = !Fbuf[inarg];

	/*
	 * If we have all the arguments that we need,
	 * wake up main thread in case it is in an event_wait
	 */
	if (Use == indx && count == Ninput) {
		event_wait_cancel();
	}

	return(1);

}

INTERNAL void
do_advance(void) {
	int     i;
	int     rc;
	int     nelem;
	Stream *Input[MAX_INPUT], *Output[MAX_OUTPUT];

	/* Increment the frame counter */
	Frame++;

if (Debug)
   printf("%s.advance: running module at timestep %d using buffer set %d\n", MyName, Frame, Use);

	/* Send a status message */
	send_status(EFI_STATUS_ADVANCE, EFI_STATUS_STARTING, Frame);

	/*
	 * Build input and output argument vectors (STREAM *)
	 * (This should probably be done once during initialization)
	 */
	for (i = 0; i < Ninput; ++i) {
		Input[i] = (Use == 0)?&InArg0[i]:&InArg1[i];

		/* Set input count for spike streams */
		if (Input[i]->datatype == EFI_STREAM_SPIKE) {
			EVENT_EFI_DATA *edata;
			Spike_CLASS    *spike;

			spike = Input[i]->data.spike;
			/* Get pointer to event header for this input */
			/*  Note: this should be just prior to the data
			 *  i.e. edata == spike - sizeof(EVENT_DATA_HEADER)
			 */
			edata = (Use == 0)?Ibuf0[i]:Ibuf1[i];
			spike->CurSpikes = edata->elems;
		}
	}
	for (i = 0; i < Noutput; ++i) {
		Output[i] = &OutArg[i];

		/* Zero count for spike streams */
		if (Output[i]->datatype == EFI_STREAM_SPIKE) {
			Spike_CLASS     *spike;

			spike = Output[i]->data.spike;
			spike->CurSpikes = 0;
		}
	}


	/* Debug: Dump out the first few input elements */
	if (Debug) for (i = 0; i < Ninput; ++i) {
		Stream *stream;

		stream = Input[i];

		printf("%s.advance: input.%d ", MyName, i+1);
		show_desc(stdout, stream);
		printf(": ");
		show_data(stdout, stream, 4);
		printf(" ...\n");
		fflush(stdout);
	}

	/* ADVANCE */
	rc = ModuleAdvance(Input, Output);

	if (Debug) {
		printf("%s.advance: module returned %d at timestep %d\n",
		       MyName, rc, Frame);
		fflush(stdout);fflush(stderr);
	}

	/* Debug: Dump out the first few input elements */
	if (Debug) for (i = 0; i < Noutput; ++i) {
		Stream *stream;

		stream = Output[i];

		printf("%s.advance: output.%d ", MyName, i+1);
		show_desc(stdout, stream);
		printf(": ");
		show_data(stdout, stream, 4);
		printf(" ...\n");
		fflush(stdout);
	}

	/* For Spike outputs use output count from data struct */
	/* N.B. Next version will use output size array instead of rc */

	/**** NEED TO DEAL WITH ERRORS *****/

	/* Switch buffer sets */
	if (Use) In1 = 0;
	else     In0 = 0;

	Use = !Use;

	/*
	 * Clear advance
	 *   NB we can not get a second advance until we post a "ready"
	 *   status event, so this is safe without locking
	 */
	--Advance;

	/* Send back a status message */
	send_status(EFI_STATUS_ADVANCE, (rc>0)?EFI_STATUS_READY:EFI_STATUS_ERROR, Frame);

	if (rc <= 0) return;

	/* THIS WILL CHANGE  IN NEXT VERSION */
	/* N.B. Next version will use output size vector instead of rc */
	nelem = rc;

	/* Deliver output */
	for (i = 0; i < Noutput; ++i) {
		Obuf[i]->frame = Frame;

		/* THIS WILL CHANGE  IN NEXT VERSION */
		/* For Spike outputs use output count from data struct */
		if (Output[i]->datatype == EFI_STREAM_SPIKE)
		     Obuf[i]->elems = Output[i]->data.spike->CurSpikes;
		else Obuf[i]->elems = nelem;

if (Debug) {
   printf("%s.advance: delivering output.%d, tag %d, to %d (%d elems)\n",
	MyName, i+1, Obuf[i]->tag, OutDest[i], Obuf[i]->elems);
	printf("%s.advance: output.%d data ", MyName, i+1);
	show_data(stdout, &OutArg[i], 3);
	printf(" ...\n");
   fflush(stdout);fflush(stderr);
}

		send_data(OutDest[i], Obuf[i]);

		/*
		 * Write data and frame meta data to files
		 */
		if (Dump) {
			dump_meta(Metaf[i], Obuf[i]);
			dump_data(Dumpf[i], Obuf[i]);
		}
	}

	/* Send probe data */
	if (Nprobe) {
		mp_lock(Probelock);
		for (i = 0; i < Nprobe; ++i) {
			int to, what, tag;

			to    = Probe[i].to;
			what  = Probe[i].arg;
			nelem = Obuf[what]->elems;

		   printf("%s.advance: sending output %d (%d elems) to probe client %d\n",
			   MyName, what+1, nelem, to);

			tag = Obuf[what]->tag;
			Obuf[what]->tag = Probe[i].tag;

			send_data(to, Obuf[what]);

			Obuf[what]->tag = tag;
		}
		mp_unlock(Probelock);
	}

	SetState(STATE_READY);

	/* Send a status message */
	send_status(EFI_STATUS_ADVANCE, EFI_STATUS_IDLE, Frame);
}

/*
 * Packetized data and send
 */
INTERNAL void
send_data(int to, EVENT_EFI_DATA *out) {
	int             tag, frame;
	int             nelem;
	int             size, nleft, nsend, elemsize, maxsize, maxsend, index;
	int             type, etype;
	int             sent;           /* number of events sent        */
	int             endian;
	int             hdrsize;        /* EFI_DATA header size (bytes) */
	int             datasize;       /* size of data payload (bytes) */
	EVENT_EFI_DATA *edata;
	BYTE            *data;
	BYTE            stash[sizeof(EVENT_EFI_DATA)];

	if (!to) {
		printf("%s.senddata: Warning no client connected to output %d\n",
		       MyName, tag);
		return;
	}

	tag   = out->tag;
	frame = out->frame;

	nelem = out->elems;
	nleft = nelem;

	/*
	 * Calculate maximum allowable elements to send per DATA event
	 */

	/* Actual data type */
	type = out->type;

	/* Size of EFI DATA event headers */
	hdrsize = sizeof(EVENT_EFI_DATA) - sizeof(edata->data);

	/* Maximum payload size (in bytes) */
	maxsize = MAX_DATASIZE - hdrsize;

	/* Number of events sent (to assure at least 1) */
	sent = 0;

	switch (type) {
	  case EFI_STREAM_WAVEFORM:
		etype = ET_EFI_WAVEDATA;
		elemsize = sizeof(TMatrixN_T);
		break;

	  case EFI_STREAM_SPIKE:
		etype = ET_EFI_SPIKEDATA;
		elemsize = sizeof(TSpike);
		break;

	  case EFI_STREAM_NONE:
		printf("send_data: *INVALID* stream type NONE\n");
		exit(4);
	  default:
		printf("send_data: *INVALID* stream type %d\n", type);
		exit(4);

	}

	/* Maximum elements that can be sent per event */
	maxsend = maxsize / elemsize;

if (Debug)
 printf("Max %s elements (%d bytes) per DATA event = %d\n",
	 StreamTypeName(type), elemsize, maxsend);

	/* Save endian-ness */
	endian = out->endian;

	/* Point to the first data element */
	data  = (BYTE *)out->data;
	index = 0;

	/* Number of events sent (to assure at least 1) */
	sent = 0;

	while (!sent || nleft) {
		/* Increment number of events sent */
		++sent;

		/* Calculate number of elements to send */
		if (nleft > maxsend) nsend = maxsend;
		else                 nsend = nleft;

		/*
		 * Pointer to the beginning of the event buffer
		 * calculated by backing off from the first data
		 * element to be sent on this loop
		 */
		edata = (EVENT_EFI_DATA *)(data - hdrsize);

		/*
		 * If this is not the first transfer, stash the preceding
		 * data that is about to be overwritten as an Event header
		 */
		if (index) memcpy(stash, edata, hdrsize);

		edata->frame  = Frame;
		edata->tag    = tag;
		edata->elems  = nelem;
		edata->index0 = index;
		edata->indexN = index+nsend-1;
		edata->type   = type;
		edata->endian = endian;

		datasize = nsend*elemsize;

		size = hdrsize + datasize;


if (Debug)
   printf("%s.senddata: sending to %d, frame %d, tag %d, %d-%d of %d elems (%d bytes)\n",
	  MyName, to, frame, tag, edata->index0, edata->indexN, nelem, size);

		event_send(to, etype, (EVENT *)edata, size);

		if (index) memcpy(edata, stash, hdrsize);

		index += nsend;
		nleft -= nsend;
		data  += datasize;
	}
}

void
dump_data(FILE *f, EVENT_EFI_DATA *obuf) {
	int elemsize, nbyte;
	int fd;

	switch(obuf->type) {
	  case EFI_STREAM_WAVEFORM:
		elemsize = sizeof(TMatrixN_T);
		break;

	  case EFI_STREAM_SPIKE:
		elemsize = sizeof(TSpike);
		break;

	  case EFI_STREAM_NONE:
		printf("send_data: *INVALID* stream type NONE\n");
		exit(4);
	  default:
		printf("send_data: *INVALID* stream type %d\n", obuf->type);
		exit(4);

	}
	fd = fileno(f);
	nbyte = obuf->elems * elemsize;
	write(fd, obuf->data, nbyte);
}

void
dump_meta(FILE *f, EVENT_EFI_DATA *obuf) {
	fprintf(f, "%d %d\n", obuf->frame, obuf->elems);
}

INTERNAL void
do_ctl(EVENT_EFI_CTL *ectl) {
	switch (ectl->type) {
	  case EFI_CTL_QUERY:
		printf("%s.ctl: from %d, query %d\n",
		       MyName, EventFrom(ectl), ectl->param);

		break;

	  case EFI_CTL_PROBE:
		do_probe(ectl->clientid, ectl->flag, ectl->param, ectl->tag);
		break;

	  case EFI_CTL_STATUS:
		printf("%s.ctl: from %d, status\n",
			MyName, EventFrom(ectl));

		break;

	  default:
		printf("%s.ctl: from %d, unknown type %d, flag %d\n",
			MyName, EventFrom(ectl), ectl->type, ectl->tag);

	}

}

INTERNAL void
show_desc(FILE *f, Stream *stream) {

	switch (stream->datatype) {
	  case EFI_STREAM_WAVEFORM: {
		TMatrixN_CLASS *M;
		ULong          *rank;
		int             i, ndim;

		fprintf(f, "MatrixN [");
		M = stream->data.waveform;
		ndim = M->nDims;
		rank = M->mDimensionRank;

		for (i = 0; i < ndim; ++i)
		   fprintf(f, "%d%s", rank[i], i!=ndim-1?"x":"");

		printf("] (%d %s)", M->nElements, sizeof(TMatrixN_T)==4?"float":"double");
	  } break;

	  case EFI_STREAM_SPIKE: {
		Spike_CLASS *spike;

		spike = stream->data.spike;

		fprintf(f, "Spike %d of %d", spike->CurSpikes, spike->MaxSpikes);
	  } break;

	  case EFI_STREAM_NONE:
		fprintf(f, "NONE");
		break;

	  default:
		fprintf(f, "*INVALID STREAM TYPE %d", stream->datatype);
		break;
	}
}


INTERNAL void
show_data(FILE *f, Stream *stream, int n) {
	int i;

	switch (stream->datatype) {
	  case EFI_STREAM_WAVEFORM: {
		MATRIXN_T *data;

		data = stream->data.waveform->mData;
		for (i = 0; i < n; ++i)
		   fprintf(f, "%f%s", data[i], i!=n-1?" ":"");

	  } break;

	  case EFI_STREAM_SPIKE: {
		TSpike   *data;

		data = stream->data.spike->Spikes;

		for (i = 0; i < n; ++i)
		   fprintf(f, "%d:%f%s", data[i].cell, data[i].time, i!=n-1?" ":"");
	  } break;

	  case EFI_STREAM_NONE:
		fprintf(f, "NONE");
		break;

	  default:
		fprintf(f, "*INVALID STREAM TYPE %d", stream->datatype);
		break;
	}
}

INTERNAL int
do_probe(int to, int enable, int param, int tag) {
	int i;

printf("%s.do_probe: %s output.%d->%d (tag %d)\n",
	MyName, enable?"add":"delete", param, to, tag);

	if (enable) {
		if (Nprobe == MAX_PROBE) {
			printf("do_probe: too many probes!!\n");
			return(0);
		}
		Probe[Nprobe].to  = to;
		Probe[Nprobe].tag = tag;
		Probe[Nprobe].arg = param;
		mp_lock(Probelock);
		++Nprobe;
		mp_unlock(Probelock);

		if (Tunnel) event_tunnel_request(to, tunnel_ready);

		printf("do_probe: added probe %d: output.%d->%d (tag %d)\n",
			Nprobe-1, param, to, tag);
		return(1);
	}

	/* Remove all probes to this client */
	if (param < 0) {
		mp_lock(Probelock);
		i = 0;
		while (i < Nprobe)
		  if (Probe[i].to == to) {
			printf("do_probe: removing probe %d: output.%d->%d (tag %d)\n",
				i, Probe[i].arg, Probe[i].to, Probe[i].tag);
			if (i < Nprobe-1) memcpy(&Probe[i], &Probe[Nprobe-1], sizeof(PROBE));
			--Nprobe;
		  }
		  else ++i;
		mp_unlock(Probelock);
		return(1);
	}

	/* Remove specified probe */
	for (i = 0; i < Nprobe; ++i)
	   if (Probe[i].to == to && Probe[i].arg == param) break;

	/* Didn't find the specified probe */
	if (i == Nprobe) {
		printf("do_probe: couldn't locate probe output.%d->%d for removal\n",
			to, param);
		return(0);
	}

	printf("do_probe: removing probe %d: output.%d->%d (tag %d)\n",
		i, Probe[i].arg, Probe[i].to, Probe[i].tag);

	mp_lock(Probelock);

	if (i < Nprobe-1)
	   memcpy(&Probe[i], &Probe[Nprobe-1], sizeof(PROBE));


	--Nprobe;
	mp_unlock(Probelock);
	return(1);
}


/*
 * This is in newer DAFFIE libs
 *  uncomment only if it comes up unresolved
 */
/*******************************
int event_wait_cancel() { printf("cancel event wait\n"); return(0); }
*******************************/

