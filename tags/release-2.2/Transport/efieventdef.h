#ifndef _EFIEVENTDEF_H
#define _EFIEVENTDEF_H

#include <daffie/eventdef.h>

#include "CMatrixN.h"
#include "CSpike.h"
#include "CEarlabDataStream.h"

#define EVENT_DATA_int    EVENT_DATA_INT
#define EVENT_DATA_float  EVENT_DATA_FLOAT
#define EVENT_DATA_double EVENT_DATA_DOUBLE

#define EVENT_DATA_TYPE (X) EVENT_DATA_ ## X

event_define(ET_EFI_WAVEDATA, "efi_wavedata", 4,
	     3, EVENT_DATA_INT,
	     2, EVENT_DATA_SHORT,
	     2, EVENT_DATA_INT,
	     0, EVENT_DATA_FLOAT /* MATRIXN_T */ );

event_define(ET_EFI_SPIKEDATA, "efi_spikedata", 4,
	     3, EVENT_DATA_INT,
	     2, EVENT_DATA_SHORT,
	     2, EVENT_DATA_INT,
      /*****
	     2, EVENT_DATA_BGNSTRUCT,
		1, EVENT_DATA_INT,
		1, EVENT_DATA_FLOAT,
	     0, EVENT_DATA_ENDSTRUCT,
       *****/
	     0, EVENT_DATA_BYTE);

event_define(ET_EFI_PARAMETER_FILE, "efi_parameter_file", 2,
	     EFI_FILENAME_SIZE, EVENT_DATA_BYTE,
	     EFI_NAMESIZE,      EVENT_DATA_BYTE);

event_define(ET_EFI_PARAMETER_SET, "efi_parameter_set", 1, 0, EVENT_DATA_BYTE);

event_define(ET_EFI_START,   "efi_start", 7,
	     1,               EVENT_DATA_INT,
	     EFI_MAXARG,      EVENT_DATA_INT,
	     EFI_MAXARG*EFI_MAXDIM, EVENT_DATA_INT,
	     1,               EVENT_DATA_INT,
	     EFI_MAXARG,      EVENT_DATA_INT,
	     EFI_MAXARG*EFI_MAXDIM, EVENT_DATA_INT,
	     EFI_MAXARG,      EVENT_DATA_INT);


event_define(ET_EFI_ADVANCE, "efi_advance", 1, 1, EVENT_DATA_INT);

event_define(ET_EFI_STOP,    "efi_stop",    1, 1, EVENT_DATA_INT);

event_define(ET_EFI_UNLOAD,  "efi_unload",  1, 1, EVENT_DATA_INT);

event_define(ET_EFI_STATUS,  "efi_status",  1, 5, EVENT_DATA_INT);

event_define(ET_EFI_SPEC, "efi_spec", 5,
	     EFI_NAMESIZE,    EVENT_DATA_BYTE,
	     1,               EVENT_DATA_INT,
	     EFI_MAXARG,      EVENT_DATA_INT,
	     1,               EVENT_DATA_INT,
	     EFI_MAXARG,      EVENT_DATA_INT);

event_define(ET_EFI_CONFIG, "efi_config", 16,
	     1,               EVENT_DATA_INT,
	     EFI_MAXARG,      EVENT_DATA_INT,
	     EFI_MAXARG,      EVENT_DATA_INT,
	     EFI_MAXARG,      EVENT_DATA_INT,
	     EFI_MAXARG,      EVENT_DATA_INT,
	     EFI_MAXARG,      EVENT_DATA_INT,
	     EFI_MAXARG*EFI_MAXDIM, EVENT_DATA_INT,
	     EFI_MAXARG,      EVENT_DATA_INT,
	     1,               EVENT_DATA_INT,
	     EFI_MAXARG,      EVENT_DATA_INT,
	     EFI_MAXARG,      EVENT_DATA_INT,
	     EFI_MAXARG,      EVENT_DATA_INT,
	     EFI_MAXARG,      EVENT_DATA_INT,
	     EFI_MAXARG,      EVENT_DATA_INT,
	     EFI_MAXARG*EFI_MAXDIM, EVENT_DATA_INT,
	     EFI_MAXARG,      EVENT_DATA_INT);

event_define(ET_EFI_CTL, "efi_ctl", 1,
	     6, EVENT_DATA_INT);
#endif

