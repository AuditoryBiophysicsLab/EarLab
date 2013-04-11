/*
 * Spew EFI data to socket connections
 */

#ifdef WINSOCK
  /* Using WINDOWS header files - must #define WINDOWS before other headers */
# define WINDOWS
  /* The following causes fd_set and friends to be defined correctly          */
# define __USE_W32_SOCKETS
#endif

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdarg.h>

#include <local/std.h>
#include <local/machin.h>

#ifdef sgi
# define _SGI_MP_SOURCE
#endif

/*
 * Socket stuff
 */
/*
 * Socket Stuff
 */
#ifdef WINSOCK
/*******************/
/* Windows Sockets */
/*******************/
# include "winsock2.h"
# include "ws2tcpip.h"

typedef fd_set FDSET;
EXTERNAL wsa_perror(char *msg);

#define SockSend(fd,buf,len) send(fd, (char *)(buf), len, 0)
#define SockRecv(fd,buf,len) recv(fd, (char *)(buf), len, 0)
#define SockErr(msg) wsa_perror(msg)
#define SockErrNo    WSAGetLastError()

#define on_sigpipe(fd,fnc) /* Don't do this with WINSOCK */

#else
/**********************/
/* Posix/Unix Sockets */
/**********************/
#include <unistd.h>

#ifdef _BSD
# define _BSD 42
#endif

#include <netdb.h>

#if (!(defined(sun)||defined(win32)))
# include <sys/select.h>
#endif

#include <sys/socket.h>
#include <netinet/in.h>

#if (defined(sgi) || defined(aix) || defined(linux))
#include <netinet/tcp.h>
#endif

#include <sys/types.h>

#ifdef aix
# define MAX_FDS 2048
# define FD_SIZE (MAX_FDS/sizeof(int))

  typedef struct sellist {
	int fd[FD_SIZE];
	int msgs[1];
  } FDSET;
#else
  typedef fd_set FDSET;
#endif

#define SockSend(fd,buf,len) send(fd, buf, len, 0)
#define SockRecv(fd,buf,len) recv(fd, buf, len, 0)
#define SockErr(msg) perror(msg)
#define SockErrNo errno

#endif

/*************************************/
/*** End of socket related headers ***/
/*************************************/

#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/file.h>

#ifdef aix
# include <sys/ioctl.h>
#endif
#ifdef sgi
# include <sys/syssgi.h>
#endif
#include <sys/time.h>

#if (defined(rs6000p4))
# define MEMSYNC mem_sync()
#else
# define MEMSYNC /* Don't need no stinking mem sync */
#endif

/*
 * FD set stuff for "select" and friends
 */
#define FDINIT(set)     FD_ZERO(set);
#define FDADD(fd, set)  FD_SET(fd, set)
#define FDTEST(fd, set) FD_ISSET(fd, set)

#include <daffie/event.h>
#include <daffie/eventdef.h>
#include <daffie/eclient.h>
#include "efievent.h"

typedef struct sockaddr_in SOCKADDR;
typedef struct hostent     HOST;
typedef struct servent     SERVENT;
typedef struct timeval     TIMEVAL;

#define SERVICE    "spew"
#define DEFLTPORT   9080
#define MAXCLIENT    100
#define K           1024
#define RBUFSIZE    10*K
#define WBUFSIZE    20*K

#ifdef DEBUG
INTERNAL int Debug = 1;
#else
INTERNAL int Debug = 0;
#endif

typedef struct {
	char  hostname[64];     /* host name                             */
	int   id;               /* client ID                             */
	FD    sockfd;           /* socket file descriptor                */
	int   alive;            /* client is alive                       */
	int   jointime;         /* wallclock time of join                */
	int   partial;          /* number of partial writes              */
	int   wrote;            /* total bytes written                   */
	int   read;             /* total bytes read                      */
} CLIENT;

INTERNAL CLIENT    Clientdata[MAXCLIENT];/* client information          */
INTERNAL CLIENT   *Client[MAXCLIENT];   /* pointers into above          */
INTERNAL CLIENT   *Wclient;             /* Active write client          */
INTERNAL FD        Sockid;              /* server socket id             */
INTERNAL FD        Socksize = 256;      /* socket window size in KB     */
INTERNAL int       Nclient = 0;         /* number of clients            */
INTERNAL int       Id = 1;              /* client id sequence           */
INTERNAL int       Text = 0;            /* text/binary flag             */
INTERNAL int       FileWait = 0;        /* wait for file flag           */

