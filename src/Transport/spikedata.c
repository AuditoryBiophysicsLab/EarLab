/*
 * Convert Spike binary dump output + metafile to a single ascii file
 *   of the form: <frame #> <cell id> <event time>
 */

#include <stdio.h>
#include <local/std.h>

#include "CSpike.h"

main(int argc, char *argv[]) {
	char  *pgm;
	char  *nmeta, *ndata;
	FILE  *fmeta, *fdata;
	FILE  *fout;
	int    frame, count;
	int    i, line = 0;
	TSpike spike;
	char   header[200];

	if (argc < 2) {
		fprintf(stderr, "usage %s meta data\n", argv[0]);
		exit(1);
	}

	pgm = argv[0];
	nmeta = argv[1];
	ndata = argv[2];

	fmeta = fopen(nmeta, "r");
	if (!fmeta) {
		fprintf(stderr, "%s: can't open meta file %s\n",
			pgm, nmeta);
		exit(1);
	}

	fdata = fopen(ndata, "r");
	if (!fdata) {
		fprintf(stderr, "%s: can't open data file %s\n",
			pgm, ndata);
		exit(1);
	}

	fgets(header, sizeof(header), fmeta);
	if (!strimatch("*spike*", header)) {
		fprintf(stderr, "%s: not a spike file - %s\n", pgm, header);
		exit(1);
	}

	fout = stdout;

	while (!feof(fmeta)) {
		++line;
		if (fscanf(fmeta, " %d %d", &frame, &count) != 2) {
			if (feof(fmeta)) break;
			fprintf(stderr, "%s: unexpected error at line %d of %s\n",
				pgm, line, nmeta);
			exit(1);
		}
		for (i = 0; i < count; ++i) {
			fread(&spike, 1, sizeof(TSpike), fdata);
			fprintf(fout, "%d %d %f\n",
				frame, spike.cell, spike.time);
		}
	}
	fclose(fmeta);
	fclose(fdata);

	exit(0);
}

