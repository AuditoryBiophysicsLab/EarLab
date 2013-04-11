#include <stdio.h>
#include <local/std.h>

main(int argc, char *argv[]) {
	char *pgm;
	int   i, sampper, chan, frames;
	FILE *out = stdout;

	pgm = argv[0];

	if (argc < 4) {
		fprintf(stderr, "usage: %s samples channels frames\n", pgm);
		exit(1);
	}

	sampper = atoi(argv[1]);
	chan    = atoi(argv[2]);
	frames  = atoi(argv[3]);

	fprintf(out, "waveform [ %d %d ] %d\n", sampper, chan, chan*sampper);
	for (i = 0; i < frames; ++i) {
		fprintf(out, "%d %d\n", i+1, chan*sampper);
	}

	exit(0);
}