INTERNAL int       Dead = 0;            /* number of dead clients       */

INTERNAL int       Quit = 0;

INTERNAL char     *Pgm;                 /* this program name            */
INTERNAL char     *Errmsg;              /* global error messages        */

INTERNAL void   init_data(CLIENT *client, char *initfile);
INTERNAL int    send_efidata(CLIENT *client, EVENT_EFI_DATA *edata, int text, int *err);
INTERNAL int    send_wavedata(CLIENT *client,  TMatrixN_T *data, int nitem, int text, int *err);
INTERNAL int    send_spikedata(CLIENT *client, TSpike *data, int nitem, int text, int *err);
INTERNAL int    send_bindata(CLIENT *client,   BYTE *buf, int nbyte, int *err);
INTERNAL int    setup(char *pgm, int port);
INTERNAL void   client_died(char *where, CLIENT *client);
INTERNAL void   monitor(void);
INTERNAL void   bury_client(void);
INTERNAL void   client_stats(char *who, CLIENT *client);
INTERNAL void   died(char *arg);
INTERNAL int    ioblock(FD fd, int flag);
INTERNAL int    iowait(FD fd, int millisec, int rdflag, int wrflag);
INTERNAL void   addr_str(int no, char *str);
INTERNAL void   setprobe(char *spec);
INTERNAL void   probe_error(char *s);
INTERNAL void   efi_init(void);
INTERNAL void   usage(char *pgm, int msg);

#define BADRC(rc) ((rc==0) || (rc<0&&errno!=EWOULDBLOCK&&errno!=EAGAIN))

main(int argc, char *argv[]) {
	char    *probe;
	char    *initfile;
	unsigned int len;
	CLIENT  *client;
	SOCKADDR clientsock;
	int      clientfd;
	char     clientname[128];
	char     buf[256];
	HOST    *host;
	int      port;
	int      ok, help;
	int      i, rc;

	/* Snarf program name */
	if (Pgm = strrchr(argv[0], '/')) ++Pgm;
	else Pgm = argv[0];
	++argv; --argc;

	port     = 0;           /* use default port       */
	initfile = NULL;        /* no initial client file */
	help     = 0;
	ok       = 1;
	while (argc && argv[0][0] == '-') {
		char *opt;

		opt = argv[0] + 1;
		++argv; --argc;

		if      (substr(opt, "debug"))   ++Debug;
		else if (substr(opt, "wait"))    FileWait = 1;
		else if (substr(opt, "nodebug")) Debug = 0;
		else if (substr(opt, "text"))    Text = 1;
		else if (substr(opt, "ascii"))   Text = 1;
		else if (substr(opt, "binary"))  Text = 0;
		else if (substr(opt, "port")) {
			if (!argc || !isdigit(argv[0][0])) {
				fprintf(stderr, "%s: missing arg to -%s\n",
					Pgm, opt);
				exit(1);
			}
			port = atoi(argv[0]);
			++argv, --argc;
		}
		else if (substr(opt, "file")) {
			if (!argc || argv[0][0] == '-') {
				fprintf(stderr, "%s: missing arg to -%s\n",
					Pgm, opt);
				exit(1);
			}
			initfile = argv[0];
			++argv, --argc;
		}
		else {
			fprintf(stderr, "%s: unknown option \"%s\"\n", Pgm, opt);
		}
	}

	if (!ok || !argc || help) {
		usage(Pgm, help);
		exit(!help);
	}
	probe  = argv[0];

	/* Initialize clients */
	for (i = 0; i < MAXCLIENT; ++i) {
		strcpy(Clientdata[i].hostname, "none");
		Clientdata[i].sockfd = -1;
		Clientdata[i].alive  = 0;
		Client[i] = &Clientdata[i];
	}
	Nclient = 0;

	/* Ignore broken connections */
	signal(SIGPIPE, (SIGFNCP)died);

	/* Set up the socket */
	if (!(port=setup(Pgm, port))) {
		printf("%s: %s\n", Pgm, Errmsg);
		perror(Pgm);
		exit(12);
	}

	printf("Listening for connections on %d\n", port);

	/* Locks for modifying the Client structure */
	Dead = 0;

	/* Set probe, join world */
	setprobe(probe);

	/*
	 * Wait for and accept connections
	 */
	while (!Quit) {
		struct timeval start, end;
		int        rtt;
		int        ipaddr;
		CLIENT    *client;
		EVENT      event;
		EVENT_EFI_DATA     *edata;
		EVENT_EFI_WAVEDATA *ewave;

		while (event_receive(&event)) {
			int   nitem;
			int   err;

			if (EventType(event) != ET_EFI_WAVEDATA &&
			    EventType(event) != ET_EFI_SPIKEDATA) {
				printf("%s: receieved event type %d from client %d\n",
					Pgm, EventType(event), EventFrom(event));
				continue;
			}

			edata = (EVENT_EFI_DATA *)&event;

			nitem = edata->indexN - edata->index0 + 1;

			if (Debug)
			   printf("sending %d %s %d items from %d to clients\n",
				   Text?"text":"binary",
				   StreamTypeName(edata->type),
				   nitem,
				   EventFrom(edata));

			for (i = 0; i < Nclient; ++i) {
				send_efidata(Client[i], edata, nitem, &err);
			}

		}
		if (iowait(Sockid, 10, 1, 0)) {
			len = sizeof(clientsock);
			bzero(&clientsock, len);
			clientfd = accept(Sockid, (struct sockaddr *)&clientsock, &len);
			if (BADRC(clientfd)) {
				printf("%s: can't accept connection\n", Pgm);
				perror(Pgm);
				exit(12);
			}
		}
		else clientfd = -1;

		/* Bury any clients that have died */
		if (Dead) bury_client();

		if (clientfd < 0) continue;

		/* Get clients name */
		ipaddr = ntohl(clientsock.sin_addr.s_addr);
		addr_str(ipaddr, buf);

#if defined(sgi)
		host = gethostbyaddr(&ipaddr, 4, AF_INET);
#else
#if defined(aix4)
		host = gethostbyaddr(ipaddr, 4, AF_INET);
#else
		host = gethostbyaddr(buf, strlen(buf), AF_INET);
#endif
#endif

		if (host != NULL) strcpy(clientname, host->h_name);
		else              strcpy(clientname, buf);

		printf("%s: Accepted connection from %s\n", Pgm, clientname);

		client = Client[Nclient];

		strcpy(client->hostname, clientname);
		client->id       = Id++;
		client->sockfd   = clientfd;
		client->alive    = 1;
		client->jointime = event_clock();
		client->partial  = 0;
		client->read     = 0;
		client->wrote    = 0;

		MEMSYNC;

		++Nclient;

		if (initfile) init_data(client, initfile);
	}
	exit(0);
}

