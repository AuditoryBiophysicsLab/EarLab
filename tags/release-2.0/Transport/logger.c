/*
 * DAFFIE logger
 *      Start a process and pipe output into event land
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <local/std.h>

#include <daffie/event.h>

#include "agentd.h"
#include "eventdef.h"

INTERNAL void usage(char *pgm, int msg);
INTERNAL int oneof(char c, char *s);

INTERNAL void agent_init(void) {
	/* Define agent events */
	event_verbose(0);
	event_init();
#include "agentddef.h"
}

#define PATHLEN   2500
#define SYS_OK    0
#define MAX_AGENT 100
#define MAX_ARGS  200

typedef struct {
	EVENT_AGENTD_REQ ereq;
	int              pid;
	int              cid;
} AGENT_STAT;

INTERNAL AGENT_STAT   Agent[MAX_AGENT];
INTERNAL int          Nagent;
INTERNAL volatile int Exit  = 0;
INTERNAL int          Quiet = 0;

INTERNAL void reader(EVENT_AGENTD_LOG *elog);

main(int argc, char *argv[]) {
	char  *pgm;
	int    ok, help, debug;
	char  *server;
	int    id, nc;
	int    i;
	int    pipeout[2], pipeerr[2];
	int    child, pid, estat;
	char   me[256], hostname[AGENTD_NAMELEN];
	char  *cmd, cmdname[AGENTD_NAMELEN], cmdbuf[4000], *cp;
	char  *args;
	EVENT_AGENTD_LOG elogout, elogerr;

	pgm = argv[0];
	++argv; --argc;

	ok    = 1;
	help  = 0;
	debug = 0;
	server = NULL;
	cmd    = "";
	Quiet  = 0;
	while (argc && *argv[0] == '-') {
		char *opt;

		opt = argv[0] + 1;
		++argv; --argc;

		if      (streq(opt, "-")) break;
		else if (streq(opt, "help"))  ++help;
		else if (streq(opt, "debug")) ++debug;
		else if (streq(opt, "quiet"))   Quiet = 1;
		else if (streq(opt, "verbose")) Quiet = 0;
		else if (streq(opt, "server")) {
			if (!argc) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					pgm, opt);
				ok = 0;
			}
			else {
				server = argv[0];
				++argv; --argc;
			}
		}
		else if (streq(opt, "exec")) {
			if (!argc) {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					pgm, opt);
				ok = 0;
			}
			else {
				cmd = argv[0];
				++argv; --argc;
			}
		}
		else {
			fprintf(stderr, "%s: ignoring argument -%s\n", pgm, opt);
			help = 1;
			ok   = 0;
		}
	}
	if (!ok || (argc == 0 && !*cmd) || !server) {
		usage(pgm, help);
		exit(!help);
	}

	strcpy(cmdbuf, cmd);

	while (argc) {
		strcat(cmdbuf, " ");
		strcat(cmdbuf, argv[0]);
		++argv; --argc;
	}

	cmd = cmdname; cp = cmdbuf;
	while (*cp &&  iswhite(*cp)) ++cp;
	while (*cp && !iswhite(*cp)) *cmd++ = *cp++;
	*cmd = '\0';

	args = cp;
	while (*args && iswhite(*args)) ++args;

	cuserid(me);
	gethostname(hostname, sizeof(hostname));

	if (pipe(pipeout) != SYS_OK) {
		fprintf(stderr, "%s: couldn't create stdout pipe\n");
		perror(pgm);
		exit(1);
	}
	if (pipe(pipeerr) != SYS_OK) {
		fprintf(stderr, "%s: couldn't create stderr pipe\n");
		perror(pgm);
		exit(1);
	}

	child = fork();
	if (child < 0) {
		fprintf(stderr, "%s: fork failed\n", pgm);
		perror(pgm);
		exit(1);
	}

	if (child == 0) {
		int   fdin;
		int   argc;
		char *argv[MAX_ARGS];

		printf("child process running as %d - %s %s\n", getpid(), cmdname, args);

		/* stdin */
		fdin = open("/dev/null", O_RDONLY);
		close(0);
		dup2(fdin, 0);
		close(fdin);

		/* stdout */
		close(1);
		dup2(pipeout[1], 1);
		close(pipeout[1]);

		/* stderr */
		close(2);
		dup2(pipeerr[1], 2);
		close(pipeerr[1]);

		argv[0] = cmdname;
		argc    = 1;

		while (*args) {
			while (*args && iswhite(*args)) ++args;
			if (!*args) break;
			argv[argc++] = args;
			while (*argv && !iswhite(*args)) ++args;
			*args++ = '\0';
		}
		argv[argc] = NULL;

		execv(cmdname, argv);

		exit(0);

	}
	printf("%s: started process %d\n", pgm, child);

	/* local initialization */
	agent_init();

	id = event_join(server, &nc);
	if (!id) {
		fprintf(stderr, "%s: couldn't join server \"%s\"\n", server);
		exit(4);
	}
	printf("%s: joined %s as client id %d (%d clients)\n", pgm, server, id, nc);

	event_register("agent", "logger", cmd);

	event_receive_enable(0);
	event_flush(1);

	event_threadsafe(EVENT_SAFE_ALL);

	event_select_type(0, 0, ET_MAX);
	event_select_type(1, ET_AGENTD_MIN, ET_AGENTD_MAX);

	elogout.head.to = EVENT_BCAST_ALL;
	elogout.type    = AGENTD_LOG_STDOUT;
	elogout.flags   = pipeout[0];
	elogout.pid     = child;
	strcpy(elogout.hostname, hostname);
	strcpy(elogout.program,  cmdname);

