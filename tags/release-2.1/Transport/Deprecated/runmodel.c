/*
 * Run a model (after all modules have been loaded)
 */

#include <stdio.h>
#include <unistd.h>
#include <local/std.h>

#include <daffie/event.h>
#include <daffie/eventdef.h>
#include <daffie/eclient.h>

#include "efievent.h"
#include "config.h"

#define MODEL 1

#include "tl.h"
#include "cl.h"

/******
#include "model.h"
******/

#define PATHLEN   2500
#define SYS_OK    0
#define MAX_AGENT 100

INTERNAL MODULE_CONFIG Modules[MAX_MODULES];
INTERNAL int           Nmodules;
INTERNAL MODULE_WIRING Wiring[MAX_MODULES];

INTERNAL void  usage(char *pgm, int msg);
INTERNAL int   oneof(char c, char *s);
INTERNAL void  make_path(char *path, char *dir, char *file);

EXTERNAL char *cuserid(char *buf);

INTERNAL void efi_init(void) {
	/* Define agent events */
	event_init();
#include "efieventdef.h"
#include "agentddef.h"
}

INTERNAL void define_model(int nmodules, MODULE_CONFIG *modules);

main(int argc, char *argv[]) {
	char *pgm;
	int   ok, help, debug;
	char *expserv;
	char *logs;
	char *outdir, outfile[PATHLEN];
	char *expname;
	char *config;
	char *fsdir  = NULL;
	int   id, nc;

	if (pgm = strchr(argv[0], '/')) ++pgm;
	else pgm = argv[0];
	++argv; --argc;

	ok      = 1;
	help    = 0;
	debug   = 0;
	outdir  = ".";
	logs    = NULL;
	while (argc && *argv[0] == '-') {
		char *opt;

		opt = argv[0] + 1;
		++argv; --argc;

		if      (streq(opt, "help"))   ++help;
		else if (substr(opt, "debug")) ++debug;
		else if (substr(opt, "fileshare") || streq(opt, "fs")) {
			fsdir = argv[0];
			++argv; --argc;
		}
		else if (substr(opt, "logserver")) {
			logs = argv[0];
			++argv; --argc;
		}
		else if (substr(opt, "output")) {
			outdir = argv[0];
			++argv; --argc;
		}
		else {
			fprintf(stderr,
				"%s: unrecognized argument -%s\n",
				pgm, opt);
			help = 1;
			ok   = 0;
		}
	}
	if (!ok || argc < 3) {
		usage(pgm, help);
		exit(!help);
	}

	/* Get experiment server host */
	expserv =  argv[0];
	++argv; --argc;

	/* Get configuration file */
	config = argv[0];
	++argv; --argc;

	/* Startup quietly */
	event_verbose(0);

	/* local initialization */
	agent_init();

	/* Read the configuration file */
	Nmodules = read_config(config, Modules);

	if (Nmodules < 0) {
		fprintf(stderr,
			"%s: error reading configuration file \"%s\"\n",
			pgm, config);
		exit(4);
	}

	if (Nmodules == 0) {
		fprintf(stderr,
			"%s: no modules found in configuration file \"%s\"\n",
			pgm, config);
		exit(4);
	}

	/* Construct module wiring for this experiment */
	if (!wire_config(Nmodules, Modules, Wiring)) {
		fprintf(stderr,
			"%s: error encountered while wiring modules in configuration file \"%s\"\n",
			pgm, config);
		exit(4);
	}

	id = event_join(expserv, &nc);
	if (!id) {
		fprintf(stderr, "%s: couldn't join server \"%s\"\n", pgm, expserv);
		exit(4);
	}
	printf("%s: joined %s as client id %d (%d clients)\n", pgm, expserv, id, nc);

	event_register("earlab", "experiment", expname);

	event_select_type(0, 0, ET_MAX);
      /***
	event_select_types(1, 3, ET_JOIN, ET_LEAVE, ET_IDENTITY);
      ***/
	event_select_type(1, ET_EFI_MIN, ET_EFI_MAX);

	eclient_monitor();

	/*
	 * Set static module properties
	 */
	 define_model(Nmodules, Modules);


	event_leave();

	exit(0);
}