INTERNAL void
init_data(CLIENT *client, char *initfile) {
	static FILE *f = NULL;
	BYTE   buf[1024];
	int    n, err;;

	while (!f) {
		f = fopen(initfile, "r");
		if (!f) {
			if (FileWait) {
				printf("init_data: Waiting for %s\n", initfile);
				sleep(1);
				continue;
			}
			fprintf(stderr,
				"%s: couldn't open initialization file %s\n",
				Pgm, initfile);
			exit(1);
		}
	}

printf("sending initfile to %s ...", client->hostname); fflush(stdout);
	while (!feof(f) && (n = fread(buf, 1, sizeof(buf), f)) > 0) {
		 send_bindata(client, buf, n, &err);
	}

	/* Send and EOF (^D) */
	buf[0] = 4;
	send_bindata(client, buf, 1, &err);

printf("done\n");
	rewind(f);
}

/*
 * Send EFI data to a DAFFIE client
 *      dispatch based on underlying type
 */
INTERNAL int
send_efidata(CLIENT *client, EVENT_EFI_DATA *edata, int text, int *errp) {
	int rc, nitem = -1;

	switch (edata->type) {
	  case EFI_STREAM_WAVEFORM: {
		EVENT_EFI_WAVEDATA *ewave;

		ewave = (EVENT_EFI_WAVEDATA *)edata;
		nitem = ewave->indexN - edata->index0 + 1;

		rc = send_wavedata(client, ewave->data, nitem, text, errp);
		break;

	  }
	  case EFI_STREAM_SPIKE: {
		EVENT_EFI_SPIKEDATA *espike;

		espike = (EVENT_EFI_SPIKEDATA *)edata;
		nitem = espike->indexN - edata->index0 + 1;

		rc = send_spikedata(client, espike->data, nitem, text, errp);
	  }
	  case EFI_STREAM_NONE:
		fprintf(stderr, "send_efidata: NON TYPED STREAM");
		rc = *errp = 0;
		break;

	  default:
		fprintf(stderr, "send_efidata: *INVALID STREAM TYPE %d", edata->type);
		rc = *errp = 0;
	}

	return(rc == nitem);
}

