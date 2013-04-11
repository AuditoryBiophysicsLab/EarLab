/*
 * Defintions for agentd-related events
 */
#include "agentd.h"

event_define(ET_AGENTD_REQ, "agentd_request", 5,
	      5,              EVENT_DATA_INT,
	      AGENTD_NAMELEN, EVENT_DATA_BYTE,
	      AGENTD_NAMELEN, EVENT_DATA_BYTE,
	      AGENTD_NAMELEN, EVENT_DATA_BYTE,
	      AGENTD_PATHLEN, EVENT_DATA_BYTE,
	      AGENTD_ARGLEN,  EVENT_DATA_BYTE);

event_define(ET_AGENTD_STAT, "agentd_staus", 1,
	      7,              EVENT_DATA_INT);

event_define(ET_AGENTD_MOD, "agentd_mod", 2,
	      3,              EVENT_DATA_INT,
	      0,              EVENT_DATA_BYTE);

event_define(ET_AGENTD_LOG, "agentd_log", 2,
	     3,             EVENT_DATA_INT,
	     0,             EVENT_DATA_BYTE);