INTERNAL void
define_model(int nmodules, MODULE_CONFIG *modules) {
	int i, j;
	int intype[MAX_ARGS], outtype[MAX_ARGS];
	int nin, nout, ndim;

	for (i = 0; i < nmodules; ++i) {
		nin  = modules[i].nin;
		nout = modules[i].nout;

		/*
		 * Create input and output dimension descripitors
		 * Fake this for now
		 */
		for (j = 0; j < nin; ++j)
		   intype[j] = modules[i].intype[j][0];

		for (j = 0; j < nout; ++j)
		   outtype[j] = modules[i].outtype[j][0];

		define_module(modules[i].instname,
			      modules[i].modname,
			      nin,  intype,
			      nout, outtype);
	}
}

EXPORT void
init_model(int nmodules, MODULE_CONFIG *modules, MODULE_WIRING *wiring) {
	int i, j;
	int id;

	for (i = 0; i < nmodules; ++i) {
		id = lookup_module(modules[i].instname);
	    /*******
		for (j = 0; j < modules[i].nin; ++j)
		    config_module_input(id, j+1,
					modules[i].inrank[j][0],
					modules[i].inrank[j][1],
					modules[i].inrank[j][2],
					modules[i].inrank[j][3]
				       );
	    *******/

		for (j = 0; j < modules[i].nout; ++j)
		    config_module_output(id, j+1,
					modules[i].outrank[j][0],
					modules[i].outrank[j][1],
					modules[i].outrank[j][2],
					modules[i].outrank[j][3]
				       );
	}
}

INTERNAL void
make_path(char *path, char *dir, char *file) {
	int n;

	strcpy(path, dir);

	n = strlen(path);
	if (!n || path[n-1] != '/') path[n++] = '/';
	strcpy(path+n, file);
}

INTERNAL int
gen_tag(void) {
	static int tag = 0, pid = 0;

	if (!pid) pid = getpid();
	return(pid + tag++);
}

INTERNAL int
oneof(char c, char *s) {
	while (*s) if (*s++ == c) return(1);
	return(0);
}


INTERNAL char *
SearchWorld(char *meta, char *name) {
	EVENT          event;
	EVENT_IDENTIFY eid;
	EVENT_IDENTITY *e;
	char msgbuf[200], *str;
	int  id, nc;
	int  i, subindx, n, menuid;
	int  response[MAX_CLIENT], nresponse = 0;
	unsigned long starttime, elapsed;

	id = event_join(meta, &nc);
	if (id < 1) {
		fprintf(stderr,
			"SearchWorld: couldn't connect to meta server at %s\n",
			meta);
		return (NULL);
	}
	event_select_type(0, 0, ET_MAX);
	event_register("agentd", "worldquery", name);

	printf("SearchWorld: connected to Metaworld at %s\n", meta);

	eid.tag = 0;
	BCAST(IDENTIFY, eid);

	starttime = event_clock();
	while (nc > nresponse) {

		/* Keep an eye on how long we have been waiting */
		elapsed = (event_clock() - starttime) / 1000;
		if (elapsed > 5) {
			printf("SearchWorld: Looking for %d more ID responses of %d\n",
				nc-nresponse, nc);
			if (elapsed > 120) {
				printf("SearchWorld: Timeout after %d\n", elapsed);
				break;
			}
		}

		event_wait();
		if (!event_receive(&event)) continue;
		switch (event.ev_head.type) {
		  case ET_IDENTITY:
		       if (elapsed > 5)
			    printf("Got Identity from %d: %s %s %s (%d of %d)\n",
				    event.ev_head.from,
				    event.ev_identity.class,
				    event.ev_identity.species,
				    event.ev_identity.instance,
				    nresponse, nc);

			/* Check for duplicate responses */
			for (i = 0; i < nresponse; ++i)
			   if (event.ev_head.from == response[i]) break;
			if (i < nresponse) break;

			response[nresponse++] = event.ev_head.from;

			e = &event.ev_identity;

			/* Look for event servers only */
			if (!streq(e->class, "eventd") ||
			    !strmatch(name, e->instance)) break;

			event_leave();

			str = (char *)malloc(strlen(e->species)+1);
			if (!str) {
				fprintf(stderr, "Couldn't allocate string!!\n");
				return(0);
			}
			strcpy(str, e->species);
			return(str);

		  case ET_JOIN:
			if (event.ev_join.id == id) break;
			if (elapsed > 5)
			   printf("Client %d joined\n", event.ev_join.id);
			for (i = 0; i < nresponse; ++i)
			   if (event.ev_join.id == response[i]) break;

			if (i < nresponse) {
				/* This a duplicate message */
				break;
			}
			++nc;
			break;


		  case ET_LEAVE:
			if (elapsed > 5)
			   printf("Client %d left\n", event.ev_leave.id);
			for (i = 0; i < nresponse; ++i)
			    if (event.ev_leave.id == response[i]) break;

			if (i == nresponse) {
				/* This client left before responding */
				--nc;
			}
			break;

		  default:
			break;
		}
	}

	event_leave();

	printf("SearchWorld: couldn't locate %s\n", name);

	return(NULL);
}

