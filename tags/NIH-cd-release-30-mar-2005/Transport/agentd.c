/*
 * DAFFIE agent initiator
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <local/std.h>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <daffie/event.h>

#include "errno.h"
#include "agentd.h"
#include "eventdef.h"

# define LOGGEREXEC "agentlogger"

#ifdef win32
# define LOGGEREXT ".exe"
#else
# define LOGGEREXT ""
#endif

#ifdef USE_GETWD
# define GetWD(s,n) getwd(s)
#else
# define GetWD(s,n) getcwd(s, n)
#endif

#define PATHLEN   2500
#define SYS_OK    0
#define MAX_RIP   100
#define MAX_AGENT 100
#define MAX_ARGS  200

/* Signal stuff */
#ifndef NSIG
# define NSIG 32
#endif

INTERNAL char *SigName[NSIG];

typedef struct {
	EVENT_AGENTD_REQ   ereq;        /* Original request     */
	int                pid;         /* PID of child (agent) */
	int                status;      /* Current status       */
	unsigned long long start, stop; /* Start and stop times */
} AGENT_REQ;

INTERNAL void  usage(char *pgm, int msg);
INTERNAL void  agent_request(EVENT_AGENTD_REQ *eagent);
INTERNAL int   agent_query(EVENT_AGENTD_REQ *eagent, DIR *dir, char *dirpath);
INTERNAL AGENT_REQ *agent_alloc(void);
INTERNAL void  agent_free(AGENT_REQ *areq);
INTERNAL int   check_auth(EVENT_AGENTD_REQ *eagent);
INTERNAL int   validate(char *file, char *args);
INTERNAL int   find_logger(void);
INTERNAL void  agentman(EVENT_AGENTD_REQ *eagent);
INTERNAL int   start_agent(AGENT_REQ *areq, char *cmd, char *args, int *retpid);
INTERNAL int   start_logger(AGENT_REQ *areq, char *logserver, char *cmd, char *args, int *retpid);
INTERNAL void  reader(EVENT_AGENTD_LOG *elog);
INTERNAL int   parse_args(char *args, char *argv[], int arg1);
INTERNAL void  make_path(char *path, char *dir, char *file);
INTERNAL int   oneof(char c, char *s);
INTERNAL void  siginit(void);
INTERNAL char *signame(int signo);

INTERNAL void logger(char *pgm, int argc, char **argv);
INTERNAL void logger_usage(char *pgm, int msg);

void agentd_init(void) {
	event_init();
#include "agentddef.h"
}

INTERNAL AGENT_REQ  Agent[MAX_AGENT];           /* Active agents        */
INTERNAL AGENT_REQ  Rip[MAX_RIP];               /* Recently dead agents */
INTERNAL int        Nagent   = 0;
INTERNAL int        Nrip     = 0;
INTERNAL char       EofStr[] = { '\03', '\n', '\0'};
INTERNAL int        AgentLock;
INTERNAL char      *RootPath = NULL;
INTERNAL DIR       *RootDir  = NULL;
INTERNAL char      *WorkDir  = NULL;
INTERNAL char       ExecPath[PATHLEN];
INTERNAL char       WorkPath[PATHLEN];
INTERNAL char       Logdir[PATHLEN] = { '\0' };
INTERNAL char       Logger[PATHLEN] = { '\0' };
INTERNAL char       SelfWD[PATHLEN];
INTERNAL int        Verbose  = 1;
INTERNAL int        Debug    = 0;
INTERNAL int        Exit     = 0;
INTERNAL int        Exitlock;
INTERNAL int        Exitwait;
INTERNAL int        AgentInit;
INTERNAL int        ReadInit;

