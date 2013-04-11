/*
 * AGENTD events header
 */
#ifndef _AGENTD_H
#define _AGENTD_H

/* Registered class and species */
#define AGENTDCLASS "daemon"
#define AGENTDSPEC  "agentd"
#define LOGGERCLASS  AGENTDCLASS
#define LOGGERSPEC  "agentd-logger"

/* Constants */
#define AGENTD_NAMELEN   64
#define AGENTD_PATHLEN  256
#define AGENTD_ARGLEN   256
#define AGENTD_MAXMSG  4000
#define AGENTD_MAXLIST   64
#define AGENTD_LISTBUF  (AGENTD_MAXLIST*AGENTD_PATHLEN)

/*
 * AGENTD control message types
 */
#define AGENTD_CTL_START    1           /* Start the specified agent */
#define AGENTD_CTL_STOP     2           /* Stop the specified agent  */
#define AGENTD_CTL_VERIFY   3
#define AGENTD_CTL_CHECK    4
#define AGENTD_CTL_STATUS   5           /* Report status of agent    */
#define AGENTD_CTL_QUERY    6           /* Query list of agents      */

/*
 * AGENTD log message types
 */
#define AGENTD_LOG_STDIN    0
#define AGENTD_LOG_STDOUT   1
#define AGENTD_LOG_STDERR   2

/*
 * AGENTD request return codes
 */
#define AGENTD_STATUS_OK        0
#define AGENTD_STATUS_BADPATH   1
#define AGENTD_STATUS_BADARGS   2
#define AGENTD_STATUS_NOFILE    3
#define AGENTD_STATUS_NOACCESS  4
#define AGENTD_STATUS_NOEXEC    5
#define AGENTD_STATUS_NOTAUTH   6
#define AGENTD_STATUS_RUNERR    7

#define AGENTD_STATUS_MAX       AGENTD_STATUS_RUNERR

static char *AgentdErrorMsg[] = {
	"ok",
	"bad path",
	"bad args",
	"file does not exist",
	"access to file not allowed",
	"execute dissallowed",
	"not authorized",
	"run time error",
	NULL
};

/*
 * AGENTD agent status codes
 */
#define AGENTD_AGENT_RUNNING 0x0001
#define AGENTD_AGENT_EXIT    0x0002
#define AGENTD_AGENT_NOGO    0x0004
#define AGENTD_AGENT_INIT    0x0010
#define AGENTD_AGENT_ALLOC   0x0040
#define AGENTD_AGENT_FREE    0x0080
#define AGENTD_AGENT_ABORTED 0x8000
#define AGENTD_AGENT_SIGGED  0x8001
#define AGENTD_AGENT_CORE    0x8002

/*
 * AGENTD flags
 */
#define AGENTD_FLAG_MORE 1

typedef struct {
	EVENT_HEAD      head;                        /* Event header         */
	int             type;                        /* Message subtype      */
	int             flags;                       /* Flags                */
	int             tag;                         /* User tag             */
	int             authtype;                    /* Authorization type   */
	int             auth;                        /* Authorization        */
	char            requester[AGENTD_NAMELEN];   /* Requester user name  */
	char            log[AGENTD_NAMELEN];         /* Log server           */
	char            server[AGENTD_NAMELEN];      /* Client world server  */
	char            module[AGENTD_PATHLEN];      /* Module name/path     */
	char            args[AGENTD_ARGLEN];         /* Module arguments     */
} EVENT_AGENTD_REQ;

typedef struct {
	EVENT_HEAD      head;                        /* Event header         */
	int             reqtype;                     /* Request type         */
	int             reqseq;                      /* Request sequence no. */
	int             flags;                       /* Flags                */
	int             rstatus;                     /* Request status       */
	int             astatus;                     /* Agent status         */
	int             result;                      /* Returned value       */
	int             tag;                         /* User tag             */
} EVENT_AGENTD_STAT;

typedef struct {
	EVENT_HEAD      head;                        /* Event header         */
	int             flags;                       /* Flags                */
	int             tag;                         /* User tag             */
	int             nmodules;                    /* Number of modules    */
	char            hostname[AGENTD_NAMELEN];    /* Hostname             */
	char            modules[AGENTD_LISTBUF];     /* Module list          */
} EVENT_AGENTD_MOD;

typedef struct {
	EVENT_HEAD      head;                        /* Event header         */
	int             type;                        /* Message subtype      */
	int             flags;                       /* Flags                */
	int             pid;                         /* Process id           */
	char            hostname[AGENTD_NAMELEN];    /* Hostname             */
	char            program[AGENTD_NAMELEN];     /* Program name         */
	char            msg[AGENTD_MAXMSG];          /* Message              */
} EVENT_AGENTD_LOG;

#define ET_AGENTD_REQ   1020
#define ET_AGENTD_STAT  1021
#define ET_AGENTD_MOD   1022
#define ET_AGENTD_LOG   1023

#define ET_AGENTD_MIN 1020
#define ET_AGENTD_MAX 1023

#endif
