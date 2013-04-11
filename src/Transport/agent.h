#ifndef _AGENT_H
#define _AGENT_H

#include "agentd.h"

typedef struct {
	int     clientid;
	int     clientip;
	char    host[AGENTD_NAMELEN];
	char    module[AGENTD_PATHLEN];
} MODULE_HOST;

EXTERNAL char *SearchWorld(char *meta, char *name);
EXTERNAL int   LocateModule(char *modname, MODULE_HOST *modlist, int maxmod);
EXTERNAL int   StartModule(int clientid, char *logger, char *module, char *args);
#endif