main(int argc, char *argv[]) {
	char *pgm, *s;
	int   ok, help;
	char *server;
	int   id, nc;
	int   i;
	char  me[256];
	char  logstr[200];

	pgm = argv[0];

	/* Get and save our original working directory */
	if (!GetWD(SelfWD, sizeof(SelfWD))) {
		fprintf(stderr, "%s: couldn't get working directory\n", pgm);
		perror(pgm);
		exit(4);
	}

	/* Get our own name and any path prefix */
	if (pgm = strrchr(argv[0], '/')) {
		*pgm++ = '\0';
		if (*argv[0] == '/') strcpy(Logdir, argv[0]);
		else                 make_path(Logdir, SelfWD, argv[0]);
		if (Verbose) printf("Logdir search is \"%s\"\n", Logdir);
	}
	else {
		strcpy(Logdir, SelfWD);
		pgm = argv[0];
	}

	++argv; --argc;

	/* See if we are running as the LOGGER */
	sprintf(logstr, "{%s,%s%s}", LOGGEREXEC, LOGGEREXEC, LOGGEREXT);
	if (strmatch(logstr, pgm)) logger(pgm, argc, argv);

	/* Default options */
	ok    = 1;
	help  = 0;

	/* Parse out command line options */
	while (argc && *argv[0] == '-') {
		char *opt;

		opt = argv[0] + 1;
		++argv; --argc;

		if      (substr(opt, "help"))    ++help;
		else if (substr(opt, "debug"))   ++Debug;
		else if (substr(opt, "nodebug")) Debug   = 0;
		else if (substr(opt, "verbose")) Verbose = 1;
		else if (substr(opt, "quiet"))   Verbose = 0;
		else if (streq(opt, "bin") || streq(opt, "dir")) {
			if (argc < 1) {
				fprintf(stderr, "%s: missing argument to -%s\n",
					pgm, opt);
				ok = 0;
				break;
			}
			RootPath = argv[0];
			++argv; --argc;
		}
		else if (streq(opt, "wd") || streq(opt, "cd")) {
			if (argc < 1) {
				fprintf(stderr, "%s: missing argument to -%s\n",
					pgm, opt);
				ok = 0;
				break;
			}
			WorkDir = argv[0];
			++argv; --argc;
		}
		else if (substr(opt, "logger")) {
			if (argc < 1) {
				fprintf(stderr, "%s: missing argument to -%s\n",
					pgm, opt);
				ok = 0;
				break;
			}
			if (argv[0][0] == '/') strcpy(Logger, argv[0]);
			else {
				int  len;
				char *name;

				/* Get the current working directory */
				if (!GetWD(Logger, sizeof(Logger))) {
					fprintf(stderr, "%s: couldn't get working directory\n", pgm);
					perror(pgm);
					exit(4);
				}
				len = strlen(Logger);
				if (Logger[len-1] != '/') Logger[len++] = '/';

				name = argv[0];
				if (substr("./", name)) name += 2;
				strcpy(Logger+len, name);
			}
			++argv; --argc;
		}
		else {
			fprintf(stderr, "%s: ignoring argument -%s\n", pgm, opt);
			help = 1;
			ok   = 0;
		}
	}
	if (!ok || argc == 0) {
		usage(pgm, help);
		exit(!help);
	}

	server = argv[0];
	++argv; --argc;

	/* Allow directory to be specified as last command line arg */
	if (argc) {
		if (RootPath) {
			if (!streq(RootPath, argv[0])) {
				fprintf(stderr,
					 "%s: can't use -dir (%s) and specify path (%s)\n",
					 pgm, RootPath, argv[0]);
				exit(4);
			}
		}
		else {
			RootPath = argv[0];
			++argv; --argc;
		}
	}

	/* Check for any extraneous args */
	if (argc) {
		fprintf(stderr, "%s: too many arguments starting with \"%s\"\n",
			pgm, argv[0]);
		usage(pgm, help);
		exit(!help);
	}

	/* Not change to our root directory */
	if (RootPath) {
		if (chdir(RootPath) != SYS_OK) {
			fprintf(stderr, "%s: couldn't chdir to \"%s\"\n", pgm, RootPath);
			exit(4);
		}
	}
	else fprintf(stderr, "%s: WARNING! no root directory specified - using .\n", pgm);

	/* Set up path to working directory for lauched agents */
	if (WorkDir)
	   if (*WorkDir == '/') strcpy(WorkPath, WorkDir);
	   else make_path(WorkPath, SelfWD, WorkDir);

	/* Open the executable directory to process queries */
	if (!GetWD(ExecPath, sizeof(ExecPath))) {
		fprintf(stderr, "%s: couldn't get working directory\n", pgm);
		perror(pgm);
		exit(4);
	}

	RootDir = opendir(ExecPath);
	if (!RootDir) {
		fprintf(stderr, "%s: couldn't open working directory \"%s\"\n", pgm, ExecPath);
		perror(pgm);
		exit(4);
	}

#ifdef win32
	/*
	 * On windows we need to run the logger as a separate helper app
	 * since the cygwin implementation of fork is not quite right.
	 * Look for it in standard places as well as looking as users $PATH
	 */
	if (!Logger[0] && !find_logger()) {
		fprintf(stderr,
			"%s: couldn't find initiator \"%s\"\n",
			pgm, LOGGEREXEC);
		exit(4);
	}
	if (Verbose) printf("Logger exec is \"%s\"\n", Logger);
#endif

	/* Make sure we found the file and it is executable */
	if (Logger[0] && access(Logger, X_OK) != SYS_OK) {
		fprintf(stderr,
			"%s: initiator \"%s\" not %s\n", pgm, Logger,
			access(Logger, F_OK)?"found":"executable");
		exit(4);
	}

	/* Deal with signals */
	siginit();

	/* local initialization */
	event_verbose(Verbose);
	event_tunnel_enable(0);
	agentd_init();

	/* Connect to our event server */
	id = event_join(server, &nc);
	if (!id) {
		fprintf(stderr, "%s: couldn't join server \"%s\"\n", pgm, server);
		exit(4);
	}
	if (Verbose) printf("%s: joined %s as client id %d (%d clients)\n", pgm, server, id, nc);

	/* Register */
	cuserid(me);

	event_register(AGENTDCLASS, AGENTDSPEC, me);

	/* Subscribe to only AGENTD type events */
	event_select_type(0, 0, ET_MAX);
	event_select_type(1, ET_AGENTD_MIN, ET_AGENTD_MAX);

	/* Initialize agent control structures */
	for (i = 0; i < MAX_AGENT; ++i) {
		Agent[i].pid    = 0;
		Agent[i].status = AGENTD_AGENT_FREE;
	}

	mp_init();  /*(not strcitly necessary, since done in event land)*/

	/* Allocate our locks and semaphores */
	AgentLock = mp_alloclock();

	Exitlock  = mp_alloclock();
	Exitwait  = mp_allocsema();
	ReadInit  = mp_allocsema();
	AgentInit = mp_allocsema();

	/*
	 * Loop forever processing agentd requests
	 */
	FOREVER {
		EVENT e;

		/* Wait for an event and then dispatch it */
		event_wait();
		if (!event_receive(&e)) continue;

		switch (e.ev_head.type) {
		  case ET_AGENTD_REQ:
			agent_request((EVENT_AGENTD_REQ *)&e);
			break;

		  default:
			if (Verbose)
			   printf("Ignoring event \'%s\'(%d) from %d\n",
				   event_lookup_name(e.ev_head.type),
				   e.ev_head.type, e.ev_head.from);
		}
	}
}

