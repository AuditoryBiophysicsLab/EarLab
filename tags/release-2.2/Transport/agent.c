/*
 * Agent Utilities
 */

#include <stdio.h>
#include <unistd.h>
#include <local/std.h>

#include <daffie/event.h>
#include <daffie/eclient.h>
#include <daffie/eventdef.h>

#include "agent.h"
#include "config.h"

#define PATHLEN   2500
#define SYS_OK    0
#define MAX_AGENT 100

#define METASERVER "scv.bu.edu"

typedef struct {
	int         nexec;
	char        modname[AGENTD_PATHLEN];
	int         nhost;
	MODULE_HOST host[1];
} MODULE_HOSTLIST;

#ifndef DEBUG
# define DEBUG 0
#endif

INTERNAL int Debug = DEBUG;

EXTERNAL char *cuserid(char *buf);

INTERNAL int
gen_tag(void) {
	static int tag = 0, pid = 0;

	if (!pid) pid = getpid();
	return(pid + tag++);
}


EXPORT char *
SearchWorld(char *meta, char *name) {
	EVENT          event;
	EVENT_IDENTIFY eid;
	EVENT_IDENTITY *e;
	char msgbuf[200], *str;
	int  id, nc;
	int  i, subindx, n, menuid;
	int  response[MAX_CLIENT], nresponse = 0;
	unsigned long starttime, elapsed;

	if (meta == NULL) meta = METASERVER;

	id = event_join(meta, &nc);
	if (id < 1) {
		fprintf(stderr,
			"SearchWorld: couldn't connect to meta server at %s\n",
			meta);
		return (NULL);
	}
	event_select_type(0, ET_USER_MIN, ET_USER_MAX);
	event_register("agentd", "worldquery", name);

	printf("SearchWorld: connected to Metaworld at %s\n", meta);

	eid.tag = 0;
	BCAST(IDENTIFY, eid);

	starttime = event_clock();
	while (nc > nresponse) {

		/* Keep an eye on how long we have been waiting */
		elapsed = (event_clock() - starttime) / 1000;
		if (elapsed > 5) {
			printf("SearchWorld: Looking for %d more ID responses of %d\n",
				nc-nresponse, nc);
			if (elapsed > 120) {
				printf("SearchWorld: Timeout after %d\n", elapsed);
				break;
			}
		}

		event_wait();
		if (!event_receive(&event)) continue;
		switch (event.ev_head.type) {
		  case ET_IDENTITY:
		       if (elapsed > 5)
			    printf("Got Identity from %d: %s %s %s (%d of %d)\n",
				    event.ev_head.from,
				    event.ev_identity.class,
				    event.ev_identity.species,
				    event.ev_identity.instance,
				    nresponse, nc);

			/* Check for duplicate responses */
			for (i = 0; i < nresponse; ++i)
			   if (event.ev_head.from == response[i]) break;
			if (i < nresponse) break;

			response[nresponse++] = event.ev_head.from;

			e = &event.ev_identity;

			/* Look for event servers only */
			if (!streq(e->class, "eventd") ||
			    !strmatch(name, e->instance)) break;

			event_leave();

			str = (char *)malloc(strlen(e->species)+1);
			if (!str) {
				fprintf(stderr, "Couldn't allocate string!!\n");
				return(0);
			}
			strcpy(str, e->species);
			return(str);

		  case ET_JOIN:
			if (event.ev_join.id == id) break;
			if (elapsed > 5)
			   printf("Client %d joined\n", event.ev_join.id);
			for (i = 0; i < nresponse; ++i)
			   if (event.ev_join.id == response[i]) break;

			if (i < nresponse) {
				/* This a duplicate message */
				break;
			}
			++nc;
			break;


		  case ET_LEAVE:
			if (elapsed > 5)
			   printf("Client %d left\n", event.ev_leave.id);
			for (i = 0; i < nresponse; ++i)
			    if (event.ev_leave.id == response[i]) break;

			if (i == nresponse) {
				/* This client left before responding */
				--nc;
			}
			break;

		  default:
			break;
		}
	}

	event_leave();

	printf("SearchWorld: couldn't locate %s\n", name);

	return(NULL);
}

