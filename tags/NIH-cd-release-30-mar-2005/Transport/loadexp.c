/*
 * Start up an distributed experiment
 *   - start experiment event server, consoles, fileservers etc
 *   - locate Earlab agentd (agent initiators) processes (one per host)
 *   - read configuration file
 *   - load modules
 */

#include <stdio.h>
#include <unistd.h>
#include <local/std.h>

#include <daffie/event.h>
#include <daffie/eclient.h>

#include "agentd.h"
#include "eventdef.h"
#include "config.h"

#define PATHLEN   2500
#define SYS_OK    0
#define MAX_AGENT 100

#define METASERVER "scv.bu.edu"
#define EARLAB     "Earlab Master"

#define EVENTD  "eventd"
#define CONSOLE "console"
#define LAUNCH  "launch"
#define AGENTD  "agentd"
#define FSERVER "fserver"

#define AGENTD_CLASS   "daemon"
#define AGENTD_SPECIES "agentd"

/*
 * Descriptor for runing Module-Host name pair
 */
typedef struct {
	int     clientid;                       /* DAFFIE Client ID         */
	int     clientip;                       /* Host IP address          */
	char    host[AGENTD_NAMELEN];           /* Host name                */
	char    module[AGENTD_PATHLEN];         /* Executable name          */
} MODULE_HOST;

/*
 * List of all running Module-Host pairs possibilities for simulation module
 */
typedef struct {
	int         nexec;                      /* Number of executions ??? */
	char        modname[AGENTD_PATHLEN];    /* Module pattern name      */
	int         nhost;                      /* Count of mod-host pairs  */
	MODULE_HOST host[1];                    /* List of mod-host pairs   */
} MODULE_HOSTLIST;

INTERNAL MODULE_HOSTLIST *ModList[MAX_MODULES]; /* Runtime modules-hosts    */
INTERNAL int              NModList = 0;

INTERNAL MODULE_CONFIG    Modules[MAX_MODULES]; /* Static Model description */
INTERNAL int              Nmodules;

INTERNAL int              Debug   = 0;
INTERNAL int              ModDebug   = 0;
INTERNAL int              ModVerbose = 0;
INTERNAL int              ModFS      = 0;
INTERNAL int              ModTrans   = 0;
INTERNAL int              Nagent;

INTERNAL char *SearchWorld(char *meta, char *name);
INTERNAL int   LocateModule(char *modname, MODULE_HOST *modlist, int maxmod);
INTERNAL int   StartModule(int clientid, char *logger, char *module, char *args);
INTERNAL void  usage(char *pgm, int msg);
INTERNAL int   oneof(char c, char *s);
INTERNAL void  make_path(char *path, char *dir, char *file);
INTERNAL int   gen_tag(void);

EXTERNAL char *cuserid(char *buf);

INTERNAL void agent_init(void) {
	/* Define agent events */
	event_init();
#include "agentddef.h"
}