/*
 * Locate the executable for the logger (for Windows)
 */
INTERNAL int
find_logger(void) {
	char cwd[PATHLEN];
	char logcmd[100];
	char *path;

	/* Construct the command name */
	sprintf(logcmd, "%s%s", LOGGEREXEC, LOGGEREXT);

	/* Check directory of this executable */
	if (Logdir[0]) {
		make_path(Logger, Logdir, logcmd);
		if (access(Logger, X_OK) == SYS_OK) return(1);
		if (Debug) printf("path \"%s\" access is %d\n", Logger, access(Logger, X_OK));
	}

	/* Get the current working directory */
	if (!GetWD(cwd, sizeof(cwd))) return(0);

	make_path(Logger, cwd, logcmd);
	if (access(Logger, X_OK) == SYS_OK) return(1);
	if (Debug) printf("path \"%s\" access is %d\n", Logger, access(Logger, X_OK));

	/* Get the path environment variable */
	path = getenv("PATH");
	if (!path) return(0);

	/* Look for the logger in the users execution path dirs */
	while (*path) {
		char *s = path;

		while (*s && *s != ':') ++s;
		*s++ = '\0';

		if (path[0] == '.' && (path[1] == '/' || !path[0])) {
			char tmp[PATHLEN];

			make_path(tmp, SelfWD, path+1);
			make_path(Logger, tmp, logcmd);
		}
		else make_path(Logger, path, logcmd);

		if (access(Logger, X_OK) == SYS_OK) return(1);
		if (Debug) printf("path \"%s\" access is %d\n", Logger, access(Logger, X_OK));

		path = s;
	}
	return(0);
}

/*
 * Logger helper app  [only for non-unix systems]
 */
INTERNAL void
logger(char *pgm, int argc, char **argv) {
	char *logserver;
	char *cmd;
	char  args[AGENTD_PATHLEN];
	int   id, nc;
	int   pid;
	int   ok, help;
	int   i;
	int   rc;

	ok      = 1;
	help    = 0;
	Verbose = 0;
	while (argc && *argv[0] == '-') {
		char *opt;

		opt = argv[0] + 1;
		++argv; --argc;

		if      (substr(opt, "help"))    ++help;
		else if (substr(opt, "debug"))   ++Debug;
		else if (substr(opt, "nodebug")) Debug   = 0;
		else if (substr(opt, "verbose")) Verbose = 1;
		else if (substr(opt, "quiet"))   Verbose = 0;
		else {
			fprintf(stderr, "%s: ignoring argument -%s\n", pgm, opt);
			help = 1;
			ok   = 0;
		}
	}
	if (!ok || argc == 0) {
		logger_usage(pgm, help);
		exit(!help);
	}

	if (argc < 2) {
		fprintf(stderr, "usage: %s logserver cmd [args]\n", pgm);
		exit(4);
	}

	logserver = argv[0];
	++argv; --argc;

	cmd = argv[0];
	++argv; --argc;

	args[0] = '\0';
	while (argc) {
		strcat(args, " ");
		strcat(args, argv[0]);
		++argv; --argc;
	}

	/* Initialize agent control structures */
	for (i = 0; i < MAX_AGENT; ++i) {
		Agent[i].pid    = 0;
		Agent[i].status = AGENTD_AGENT_FREE;
	}

	event_verbose(Verbose);
	event_tunnel_enable(0);

	agentd_init();

	siginit(); /* deal with signals */

	id = event_join(logserver, &nc);
	if (id <= 0) {
		fprintf(stderr, "%s: couldn't join server %s\n", pgm, logserver);
		exit(4);
	}

	/*
	 * N.B. For the moment disable the reading of all events
	 *  if we need to process incoming events, for some reason, we
	 *  should launch a separate thread, similar to the stdout/err
	 * "reader" threads that get started in "start_agent".
	 * Note that start_agent does not return until the subproccess,
	 * i.e. the agent, terminates
	 */
	event_select_type(0, 0, ET_MAX);
     /*** No, not even these
	event_select_type(1, ET_AGENTD_MIN, ET_AGENTD_MAX);
     ***/
	event_receive_enable(0);
	event_flush(1);

	event_register(LOGGERCLASS, LOGGERSPEC, cmd);

	rc = start_agent(NULL, cmd, args, &pid);

	event_leave();

	exit(rc);
}

/*
 * Construct a file name path given a directory and a file name
 */
INTERNAL void
make_path(char *path, char *dir, char *file) {
	int n;

	strcpy(path, dir);

	n = strlen(path);
	if (!n || path[n-1] != '/') path[n++] = '/';
	strcpy(path+n, file);
}

/*
 * Process requests
 */