INTERNAL int
StartModule(int clientid, char *logger, char *module, char *args) {
	EVENT_AGENTD_REQ eagent;
	char             me[200];
	int              exectag;

	cuserid(me);
	/* Construct and send a start request */

	memset(&eagent, 0, sizeof(eagent));
	eagent.type   = AGENTD_CTL_START;
	eagent.flags  = 0;
	eagent.auth   = 0;
	eagent.tag    = exectag = gen_tag();
	strcpy(eagent.requester, me);
	strcpy(eagent.log,       logger?logger:"");
	strcpy(eagent.server,    "");
	strcpy(eagent.module,    module);
	strcpy(eagent.args,      args);

	SEND(clientid, AGENTD_REQ, eagent);

	/* Wait for an acknowledgement */
	FOREVER {
		EVENT       e;

		event_wait();
		if (!event_receive(&e)) continue;

		if (EventType(e) == ET_AGENTD_STAT) {
			EVENT_AGENTD_STAT *estat;

			estat = (EVENT_AGENTD_STAT *)&e;
		     /*****
			if (estat->type != AGENTD_CTL_STATUS) break;
		      *****/

			/* Check to see if this is from the start request */
			if (EventFrom(e) == clientid && estat->tag == exectag) {
			     printf("%s: agentd %d started module as pid %d - %s\n",
				     "StartModule",
				     estat->head.from, estat->result,
				     (estat->rstatus>=0 && estat->rstatus<=AGENTD_STATUS_MAX)?
				     AgentdErrorMsg[estat->rstatus]:"Bad status code!");

				return(estat->rstatus);
			}

		  }
	}
}
#define PRT(opt,str) \
	printf(msg?"\t%s%s%s\n":"%s ", opt, (strlen(opt)<8)?"\t\t":"\t", str)

INTERNAL void
usage(char *pgm, int msg) {
	if (msg) printf("usage: %s [options] server host command\n", pgm);
	else     printf("usage: %s ", pgm);
	PRT("[-help]",          "display this message");
	PRT("[-debug]",         "turning on debugging mode");
	PRT("[-noconsole]",     "don't start a console");
	PRT("[-console]",       "start a console [default]");
	PRT("[-output dir]",    "specify output directory");
	PRT("[-fileshare dir]", "start fileserver for specified directory");
	PRT("[-modules dir]",   "specify module directory");
	PRT("[-logserver host]","specify (event) server host for logging");
	PRT("[-port n]",        "specify experiment server port");
	PRT("expserver",        "experiment server name or \".\" to start locally]");
	PRT("expname",          "exeperiment name)");
	PRT("earlabserver",     "earlab world server or -private or -search");
	PRT("config",           "configuration file");

	if (!msg) printf("\n");
}