strcpy(elogout.msg, "Logger started\n"); BCAST(AGENTD_LOG, elogout);

	mp_task(reader, &elogout, 0);

	elogerr.head.to = EVENT_BCAST_ALL;
	elogerr.type    = AGENTD_LOG_STDERR;
	elogerr.flags   = pipeerr[0];
	elogerr.pid     = child;
	strcpy(elogerr.hostname, hostname);
	strcpy(elogerr.program,  cmdname);

	mp_task(reader, &elogerr, 0);

	printf("%s: waiting for child to exit\n", pgm);
	pid = wait(&estat);
	printf("%s: process %d exited with %d\n", pgm, pid, estat&0xff);

	sleep(10);
	Exit = 1;
	sleep(1);

	close(pipeout[0]);
	close(pipeerr[0]);

	event_leave();

}

INTERNAL void
reader(EVENT_AGENTD_LOG *etmp) {
	EVENT_AGENTD_LOG *elog = etmp;
	int  fd = elog->flags, to = elog->head.to;
	int  eof = 0;
	int  i, rc;
	char c;

	elog->msg[0] = '\0';

	if (!Quiet)
	   printf("reader: thread %d fd %d, sending to client %d\n",
		   mp_gettid(), fd, to);
	while (!eof && !Exit) {
		for (i = 0; i < AGENTD_MAXMSG-1; ) {
			rc = read(fd, &c, 1);
			if (rc < 0 && !Exit) {
				fprintf(stderr, "reader: error reading fd %d\n", fd);
				perror("reader");
				return;
			}
			if (rc == 0) { printf("reader: eof\n"); eof = 1; break; }
			elog->msg[i++] = c;
			if (c == '\n') break;
		}
		elog->msg[i] = '\0';

		if (!Quiet) {
			printf("%s [%d]: %s",
				elog->type==AGENTD_LOG_STDERR?"stderr":"stdout",
				fd, elog->msg);
			fflush(stdout);
		}
		event_send(to, ET_AGENTD_LOG, (EVENT *)elog, sizeof(*elog));
	}

	if (!Quiet) printf("reader: fd %d exiting (Exit=%d)\n", fd, Exit);

	mp_exit(0);
}


INTERNAL int
oneof(char c, char *s) {
	while (*s) if (*s++ == c) return(1);
	return(0);
}


#define PRT(opt,str) \
	printf(msg?"\t%s%s%s\n":"%s ", opt, (strlen(opt)<8)?"\t\t":"\t", str)

INTERNAL void
usage(char *pgm, int msg) {
	if (msg) printf("usage: %s [options] -server hostname -- command\n", pgm);
	else     printf("usage: %s ", pgm);
	PRT("[-help]",          "display this message");
	PRT("[-debug]",         "turning on debugging mode");
	PRT("[-dir path]",      "agent directory path");
	PRT("-server host",     "event server name");
	PRT("command",          "command to execute");
	if (!msg) printf("\n");
}