INTERNAL void
agent_request(EVENT_AGENTD_REQ *eagent) {
	int rc, pid, retres = 0;
	EVENT_AGENTD_STAT estat;

	if (Verbose)
	   printf("processing agentd request from %d\n", eagent->head.from);

	/* Dispatch based on subtype */
	switch(eagent->type) {
	  case AGENTD_CTL_START: /* Start up an agent */
		rc = validate(eagent->module, eagent->args);

		if (rc == AGENTD_STATUS_OK) rc = check_auth(eagent);

		if (rc == AGENTD_STATUS_OK) {
			/* Start a thread which waits for task to complete */
			if (Debug) printf("spawning agentman\n");
			rc = mp_task(agentman, eagent, 0);
			if (rc <= 0) {
				fprintf(stderr,
					"agent_request: mp_task failed (%d)!!!\n",
					rc);
				retres = rc;
				rc     = AGENTD_STATUS_RUNERR;
				break;
			}
			else {
				if (Debug) printf("spawned agentman - rc = %d - waiting for initialization\n", rc);
				mp_decsema(AgentInit);
				if (Debug) printf("spawned agentman - finished initializing\n");
			}
		}

		/* On failure, send back notification */
		if (rc != AGENTD_STATUS_OK) { /* Send back error status */
			estat.reqtype  = AGENTD_CTL_START;
			estat.reqseq   = EventSeq(eagent);
			estat.tag      = eagent->tag;
			estat.flags    = 0;
			estat.result   = retres;
			estat.rstatus  = rc;
			estat.astatus  = AGENTD_AGENT_NOGO;

			SEND(EventFrom(eagent), AGENTD_STAT, estat);
			return;
		}

		break;


	  case AGENTD_CTL_QUERY:  /* Query available agents to this daemon */
		rewinddir(RootDir);

		rc = agent_query(eagent, RootDir, "");
		if (rc < 0) {
			estat.rstatus = AGENTD_STATUS_RUNERR;
			estat.astatus = AGENTD_AGENT_NOGO;
			estat.result  = 0;
		}
		else {
			estat.rstatus = AGENTD_STATUS_OK;
			estat.astatus = AGENTD_AGENT_NOGO;
			estat.result  = rc;
		}

		estat.reqtype = AGENTD_CTL_QUERY;
		estat.reqseq  = EventSeq(eagent);
		estat.tag     = eagent->tag;
		estat.astatus = 0;

		SEND(EventFrom(*eagent), AGENTD_STAT, estat);

		break;

	  /***** TO DO ******/
	  case AGENTD_CTL_STOP:
	  case AGENTD_CTL_VERIFY:
	  case AGENTD_CTL_CHECK:
	  case AGENTD_CTL_STATUS:
		printf("agentd request type %d not yet implemented\n", eagent->type);
	  default:
		fprintf(stderr, "agent_request: unknown agentd request type %d!!\n", eagent->type);
	}

	if (Verbose)
	   printf("completed agentd request from %d\n", eagent->head.from);

}

/*
 * Process queries - return list of available agent programs
 */
INTERNAL int
agent_query(EVENT_AGENTD_REQ *eagent, DIR *dir, char *dirpath) {
	EVENT_AGENTD_MOD emod;
	struct dirent   *dirent;
	struct stat      statb;
	char             path[PATHLEN];
	char            *bufp;
	int              ntot, nmod, len, bytes;
	int              rc;

	if (Verbose)
	  printf("processing agentd query from %d\n", eagent->head.from);

	ntot  = 0;
	bufp  = &emod.modules[0];
	bytes = AGENTD_LISTBUF-1;
	nmod  = 0;

	while (dirent = readdir(dir)) {
		if (*dirpath) sprintf(path, "%s/%s", dirpath, dirent->d_name);
		else          strcpy(path,  dirent->d_name);
		if (stat(path, &statb) != SYS_OK) {
			if (errno == EACCES) continue;
			fprintf(stderr, "error stating %s\n", path);
			continue;
		}

		if (Debug)
		  printf("agent_query: processing file %s %o\n", path, statb.st_mode);

		if (statb.st_mode&S_IFDIR) {
			DIR *subdir;

			if (dirent->d_name[0] == '.') continue;
			subdir = opendir(path);
			if (!subdir) {
				fprintf(stderr, "error opening subdir %s\n", path);
				continue;
			}
			ntot += agent_query(eagent, subdir, path);
			closedir(subdir);
			continue;
		}
		if (!(statb.st_mode&S_IFREG) ||
		    !strmatch(eagent->module, path) ||
		    access(path, X_OK)
		    ) continue;

		/* Copy module name to query event */
		len = strlen(path)+1;
		if (len >= AGENTD_LISTBUF-1) { /* This would be very, very bad */
			fprintf(stderr,
				"path too long for event buffer!!!\"%s\"\n",
				path);
			continue;
		}
		if (len > bytes) {      /* If no room send and start anew */
			*bufp         = '\0';
			emod.flags    = AGENTD_FLAG_MORE;
			emod.tag      = eagent->tag;
			emod.nmodules = nmod;
			event_clientname(emod.hostname);
			SEND(EventFrom(*eagent), AGENTD_MOD, emod);

			bytes = AGENTD_LISTBUF-1;
			nmod  = 0;
			bufp  = &emod.modules[0];
		}
		strcpy(bufp, path);
		bufp  += len;
		bytes -= len;
		++nmod;
	}
	if (nmod) {
		*bufp         = '\0';
		emod.flags    = 0;
		emod.tag      = eagent->tag;
		emod.nmodules = nmod;
		event_clientname(emod.hostname);
		SEND(EventFrom(*eagent), AGENTD_MOD, emod);
	}
	return(ntot+nmod);
}

/*
 * Validate command/agent path and argument list
 */
