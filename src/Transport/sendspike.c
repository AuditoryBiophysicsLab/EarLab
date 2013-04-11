/*
 * Test program to send spike data
 */
#include <stdio.h>
#include <math.h>
#include <local/std.h>
#include <daffie/event.h>
#include <daffie/eventdef.h>
#include "efievent.h"

INTERNAL void efi_init(void) {
	/* Define agent events */
	event_init();

#include "efieventdef.h"
#include "agentddef.h"
}

INTERNAL int irand(int a, int b);

main(int argc, char *argv[]) {
	char    *pgm, *host;
	int     to;
	int     id, nc;
	int     i, j;
	int     nevent;
	int     nspike;
	int     hdrsize, size;
	int     lowone = 1;
	int     delay  = 1000;
	BYTE    *lowbyte = (BYTE *)&lowone;
	TSpike *spike;
	EVENT_EFI_SPIKEDATA *espike;

	pgm = argv[0];

	if (argc < 3) {
		printf("usage: %s host to\n", pgm);
		exit(1);
	}

	host = argv[1];
	to   = atoi(argv[2]);

	if (argc > 3) nevent = atoi(argv[3]);
	else          nevent = 5;

	if (argc > 4) delay = atoi(argv[4]);

printf("joining %s\n", host);

	efi_init();
	id = event_join("localhost", &nc);

	if (!id) exit(1);

	hdrsize = sizeof(EVENT_EFI_SPIKEDATA) - sizeof(espike->data);
	printf("header size = %d\n", hdrsize);

	espike = (EVENT_EFI_SPIKEDATA *)malloc(66000);

	for (i = 0; i < nevent; ++i) {
		nspike = 500 + irand(0, 500);

		for (j = 0; j < nspike; ++j) {
			spike = &espike->data[j];
			spike->cell = irand(1, 1000);
			spike->time = j/(double)nspike;
		}
		espike->elems  = nspike;
		espike->tag    = id;
		espike->frame  = i;
		espike->endian = (*lowbyte == 0);
		espike->type   = EFI_STREAM_SPIKE;
		espike->index0 = 0;
		espike->indexN = nspike-1;

		size = hdrsize + nspike*sizeof(TSpike);
		printf("Sending %d spikes (%d) - total %d bytes to %d\n",
			nspike, sizeof(TSpike), size, to);

		event_send(to, ET_EFI_SPIKEDATA, (EVENT *)espike, size);
		if (delay) event_sleep(delay);
	}
	event_leave();
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

