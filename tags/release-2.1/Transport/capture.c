#include <stdio.h>
#include <local/std.h>

#include <daffie/event.h>

INTERNAL void   usage(char *pgm, int msg);
INTERNAL void   setprobe(char *spec);

INTERNAL char  *Pgm;
INTERNAL char  *File;
INTERNAL FILE  *Fout;
INTERNAL int    Done = 0;

main(int argc, char *argv[]) {
	char     *probe;                        /* data file name       */
	int       ok, help, chann, nchan;       /* cmd line options     */
	int       nitem;                        /* total data, items per channel */

	Pgm = argv[0];

	++argv; --argc;

	help  = 0;
	ok    = 1;
	probe   = 0;
	while (argc && *argv[0] == '-') {
		char *opt;

		opt = argv[0]+1;
		++argv; --argc;

		if (substr(opt, "help")) ++help;
		else {
			 fprintf(stderr, "%s: ignoring argument -%s\n", Pgm, opt);
			 ok = 0;
		}
	}

	if (!ok || help || argc < 2) {
		usage(Pgm, help);
		exit(!help);
	}

	probe = argv[0];
	++argv; --argc;

	File = argv[0];
	++argv; --argc;

	Fout = fopen(File, "w");
	if (!Fout) {
		fprintf(stderr, "%s: can't open output file %s\n",
			Pgm, File);
		exit(4);
	}

	setprobe(probe);

	while (!Done) {
		sleep(1);
	}
	fclose(Fout);
}

#include <daffie/event.h>
#include <daffie/eventdef.h>
#include <daffie/eclient.h>
#include "efievent.h"

INTERNAL void
probe_error(s) {
	fprintf(stderr, "%s: invalid probe \"%s\"\n", Pgm, s);
	exit(4);
}

INTERNAL void
efi_init(void) {
	event_verbose(0);
	event_init();
#include "agentddef.h"
#include "efieventdef.h"
}

INTERNAL int
receive_wavedata(EVENT_EFI_WAVEDATA *edata) {
	int       nitem;
	MATRIXN_T *data;

	nitem = edata->indexN - edata->index0 + 1;
	printf("%s: received EFI %d datum, frame %d, tag %d from %d\n",
	       Pgm, nitem, edata->frame, edata->tag,
	       EventFrom(edata));

	data = &edata->data[0];

	fwrite(data, sizeof(MATRIXN_T), nitem, Fout);

	return(1);
}

INTERNAL int
receive_spikedata(EVENT_EFI_SPIKEDATA *edata) {
	int       nitem;
	TSpike   *data;

	nitem = edata->indexN - edata->index0 + 1;
	printf("%s: received EFI %d datum, frame %d, tag %d from %d\n",
	       Pgm, nitem, edata->frame, edata->tag,
	       EventFrom(edata));

	data = &edata->data[0];

	fwrite(data, sizeof(TSpike), nitem, Fout);

	return(1);
}

#define PREFIX "daffie://"

INTERNAL void
setprobe(char *spec) {
	char server[100];
	char instname[100];
	char *s, *t;
	int  param;
	int  id, nc;
	int  clientid = 0;
	ECLIENT_LIST  clients[MAX_CLIENT];
	EVENT_EFI_CTL eprobe;

	s = spec;

	if (substr(PREFIX, s)) s += strlen(PREFIX);
	if (substr("//", s))   s += 2;

	t = server;
	while (*s && *s != '/') *t++ = *s++;
	*t = '\0';

	if (*s++ != '/') probe_error(spec);
	if (*s == '/') ++s;

	t = instname;
	while (*s != '.') *t++ = *s++;
	*t = '\0';

	if (*s++ != '.') probe_error(spec);

	if (isdigit(instname[0])) clientid = atoi(instname);

	param = atoi(s);
	if (!param) probe_error(spec);

	printf("setprobe: setting probe daffie://%s/%s.%d\n",
		server, instname, param);

	efi_init();

	id = event_join(server, &nc);
	if (!id) {
		fprintf(stderr, "%s: couldn't join server \"%s\"\n", Pgm, server);
		exit(4);
	}
	printf("%s: joined %s as client id %d (%d clients)\n", Pgm, server, id, nc);

	event_register("earlab", "control", Pgm);

	event_select_type(0, 0, ET_MAX);
	event_select_type(1, ET_EFI_DATA_LO, ET_EFI_DATA_HI);
	event_callback(ET_EFI_WAVEDATA,  receive_wavedata);
	event_callback(ET_EFI_SPIKEDATA, receive_spikedata);
	event_receive_enable(0);
	event_flush(0);

	eclient_monitor();
	if (!clientid)
	   nc = eclient_match(clients, "*", "earlab", "*", instname);
	else
	   nc = eclient_match_id(clients, clientid);

	if (nc < 1) {
		fprintf(stderr, "%s: couldn't find match for %s\n",
			Pgm, instname);
		event_leave();
		exit(1);
	}
	if (nc > 1) {
		fprintf(stderr, "%s: %s matched multiple (%d) clients\n",
			Pgm, instname, nc);
		event_leave();
		exit(1);
	}

	eprobe.type     = EFI_CTL_PROBE;
	eprobe.clientid = id;
	eprobe.tag      = getpid();
	eprobe.param    = param - 1;
	eprobe.flag     = 1;
	eprobe.value    = 0;

	SEND(clients[0].id, EFI_CTL, eprobe);

}

#define PRT(opt,str) \
	printf(msg?"\t%s%s%s\n":"%s ", opt, (strlen(opt)<8)?"\t\t":"\t", str)

INTERNAL void
usage(char *pgm, int msg) {
	if (msg) printf("usage: %s [options] datafile\n", pgm);
	else     printf("usage: %s ", pgm);
	PRT("[-help]",          "display this message");
	PRT("probe",            "Earlab/Daffie probe");
	PRT("outputfile",       "ascii data file");
	printf("\n");
}