INTERNAL int
validate(char *file, char *args) {
	char *s;

	if (Verbose) printf("validating %s %s\n", file, args);

	if (*file == '/') return(AGENTD_STATUS_BADPATH);
	for (s = file; *s; ++s) {
		if (s[0] == '.' && s[1] == '.') return(AGENTD_STATUS_BADPATH);
		if (oneof(*s, "[]|><&*?!~`"))   return(AGENTD_STATUS_BADPATH);
	}
	if (access(file, X_OK) != SYS_OK) {
		if (access(file, F_OK) != SYS_OK) return(AGENTD_STATUS_NOFILE);
		else                              return(AGENTD_STATUS_NOEXEC);
	}

	if (oneof(*s, "<>&|*?![]{}")) return(AGENTD_STATUS_BADARGS);

	return(AGENTD_STATUS_OK);
}

INTERNAL int
oneof(char c, char *s) {
	while (*s) if (*s++ == c) return(1);
	return(0);
}

/*
 * Validate authorization
 * (This is the Auth/AuthZ hook)
 */
INTERNAL int
check_auth(EVENT_AGENTD_REQ *eagent) {
	if (Verbose) printf("check_auth: Authorizing %s-%s\n", eagent->requester, eagent->module);
	return(AGENTD_STATUS_OK);
}

INTERNAL void
agentman(EVENT_AGENTD_REQ *eagent) {
	int        pid, exitstat;
	int        me;
	int        rc;
	AGENT_REQ *areq;

	if (Debug)
	   printf("agentman: starting\n");

	/* Copy arg data into active Agent list & unblock parent */
	mp_lock(AgentLock);

printf("Allocating agent control block\n");
	areq = agent_alloc();
	if (!areq) {
		fprintf(stderr, "agentman: can't start %s\n", eagent->module);
		mp_exit(0);
	}
printf("Initializing agent control block\\n");

	memcpy(&areq->ereq, eagent, sizeof(EVENT_AGENTD_REQ));

	mp_unlock(AgentLock);

	/* Unblock parent thread */
	mp_incsema(AgentInit);

	if (Debug)
	   printf("agentman: starting as thread %d.%d\n", getpid(), mp_gettid());

	areq->pid    = 0;
	areq->status = AGENTD_AGENT_INIT;
	areq->start  = event_clock();

	if (areq->ereq.log[0] && areq->ereq.log[0] != ' ')
	    rc = start_logger(areq, areq->ereq.log, areq->ereq.module, areq->ereq.args, &pid);
	 else
	    rc = start_agent(areq, areq->ereq.module, areq->ereq.args, &pid);

	if (Debug)
	   printf("agentman: thread (%d.%d) done and exiting\n",
		   getpid(), mp_gettid());

	/* Copy to obiturary list */
	if (++Nrip == MAX_RIP) Nrip = 0;
	memcpy(Rip, areq, sizeof(AGENT_REQ));

	/* Remove entry for this agent */
	mp_lock(AgentLock);
	agent_free(areq);
	mp_unlock(AgentLock);

	mp_exit(0);
}

/*
 * Allocate an agent control block
 */
INTERNAL AGENT_REQ *
agent_alloc(void) {
	int i;

	if (Nagent == MAX_AGENT) return(NULL);

	for (i = Nagent; ; ) {
		if (Agent[i].status == AGENTD_AGENT_FREE) {
			Agent[i].status = AGENTD_AGENT_ALLOC;
			++Nagent;
			return(&Agent[i]);
		}
		if (++i == MAX_AGENT) i = 0;
		if (i == Nagent) break;
	}
	fprintf(stderr, "agent_alloc: Maximum (%d) agents already running\n", Nagent);
	return(NULL);
}

/*
 * Free an agent control block
 */
INTERNAL void
agent_free(AGENT_REQ *areq) {
	areq->status = AGENTD_AGENT_FREE;
	areq->pid    = 0;
	memclr(areq, sizeof(AGENT_REQ));
	--Nagent;
}

/*
 * Start up the logger proc and reader threads
 */
#ifndef win32
INTERNAL int
start_logger(AGENT_REQ *areq, char *logserver, char *cmd, char *args, int *retpid) {
	int   argc;
	int   logpid;
	int   rc, id, nc;

	if (Debug)
	   printf("starting logger as proc %d.%d for server %s\n",
		   getpid(), mp_gettid(), logserver);

	logpid = fork();
	if (logpid < 0) {
		fprintf(stderr, "start_logger: fork failed\n");
		perror("start_logger");
		return(0);
	}

	/* Parent process, send a response to requester and return */
	if (logpid > 0) {
		EVENT_AGENTD_STAT estat;
		if (areq) {
			areq->status = AGENTD_AGENT_RUNNING;
			areq->pid    = logpid;

			estat.reqseq  = EventSeq(areq->ereq);
			estat.tag     = areq->ereq.tag;
			estat.reqtype = AGENTD_CTL_START;
			estat.result  = logpid;
			estat.rstatus = AGENTD_STATUS_OK;
			SEND(EventFrom(areq->ereq), AGENTD_STAT, estat);
		}

		*retpid = logpid;

		return(AGENTD_STATUS_OK);
	}

	/*
	 * We are now in the logger child proc
	 *  - leave agent event server
	 *  - join logger server
	 *  - start agent task
	 */
	event_exit_();
	mp_fini();

	mp_init();

	fprintf(stderr, "logger: running as proc %d.%d\n", getpid(), mp_gettid());
	Exitlock = mp_alloclock();
	Exitwait = mp_allocsema();

	event_tunnel_enable(0);
	event_init();
	event_threadsafe(EVENT_SAFE_ALL);

	id = event_join(logserver, &nc);
	if (!id) {
		fprintf(stderr, "start_logger: couldn't join %s\n", logserver);
		exit(8);
	}

	/*
	 * N.B. For the moment disable the reading of all events
	 *  if we need to process incoming events, for some reason, we
	 *  should launch a separate thread, similar to the stdout/err
	 * "reader" threads that get started in "start_agent".
	 * Note that start_agent does not return until the subproccess,
	 * i.e. the agent, terminates
	 */
	event_select_type(0, 0, ET_MAX);
     /*** No, not even these
	event_select_type(1, ET_AGENTD_MIN, ET_AGENTD_MAX);
     ***/
	event_receive_enable(0);
	event_flush(1);

	fprintf(stderr, "logger: %d.%d joined %s as client %d\n",
		getpid(), mp_gettid(), logserver, id);

	event_register(LOGGERCLASS, LOGGERSPEC, cmd);

	rc = start_agent(NULL, cmd, args, retpid);

	fprintf(stderr, "logger: %d.%d done and exiting with %d\n",
		getpid(), mp_gettid(), rc);

	event_leave();

	exit(rc);
}
#else
/*
 * Use helper app on Cygwin/win32 systems
 */