/*
 * Send waveform data
 */
INTERNAL int
send_wavedata(CLIENT *client, TMatrixN_T *data, int nitems, int text, int *errp) {
	BYTE  buf[100];
	int   i, size, len, rc;
	char *fmt;

	size = sizeof(TMatrixN_T);

	/* If binary, use common sender */
	if (!Text) {
		 len = nitems * size;
		 rc = send_bindata(client, (BYTE *)data, len, errp);
		 return(rc/size);
	}

	if (size == 4) fmt = "%f\n";
	else           fmt = "%lf\n";

	for (i = 0; i < nitems; ++i) {
		sprintf(buf, fmt, data[i]);
		len = strlen(buf);
		if (!send_bindata(client, buf, len, errp)) return(i);
	}
	return(i);
}

/*
 * Send spike data
 */
INTERNAL int
send_spikedata(CLIENT *client, TSpike *data, int nitems, int text, int *errp) {
	BYTE  buf[100];
	int   i, size, len, rc;
	char *fmt;

	if (!nitems) return(0);

	size = sizeof(TSpike);

	/* If binary, use common sender */
	if (!Text) {
		 len = nitems * size;
		 rc = send_bindata(client, (BYTE *)data, len, errp);
		 return(rc/size);
	}
	if (sizeof(Spike_T) == 4) fmt = "%d %f\n";
	else                      fmt = "%d %lf\n";

	for (i = 0; i < nitems; ++i) {
		sprintf(buf, fmt, data[i].cell, data[i].time);
		len = strlen(buf);
		if (!send_bindata(client, buf, len, errp)) return(i);
	}
	return(i);
}

/*
 * Send binary data
 */
INTERNAL int
send_bindata(CLIENT *client, BYTE *buf, int n, int *errp) {
	int left, rc;

	if (!n) { *errp = 0; return(0); }

	left = n;
	while (left) {
		Wclient = client;
		rc = SockSend(client->sockfd, buf, left);
		if (rc == left) return(n);

		if (rc < 1) break; /* Error occurred */

		left -= rc;
		buf  += rc;
	}

#ifdef WINSOCK
	if (rc == SOCKET_ERROR) {
		*errp = WSAGetLastError();
		WSASetLastError(0);

		if (!(*errp == WSAEWOULDBLOCK || *errp == WSAEMSGSIZE)) return(0);

		 printf("send_bindata: error sending to %s\n", client->hostname);
		++Dead;
	}
#else
	if (BADRC(rc)) {
		*errp = SockErrNo;
		 printf("send_bindata: error sending to %s\n", client->hostname);
		++Dead;
		return(0);
	}
#endif
	*errp = EWOULDBLOCK;

	return(0);
}


#define PREFIX "daffie://"

INTERNAL void
setprobe(char *spec) {
	char server[100];
	char instname[100];
	char *s, *t;
	int  param;
	int  id, nc;
	int  clientid = 0;
	ECLIENT_LIST  clients[MAX_CLIENT];
	EVENT_EFI_CTL eprobe;

	s = spec;

	if (substr(PREFIX, s)) s += strlen(PREFIX);
	if (substr("//", s))   s += 2;

	t = server;
	while (*s && *s != '/') *t++ = *s++;
	*t = '\0';

	if (*s++ != '/') probe_error(spec);
	if (*s == '/') ++s;

	t = instname;
	while (*s != '.') *t++ = *s++;
	*t = '\0';

	if (*s++ != '.') probe_error(spec);

	if (isdigit(instname[0])) clientid = atoi(instname);

	param = atoi(s);
	if (!param) probe_error(spec);

	printf("setprobe: setting probe daffie://%s/%s.%d\n",
		server, instname, param);

	efi_init();

	id = event_join(server, &nc);
	if (!id) {
		fprintf(stderr, "%s: couldn't join server \"%s\"\n", Pgm, server);
		exit(4);
	}
	printf("%s: joined %s as client id %d (%d clients)\n", Pgm, server, id, nc);

	event_register("earlab", "control", Pgm);

	event_select_type(0, 0, ET_MAX);
	event_flush(1);

	eclient_monitor();
	if (!clientid)
	   nc = eclient_match(clients, "*", "earlab", "*", instname);
	else
	   nc = eclient_match_id(clients, clientid);

	if (nc < 1) {
		fprintf(stderr, "%s: couldn't find match for %s\n",
			Pgm, instname);
		event_leave();
		exit(1);
	}
	if (nc > 1) {
		fprintf(stderr, "%s: %s matched multiple (%d) clients\n",
			Pgm, instname, nc);
		event_leave();
		exit(1);
	}

	event_select_type(1, ET_EFI_WAVEDATA, ET_EFI_WAVEDATA);

	eprobe.type     = EFI_CTL_PROBE;
	eprobe.clientid = id;
	eprobe.tag      = getpid();
	eprobe.param    = param - 1;
	eprobe.flag     = 1;
	eprobe.value    = 0;

	SEND(clients[0].id, EFI_CTL, eprobe);
}

