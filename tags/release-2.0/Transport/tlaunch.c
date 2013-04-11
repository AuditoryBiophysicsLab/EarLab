/*
 * DAFFIE sample agent launcher
 */

#include <stdio.h>
#include <unistd.h>
#include <local/std.h>

#include <daffie/event.h>

#include "agentd.h"
#include "eventdef.h"

INTERNAL void usage(char *pgm, int msg);
INTERNAL int oneof(char c, char *s);

INTERNAL void agent_init(void) {
	/* Define agent events */
	event_init();
#include "agentddef.h"
}

#define PATHLEN   2500
#define SYS_OK    0
#define MAX_AGENT 100
#define MAX_ARGS  200

typedef struct {
	EVENT_AGENTD_CTL ectl;
	int              pid;
	int              cid;
} AGENT_STAT;

INTERNAL AGENT_STAT Agent[MAX_AGENT];
INTERNAL int        Nagent;

main(int argc, char *argv[]) {
	char *pgm;
	int   ok, help, debug;
	char *server;
	int   id, nc;
	int   i;
	char  me[256];
	char  *logger = "";
	char  *cmd = "echo";
	char  *query = "*";
	char   cmdargs[1000], *argp;
	EVENT_AGENTD_CTL eagent;

	pgm = argv[0];
	++argv; --argc;

	ok    = 1;
	help  = 0;
	debug = 0;
	while (argc && *argv[0] == '-') {
		char *opt;

		opt = argv[0] + 1;
		++argv; --argc;

		if      (streq(opt, "help"))   ++help;
		else if (substr(opt, "debug")) ++debug;
		else if (substr(opt, "logger")) {
			logger = argv[0];
			++argv; --argc;
		}
		else if (substr(opt, "query")) {
			query = argv[0];
			++argv; --argc;
		}
		else {
			fprintf(stderr, "%s: ignoring argument -%s\n", pgm, opt)
;
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

	if (argc) {
		cmd = argv[0];
		 ++argv; --argc;
	}
	argp = cmdargs;
	*argp = '\0';

	while (argc) {
		char *s;
		*argp++ = ' ';

		s = argv[0];
		while (*s) *argp++ = *s++;

		++argv; --argc;
	}
	*argp++ = '\0';

	/* local initialization */
	agent_init();

	id = event_join(server, &nc);
	if (!id) {
		fprintf(stderr, "%s: couldn't join server \"%s\"\n", pgm, server);
		exit(4);
	}
	printf("%s: joined %s as client id %d (%d clients)\n", pgm, server, id, nc);

	cuserid(me);

	event_register("agent", "launch", pgm);

	event_select_type(0, 0, ET_MAX);
	event_select_type(1, ET_AGENTD_MIN, ET_AGENTD_MAX);

	eagent.type   = AGENTD_CTL_QUERY;
	eagent.flags  = 0;
	eagent.status = 0;
	eagent.auth   = 0;
	eagent.tag    = getpid(); /* identifying tag */
	strcpy(eagent.requester, me);
	strcpy(eagent.log,    "");
	strcpy(eagent.server, "");
	strcpy(eagent.module, query);
	strcpy(eagent.args,   "");
	BCAST(AGENTD_CTL, eagent);

	eagent.type   = AGENTD_CTL_START;
	eagent.flags  = 0;
	eagent.status = 0;
	eagent.auth   = 0;
	eagent.tag    = getpid(); /* identifying tag */
	strcpy(eagent.requester, me);
	strcpy(eagent.log,    logger);
	strcpy(eagent.server, "");
	strcpy(eagent.module, cmd);
	strcpy(eagent.args,   cmdargs);

	printf("Broacasting agent start request \n");
	BCAST(AGENTD_CTL, eagent);

	FOREVER {
		EVENT e;
		EVENT_AGENTD_CTL *estat;

		event_wait();
		if (!event_receive(&e)) continue;
		printf("%s: received %s from %d\n", pgm,
			event_lookup_name(EventType(e)), EventFrom(e));
		if (e.ev_head.type == ET_AGENTD_MOD) {
			EVENT_AGENTD_MOD *emod = (EVENT_AGENTD_MOD *)&e;
			char             *modname;
			int               i, len;

			modname = &emod->modules[0];
			for (i = 0; i < emod->nmodules; ++i) {
				printf("query: module \"%s\" available on client %d\n",
					modname, EventFrom(e));
				modname += strlen(modname)+1;
			}
			continue;
		}

		estat = (EVENT_AGENTD_CTL *)&e;
		if (e.ev_head.type != ET_AGENTD_CTL ||
		    estat->type != AGENTD_CTL_STATUS) continue;

		printf("%s: agentd %d started module as pid %d - %s\n",
			pgm, estat->head.from, estat->result,
			(estat->status>=0&&estat->status<= AGENTD_STATUS_MAX)?
			AgentdErrorMsg[estat->status]:"Bad status code!");
		break;
	}

	event_leave();
}

INTERNAL int
oneof(char c, char *s) {
	while (*s) if (*s++ == c) return(1);
	return(0);
}


#define PRT(opt,str) \
	printf(msg?"\t%s%s%s\n":"%s ", opt, (strlen(opt)<8)?"\t\t":"\t", str)

INTERNAL void
usage(char *pgm, int msg) {
	if (msg) printf("usage: %s [options] server\n", pgm);
	else     printf("usage: %s ", pgm);
	PRT("[-help]",          "display this message");
	PRT("[-debug]",         "turning on debugging mode");
	PRT("[-dir path]",      "agent directory path");
	PRT("server",           "event server name");
	if (!msg) printf("\n");
}