INTERNAL int
start_logger(AGENT_REQ *areq, char *logserver, char *cmd, char *args, int *retpid) {
	char shcmd[AGENTD_PATHLEN];
	char opts[AGENTD_PATHLEN];
	int  pid;
	int  rc;

	opts[0] = '\0';

	sprintf(shcmd, "%s %s %s %s %s &",
		Logger,
		opts,
		logserver,
		cmd,
		args);

	if (Verbose) printf("start_logger: using helper app \"%s\"\n", shcmd);
	rc = !system(shcmd);

	pid = 0; /* Should really use fork,exec and get PID */

	if (areq) {
		EVENT_AGENTD_STAT estat;

		areq->status = AGENTD_AGENT_RUNNING;
		areq->pid    = pid;

		estat.reqseq  = EventSeq(areq->ereq);
		estat.tag     = areq->ereq.tag;
		estat.reqtype = AGENTD_CTL_START;
		estat.result  = pid;
		estat.rstatus = rc?AGENTD_STATUS_OK:AGENTD_STATUS_RUNERR;
		SEND(EventFrom(areq->ereq), AGENTD_STAT, estat);
	}

	return(rc);
}
#endif

/*
 * Start up the process with the given args
 */
INTERNAL int
start_agent(AGENT_REQ *areq, char *cmd, char *args, int *retpid) {
	int   argc;
	char *argv[MAX_ARGS];
	char  hostname[AGENTD_NAMELEN];
	int   childpid;
	int   pid, exitstat;
	int   pipeout[2], pipeerr[2];
	int   rc;
	EVENT_AGENTD_STAT estat;
	EVENT_AGENTD_LOG  elogout, elogerr;
	char  msg[200];
	char  cmdpath[PATHLEN];

	if (WorkDir) make_path(cmdpath, ExecPath, cmd);
	else         strcpy(cmdpath, cmd);

	argv[0] = cmd;
	argc    = 1;

	if (Verbose) printf("0: %s (%s)\n", argv[0], cmdpath);
	rc = parse_args(args, argv, 1);
	if (rc < 0) {
		return(0);
	}
	argc = rc;

/**************************/
/****** START DEBUG *******/
#ifdef FAKE_THIS_FOR_DEBUGGING
printf("start_agent: sleeping\n");
	if (areq) {
		areq->status = AGENTD_AGENT_RUNNING;
		areq->pid    = 9999999;

		estat.reqseq  = EventSeq(areq->ereq);
		estat.tag     = areq->ereq.tag;
		estat.reqtype = AGENTD_CTL_START;
		estat.result  = 9999999;
		estat.rstatus = AGENTD_STATUS_OK;
		SEND(EventFrom(areq->ereq), AGENTD_STAT, estat);
	}
sleep(10);
	if (areq) {
		areq->status = AGENTD_AGENT_EXIT;

		estat.result  = 0;
		estat.rstatus = AGENTD_AGENT_EXIT;
		SEND(EventFrom(areq->ereq), AGENTD_STAT, estat);
	}
printf("start_agent: exiting\n");
return(AGENTD_STATUS_OK);
#endif
/******** END DEBUG *******/
/**************************/

	if (pipe(pipeout) != SYS_OK) {
		fprintf(stderr, "start_agent: couldn't create stdout pipe\n");
		perror("start_agent");
		return(AGENTD_STATUS_RUNERR);
	}
	if (pipe(pipeerr) != SYS_OK) {
		fprintf(stderr, "start_agent: couldn't create stderr pipe\n");
		perror("start_agent");
		return(AGENTD_STATUS_RUNERR);
	}

	childpid = fork();
	if (childpid < 0) {        /* Error occurred */
		fprintf(stderr, "start_agent: fork failed\n");
		perror("start_agent");

		close(pipeout[0]);
		close(pipeout[1]);
		close(pipeerr[0]);
		close(pipeerr[1]);

		return(AGENTD_STATUS_RUNERR);
	}

	/*
	 * Exec command from newly forked process
	 */
	if (childpid == 0) {       /* Child process */
		int   fdin;
		int   argc;
		int   rc;

	   /****
		event_exit_();
	    ****/

		if (WorkDir) chdir(WorkPath);

		/* stdin */
		fdin = open("/dev/null", O_RDONLY);
		dup2(fdin, 0);
		close(fdin);

		/* stdout */
		rc = dup2(pipeout[1], 1);
		close(pipeout[0]);
		close(pipeout[1]);
		if (rc < 0) printf("start_agent: couldn't dup pipe as stdout");

		/* stderr */
		rc = dup2(pipeerr[1], 2);
		close(pipeerr[0]);
		close(pipeerr[1]);
		if (rc < 0) printf("start_agent: couldn't dup pipe as stderr");

		/* Create a new process group */
		setpgrp();

		fprintf(stderr,
			"agentd: child proc %d, pgrp %d execing %s\n",
			 getpid(), getpgrp(), cmd);

		execv(cmdpath, argv);

		/* If exec returns it means an error occurred */
		close(0);
		close(1);
		close(2);

		exit(AGENTD_STATUS_RUNERR);
	}

	signal(SIGABRT, SIG_IGN);

	if (Verbose) printf("start_agent: created child process %d\n", childpid);

	/* Report startup status to requester */
	if (areq) {
		areq->status = AGENTD_AGENT_RUNNING;
		areq->pid    = childpid;

		estat.reqseq  = EventSeq(areq->ereq);
		estat.tag     = areq->ereq.tag;
		estat.reqtype = AGENTD_CTL_START;
		estat.result  = childpid;
		estat.rstatus = AGENTD_STATUS_OK;
		SEND(EventFrom(areq->ereq), AGENTD_STAT, estat);
	}

	gethostname(hostname, sizeof(hostname));

	/* Set up stderr */
	elogerr.head.to = EVENT_BCAST_NOTME;
	elogerr.type    = AGENTD_LOG_STDERR;
	elogerr.flags   = pipeerr[0];
	elogerr.pid     = childpid;
	strcpy(elogerr.hostname, hostname);
	strcpy(elogerr.program,  cmd);

	/* Start reader thread and wait for it to initialize */
	if (Verbose) printf("start_agent: starting reader thread for stderr\n");
	mp_task(reader, &elogerr, 0);
	mp_decsema(ReadInit);

	/* Broadcast a message */
	sprintf(elogerr.msg, "agentd: agent started as pid %d\n", childpid);
	SEND(EVENT_BCAST_NOTME, AGENTD_LOG, elogerr);

	/* Now do the same for stdout */
	elogout.head.to = EVENT_BCAST_NOTME;
	elogout.type    = AGENTD_LOG_STDOUT;
	elogout.flags   = pipeout[0];
	elogout.pid     = childpid;
	strcpy(elogout.hostname, hostname);
	strcpy(elogout.program,  cmd);

	if (Verbose) printf("start_agent: starting reader thread for stdout\n");
	mp_task(reader, &elogout, 0);
	mp_decsema(ReadInit);

	/* Wait for child (agent) process to exit */
	if (Verbose) printf("start_agent: waiting for agent proc %d\n", childpid);
	pid = waitpid(childpid, &exitstat, 0);
	if (pid <= 0) {
		perror("start_agent: waitpid");
	}

	if (WIFSIGNALED(exitstat))
	   sprintf(msg, "agent %d exited on signal %s with rc %d",
		childpid,
		signame(WTERMSIG(exitstat)),
		WEXITSTATUS(exitstat));
	else
	   sprintf(msg, "agent %d exited normally with rc %d",
		childpid, WEXITSTATUS(exitstat));

	if (Verbose) printf("start_agent: %s\n", msg);

	/* Set exit and write to pipe to wake up readers */
	mp_lock(Exitlock);

	Exit = childpid;

	sleep(5); /* delay to let things quiesce */

	write(pipeout[1], EofStr, strlen(EofStr)+1);
	write(pipeerr[1], EofStr, strlen(EofStr)+1);

	if (Verbose) printf("start_agent: %d.%d waiting for 2 %d.readers to exit\n", getpid(), mp_gettid(), childpid);
	mp_decsema(Exitwait);
	if (Verbose) printf("start_agent: %d.%d waiting for 1 %d.reader to exit\n", getpid(), mp_gettid(), childpid);
	mp_decsema(Exitwait);
	if (Verbose) printf("start_agent: %d.%d all %d.readers exited\n", getpid(), mp_gettid(), childpid);

	Exit = 0;
	mp_unlock(Exitlock);

	/* Broadcast a message */
	sprintf(elogerr.msg, "agentd: %s\n", msg);
	SEND(EVENT_BCAST_NOTME, AGENTD_LOG, elogerr);

	close(pipeout[0]);
	close(pipeout[1]);
	close(pipeerr[0]);
	close(pipeerr[1]);

	/*
	 * Report exit status back to requester
	 */
	if (areq) {
		areq->status = AGENTD_AGENT_EXIT;

		estat.result  = exitstat;
		estat.rstatus = AGENTD_AGENT_EXIT;
		SEND(EventFrom(areq->ereq), AGENTD_STAT, estat);
	}

	if (retpid) *retpid = childpid;

	return(AGENTD_STATUS_OK);
}