main(int argc, char *argv[]) {
	char *pgm;
	int   ok, help;
	char *univs;
	char *exps, exphost[200], expdaff[200];
	char *logs;
	char *outdir, outfile[PATHLEN];
	int   port;
	int   tunnel;
	int   dump;
	char *expname;
	char *config;
	char *moddir = NULL;
	char *fsdir  = NULL;
	char  me[200];
	char  cmd[PATHLEN];
	char  logopts[PATHLEN];
	char *s;
	int   console;
	int   id, nc;
	int   i, nmod;
	MODULE_HOST modlocs[MAX_AGENT];

	if (pgm = strchr(argv[0], '/')) ++pgm;
	else pgm = argv[0];
	++argv; --argc;

	ok      = 1;
	help    = 0;
	Debug   = 0;
	port    = 0;
	console = 1;
	tunnel  = 0;
	dump    = 0;
	outdir  = ".";
	logs    = NULL;
	while (argc && *argv[0] == '-') {
		char *opt;

		opt = argv[0] + 1;
		++argv; --argc;

		if      (streq(opt, "help"))       ++help;
		else if (substr(opt, "debug"))     ++Debug;
		else if (substr(opt, "moddebug"))  ++ModDebug;
		else if (substr(opt, "modverbose"))++ModVerbose;
		else if (substr(opt, "modtransport")) ++ModTrans;
		else if (substr(opt, "modfs"))     ++ModFS;
		else if (substr(opt, "console"))   console = 1;
		else if (substr(opt, "noconsole")) console = 0;
		else if (substr(opt, "tunnel"))    tunnel = 1;
		else if (substr(opt, "notunnel"))  tunnel = 0;
		else if (substr(opt, "dump"))      dump = 1;
		else if (substr(opt, "nodump"))    dump = 0;
		else if (substr(opt, "fileshare") || streq(opt, "fs")) {
			fsdir = argv[0];
			++argv; --argc;
		}
		else if (substr(opt, "modules")) {
			moddir = argv[0];
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
		else if (substr(opt, "port")) {
			port = atoi(argv[0]);
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
	if (!ok || argc < 4 || help) {
		usage(pgm, help);
		exit(!help);
	}

	/* Get experiment server host */
	exps =  argv[0];
	++argv; --argc;

	/* Construct both hostname and hostname:port variants */
	if (s = strchr(exps, ':')) {
		int len;

		if (port && atoi(s+1) != port) {
			fprintf(stderr,
				"%s: -port %d option conflicts with host specification %s\n",
				pgm, port, exps);
			exit(4);
		}
		len = s - exps;
		memcpy(exphost, exps, len);
		exphost[len] = '\0';
		exps = exphost;

		port = atoi(s+1);
		sprintf(expdaff, "%s:%d", exps, port);
	}
	else if (port) sprintf(expdaff, "%s:%d", exps, port);
	else strcpy(expdaff, exps);

	/* Get experiment name */
	expname =  argv[0];
	++argv; --argc;

	/* Get universe server */
	univs = argv[0];
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
		config_error(stderr, pgm);
		exit(4);
	}

	if (Nmodules == 0) {
		fprintf(stderr,
			"%s: no modules found in configuration file \"%s\"\n",
			pgm, config);
		exit(4);
	}

	print_config(stdout, Modules, Nmodules);

	/* Search for Earlab master server if "-search" specified */
	if (streq(univs, "-search"))
	  if(!(univs = SearchWorld(METASERVER, EARLAB))) {
		fprintf(stderr,
			"%s: couldn't locate master server \"%s\" on metaserver \"%s\"\n",
			pgm, EARLAB, METASERVER);
		exit(4);
	  }
	  else printf("%s: Found %s on host %s\n", pgm, EARLAB, univs);

	/* Start local server and ancillary clients of host specified as "." */
	if (streq(exps, ".")) {
		char  portopts[30];
		char  uniopts[200];
		char  hostname[200];
		char *s;

		if (substr(univs, "-private")) strcpy(uniopts, "-private");
		else sprintf(uniopts, "-join %s", univs);

		event_clientname(hostname);
		if (!strmatch("*.*.*", hostname)) event_clientip(hostname);

		if (port) {
			  sprintf(portopts, "-port %d", port);
			  sprintf(expdaff, "%s:%d", hostname, port);
		}
		else {
			  portopts[0] = '\0';
			  strcpy(expdaff, hostname);
		}
		strcpy(exphost, "localhost");
		exps = exphost;

		make_path(outfile, outdir, "eventd");
		sprintf(logopts, "-log %s-%s.log", outfile, expname);

		sprintf(cmd, "%s -world \"%s\" -runtime 60 %s %s %s &",
			EVENTD,
			expname,
			logopts,
			uniopts,
			portopts);

		printf("%s: starting event server for experiment %s '%s'\n",
		       pgm, expname, cmd);
		system(cmd);

		/* Wait for server to startup */
		FOREVER {
			sleep(2);
			id = event_join(expdaff, &nc);
			if (id > 0) {
				printf("%s: started server on %s\n", expdaff);
				event_leave();
				break;
			}
		}

		if (substr(univs, "-private")) univs = expdaff;
	}

	/* Start up an agentd if specified */
	if (moddir) {
		sprintf(cmd,
			"%s -dir %s %s >\"agentd-%s.log\" 2>1 &",
			AGENTD, moddir?moddir:".", univs, expname);
		printf("%s: starting agentd: '%s'\n", pgm, cmd);
		system(cmd);
		sleep(5);
	}

	/* Start up a fileserver if specified */
	if (fsdir) {
		sprintf(cmd,
			"%s -dir %s %s >\"fserver-%s.log\" 2>1 &",
			FSERVER, fsdir, expdaff, expname);
		printf("%s: starting file server: '%s'\n", pgm, cmd);
		system(cmd);
		sleep(5);
	}

	/* Start up a console */
	/** for now hardcode for an xterm **/
	if (console) {
		make_path(outfile, outdir, "console");
		sprintf(logopts, "-log \"%s-%s.log\"", outfile, expname);
		sprintf(cmd, "xterm -geom 120x32 -rightbar -T \"Console: %s\" -n \"%s\" -exec %s %s %s &",
			expname, expname, CONSOLE, logopts,
			logs?logs:expdaff);
		printf("%s: starting console: '%s'\n", pgm, cmd);
		system(cmd);
	}

	/* Join the Earlab master server */
	id = event_join(univs, &nc);
	if (!id) {
		fprintf(stderr, "%s: couldn't join server \"%s\"\n", pgm, univs);
		exit(4);
	}
	printf("%s: joined %s as client id %d (%d clients)\n", pgm, univs, id, nc);

	/*
	 * Register and subscribe to events of interest
	 */

	cuserid(me);

	event_register("earlab", "experiment", expname);

	event_select_type(0, 0, ET_MAX);
      /***
	event_select_types(1, 3, ET_JOIN, ET_LEAVE, ET_IDENTITY);
      ***/
	event_select_type(1, ET_AGENTD_MIN, ET_AGENTD_MAX);

	eclient_monitor();

	/*
	 * Locate all possible hosts of each module in the Model configuration
	 * NB. Module names and PreferredHosts may be wildcarded, so we
	 * do pattern matching on both strings.
	 *
	 * At the end of this loop, we will have found all the modules required
	 * for the model/experiment, as specified in its configuration file,
	 * well have been located and we will have a list of matching
	 * host-executables pairs in ModList. If any modules are not find we
	 * fail and exit.
	 */
	for (i = 0; i < Nmodules; ++i) {
		int j;
		char mstr[AGENTD_PATHLEN];
		int  bytes;
		MODULE_HOSTLIST *mhl;

		/* See if we already looked up this module */
		for (j = 0; j < NModList; ++j)
		  if (streq(ModList[j]->modname, Modules[i].modname)) break;

		if (j < NModList) continue; /* we found it */

		/* Build a match string */
		sprintf(mstr, "{%s,%s.exe}", Modules[i].modname, Modules[i].modname);

		/* Search for it based on pattern string - put results in tmp */
		nmod = LocateModule(mstr, modlocs, MAX_AGENT);

		/* Fail if we didn't find it */
		if (nmod == 0) {
			fprintf(stderr,
				"%s: Module \"%s\" not available (as %s)\n",
				pgm, Modules[i].modname, mstr);
			exit(4);
		}

		/* Print out list of available hosts-executables */
		printf("%s: module %s available on %d host%s:",
		       pgm, Modules[i].modname, nmod, nmod!=1?"s":"");
		for (j = 0; j < nmod; ++j) printf(" %s", modlocs[j].host);
		printf("\n");

		/* Copy the host locations into our global module-host array */
		bytes = sizeof(MODULE_HOSTLIST) + (nmod-1)*sizeof(MODULE_HOST);
		mhl = (MODULE_HOSTLIST *)malloc(bytes);
		if (!mhl) {
			fprintf(stderr,
				"%s: ran out of memory after %d modules (needed %d bytes)!!\n",
				pgm, NModList, bytes);
			exit(4);
		}
		mhl->nexec = 0;
		mhl->nhost = nmod;

		strcpy(mhl->modname, Modules[i].modname);

		for (j = 0; j < nmod; ++j)
		  memcpy(&mhl->host[j], &modlocs[j], sizeof(MODULE_HOST));

		ModList[NModList++] = mhl;

	}

	/* Launch all the modules needed for the experiment */
	for (i = 0; i < Nmodules; ++i) {
		int          j, k;
		static int   count = 0;
		char         args[AGENTD_PATHLEN];
		char         modopts[300];
		MODULE_HOST *mh;

		/* Find this module name in our host-executable list */
		for (j = 0; j < NModList; ++j)
		  if (streq(ModList[j]->modname, Modules[i].modname)) break;

		if (j == NModList) {
			/* THIS PARTICULAR SHIT SHOULD NOT HAVE HAPPENED!! */
			fprintf(stderr,
				"%s: couldn't locate module %s!!\n",
				pgm, Modules[i].modname);
			exit(4);
		}

		/*
		 * Start the module up on a host
		 */

		/* Check for preferred hosts */
		k = ModList[j]->nhost;
		if (Modules[i].host[0] && !streq(Modules[i].host, "*")) {
			printf("Looking for preferred host %s\n", Modules[i].host);
			for (k = 0; k < ModList[j]->nhost; ++k)
			  if (strmatch(Modules[i].host, ModList[j]->host[k].host))
			     break;
		}
		/* If we have not found a PreferredHost simply round-robin */
		if (k == ModList[j]->nhost) k  = count++ % ModList[j]->nhost;

		mh = &ModList[j]->host[k];

		/* Build the command string */
		modopts[0] = '\0';
		strcat(modopts, tunnel?"-tunnel":"-notunnel");
		if (ModVerbose) strcat(modopts, " -verbose");
		if (dump)       strcat(modopts, " -dump");
		for (k = 0; k < ModDebug; ++k) strcat(modopts, " -debug");
		for (k = 0; k < ModTrans; ++k) strcat(modopts, " -Debug");
		for (k = 0; k < ModFS; ++k)    strcat(modopts, " -fsdebug");

		sprintf(args, "-inst %s -exp %s %s %s",
			Modules[i].instname,
			expname,
			modopts,
			expdaff);

		printf("%s: starting module %s on host %s(%d) '%s %s'\n",
			pgm, Modules[i].instname,
			mh->host, mh->clientid,
			mh->module, args);

		StartModule(mh->clientid, logs?logs:expdaff,
			    mh->module, args);

	}

	event_leave();

	exit(0);
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
LocateModule(char *modname, MODULE_HOST *modlist, int maxmod) {
	ECLIENT_LIST     agents[MAX_AGENT];
	EVENT_AGENTD_REQ equery;
	static int times = 0;
	int   nmod;
	int   nagent;
	int   ndone, done[MAX_AGENT];
	int   tag;
	int   i;

	if (times == 0) eclient_monitor();

	++times;
	tag = getpid()<<16 | times;

	event_select_type(1, ET_LEAVE, ET_LEAVE);

	/* Locate all agents */
	nagent = eclient_match(agents, "*", "daemon", "agentd", "*", "*");

	/* Now send a module query to each one */
	equery.type     = AGENTD_CTL_QUERY;
	equery.flags    = 0;
	equery.auth     = 0;
	equery.authtype = 0;
	equery.tag      = tag;           /* identifying tag */
	strcpy(equery.requester, cuserid(NULL));
	strcpy(equery.log,    "");
	strcpy(equery.server, "");
	strcpy(equery.module, modname);
	strcpy(equery.args,   "");

	if (!nagent) {
		fprintf(stderr, "LocateModule: no agentd servers found!\n");
		return(0);
	}

	for (i = 0; i < nagent; ++i) {
		if (Debug)
		   printf("Sending query for \"%s\" to %s (%d)\n",
			   modname, agents[i].host, agents[i].id);
		SEND(agents[i].id, AGENTD_REQ, equery);
	}

	ndone = nmod = 0;
	while (ndone < nagent) {
		EVENT e;

		event_wait();
		if (!event_receive(&e)) continue;

		if (Debug)
		   printf("query: received %s from %d\n",
			   event_lookup_name(EventType(e)), EventFrom(e));

		/* Check for agentds exiting */
		if (e.ev_head.type == ET_LEAVE) {
			for (i = 0; i < nagent; ++i)
			  if (agents[i].id == e.ev_leave.id) {
				int j;

				for (j = 0; j < ndone; ++j)
				  if (done[j] == e.ev_leave.id) break;

				if (j == ndone) { /* This agent did not complete */
					nagent--;
					memcpy(&agents[i], &agents[--nagent], sizeof(ECLIENT_LIST));
				}

				/* Remove all listings for this agentd */
				for (j = 0; j < nmod; )
				  if (modlist[j].clientid == e.ev_leave.id)
				     memcpy(&modlist[j], &modlist[--nmod], sizeof(MODULE_HOST));
				  else ++j;
			  }
			  continue;
		}

		if (e.ev_head.type == ET_AGENTD_MOD) {
			EVENT_AGENTD_MOD *emod = (EVENT_AGENTD_MOD *)&e;
			char             *avail;
			int               i, len;

			avail = &emod->modules[0];
			for (i = 0; i < emod->nmodules; ++i) {
			    /*********
				printf("query: module \"%s\" available on client %d\n",
					avail, EventFrom(e));
			     *********/
				if (strmatch(modname, avail)) {
					ECLIENT_LIST eclient;

					strcpy(modlist[nmod].module, avail);
					modlist[nmod].clientid = EventFrom(e);

					if (eclient_match_id(&eclient, EventFrom(e))) {
						strcpy(modlist[nmod].host, eclient.host);
					}
					else {
						strcpy(modlist[nmod].host, "unknown");
					}

					++nmod;
				}
				avail += strlen(avail)+1;
			}
			continue;
		}

		if (e.ev_head.type == ET_AGENTD_STAT) {
			EVENT_AGENTD_STAT *estat;

			estat = (EVENT_AGENTD_STAT *)&e;

			if (estat->reqtype != AGENTD_CTL_QUERY) continue;

			done[ndone++] = EventFrom(e);
		     /***
			printf("query: client %d completed\n", EventFrom(e));
		      ***/
		}
	}

	return(nmod);
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

printf("StartModule: %s %s (logger %s, for %s)\n",
	eagent.module, eagent.args, eagent.log, eagent.requester);

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

/*
 * Usage message
 */
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
	PRT("[-tunnel]",        "use tunnels for data transfer");
	PRT("[-notunnel]",      "don't tunnels for data transfer");
	PRT("[-output dir]",    "specify output directory");
	PRT("[-fileshare dir]", "start fileserver for specified directory");
	PRT("[-modules dir]",   "specify module directory");
	PRT("[-logserver host]","specify (event) server host for logging");
	PRT("[-port n]",        "specify experiment server port");
	PRT("expserver",        "experiment server name or \".\" to start locally]");
	PRT("expname",          "experiment name");
	PRT("earlabserver",     "earlab world server or -private or -search");
	PRT("config",           "configuration file");

	if (!msg) printf("\n");
}

