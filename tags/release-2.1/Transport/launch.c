/*
 * DAFFIE sample agent locater
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <local/std.h>

#include <daffie/event.h>
#include <daffie/eclient.h>

#include "agentd.h"
#include "agent.h"

#include "eventdef.h"
INTERNAL void agent_init(void) {
	/* Define agent events */
	event_init();
#include "agentddef.h"
}

#define MAX_AGENT 100

INTERNAL void  usage(char *pgm, int msg);

main(int argc, char *argv[]) {
	char *pgm;
	int   ok, help, debug, verbose, query;
	char *what = "*", whatbuf[256] = { '\0' };
	char *world;
	char *server;
	char *logger;
	char *host;
	int   id, nc;
	int   i, nmod;
	char  argbuf[256];
	MODULE_HOST modlocs[MAX_AGENT];

	pgm = argv[0];
	++argv; --argc;

	ok      = 1;
	help    = 0;
	debug   = 0;
	verbose = 0;
	query   = 0;
	world   = NULL;
	logger  = NULL;
	host    = NULL;
	while (argc && *argv[0] == '-') {
		char *opt;

		opt = argv[0] + 1;
		++argv; --argc;

		if      (streq(opt, "help"))    ++help;
		else if (substr(opt, "debug"))  ++debug;
		else if (substr(opt, "verbose"))++verbose;
		else if (substr(opt, "earlab")) {
			world  = "Earlab*Master";
		}
		else if (substr(opt, "search")) {
			if (!argc || argv[0][0] == '-') {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					pgm, opt);
				ok = 0;
			}
			else {
				world = argv[0];
				++argv; --argc;
			}
		}
		else if (substr(opt, "logserver")) {
			if (!argc || argv[0][0] == '-') {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					pgm, opt);
				ok = 0;
			}
			else {
				logger = argv[0];
				++argv; --argc;
			}
		}
		else if (substr(opt, "module")) {
			if (!argc || argv[0][0] == '-') {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					pgm, opt);
				ok = 0;
			}
			else {
				if (!whatbuf[0]) strcpy(whatbuf, "{");
				else             strcat(whatbuf, ",");
				strcat(whatbuf, argv[0]);
				what = whatbuf;

				++argv; --argc;
			}
		}
		else if (substr(opt, "host")) {
			if (!argc || argv[0][0] == '-') {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					pgm, opt);
				ok = 0;
			}
			else {
				host = argv[0];
				++argv; --argc;
			}
		}
		else {
			fprintf(stderr, "%s: ignoring argument -%s\n", pgm, opt)
;
			help = 1;
			ok   = 0;
		}
	}
	if (!ok || help || (argc == 0 && !world)) {
		usage(pgm, help);
		exit(!help);
	}
	event_verbose(verbose);

	if (world) {
		server = SearchWorld(NULL, world);
		if (!server) {
			fprintf(stderr, "%s: couldn't find earlab master server\n", pgm);
			exit(1);
		}
	}
	else if (argc) {
		server = argv[0];
		++argv; --argc;
	}
	else {
		usage(pgm, help);
		exit(1);
	}

	if (argc) {
		if (!whatbuf[0]) what = argv[0];
		else {
			strcat(whatbuf, ",");
			strcat(whatbuf, argv[0]);
			strcat(whatbuf, "}");
		}
		++argv; --argc;
	}
	else if (!whatbuf[0]) {
		usage(pgm, 0);
		exit(1);
	}

	argbuf[0] = '\0';
	while (argc) {
		strcat(argbuf, argv[0]);
		++argv; --argc;
		if (argc) strcat(argbuf, " ");
	}


printf("Module search is \"%s\"\n", what);

	/* local initialization */
	agent_init();

	id = event_join(server, &nc);
	if (!id) {
		fprintf(stderr, "%s: couldn't join server \"%s\"\n", server);
		exit(4);
	}
	printf("%s: joined %s as client id %d (%d clients)\n", pgm, server, id, nc);

	event_register("earlab", "control", pgm);

	event_select_type(0, 0, ET_MAX);
	event_select_type(1, ET_AGENTD_MIN, ET_AGENTD_MAX);

	nmod = LocateModule(what, modlocs, MAX_AGENT);

	for (i = 0; i < nmod; ++i) {
		printf("%s: module \"%s\" available on host %s\n",
		       pgm, modlocs[i].module, modlocs[i].host);
	}

	if (query) {
		event_leave();
		exit(0);
	}

	if (host) for (i = 0; i < nmod; ++i) {
		 if (strmatch(host, modlocs[i].host)) break;
	}
	else i = 0;

	if (nmod == 0 || i == nmod) {
		printf("%s: module \"%s\" not available on host %s\n",
		       pgm, what, host?host:"*");
		event_leave();
		exit(1);
	}
	StartModule(modlocs[i].clientid, logger,
		    modlocs[i].module, argbuf);

	event_leave();

	exit(0);
}

#define PRT(opt,str) \
	printf(msg?"\t%s%s%s\n":"%s ", opt, (strlen(opt)<8)?"\t\t":"\t", str)

INTERNAL void
usage(char *pgm, int msg) {
	if (msg) printf("usage: %s [options] server agent [agent_args]\n", pgm);
	else     printf("usage: %s ", pgm);
	PRT("[-help]",          "display this message");
	PRT("[-debug]",         "turning on debugging mode");
	PRT("[-module name]",   "agent module name to locate (same as [agent])");
	PRT("[-host name]",     "host on which to run module");
	PRT("[-log logserver]", "eventserver to log to");
	PRT("{-search world|server}", "event server name or search for world");
	PRT("agent",            "agent (executable) to run");
	PRT("[agentargs]",      "args for above");
	printf("\n");
}