INTERNAL int
parse_args(char *args, char *argv[], int argc) {
	while (*args) {
		while (*args && iswhite(*args)) ++args;
		if (!*args) break;
		if (*args == '"') {
			argv[argc++] = ++args;
			while (*args && *args != '"') ++args;
			if (*args) *args++ = '\0';
		}
		else {
			argv[argc++] = args;
			while (*args && !iswhite(*args)) ++args;
			if (*args) *args++ = '\0';
		}
		if (Verbose) printf("%d: %s\n", argc-1, argv[argc-1]);
	}
	argv[argc] = NULL;

	return(argc);
}

INTERNAL void
ignore_sigpipe(int fd) {
	if (Verbose) printf("Got sigpipe on %d\n", fd);
}

INTERNAL void
reader(EVENT_AGENTD_LOG *etmp) {
	EVENT_AGENTD_LOG elogsaved, *elog;
	int  fd, to;
	int  eof, checkexit;
	int  i, rc;
	char c;

	memcpy(&elogsaved, etmp, sizeof(EVENT_AGENTD_LOG));

	mp_incsema(ReadInit);

	elog = &elogsaved;

	fd = elog->flags;
	to = elog->head.to;
/****
	on_sigpipe(fd, ignore_sigpipe);
 ****/

	elog->msg[0] = '\0';

	if (Verbose)
	  printf("reader: proc %d.%d monitoring %d.%s on fd %d, sending to client %d\n",
		   getpid(), mp_gettid(),
		   elog->pid,
		   elog->type==AGENTD_LOG_STDERR?"stderr":"stdout",
		   fd, to);

	eof = checkexit = 0;
	FOREVER {
		for (i = 0; i < AGENTD_MAXMSG-1; ) {
			rc = read(fd, &c, 1);
			if (rc < 0) {
				if (Exit != elog->pid) {
					fprintf(stderr, "reader: error reading fd %d\n", rc);
					perror("reader");
				}
				eof = 1;
				break;
			}
			if (rc == 0) {
				if (Debug) printf("reader: eof\n");
				eof = 1;
				break;
			}
			if (c == EofStr[0] && Exit == elog->pid) {
				eof = 1;
				break;
			}
			elog->msg[i++] = c;

			if (c == '\n') break;
			else checkexit = 0;
		}
		if (eof) break;

		elog->msg[i] = '\0';

		if (Verbose) {
			printf("%s [%d]: %s",
				elog->type==AGENTD_LOG_STDERR?"stderr":"stdout",
				fd, elog->msg);
			fflush(stdout);
		}
		event_send(to, ET_AGENTD_LOG, (EVENT *)elog, sizeof(*elog));
		i = 0;
	}

	/* Send any partial message */
	if (i != 0) {
		elog->msg[i] = '\0';

		if (Verbose) {
			printf("%s [%d]: %s",
				elog->type==AGENTD_LOG_STDERR?"stderr":"stdout",
				fd, elog->msg);
			fflush(stdout);
		}
		event_send(to, ET_AGENTD_LOG, (EVENT *)elog, sizeof(*elog));
	}

	if (Verbose)
	  printf("reader: close and exit fd %d on %s\n", fd,
		 (Exit == elog->pid)?"Exit PID":"eof");

	mp_incsema(Exitwait);

	close(fd);
	mp_exit(0);
}

