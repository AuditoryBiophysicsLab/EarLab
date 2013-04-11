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
#define MAX_MODS  100

INTERNAL void  usage(char *pgm, int msg);

INTERNAL MODULE_HOST modulelocs[MAX_AGENT*MAX_MODS];

main(int argc, char *argv[]) {
	char *pgm;
	int   ok, help, debug, verbose;
	char *what = "*", whatbuf[256] = { '\0' };
	char *world;
	char *server;
	int   id, nc;
	int   i, nmod;

	pgm = argv[0];
	++argv; --argc;

	ok      = 1;
	help    = 0;
	debug   = 0;
	verbose = 0;
	world   = NULL;
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
		else {
			fprintf(stderr, "%s: ignoring argument -%s\n", pgm, opt)
;
			help = 1;
			ok   = 0;
		}
	}
	if (!ok || (argc == 0 && !world) || (argc > 0 && world)) {
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
	else {
		server = argv[0];
		++argv; --argc;
	}

	if (argc) {
		if (argc == 1 && !whatbuf[0]) what = argv[0];
		else while (argc) {
			if (!whatbuf[0]) strcat(whatbuf, "{");
			else             strcat(whatbuf, ",");
			strcat(whatbuf, argv[0]);
			++argv; --argc;
		}
	}

	if (whatbuf[0]) {
		strcat(whatbuf, "}");
		what = whatbuf;
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

	event_select_type(0, ET_MIN, ET_MAX);
	event_select_type(1, ET_AGENTD_MIN, ET_AGENTD_MAX);

	nmod = LocateModule(what, modulelocs, MAX_AGENT);

	for (i = 0; i < nmod; ++i) {
		printf("%s: module \"%s\" available on host %s\n",
		       pgm, modulelocs[i].module, modulelocs[i].host);
	}

	event_leave();

	exit(0);
}

#define PRT(opt,str) \
	printf(msg?"\t%s%s%s\n":"%s ", opt, (strlen(opt)<8)?"\t\t":"\t", str)

INTERNAL void
usage(char *pgm, int msg) {
	if (msg) printf("usage: %s [options] server\n", pgm);
	else     printf("usage: %s ", pgm);
	PRT("[-help]",          "display this message");
	PRT("[-debug]",         "turning on debugging mode");
	PRT("[-module name]",   "agent module name to locate (same as [agent])");
	PRT("{-search world|server}", "event server name or search for world");
	PRT("[agent]",          "agent (executable) name [default=*]");
}
