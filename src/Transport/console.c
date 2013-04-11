/*
 * DAFFIE agent console
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <local/std.h>

#include <daffie/event.h>

#include "agentd.h"
#include "eventdef.h"

INTERNAL void usage(char *pgm, int msg);

void agentd_init(void) {
	event_init();
#include "agentddef.h"
}

#define PATHLEN   2500
#define SYS_OK    0
#define MAX_AGENT 100
#define MAX_ARGS  200

typedef struct {
	EVENT_AGENTD_REQ ereq;
	int              pid;
	int              cid;
} AGENT_STAT;

INTERNAL int Verbose = 0;

main(int argc, char *argv[]) {
	char *pgm;
	int   ok, help, debug;
	char *server;
	int   id, nc;
	int   i;
	char  me[256];
	char *log;
	FILE *logf;

	if (pgm = strrchr(argv[0], '/')) ++pgm;
	else pgm = argv[0];
	++argv; --argc;

	ok    = 1;
	help  = 0;
	debug = 0;
	log   = NULL;
	while (argc && *argv[0] == '-') {
		char *opt;

		opt = argv[0] + 1;
		++argv; --argc;

		if      (streq(opt,  "help"))    ++help;
		else if (substr(opt, "debug"))   ++debug;
		else if (substr(opt, "verbose")) ++Verbose;
		else if (substr(opt, "quiet"))   Verbose = 0;
		else if (substr(opt, "log")) {
			if (!argc) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					pgm, opt);
				ok = 0;
			}
			else {
				log = argv[0];
				++argv; --argc;
			}
		}
		else {
			fprintf(stderr, "%s: ignoring argument -%s\n", pgm, opt);
			help = 1;
			ok   = 0;
		}
	}
	if (!ok || argc == 0) {
		usage(pgm, help);
		exit(!help);
	}

	server = argv[0];
	++argv; --argc;

	/* Open log file if desired */
	if (log) {
		logf = fopen(log, "w");
		if (!logf) {
			fprintf(stderr,
				"%s: open failed for writing to log file \"%s\"\n",
				log);
			exit(4);
		}
	}
	else logf = NULL;

	/* Set verbosity */
	event_verbose(Verbose);

	/* local initialization */
	event_tunnel_enable(0);
	agentd_init();

	id = event_join(server, &nc);
	if (!id) {
		fprintf(stderr, "%s: couldn't join server \"%s\"\n", pgm, server);
		exit(4);
	}
	printf("%s: joined %s as client id %d (%d clients)\n", pgm, server, id, nc);

	cuserid(me);

	event_register("agent", "console", pgm);

	event_select_type(0, ET_MIN, ET_MAX);
	event_select_type(1, ET_AGENTD_MIN, ET_AGENTD_MAX);

	FOREVER {
		EVENT e;
		EVENT_AGENTD_LOG *elog;

		event_wait();
		if (!event_receive(&e)) continue;

		switch (e.ev_head.type) {
		  case ET_AGENTD_LOG:
			elog = (EVENT_AGENTD_LOG *)&e;
			printf("%s@%s [%s]: %s",
				elog->program,
				elog->hostname,
				elog->type==AGENTD_LOG_STDERR?"stderr":"stdout",
				elog->msg);
			if (logf)
			 fprintf(logf,
				 "%s@%s [%s]: %s",
				 elog->program,
				 elog->hostname,
				 elog->type==AGENTD_LOG_STDERR?"stderr":"stdout",
				 elog->msg);

			fflush(stdout); if (logf) fflush(logf);
			break;

		  default:
			if (Verbose)
			  printf("Ignoring event \'%s\'(%d) from %d\n",
				  event_lookup_name(e.ev_head.type),
				  e.ev_head.type, e.ev_head.from);
		}
	}
}

#define PRT(opt,str) \
	printf(msg?"\t%s%s%s\n":"%s ", opt, (strlen(opt)<8)?"\t\t":"\t", str)

INTERNAL void
usage(char *pgm, int msg) {
	if (msg) printf("usage: %s [options] server\n", pgm);
	else     printf("usage: %s ", pgm);
	PRT("[-help]",          "display this message");
	PRT("[-debug]",         "turning on debugging mode");
	PRT("[-verbose]",       "enable verbose diagnositics");
	PRT("[-quiet]",         "not verbose");
	PRT("[-log file]",      "send log to file");
	PRT("server",           "event server name");
	if (!msg) printf("\n");
}