EXPORT int
LocateModule(char *modname, MODULE_HOST *modlist, int maxmod) {
	ECLIENT_LIST     agents[MAX_AGENT];
	EVENT_AGENTD_REQ equery;
	static int times = 0;
	int   nmod;
	int   nagent;
	int   ndone, done[MAX_AGENT];
	int   tag;
	int   i;

	if (times == 0) eclient_monitor();

	++times;
	tag = getpid()<<16 | times;

	event_select_type(1, ET_LEAVE, ET_LEAVE);

	/* Locate all agents */
	nagent = eclient_match(agents, "*", "daemon", "agentd", "*", "*");

	/* Now send a module query to each one */
	equery.type     = AGENTD_CTL_QUERY;
	equery.flags    = 0;
	equery.auth     = 0;
	equery.authtype = 0;
	equery.tag      = tag;           /* identifying tag */
	strcpy(equery.requester, cuserid(NULL));
	strcpy(equery.log,    "");
	strcpy(equery.server, "");
	strcpy(equery.module, modname);
	strcpy(equery.args,   "");

	if (!nagent) {
		fprintf(stderr, "LocateModule: no agentd servers found!\n");
		return(0);
	}

	for (i = 0; i < nagent; ++i) {
		if (Debug)
		   printf("Sending query for \"%s\" to %s (%d)\n",
			   modname, agents[i].host, agents[i].id);
		SEND(agents[i].id, AGENTD_REQ, equery);
	}

	ndone = nmod = 0;
	while (ndone < nagent) {
		EVENT e;

		event_wait();
		if (!event_receive(&e)) continue;

		if (Debug)
		   printf("query: received %s from %d\n",
			   event_lookup_name(EventType(e)), EventFrom(e));

		/* Check for agentds exiting */
		if (e.ev_head.type == ET_LEAVE) {
			for (i = 0; i < nagent; ++i)
			  if (agents[i].id == e.ev_leave.id) {
				int j;

				for (j = 0; j < ndone; ++j)
				  if (done[j] == e.ev_leave.id) break;

				if (j == ndone) { /* This agent did not complete */
					nagent--;
					memcpy(&agents[i], &agents[--nagent], sizeof(ECLIENT_LIST));
				}

				/* Remove all listings for this agentd */
				for (j = 0; j < nmod; )
				  if (modlist[j].clientid == e.ev_leave.id)
				     memcpy(&modlist[j], &modlist[--nmod], sizeof(MODULE_HOST));
				  else ++j;
			  }
			  continue;
		}

		if (e.ev_head.type == ET_AGENTD_MOD) {
			EVENT_AGENTD_MOD *emod = (EVENT_AGENTD_MOD *)&e;
			char             *avail;
			int               i, len;

			avail = &emod->modules[0];
			for (i = 0; i < emod->nmodules; ++i) {
			    /*********
				printf("query: module \"%s\" available on client %d\n",
					avail, EventFrom(e));
			     *********/
				if (strmatch(modname, avail)) {
					ECLIENT_LIST eclient;

					strcpy(modlist[nmod].module, avail);
					modlist[nmod].clientid = EventFrom(e);

					if (eclient_match_id(&eclient, EventFrom(e))) {
						strcpy(modlist[nmod].host, eclient.host);
					}
					else {
						strcpy(modlist[nmod].host, "unknown");
					}

					++nmod;
				}
				avail += strlen(avail)+1;
			}
			continue;
		}

		if (e.ev_head.type == ET_AGENTD_STAT) {
			EVENT_AGENTD_STAT *estat;

			estat = (EVENT_AGENTD_STAT *)&e;

			if (estat->reqtype != AGENTD_CTL_QUERY) continue;

			done[ndone++] = EventFrom(e);
		     /***
			printf("query: client %d completed\n", EventFrom(e));
		      ***/
		}
	}

	return(nmod);
}

EXPORT int
StartModule(int clientid, char *logger, char *module, char *args) {
	EVENT_AGENTD_REQ eagent;
	char             me[200];
	int              exectag;

	cuserid(me);
	/* Construct and send a start request */

	memset(&eagent, 0, sizeof(eagent));
	eagent.type   = AGENTD_CTL_START;
	eagent.flags  = 0;
	eagent.auth   = 0;
	eagent.tag    = exectag = gen_tag();
	strcpy(eagent.requester, me);
	strcpy(eagent.log,       logger?logger:"");
	strcpy(eagent.server,    "");
	strcpy(eagent.module,    module);
	strcpy(eagent.args,      args);

	printf("StartModule: %s %s (logger %s, for %s)\n",
	eagent.module, eagent.args, eagent.log, eagent.requester);

	SEND(clientid, AGENTD_REQ, eagent);

	/* Wait for an acknowledgement */
	FOREVER {
		EVENT       e;

		event_wait();
		if (!event_receive(&e)) continue;

		if (EventType(e) == ET_AGENTD_STAT) {
			EVENT_AGENTD_STAT *estat;

			estat = (EVENT_AGENTD_STAT *)&e;
		     /*****
			if (estat->type != AGENTD_CTL_STATUS) break;
		      *****/

			/* Check to see if this is from the start request */
			if (EventFrom(e) == clientid && estat->tag == exectag) {
			     printf("%s: agentd %d started module as pid %d - %s\n",
				     "StartModule",
				     estat->head.from, estat->result,
				     (estat->rstatus>=0 && estat->rstatus<=AGENTD_STATUS_MAX)?
				     AgentdErrorMsg[estat->rstatus]:"Bad status code!");

				return(estat->rstatus);
			}

		  }
	}
}