INTERNAL void
probe_error(char *s) {
	fprintf(stderr, "%s: invalid probe \"%s\"\n", Pgm, s);
	exit(4);
}

INTERNAL void
efi_init(void) {
	event_verbose(0);
	event_init();
#include "efieventdef.h"
#include "agentddef.h"
}

/*
 * Catch SIGPIPEs for I/O on dead sockets
 */
INTERNAL void
died(char *unused) {
	fprintf(stderr, "Somebody SIGPIPED\n");
	signal(SIGPIPE, (SIGFNCP)died);
	return;
}

/*
 * Deal with clients that have died
 */
INTERNAL void
client_died(char *where, CLIENT *client) {
	client->alive = 0;
	fprintf(stderr, "%s: client %s (%d) seems to have died (errno=%d)\n",
		where, client->hostname, client->id, errno);
	++Dead;
	return;
}

/*
 * Remove dead clients and broadcast a Leave event
 */
INTERNAL void
bury_client(void) {
	CLIENT *client;
	int i, j;

	printf("Cleaning up clients\n");

	for (i = 0; i < Nclient; ++i) {
		client = Client[i];
		if (client->alive) continue;

		printf("bury_client: removing %s (%d) from slot %d\n",
			client->hostname, client->id, i);
		client_stats("bury_client", client);

		close(client->sockfd);
		client->sockfd = -1;
		for (j = Nclient-1; i < j && !Client[j]->alive; --j, --Nclient) {
			printf("bury_client: removing %s (%d) from slot %d\n",
				Client[j]->hostname, Client[j]->id, j);
			client_stats("bury_client", Client[j]);
			close(Client[j]->sockfd);
			Client[j]->sockfd = -1;
		}
		--Nclient;
		if (i >= j) continue;
		Client[i]  = Client[j];
		Client[j]  = client;
		printf("bury_client: moved %s (%d) from slot %d to slot %d\n",
		       Client[i]->hostname, Client[i]->id, j, i);
	}
	printf("bury_client: %d clients left\n", Nclient);

	/* Reset Ids if no clients left */
	if (!Nclient) {
		Id  = 1;
	}
	Dead = 0;
}

INTERNAL void
client_stats(char *who, CLIENT *client) {
	printf("%s: %s alive for %d secs, read %d bytes, wrote %d bytes (%d partial writes)\n",
		who, client->hostname, time(0)-client->jointime,
		client->read, client->wrote, client->partial);
}

/*
 * Enable/disable blocking I/O on a file
 */
INTERNAL int
ioblock(FD fd, int flag) {
	int on, rc;

	/*
	 * Set non-blocking i/o
	 */
	if (flag == 0) {
#ifdef sgi
		on = O_NONBLOCK|O_NDELAY;
		rc = (fcntl(fd, F_SETFL, on) < 0);
#endif
#ifdef sun
		on = FNBIO|O_NDELAY;
		rc = (fcntl(fd, F_SETFL, on) < 0);
#endif
#ifdef aix
		on = 1;
		rc = (ioctl(fd, FIONBIO, on) < 0);
#endif
	}

	/*
	 * Set blocking i/o
	 */
	 else {
#ifdef sgi
		on = 0;
		rc = (fcntl(fd, F_SETFL, on) < 0);
#endif

#ifdef sun
		on = 0;
		rc = (fcntl(fd, F_SETFL, on) < 0);
#endif

#ifdef aix
		on = 0;
		rc = (ioctl(fd, FIONBIO, on) < 0);
#endif
	}

	return(rc==0);
}

#define PRT(opt,str) \
	printf(msg?"\t%s%s%s\n":"%s ", opt, (strlen(opt)<8)?"\t\t":"\t", str)

INTERNAL void
usage(char *pgm, int msg) {
	if (msg) printf("usage: %s [options] datafile\n", pgm);
	else     printf("usage: %s ", pgm);
	PRT("[-help]",          "display this message");
	PRT("probe",            "probe");
	printf("\n");
}


