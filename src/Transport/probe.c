/*
 * Set a probe
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <local/std.h>

#include <daffie/event.h>
#include <daffie/eclient.h>

#include "efievent.h"
#include "agentd.h"

#include "eventdef.h"

INTERNAL void efi_init(void) {
	/* Define agent events */
	event_init();

#include "efieventdef.h"
#include "agentddef.h"
}

INTERNAL void  usage(char *pgm, int msg);

main(int argc, char *argv[]) {
	char *pgm;
	int   ok, help, debug, verbose;
	char *who, instname[200], *s;
	int   what;
	int   to, tag, enable, eat;
	char *outfile = NULL;
	char *server;
	int   clientid = 0;
	int   id, nc;
	int   i;
	FILE *fout = NULL;
	ECLIENT_LIST  clients[MAX_CLIENT];
	EVENT_EFI_CTL eprobe;

	pgm = argv[0];
	++argv; --argc;

	ok      = 1;
	help    = 0;
	debug   = 0;
	verbose = 0;
	tag     = 0;
	to      = 0;
	eat     = 0;
	enable  = 1;
	while (argc && *argv[0] == '-') {
		char *opt;

		opt = argv[0] + 1;
		++argv; --argc;

		if      (substr(opt, "help"))   ++help;
		else if (substr(opt, "debug"))  ++debug;
		else if (substr(opt, "verbose"))++verbose;
		else if (substr(opt, "on"))     enable = 1;
		else if (substr(opt, "off"))    enable = 0;
		else if (substr(opt, "eat"))    eat = 1;
		else if (substr(opt, "tag")) {
			if (!argc || argv[0][0] == '-') {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					pgm, opt);
				ok = 0;
			}
			else {
				tag = atoi(argv[0]);
				++argv; --argc;
			}
		}
		else if (substr(opt, "output")) {
			if (!argc || argv[0][0] == '-') {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					pgm, opt);
				ok = 0;
			}
			else {
				eat = 1;
				outfile = argv[0];
				++argv; --argc;
			}
		}
		else if (substr(opt, "to")) {
			if (!argc || argv[0][0] == '-') {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					pgm, opt);
				ok = 0;
			}
			else {
				to = atoi(argv[0]);
				++argv; --argc;
			}
		}
		else {
			fprintf(stderr, "%s: ignoring argument -%s\n", pgm, opt);
			help = 1;
			ok   = 0;
		}
	}
	if (!ok || help || argc < 2) {
		usage(pgm, help);
		exit(!help);
	}
	event_verbose(verbose);
	efi_init();

	server = argv[0];
	++argv; --argc;

	who = argv[0];
	++argv; --argc;

	what = 0;
	if (isdigit(*who)) {
		while (isdigit(*who)) clientid = 10*clientid + *who++ - '0';
		sprintf(instname, "client %d", clientid);
	}
	else if (!argc) {
		s = instname;
		while (*who && *who != '.') *s++ = *who++;
		*s = '\0';
	}
	else {
		strcpy(instname, who);
		who = "";
	}

	if (*who == '.') ++who;
	if (*who) while (*who) what = 10*what + *who++ - '0';

	if (!what && !argc) {
		usage(pgm, 0);
		exit(1);
	}
	if (!what) {
		what = atoi(argv[0]);
		++argv; --argc;
	}

	id = event_join(server, &nc);
	if (!id) {
		fprintf(stderr, "%s: couldn't join server \"%s\"\n", server);
		exit(4);
	}
	printf("%s: joined %s as client id %d (%d clients)\n", pgm, server, id, nc);

	event_register("earlab", "control", pgm);

	event_select_type(0, ET_MIN, ET_MAX);
	event_select_type(1, ET_EFI_MIN, ET_EFI_MAX);

	if (eat) to = id;

	eclient_monitor();
	if (!clientid)
	   nc = eclient_match(clients, "*", "earlab", "*", instname);
	else
	   nc = eclient_match_id(clients, clientid);

printf("%s: probing %s.%d\n", pgm, instname, what);

	if (nc < 1) {
		fprintf(stderr, "%s: couldn't find match for %s\n",
			pgm, instname);
		event_leave();
		exit(1);
	}
	if (eat && !tag) tag = getpid();
	eprobe.type     = EFI_CTL_PROBE;
	eprobe.clientid = to;
	eprobe.tag      = tag;
	eprobe.param    = what - 1;
	eprobe.flag     = enable;
	eprobe.value    = 0;

	if (outfile) {
		fout = fopen(outfile, "w");
		if (!fout) {
			fprintf(stderr,
				"%s: couldn't open output file \"%s\"\n",
				pgm, outfile);
			exit(1);
		}
	}

	for (i = 0; i < nc; ++i) {
		printf("%s: sending probe to %s@%s (%d)\n",
			pgm,
			clients[i].species,
			clients[i].host,
			clients[i].id);

		SEND(clients[i].id, EFI_CTL, eprobe);
	}

	if (eat) FOREVER {
		EVENT e;

		event_wait();
		if (!event_receive(&e)) continue;
		if (EventType(e) == ET_EFI_WAVEDATA ||
		    EventType(e) == ET_EFI_SPIKEDATA) {
			EVENT_EFI_DATA *edata;
			int i;

			edata = (EVENT_EFI_DATA *)&e;
			printf("%s: received EFI %d datum, frame %d, tag %d from %d\n",
				pgm, edata->elems, edata->frame, edata->tag,
				EventFrom(e));
			if (fout) for (i = 0; i < edata->elems; ++i)
			   fprintf(fout, "%f\n", edata->data[i]);
		}
		else if (verbose)
		  printf("%s: received %s from %d\n", pgm,
			 event_lookup_name(EventType(e)),
			 EventFrom(e));
	}

	event_drain();
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
	PRT("server",           "event server");
	PRT("module[.arg]",      "module and output argument position");
	PRT("[arg]",             "output argument position");
	printf("\n");
}
