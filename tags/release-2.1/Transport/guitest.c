#include <stdio.h>
#include <local/std.h>

#include <daffie/event.h>

#include "efievent.h"
#include "cl.h"
#define MAX_MODULES 100

INTERNAL void efi_init(void);
INTERNAL void send_state(int module, int state);
INTERNAL void send_frame(int frame, int total);
INTERNAL int  irand(int a, int b);

main(int argc, char *argv[]) {
	char *pgm;
	char *server;
	int   i, j;
	int   id, nc;
	int   nstep   = 10;
	int   nmodule = 5;
	int   running[MAX_MODULES], runstate[MAX_MODULES], nrunning;

	pgm = argv[0];
	++argv; --argc;

	if (!argc) {
		fprintf(stderr, "usage: %s server [nstep] [nmodule]\n", pgm);
		exit(1);
	}
	server = argv[0];
	++argv; --argc;

	if (argc) {
		nstep = atoi(argv[0]);
		++argv; --argc;
	}

	if (argc) {
		nmodule = atoi(argv[0]);
		++argv; --argc;
	}

	if (argc) {
		fprintf(stderr, "usage: %s server [nstep] [nmodule]\n", pgm);
		exit(1);
	}

	event_verbose(0);
	efi_init();

	id = event_join(server, &nc);
	if (!id) {
		fprintf(stderr, "%s: couldn't join server %s\n", pgm, server);
		exit(1);
	}
	fprintf(stderr, "%s: Joined %s as client id %d (%d clients)\n",
		pgm, server, id, nc);

	/* Don't receive any events */
	event_select_type(0, 0, ET_MAX);
	event_receive_enable(0);

	/* Send loaded messages */
	for (i = 0; i < nmodule; ++i) {
		send_state(i, 2);
		event_sleep(irand(100, 1000));
	}

	/* Send an idle messages */
	for (i = 0; i < nmodule; ++i) {
		send_state(i, 3);
		event_sleep(irand(100, 1000));
	}

	/* Cycle run-send-idle for each step */
	for (i = 0; i < nstep; ++i) {
		/* Set all modules to IDLE */
		for (j = 0; j < nmodule; ++j) {
			running[j]  = j;
			runstate[j] = 3;
		}
		nrunning = nmodule;

		/* Randomly cycle a module */
		while (nrunning) {
			j = irand(0, nrunning-1);

			if (runstate[j] == 5) runstate[j] = 3;
			else ++runstate[j];

			send_state(running[j], runstate[j]);

			/* Remove modules that have gone idle */
			if (runstate[j] == 3) {
				running[j]  = running[nrunning-1];
				runstate[j] = runstate[nrunning-1];
				nrunning--;
			}

			event_sleep(irand(100, 2000));
		}
		send_frame(i+1, nstep);
		printf("Finished frame %d\n", i);
	}

	/* Send an unload messages */
	for (i = 0; i < nmodule; ++i) {
		send_state(i, 1);
		event_sleep(irand(100, 1000));
	}

}

INTERNAL void
send_state(int module, int state) {
	EVENT_EFI_STATUS estat;

	estat.type   = EFI_STATUS_CONTROL;
	estat.status = state;
	estat.detail = module;

	BCAST(EFI_STATUS, estat);
}

INTERNAL void
send_frame(int frame, int total) {
	EVENT_EFI_STATUS estat;

	estat.type   = EFI_STATUS_FRAME;
	estat.status = frame;
	estat.detail = total;

	BCAST(EFI_STATUS, estat);
}

/*
 * DAFFIE definitions for EFI events
 * (This will get moved into the DAFFIE event defs eventually)
 */
#include "eventdef.h"
INTERNAL void
efi_init(void) {
	event_verbose(0);
	event_init();
#include "efieventdef.h"
}

#define CONS 0xffffff

INTERNAL int
irand(int a, int b) {
	int   val;
	float alpha;

	alpha = (random() & CONS) / (float)(CONS+1);
	val = (b - a + 1) * alpha + a;
	return(val);
}