INTERNAL int
iowait(FD fd, int millisec, int rdflag, int wrflag) {
	TIMEVAL timeout;
	fd_set  waitrd, waitwr, *wr=NULL, *ww=NULL;
	int     rc;

	FD_ZERO(&waitrd);
	FD_ZERO(&waitwr);
	FD_SET(fd, &waitrd);
	FD_SET(fd, &waitwr);
	if (rdflag) wr = &waitrd;
	if (wrflag) ww = &waitwr;
	if (millisec > 1000) {
		timeout.tv_sec = millisec / 1000;
		timeout.tv_usec = 1000*(millisec%1000);
	}
	else {
		timeout.tv_sec  = 0;
		timeout.tv_usec = 1000*millisec;
	}
	rc = select(fd+1, wr, ww, NULL, &timeout);

	return(rc > 0);
}


INTERNAL int
setup(char *pgm, int port) {
	SOCKADDR server;
	HOST     *host;
	SERVENT  *servent;
	char     servername[128];
	int      service;
	int      opt;
	int      rc;

	bzero((void *)&server, sizeof(SOCKADDR));

	/*
	 * Create the socket
	 */
	Sockid = socket(AF_INET, SOCK_STREAM, 0);
	if (Sockid < 0) {
		Errmsg = "can't create socket";
		return(0);
	}

	/* Set socket options */
#ifdef SO_REUSEADDR
	opt = 1;
	setsockopt(Sockid, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

#ifdef SO_REUSEPORT
	opt = 1;
	setsockopt(Sockid, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
#endif

	opt = Socksize*K;
	rc = setsockopt(Sockid, SOL_SOCKET, SO_RCVBUF, &opt, sizeof(opt));
	if (rc < 0) {
		fprintf(stderr, "setup: failed to set read socket buffer size\n");
		exit(1);
	}

	opt = Socksize*K;
	rc = setsockopt(Sockid, SOL_SOCKET, SO_SNDBUF, &opt, sizeof(opt));
	if (rc < 0) {
		fprintf(stderr, "setup: failed to set write socket buffer size\n");
		exit(1);
	}

	/* Set NODELAY option */
	opt = 1;
	setsockopt(Sockid, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));

	/* Set FASTACK option */
#ifdef TCP_FASTACK
	opt = 1;
	setsockopt(Sockid, IPPROTO_TCP, TCP_FASTACK, &opt, sizeof(opt));
#endif

	/*
	 * Set up Internet Adress of server
	 */
	if (!port) {
		 servent = getservbyname(SERVICE, NULL);
		 if (servent) service = servent->s_port;
		 else         service = DEFLTPORT;
	}
	else service = port;

	gethostname(servername, 128);
	host = gethostbyname(servername);
	if (host == NULL) {
		Errmsg = "Can't get host address";
		return(0);
	}
	server.sin_family = host->h_addrtype;
	server.sin_port   = htons(service);
	server.sin_addr.s_addr = INADDR_ANY;

	/* Bind the socket to the server address */
	if (bind(Sockid, (struct sockaddr *)&server, sizeof(server)) < 0) {
		Errmsg = "can't bind socket";
		return(0);
	}

	/* Establish maximum queue length */
	if (listen(Sockid, 5) < 0) {
		Errmsg = "can't listen on socket";
		return(0);
	}
	return(service);
}

/*
 * Convert an Internet address to a "dotted" decimal string
 */
INTERNAL void
addr_str(int no, char *str) {

	sprintf(str, "%d.%d.%d.%d",
		(no>>24)&0xff, (no>>16)&0xff, (no>>8)&0xff, no&0xff);
}


/*
 * Convert an Internet address to a "dotted" hex string
 */
INTERNAL void
addr_hex(int no, char *str) {
	sprintf(str, "%02x.%02x.%02x.%02x",
		(no>>24)&0xff, (no>>16)&0xff, (no>>8)&0xff, no&0xff);
}

/*
 * Encode an Internet address
 */
INTERNAL void
addr_encode(int no, char *str) {
	int  i;

	str[8] = '\0';
	for (i = 7; i > -1; --i) {
		str[i] = (no&15) + 'a';
		no >>= 4;
	}
}


INTERNAL int
addr_decode(char *str) {
	int i = 0;

	while (*str) {
		if (*str < 'a' || *str > 'p') return(0);
		i <<= 4;
		i += *str++ - 'a';
	}
	return(i);
}