/*
 * Miscellaneous signal stuff
 */
INTERNAL void
siginit(void) {
	int i;

	/* Fill in names for common signals */
	for (i = 0; i < NSIG; ++i) SigName[i] = "";
	SigName[SIGABRT]  = "Abort";
	SigName[SIGINT]   = "^C";
	SigName[SIGSTOP]  = "Stop";
	SigName[SIGKILL]  = "Kill";
	SigName[SIGUSR1]  = "User1";
	SigName[SIGUSR2]  = "User2";
	SigName[SIGSEGV]  = "SegV";
	SigName[SIGTERM]  = "Term";
	SigName[SIGHUP]   = "HUP";
	SigName[SIGFPE]   = "FPE";
}

/*
 * Return name of signal - WARNING not re-entrant
 */
INTERNAL char *
signame(int sig) {
	static char msg[100];

	if (sig < 0 || sig >= NSIG) {
		sprintf(msg, "Bad sig %d", sig);
		return(msg);
	}
	else if (SigName[sig] && *SigName[sig]) return(SigName[sig]);
	else {
		sprintf(msg, "signal=%d", sig);
		return(msg);
	}
}
#define PRT(opt,str) \
	printf(msg?"\t%s%s%s\n":"%s ", opt, (strlen(opt)<8)?"\t\t":"\t", str)

INTERNAL void
usage(char *pgm, int msg) {
	if (msg) printf("usage: %s [options] server\n", pgm);
	else     printf("usage: %s ", pgm);
	PRT("[-help]",          "display this message");
	PRT("[-quiet|-verbose]","DAFFIE noise control");
	PRT("[-debug]",         "turning on debugging mode");
	PRT("[-nodebug]",       "turning off debugging mode");
	PRT("[-dir path]",      "agent executable directory path");
	PRT("[-wd path]",       "agent working directory path");
	PRT("[-bin path]",      "same as -dir");
	PRT("[-cd path]",       "same as -wd");
	PRT("[-logger pgm]",    "logging job initiator");
	PRT("server",           "event server name");
	if (!msg) printf("\n");
}

INTERNAL void
logger_usage(char *pgm, int msg) {
	if (msg) printf("usage: %s [options] server\n", pgm);
	else     printf("usage: %s ", pgm);
	PRT("[-help]",          "display this message");
	PRT("[-quiet|-verbose]","DAFFIE noise control");
	PRT("[-debug]",         "turning on debugging mode");
	PRT("server",           "event server name");
	PRT("cmd",              "command to execute");
	PRT("[args]",           "command args");
	if (!msg) printf("\n");
}

