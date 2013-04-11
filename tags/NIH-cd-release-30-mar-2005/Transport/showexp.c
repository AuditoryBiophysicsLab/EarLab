/*
 * Display state of an experiment
 */
#ifdef WINAPP
# define WINVER 0x0600
# include <windows.h>
# include <local/winapp.h>
# include <local/gd.h>
# define main app_main
# define PGMNAME /* "Distributed Earlab" */ "showexp"

# include "showexpmenu.h"
#endif

#include <stdio.h>
#include <unistd.h>
#include <local/std.h>

#include <daffie/event.h>
#include <daffie/eclient.h>

#include "agentd.h"
#include "eventdef.h"
#include "config.h"

#define METASERVER   "scv.bu.edu"
#define EARLAB_WORLD "Earlab*Master"
#define EARLAB_HOST  "*"

INTERNAL MODULE_CONFIG    Modules[MAX_MODULES];
INTERNAL int              Nmodules;
INTERNAL struct {
	int     state;
	int     x0, y0, x1, y1;
}                         ModState[MAX_MODULES];

INTERNAL char *Pgm;
INTERNAL char *Meta    = METASERVER;
INTERNAL char *Master  = NULL;
INTERNAL char *Server  = NULL;
INTERNAL char *ExpName = NULL;
INTERNAL char *Fileshare = NULL;
INTERNAL char  Config[1000] = "";
INTERNAL char  ConfigName[200] = "";
INTERNAL int   Joined = 0;
INTERNAL int   Debug  = 0;
INTERNAL int   Console= 0;
INTERNAL int   Unload = 1;
INTERNAL int   Tunnel = 0;
INTERNAL int   Dump   = 0;
INTERNAL int   Done   = 0;
INTERNAL int   Redraw = 0;
INTERNAL int   Nstep  = 1;
INTERNAL char *Servers[100];
INTERNAL char *Exphosts[100];
INTERNAL int   Nservers = 0;
INTERNAL int   Nexphost = 0;
INTERNAL int   ModDebug   = 0;
INTERNAL int   ModVerbose = 0;
INTERNAL int   ModTrans   = 0;
INTERNAL int   ModFS      = 0;
#ifdef WINAPI
INTERNAL char  ProbeExec[100];
INTERNAL char *ProbeName[PROBE_TYPE_N-PROBE_TYPE_0+1];
#endif

#define NSTATE 7
INTERNAL char *StateName[NSTATE];
INTERNAL int   StateColor[NSTATE];
INTERNAL int   StateMap[EFI_STATUS_MAXTYPE+1][EFI_STATUS_MAXSTATUS+1];

INTERNAL void  init_modules(void);
INTERNAL int   update_modules(void);
INTERNAL int   load_config(char *file);
INTERNAL int   watch_file(char *file);
INTERNAL void  do_run_run(void);
INTERNAL void  show_init(void);
INTERNAL void  show_state(char type, int indx, int state);
INTERNAL void  show_drawall(void);
INTERNAL void  show_draw(int);
INTERNAL void  show_layout(void);
INTERNAL void  usage(char *from, int msg);
INTERNAL int   oneof(char c, char *s);

INTERNAL int   Connect(char *server);
INTERNAL char *SearchWorld(char *meta, char *name, char *host);
INTERNAL int   FindServers(char *master, char *server[], char *agent[], char *name, char *host);
INTERNAL void  AddHostItems(int n, char *host[]);
INTERNAL void  AddExpHostItems(int n, char *host[]);
INTERNAL void  HostMenu(int enable);
INTERNAL void  AddProbes(int i);
INTERNAL void  DelProbes(int imod);
INTERNAL void  SetDebugMenu(void);

INTERNAL void efi_init(void) {
	/* Define agent events */
	event_init();
#include "efieventdef.h"
#include "agentddef.h"
}

main(int argc, char *argv[]) {
	int   ok, help;
	int   need;
	char *expname;
	int   id, nc;
	int   i, j, nmod;
	int   from, state;
	int   indx;
	int   run;
	int   verbose;
	int   gotservers = 0, updated;
	char  msg[200];
	EVENT_EFI_STATUS *estat;
	EVENT             e;

#ifdef WINAPP
	Pgm = PGMNAME;
	WinTitle(Pgm);
	StatusAreas(4, 20, 20, 20);
	StatusSelect(0);
	StatusMsg("  -");
#else
	if (Pgm = strchr(argv[0], '/')) ++Pgm;
	else Pgm = argv[0];
#endif
	++argv; --argc;

	/* Set known probe names */
#ifdef WINAPI
	ProbeName[PROBE_TYPE_WPLOT  - PROBE_TYPE_0] = "wplot";
	ProbeName[PROBE_TYPE_XPLOT  - PROBE_TYPE_0] = "xplot";
	ProbeName[PROBE_TYPE_WAVE   - PROBE_TYPE_0] = "waveview";
	ProbeName[PROBE_TYPE_XWAVE  - PROBE_TYPE_0] = "xwaveview";
	ProbeName[PROBE_TYPE_SPIKE  - PROBE_TYPE_0] = "spikeview";
	ProbeName[PROBE_TYPE_XSPIKE - PROBE_TYPE_0] = "xspikeview";
	strcpy(ProbeExec, ProbeName[0]);
#endif

	/* Set state names and colors */
	StateName[0] = "None";
	StateName[1] = "Loaded";
	StateName[2] = "Starting";
	StateName[3] = "Idle";
	StateName[4] = "Running";
	StateName[5] = "SendData";
	StateName[6] = "WaitData";

	StateColor[0] = 0;
	StateColor[1] = 1;
	StateColor[2] = 2;
	StateColor[3] = 3;
	StateColor[4] = 4;
	StateColor[5] = 5;
	StateColor[6] = 6;

	ok      = 1;
	help    = 0;
	Debug   = 0;
	verbose = 0;
	run     = 0;
	while (argc && *argv[0] == '-') {
		char *opt;

		opt = argv[0] + 1;
		++argv; --argc;

		if      (streq(opt, "help"))   ++help;
		else if (substr(opt, "debug")) ++Debug;
		else if (substr(opt, "verbose")) ++verbose;
		else if (substr(opt, "moddebug"))     ++ModDebug;
		else if (substr(opt, "modverbose"))   ++ModVerbose;
		else if (substr(opt, "modtransport")) ++ModTrans;
		else if (substr(opt, "modfs"))        ++ModFS;
		else if (substr(opt, "quiet"))   verbose = 0;
		else if (substr(opt, "run"))     run = 1;
		else if (substr(opt, "unload"))   Unload  = 1;
		else if (substr(opt, "nounload")) Unload  = 0;
		else if (substr(opt, "tunnel"))   Tunnel  = 1;
		else if (substr(opt, "notunnel")) Tunnel  = 0;
		else if (substr(opt, "dump"))     Dump    = 1;
		else if (substr(opt, "nodump"))   Dump    = 0;
		else if (substr(opt, "capture"))  Dump    = 1; /* same as dump */
		else if (substr(opt, "earlab") || substr(opt, "master")) {
			if (!argc || *argv[0] == '-') {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
			}
			else {
				Master = argv[0];
				++argv; --argc;
			}
		}
		else if (substr(opt, "steps")) {
			if (!argc || *argv[0] == '-') {
				fprintf(stderr,
					"%s: missing argument to -%s\n",
					Pgm, opt);
				ok = 0;
			}
			else {
				Nstep = atoi(argv[0]);
				++argv; --argc;
			}
		}
		else {
			fprintf(stderr,
				"%s: unrecognized argument -%s\n",
				Pgm, opt);
			help = 1;
			ok   = 0;
		}
	}

#ifdef WINAPP
	need = 0;
#else
	need = 2;
#endif
	if (!ok || argc < need || help) {
		usage(Pgm, help);
		exit(!help);
	}

	/* Get experiment server host */
	if (argc) {
		Server =  argv[0];
		++argv; --argc;
	}

	/* Initialize display */
	show_init();

	/* Get configuration file name */
	if (argc) {
		load_config(argv[0]);
		++argv; --argc;
	}

	/* Initialize the state mapping table */
	for (i = 0; i <= EFI_STATUS_MAXTYPE; ++i)
	  for (j = 0; j <= EFI_STATUS_MAXSTATUS; ++j)
	    StateMap[i][j] = -1;

	StateMap[EFI_STATUS_LOAD][EFI_STATUS_STARTING]    = 1;
	StateMap[EFI_STATUS_PARAM][EFI_STATUS_READY]      = 2;
	StateMap[EFI_STATUS_START][EFI_STATUS_READY]      = 3;
	StateMap[EFI_STATUS_ADVANCE][EFI_STATUS_STARTING] = 4;
	StateMap[EFI_STATUS_ADVANCE][EFI_STATUS_READY]    = 5;
	StateMap[EFI_STATUS_ADVANCE][EFI_STATUS_IDLE]     = 3;
	StateMap[EFI_STATUS_ADVANCE][EFI_STATUS_WAITDATA] = 6;
	StateMap[EFI_STATUS_STOP][EFI_STATUS_READY]       = 1;
	StateMap[EFI_STATUS_UNLOAD][EFI_STATUS_READY]     = 0;


	event_verbose(verbose);
	efi_init();

/* FOR TESTING ****
	Servers[0] = "localhost";
	Servers[1] = "scv.bu.edu";
	Servers[2] = "something.sc03.bu.edu";
	Nservers = 3;

	AddHostItems(Nservers, Servers);
******************/

#ifdef WINAPP
	if (Master) {
		StatusSelect(0);
		StatusMsg(Master);
		EnableMenuItem(MainMenu, FILE_MENU_MASTER, MF_GRAYED);
		EnableMenuItem(MainMenu, HOST_MENU_SCAN,   MF_ENABLED);
		EnableMenuItem(MainMenu, TOOL_MENU_RESMON, MF_ENABLED);
	}
	CheckMenuItem(MainMenu, OPT_MENU_UNLOAD, Unload?MF_CHECKED:MF_UNCHECKED);
	CheckMenuItem(MainMenu, OPT_MENU_TUNNEL, Tunnel?MF_CHECKED:MF_UNCHECKED);
	CheckMenuItem(MainMenu, OPT_MENU_DUMP,   Dump?MF_CHECKED:MF_UNCHECKED);

	SetDebugMenu();

#endif
	/*
	 * Wait until we have a master
	 */
	while (!Master || !Server) {
		if (Master && !Server && !gotservers) {
			Nservers = FindServers(Master, Servers, Exphosts, "*", "*");
			AddHostItems(Nservers, Servers);

			/* Count Exphosts */
			for (Nexphost = 0; Exphosts[Nexphost]; ++Nexphost) /* nothing*/;
			AddExpHostItems(Nexphost, Exphosts);

			gotservers = 1;
		}
		if (Redraw) {
			show_drawall();
			Redraw = 0;
		}
		if (watch_file(Config)) load_config(Config);
		event_sleep(250);
	}

#ifdef WINAPP
	StatusSelect(0);
	StatusMsg(Master);
	EnableMenuItem(MainMenu, FILE_MENU_MASTER, MF_GRAYED);
	EnableMenuItem(MainMenu, HOST_MENU_SCAN,   MF_ENABLED);
	EnableMenuItem(MainMenu, TOOL_MENU_RESMON, MF_ENABLED);
#endif

	id = Connect(Server);

	if (run) do_run_run();

	while (!Done) {
		int etype;

		/* Check the configuration file for changes */
		if (watch_file(Config)) {
			load_config(Config);
		}

		/* Wait for and read an event */
		event_wait_timeout(250);
		if (Redraw) {
			show_drawall();
			Redraw = 0;
		}
		if (!event_receive(&e)) continue;

		etype = EventType(e);

		/* Try to deal gracefully with lost server connection */
		if (etype == ET_SHUTDOWN && e.ev_head.from == id) {
			fprintf(stderr, "%s: server shutdown\n", Pgm);
			break;
		}

		/* Check for client comings and going */
		if (etype == ET_IDENTITY || etype == ET_LEAVE)
		   updated = update_modules();
#ifdef WINAPP
		if (updated) show_drawall();
#endif

		/* If this is not a STATUS print a message and ignore */
		if (etype != ET_EFI_STATUS) {
			if (verbose)
			  fprintf(stderr, "%s: received %s (%d) from %d\n",
				  Pgm,
				  event_lookup_name(e.ev_head.type),
				  e.ev_head.type,
				  e.ev_head.from);
			continue;
		}

		/*
		 * Format up a message
		 *  1) Map DAFFIE clients to Earlab modules
		 *  2) Map internal state messages to Control Layer state
		 */

		/* Get client id of sender */
		from  = e.ev_head.from;

		/* Set pointer to STATUS event */
		estat = (EVENT_EFI_STATUS *)&e;

		/* See if this is a message to be passed as-is */
		if (estat->type == EFI_STATUS_CONTROL) {
			state = estat->status;
			indx  = estat->detail;

			fprintf(stderr,
				"%s: Passing state %s for module id %d\n",
				Pgm, StateName[state], id);

			show_state('C', indx, state);
			continue;
		}

		/* See if this is a frame complete message */
		if (estat->type == EFI_STATUS_FRAME) {
			int step, tot;

			step = estat->status;
			tot  = estat->detail;

			fprintf(stderr,
				"%s: Frame %d of %d complete\n",
				Pgm, step, tot);

			show_state('F', step, tot);
			continue;
		}

		/* Lookup Earlab module using clientid */
		for (j = 0; j < Nmodules; ++j) {
			if (from == Modules[j].clientid) break;
		}
		if (j == Nmodules) {
			fprintf(stderr,
				"%s: couldn't find module for client id %d!!\n",
				Pgm, from);
			continue;
		}
		indx = j;

		/* Map message to upper layer state definitions */
		state = StateMap[estat->type][estat->status];

		if (state < 0) {
			fprintf(stderr,
				"%s: ignoring type-status %d-%d from %s (client %d)\n",
				Pgm, estat->type, estat->status,
				Modules[indx].instname, from);
			continue;
		}
		if (Debug || verbose)
		  fprintf(stderr,
			  "%s: %s(%s,%s)-%s@%s (client id %d)\n",
			  Pgm, StateName[state],
			  StatusTypeMsg[estat->type],
			  StatusStatusMsg[estat->status],
			  Modules[indx].instname,
			  Modules[indx].host,
			  from);

		/* Send it to clients */
		show_state('M', indx, state);

	}

	event_leave();
}

INTERNAL int
Connect(char *server) {
	int id, nc;
	char msg[200];

	id = event_join(Server, &nc);
	if (!id) {
		fprintf(stderr,
			"%s: couldn't connect to event server %s\n",
			Pgm, Server);
		Server = NULL;
		return(0);
	}
	sprintf(msg, "Joined %s as client id %d (%d clients)\n",
		Server, id, nc);

	fprintf(stderr, "%s: %s\n", Pgm, msg);


	event_register("user", "control", Pgm);

	/* Select only state messages */
	event_select_type(0, 0, ET_MAX);
      /***
	event_select_types(1, 3, ET_JOIN, ET_LEAVE, ET_IDENTITY);
      ***/
	event_select_type(1, ET_EFI_STATUS, ET_EFI_STATUS);

	/* Use Daffie services to find modules */
	eclient_monitor();
	init_modules();

	Joined = 1;

#ifdef WINAPP
	StatusSelect(1);
	StatusMsg(Server);
	StatusSelect(3);
	StatusMsg(msg);
	HostMenu(0);
	EnableMenuItem(MainMenu, HOST_MENU_CONN, MF_GRAYED);
#endif
	return(id);
}

INTERNAL void
init_modules(void) {
	int i;

	for (i = 0; i < Nmodules; ++i) {
		ModState[i].state   = 0;
		Modules[i].clientid = 0;
		Modules[i].host[0]  = '\0';
		DelProbes(i);
	}
}

INTERNAL int
update_modules(void) {
	int          i, j, nc, nup = 0;
	ECLIENT_LIST clients[MAX_CLIENT], *clp;

	if (!Joined || !Nmodules) return(0);

	/* Get client list */
	nc = eclient_match(clients, "*", "earlab", "*", "*");

	/* Lookup client by DAFFIE id */
	for (i = 0; i < Nmodules; ++i) {
		for (j = 0; j < nc; ++j) {
			if (strmatch(Modules[i].instname, clients[j].instance))
			   break;
		}
		/* See if we found the module */
		if (j == nc) {
			DelProbes(i);
			ModState[i].state   = 0;
			Modules[i].clientid = 0;
			Modules[i].host[0]  = '\0';
			++nup;
			continue;
		}
		if (!ModState[i].state) ModState[i].state = 1;
		if (Modules[i].clientid != clients[j].id) {
			printf("update_modules: Identified %s %d\n",
				Modules[i].instname,
				clients[j].id);
			ModState[i].state = 1;
			Modules[i].clientid = clients[j].id;
			strncpy(Modules[i].host, clients[j].host, CONFIG_NAMELEN);
			DelProbes(i);
			AddProbes(i);
			++nup;
		}
	}
	return(nup);
}

INTERNAL int
load_config(char *config) {
	char *s;
	int   len;
#ifdef WINAPP
	char msgbuf[300];

	sprintf(msgbuf, "Loading %s ...", config);
	StatusSelect(2);
	StatusMsg(msgbuf);
#endif

	/* Reset all the old modules */
	init_modules();

	/* Read the configuration file */
	Nmodules = read_config(config, Modules);

	if (Nmodules < 0) {
		fprintf(stderr,
			"%s: error reading configuration file \"%s\"\n",
			Pgm, config);
		config_error(stderr, Pgm);
		Config[0] = '\0';
		return(0);
	}

	if (Nmodules == 0) {
		fprintf(stderr,
			"%s: no modules found in configuration file \"%s\"\n",
			Pgm, config);
		Config[0] = '\0';
		return(0);
	}

	/* Stash configuration file name */
	strcpy(Config, config);
	if (s = strrchr(Config, '/')) strcpy(ConfigName, s+1);
	else strcpy(ConfigName, Config);

	len = strlen(ConfigName);
	if      (strimatch("*.diagram", ConfigName)) ConfigName[len-8] = '\0';
	else if (strimatch("*.model", ConfigName))   ConfigName[len-6] = '\0';

	init_modules();
	update_modules();

	show_layout();
	show_drawall();
#ifdef WINAPP
	EnableMenuItem(MainMenu, RUN_MENU_LOAD, MF_BYCOMMAND|MF_ENABLED);
	StatusSelect(2);
	StatusMsg(ConfigName);
#endif

	watch_file(Config);

	return(1);
}

/*
 * Watch a file for updates/changes
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef unsigned int  Uint32;

#ifdef sgi
typedef off64_t       Ulongint;
typedef struct stat64 STATBUF;
#define SysStat       stat64
#define ModTime(s)    ((s).st_mtim.tv_sec)
#define SameModTime(f,s) \
		      ((f).st_mtim.tv_sec == (s).st_mtim.tv_sec&& \
		       (f).st_mtim.tv_nsec== (s).st_mtim.tv_nsec)
#define SameSize(f,s) ((f).st_size == (s).st_size)
#else
typedef unsigned long long int Ulongint;
typedef struct stat   STATBUF;
#define SysStat       stat
#define ModTime(s)    ((s).st_mtime)
#define SameModTime(f,s) \
		      ((f).st_mtime == (s).st_mtime)
#define SameSize(f,s) ((f).st_size  == (s).st_size)
#endif

INTERNAL int
watch_file(char *file) {
	static char watchfile[1000] = {'\0'};
	static STATBUF stash;
	STATBUF stbuf;
	int newfile = 0;

	if (!file || !*file) return(0);

	newfile = !streq(watchfile, file);
	if (newfile) strcpy(watchfile, file);
	if (SysStat(file, &stbuf) < 0) return(0);
  /****
	printf("watching %s %lu %lu\n", Config, stbuf.st_size, stbuf.st_mtime);
   ****/
	if (newfile || !SameModTime(stbuf, stash) || !SameSize(stbuf, stash)) {
		memcpy(&stash, &stbuf, sizeof(STATBUF));
		newfile = 1;
	}
	return(newfile);
}

INTERNAL int
oneof(char c, char *s) {
	while (*s) if (*s++ == c) return(1);
	return(0);
}

INTERNAL int
GetChannels(char *spec) {
	char modname[100];
	int  arg;
	int  i, n;

	if (!spec) return(0);
	for (i = 0; *spec && *spec != '.'; ++i) modname[i] = *spec++;
	modname[i] = '\0';

	if (*spec != '.') return(0);
	arg = atoi(spec+1);

	for (i = 0; i < Nmodules; ++i) {
		if (strmatch(modname, Modules[i].instname)) break;
	}
	if (i == Nmodules) {
		printf("GetChannels: couldn't find %s!!\n", modname);
		return(0);
	}
	if (arg < 1 || arg > Modules[i].noutput) {
		printf("GetChannels: bad arg %d for %s!!\n", arg, modname);
		return(0);
	}
	--arg;
	if (Modules[i].out_dim[arg] < 2) return(1);
	if (Modules[i].out_type[arg] == EFI_STREAM_SPIKE)
	    n = Modules[i].out_rank[arg][0];
	else
	    n = Modules[i].out_rank[arg][1];

	printf("get_channel: %s.%d = %d\n", modname, arg+1, n);
	return(n);
}

INTERNAL int
GetDimstr(char *spec, char *str) {
	char modname[100];
	int  arg;
	int  i, j, n;

	if (!spec) return(0);
	for (i = 0; *spec && *spec != '.'; ++i) modname[i] = *spec++;
	modname[i] = '\0';

	if (*spec != '.') return(0);
	arg = atoi(spec+1);

	for (i = 0; i < Nmodules; ++i) {
		if (strmatch(modname, Modules[i].instname)) break;
	}
	if (i == Nmodules) {
		printf("GetChannels: couldn't find %s!!\n", modname);
		return(0);
	}
	if (arg < 1 || arg > Modules[i].noutput) {
		printf("GetChannels: bad arg %d for %s!!\n", arg, modname);
		return(0);
	}
	--arg;
	*str = '\0';
	for (j = 0; j < Modules[i].out_dim[arg]; ++j) {
		char buf[20];

		sprintf(buf, "%s%d", j?"x":"", Modules[i].out_rank[arg][j]);
		strcat(str, buf);
	}

	printf("GetDimstr: %s = %s\n", modname, str);

	return(j);
}

/*
 * Usage message
 */
#define PRT(opt,str) \
	printf(msg?"\t%s%s%s\n":"%s ", opt, (strlen(opt)<8)?"\t\t":"\t", str)

INTERNAL void
usage(char *pgmname, int msg) {
	if (msg) printf("usage: %s [options] server host command", pgmname);
	else     printf("usage: %s ", pgmname);
	PRT("[-help]",          "display this message");
	PRT("[-debug]",         "enable debugging output");
	PRT("[-moddebug]",      "enable module debugging output");
	PRT("[-modverbose]",    "enable module verbose output");
	PRT("[-unload]",        "unload modules on STOP");
	PRT("[-nounload]",      "leave modules loaded");
	PRT("[-tunnel]",        "enable module tunneling");
	PRT("[-notunnel]",      "disable module tunneling");
	PRT("[-master server]", "sepcify master Earlab server host");
	PRT("[-earlab server]", "synonymous with \"master\"");
	PRT("[-step n]",        "number of steps to run");
	PRT("[-run]",           "load and run the model");
#ifdef WINAPP
	PRT("[server]",         "experiment event server");
	PRT("[config]",         "configuration file");
#else
	PRT("[server]",         "experiment event server");
	PRT("config",           "configuration file");
#endif
	printf("\n");
}

#define XMARGIN     10
#define YMARGIN     10
#define BOX_XSIZE  150
#define BOX_XSPACE 200
#define BOX_YSIZE   50
#define BOX_YSPACE 100

#define DEF_XSIZE 1000
#define DEF_YSIZE 300

app_init() {
	xw_size(DEF_XSIZE, DEF_YSIZE);
}

INTERNAL void
show_init(void) {
#ifdef XWINDOWS
	xw_backing(1);
#endif

	xw_size(DEF_XSIZE, DEF_YSIZE);
	xw_init();

	xw_setcol(0, 0, 0, 0);
	xw_setcol(7, 255,  255, 255);

	xw_bkg(0);
	xw_erase();

	xw_setcol(1,   0, 128, 128); /* Loaded               */
	xw_setcol(2, 155, 155,  0);  /* Starting             */
	xw_setcol(3,   0, 155,   0); /* Ready                */
	xw_setcol(4, 255,   0,   0); /* Running              */
	xw_setcol(5, 255, 180,  50); /* Sending Data         */
	xw_setcol(6, 150,  50,  100); /* Waiting data        */
}

INTERNAL void
show_layout(void) {
	int i;
	int x0, y0, x1, y1;

	xw_color(7);
	y0 = YMARGIN;
	x0 = XMARGIN;
	for (i = 0; i < Nmodules; ++i) {
		x1 = x0 + BOX_XSIZE;
		y1 = y0 + BOX_YSIZE;

		ModState[i].x0 = x0;
		ModState[i].y0 = y0;
		ModState[i].x1 = x1;
		ModState[i].y1 = y1;

		xw_move(x0, y0);
		xw_box(x1, y1);

		xw_textbox(Modules[i].instname, x0, y0, x1, y1);

		x0 += BOX_XSPACE;

		if (Modules[i].noutput == 0) {
			x0 =  XMARGIN;
			y0 += BOX_YSPACE;
		}

	}
}

INTERNAL void
show_drawall(void) {
	int i;
	int xs, ys;
	int x0, y0, x1, y1;
	int w, h, space;

	xw_erase();
	xw_qsize(&xs, &ys);

	for (i = 0; i < Nmodules; ++i) show_draw(i);

	space = 10;
	w = 100; h = 20;
	x0 = space;
	y0 = ys-space-h;
	for (i = 0; i < NSTATE; ++i) {

		x1 = x0 + w;
		y1 = y0 + h;
		xw_color(7);
		xw_move(x0, y0);
		xw_box(x1, y1);
		xw_color(StateColor[i]);
		xw_move(x0+1, y0+1);
		xw_flbox(x1-1, y1-1);
		xw_color(7);
		xw_textbox(StateName[i], x0, y0, x1, y1);

		x0 += w+space;
	}
/***********
***********/
	/* HACK HACK HACK */
	xw_color(0); xw_move(x0,y0); xw_text("*");
	xw_color(0); xw_move(x0,y0); xw_text("*");
	xw_move(0,0); xw_flbox(1,1);

	xw_flush();

}

INTERNAL void
show_draw(int imod) {
	int x0, y0, x1, y1;
	int cx0, cy0, cx1, cy1;
	int state;
	int i, j, k, nout, nin;
	float ydelta;

	x0 = ModState[imod].x0;
	y0 = ModState[imod].y0;
	x1 = ModState[imod].x1;
	y1 = ModState[imod].y1;
	state = ModState[imod].state;

	xw_color(7);
	xw_move(x0, y0);
	xw_box(x1, y1);

	xw_color(state);
	xw_move(x0+1, y0+1);
	xw_flbox(x1-1, y1-1);

	xw_color(7);
	xw_textbox(Modules[imod].instname, x0, y0+10, x1, y1);
	if (Modules[imod].host[0])
	  xw_textbox(Modules[imod].host, x0, y0+2, x1, y0+10);

	nout = Modules[imod].noutput;
	if (!nout) return;

	ydelta = (float)(y1-y0+1) / (float)(nout+1);
	cx0 = ModState[imod].x1 + 1;

	for (i = 0; i < nout; ++i) {
		cy0 = y0 + (i+1)*ydelta + 0.5;

		j = Modules[imod].out_dst[i];
		k = Modules[imod].out_pos[i];
		cx1 = ModState[j].x0;
		nin = Modules[j].ninput;
		cy1 = ModState[j].y0 + 0.5 +
		      k * (ModState[j].y1 - ModState[j].y0 + 1) / (float)(nin+1);

		xw_move(cx0, cy0);
		xw_draw(cx1, cy1);
	}
}

INTERNAL void
show_state(char type, int indx, int state) {
	int x0, y0, x1, y1;
	int i;

	if (type == 'M') {
		ModState[indx].state = state;
		show_draw(indx);
		if (Debug)
		  printf("Module %s->%s\n", Modules[indx].instname, StateName[state]);
	}
	else if (type = 'F') {
		char msg[100];
		sprintf(msg, "Frame %d of %d complete", indx, state);
#ifdef WINAPP
		StatusSelect(3);
		StatusMsg(msg);
#else
		printf("%s\n", msg);
#endif
	}

	else printf("State: %c %d %d\n", type, indx, state);
}

INTERNAL void
do_run_run(void) {
	char buf[1000];
	char host[100], world[100];

	sprintf(buf,
		"runmodel -steps %d %s %s %s &",
		Nstep, Unload?"-unload":"-nounload", Server, Config);
#ifdef WINAPP
	StatusSelect(3);
	StatusMsg(buf);
#endif
	printf("system: '%s'\n", buf);
	system(buf);
	printf("Running %s for %d steps\n", ConfigName, Nstep);
}

#ifdef WINAPP
INTERNAL int LocateItem(HMENU menu, char *str);

do_redraw() { Redraw = 1; return 1; }

do_cleanup() { Done = 1; return 1; }

static char Filters[2000] = {
    'p', 'a', 'r', '\0',        '*', '.', 'p', 'a', 'r', '\0',
    'A', 'N', 'Y', '\0',        '*', '\0',
    '\0', '\0'
};

INTERNAL int
filterlen(char *str) {          /* Search for double eos characters */
	int i;

	/* articially limit to 200 chars in case it is not terminated */
	for (i = 0; i < 2000; ++i) {
		if (*str == '\0' && *(str+1) == '\0') return (i);
		++str;
	}
	return(0);
}

INTERNAL void
file_filter(char *filter, char *desc, char *match) {
	int  n;
	char buf[500];

	n = filterlen(filter);

	if (!n) { strcpy(filter, desc);       n += strlen(desc) + 1; }
	else    { strcpy(&filter[n+1], desc); n += strlen(desc) + 2; }

	strcpy(&filter[n], match);
	n += strlen(match) + 2;
	filter[n]   = '\0';
	filter[n+1] = '\0';
}

INTERNAL void init_file_filters(void) {
	static int init = 0;

	if (init) return;
	Filters[0] = Filters[1] = Filters[2] = '\0';

	file_filter(Filters, "diagram",    "*.dia*");
	file_filter(Filters, "parameters", "*.par*");
	file_filter(Filters, "text",       "*.txt");
	file_filter(Filters, "ALL",        "*");

	init = 1;
}


INTERNAL void
config_file(void) {
	OPENFILENAME filename;
	char path[1024], fpath[1024], file[256];
	int  rc;
	FILE *f;

	path[0] = '\0';
	memset(&filename, 0, sizeof(filename));
	filename.lStructSize        = sizeof(OPENFILENAME);
	filename.hwndOwner          = MainWin;
	filename.lpstrFilter        = Filters;
	filename.lpstrCustomFilter  = NULL;
	filename.nFilterIndex       = 1;
	filename.lpstrFile          = path;
	filename.nMaxFile           = sizeof(path)-1;
	filename.lpstrFileTitle     = file;
	filename.nMaxFileTitle      = sizeof(file)-1;
	filename.lpstrInitialDir    = NULL;
	filename.Flags              = OFN_FILEMUSTEXIST;

	init_file_filters();

	rc = GetOpenFileName(&filename);
	if (!rc) {
		MessageBox(MainWin, "Open cancelled", "quiter", MB_OK);
		return;
	}
	cygwin_conv_to_full_posix_path(path, fpath);

	load_config(fpath);
}

#include <shlobj.h>

INTERNAL void
fileshare_dir(void) {
	static int cominit = 0;
	BROWSEINFO   browsedir;
	LPITEMIDLIST rpidl;
	char path[1024], fpath[1024], buf[1024];

	if (!cominit) {
		CoInitialize(NULL);
		cominit = 1;
	}

	if (!Server) {
		MessageBox(MainWin, "No Server", "Okay", MB_OK);
		return;
	}

	path[0] = '\0';
	memset(&browsedir, 0, sizeof(browsedir));

	browsedir.hwndOwner          = MainWin;
	browsedir.pidlRoot           = NULL;
	browsedir.pszDisplayName     = path;
	browsedir.lpszTitle          = "Select Directory To Share";
	browsedir.ulFlags            = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;
	browsedir.lpfn               = NULL;
	browsedir.lParam             = 0;
	browsedir.iImage             = 0;

	rpidl = SHBrowseForFolder(&browsedir);
	if (rpidl == NULL) return;

	cygwin_conv_to_full_posix_path(path, fpath);

	if (Fileshare) free(Fileshare);
	Fileshare = (char *)malloc(strlen(fpath)+1);
	if (!Fileshare) {
		printf("%s: not enough memory for %d byte text string!!!\n",
			Pgm, strlen(fpath)+1);
		exit(4);
	}
	strcpy(Fileshare, fpath);

	sprintf(buf, "fserver -dir %s %s &", Fileshare, Server);

	printf("Starting fileserver: %s\n", buf);

	system(buf);
}

do_menu(int item) {
	char buf[1000];
	char host[100], world[100];
	char opts[200];
	int  i, rc;

	/* Deal with host selection items */
	if (HOST_MENU_HOST_0 <= item && item <= HOST_MENU_HOST_N) {
		int i = item - HOST_MENU_HOST_0;

		if (i > Nservers) {
			fprintf(stderr, "Whoow: bad host index %d!!\n", i);
			return(1);
		}
		Server = Servers[i];
		StatusSelect(1);
		StatusMsg(Server);
		return(1);
	}

	if (HOST_MENU_NEW_0 <= item && item <= HOST_MENU_NEW_N) {
		char cmd[1000];
		int  i = item - HOST_MENU_NEW_0;
		int  port;

		if (i > Nexphost) {
			fprintf(stderr, "Whoow: bad exp host index %d!!\n", i);
			return(1);
		}

		port = 9061+i;

		sprintf(cmd,
			"launch -host %s %s eventd -port %d -meta %s -world \\\"%s\\\" &",
			Exphosts[i], Master, port, Master,
			ExpName?ExpName:cuserid(NULL));

		printf("Start eventd: %s\n", cmd);
		system(cmd);

		sprintf(cmd, "%s:%d", Exphosts[i], port);
		Server = malloc(strlen(cmd)+1);
		if (!Server) {
			Server = "";
			fprintf(stderr, "do_menu: New Experiment couldn't allocate memory\n");
			return(1);
		}
		strcpy(Server, cmd);

		StatusSelect(1);
		StatusMsg(Server);

		sleep(5);

		return(1);
	}

	if (PROBE_MENU_PROBE_0 <= item && item <= PROBE_MENU_PROBE_N) {
		HMENU pmenu;
		int   pindx;
		char  probename[100];
		int   nchann = 32;
		char  dimstr[100];

		if (!Server) {
			MessageBox(MainWin, "No Server", "Okay", MB_OK);
			return(1);
		}
		pindx = LocateItem(MainMenu, "*robe*");
		if (pindx < 0) {
			printf("Couldn't find probe menu!!\n");
			return(1);
		}
		pmenu  = GetSubMenu(MainMenu, pindx);

		GetMenuString(pmenu, item, probename, sizeof(probename), MF_BYCOMMAND);

		nchann = GetChannels(probename);
		GetDimstr(probename, dimstr);

		sprintf(buf, "%s -autopan -channel %d daffie://%s/%s &",
			ProbeExec, nchann, Server, probename);

		printf("Probe: %s\n", buf);
		system(buf);

		return(1);
	}

	if (PROBE_TYPE_0 <= item && item <= PROBE_TYPE_N) {
		for (i = PROBE_TYPE_0; i <= PROBE_TYPE_N; ++i)
		    CheckMenuItem(MainMenu, i,
				  (i==item)?MF_CHECKED:MF_UNCHECKED);
		strcpy(ProbeExec, ProbeName[item-PROBE_TYPE_0]);
		printf("%s: using probe %s\n", Pgm, ProbeExec);
		return(1);
	}

	if (DEBUG_MENU_DEBUG_0 <= item && item <= DEBUG_MENU_DEBUG_MAX) {
		ModDebug = item - DEBUG_MENU_DEBUG_0;
		SetDebugMenu();
		return(1);
	}

	switch (item) {
	  case FILE_MENU_OPEN:
		config_file();
		return(1);

	  case FILE_MENU_MASTER:
		DialogSetText(EARLAB_WORLD, DLG_MASTER_WORLD_TEXTBOX);
		DialogSetText(EARLAB_HOST,  DLG_MASTER_HOST_TEXTBOX);
		DialogBegin("MasterDlg");
		return(1);

	  case DLG_MASTER_HOST_TEXTBOX:
	  case DLG_MASTER_WORLD_TEXTBOX:
		return(0);

	  case DLG_MASTER_CANCEL:
		DialogEnd();
		return(1);

	  case DLG_MASTER_ENTER:
		DialogGetText(world, DLG_MASTER_WORLD_TEXTBOX);
		DialogGetText(host,  DLG_MASTER_HOST_TEXTBOX);
		DialogEnd();
		sprintf(buf, "Searching for %s on %s\n", world, host);
		StatusSelect(3);
		StatusMsg(buf);
		Master = SearchWorld(Meta, world, host);
		if (Master) {
			sprintf(buf, "Earlab master is %s\n", Master);
			EnableMenuItem(MainMenu, FILE_MENU_MASTER,
				       MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(MainMenu, HOST_MENU_SCAN,   MF_ENABLED);
			EnableMenuItem(MainMenu, TOOL_MENU_RESMON, MF_ENABLED);
			StatusSelect(0);
			StatusMsg(Master);
		}
		else {
			sprintf(buf, "Coundn't find master %s on %s\n",
				world, host);
		}
		StatusSelect(3);
		StatusMsg(buf);
		return(1);

	  case DLG_STEPS_TEXTBOX:
		return(0);

	  case RUN_MENU_STEPS:
		sprintf(buf, "%d", Nstep);
		DialogSetText(buf, DLG_STEPS_TEXTBOX);
		DialogBegin("StepsDlg");
		return(1);

	  case DLG_STEPS_ENTER:
		DialogGetText(buf, DLG_STEPS_TEXTBOX);
		DialogEnd();
		printf("Set steps to %s\n", buf);
		Nstep  = atoi(buf);
		return(1);

	  case DLG_STEPS_CANCEL:
		DialogEnd();
		printf("cancelled steps dialog\n");
		return(1);

	  case HOST_MENU_CONN:
		if (Server) DialogSetText(Server, DLG_CONN_TEXTBOX);
		DialogBegin("ConnectDlg");
		return(1);

	  case DLG_CONN_TEXTBOX:
		return(0);

	  case DLG_CONN_ENTER:
		if (Server) {
			free(Server);
			Server = NULL;
		}
		DialogGetText(buf, DLG_CONN_TEXTBOX);
		DialogEnd();
		if (buf[0]) {
			Server = (char *)malloc(strlen(buf)+1);
			strcpy(Server, buf);

		}
		return(1);

	  case DLG_CONN_CANCEL:
		DialogEnd();
		return(1);

	  case HOST_MENU_DISCONN:
		if (!Joined) return(1);

		Joined = 0;
		event_leave();
		init_modules();
		HostMenu(1);

		return(1);

	  case RUN_MENU_RUN:
		if (!Config[0]) {
			rc = MessageBox(MainWin, "No configuration file", "Okay", MB_OK);
			return(1);
		}
		if (!Server) {
			rc = MessageBox(MainWin, "No event server", "Okay", MB_OK);
			return(1);
		}
		do_run_run();
		return(1);

	  case RUN_MENU_LOAD:
		if (!Config[0]) {
			rc = MessageBox(MainWin, "No configuration file", "Okay", MB_OK);
			return(1);
		}
		if (!Server) {
			rc = MessageBox(MainWin, "No event server", "Okay", MB_OK);
			return(1);
		}

		opts[0] = '\0';
		strcat(opts, Tunnel?"-tunnel":"-notunnel");
		if (ModVerbose)                strcat(opts, " -modverbose");
		if (Dump)                      strcat(opts, " -dump");
		for (i = 0; i < ModDebug; ++i) strcat(opts, " -moddebug");
		for (i = 0; i < ModTrans; ++i) strcat(opts, " -modtrans");
		for (i = 0; i < ModFS; ++i)    strcat(opts, " -modfs");

		sprintf(buf,
			"loadexp -noconsole %s %s \"%s\" %s %s &",
			opts,
			Server,
			ConfigName,
			Master?Master:"-search",
			Config);

		printf("system: '%s'\n", buf);
		system(buf);
		return(1);

	  case RUN_MENU_EDIT: {
		char *edit;
		if (!Config[0]) {
			rc = MessageBox(MainWin, "No configuration file", "Okay", MB_OK);
			return(1);
		}
		if (!(edit = (char *)getenv("EDITOR"))) edit = "notepad";

		sprintf(buf,
			"%s %s &",
			edit, Config);
		printf("system: '%s'\n", buf);
		system(buf);
		return(1);
	  }

	  case DEBUG_MENU_VERBOSE:
		ModVerbose = !ModVerbose;
		CheckMenuItem(MainMenu, DEBUG_MENU_VERBOSE,
			      ModVerbose?MF_CHECKED:MF_UNCHECKED);
		return(1);

	  case DEBUG_MENU_TRANSPORT:
		ModTrans = !ModTrans;
		CheckMenuItem(MainMenu, DEBUG_MENU_TRANSPORT,
			      ModTrans?MF_CHECKED:MF_UNCHECKED);
		return(1);

	  case DEBUG_MENU_FS:
		ModFS = !ModFS;
		CheckMenuItem(MainMenu, DEBUG_MENU_FS,
			      ModFS?MF_CHECKED:MF_UNCHECKED);
		return(1);

	  case OPT_MENU_UNLOAD:
		Unload = !Unload;
		CheckMenuItem(MainMenu, OPT_MENU_UNLOAD,
			      Unload?MF_CHECKED:MF_UNCHECKED);
		return(1);

	  case OPT_MENU_TUNNEL:
		Tunnel = !Tunnel;
		CheckMenuItem(MainMenu, OPT_MENU_TUNNEL,
			      Tunnel?MF_CHECKED:MF_UNCHECKED);
		return(1);

	  case OPT_MENU_DUMP:
		Dump = !Dump;
		CheckMenuItem(MainMenu, OPT_MENU_DUMP,
			      Dump?MF_CHECKED:MF_UNCHECKED);
		return(1);


	  case TOOL_MENU_EXPMON:
		if (!Server) {
			rc = MessageBox(MainWin, "No event server", "Okay", MB_OK);
			return(1);
		}
		sprintf(buf,
			"winwatch -title \"Experiment %s monitor\" %s &",
			ConfigName,
			Server);
		printf("system: '%s'\n", buf);
		system(buf);
		return(1);

	  case TOOL_MENU_RESMON:
		if (!Master) {
			rc = MessageBox(MainWin, "No Earlab Master", "Okay", MB_OK);
			return(1);
		}
		sprintf(buf,
			"winwatch -title \"Earlab Resource monitor\" %s &",
			Master);
		printf("system: '%s'\n", buf);
		system(buf);
		return(1);

	  case TOOL_MENU_CLEANUP:
		if (!Server) {
			rc = MessageBox(MainWin, "No event server", "Okay", MB_OK);
			return(1);
		}
		sprintf(buf, "bringdown -class earlab %s", Server);
		printf("cleanup: '%s'\n", buf);
		system(buf);
		return(1);

	  case TOOL_MENU_CONSOLE:
		if (!Server) {
			rc = MessageBox(MainWin, "No Evert Server", "Okay", MB_OK);
			return(1);
		}
		sprintf(buf, "xconsole %s -T \"Console %s@%s\" &",
			Server,
			(ExpName||ConfigName[0])?(ExpName?ExpName:ConfigName):" ",
			Server);
	  printf("Console: %s\n", buf);
		system(buf);
		return(1);

	  case TOOL_MENU_FSERV:
		fileshare_dir();
		return(1);

		if (Fileshare) DialogSetText(Fileshare, DLG_FSERV_TEXTBOX);
		DialogBegin("FservDlg");
		return(1);

	  case DLG_FSERV_TEXTBOX:
		return(1);

	  case DLG_FSERV_ENTER:
		DialogGetText(buf, DLG_FSERV_TEXTBOX);
		DialogEnd();
		printf("Share files %s\n", buf);
		if (Fileshare) free(Fileshare);
		Fileshare = (char *)malloc(strlen(buf)+1);
		if (!Fileshare) {
			printf("%s: not enough memory for %d byte text string!!!\n",
				Pgm, strlen(buf)+1);
			exit(4);
		}
		strcpy(Fileshare, buf);
		return(1);

	  case DLG_FSERV_CANCEL:
		DialogEnd();
		return(1);

	  case TOOL_MENU_EXPNAME:
		if (ExpName) DialogSetText(ExpName, DLG_EXPNAME_TEXTBOX);
		DialogBegin("ExpnameDlg");
		return(1);

	  case DLG_EXPNAME_TEXTBOX:
		return(0);

	  case DLG_EXPNAME_ENTER:
		DialogGetText(buf, DLG_EXPNAME_TEXTBOX);
		DialogEnd();
		printf("ExpName \"%s\"\n", buf);
		if (ExpName) free(ExpName);
		ExpName = (char *)malloc(strlen(buf)+1);
		if (!ExpName) {
			printf("%s: not enough memory for %d byte text string!!!\n",
				Pgm, strlen(buf)+1);
			exit(4);
		}
		strcpy(ExpName, buf);
		WinTitle(ExpName);
		if (!ConfigName[0]) strcpy(ConfigName, ExpName);
		return(1);

	  case DLG_EXPNAME_CANCEL:
		DialogEnd();
		return(1);

	  default:
		printf("do_menu: didn't recognize item %d\n", item);
		break;
	}
	return(1);
}

INTERNAL int
LocateItem(HMENU menu, char *str) {
	int i, n;
	char mname[100];

	n = GetMenuItemCount(menu);
	for (i = 0; i < n; ++i) {
		GetMenuString(menu, i, mname, sizeof(mname), MF_BYPOSITION);
		if (strmatch(str, mname)) return(i);
	}
	return(-1);
}

INTERNAL void
AddHostItems(int nhost, char *host[]) {
	int   i, pos, n;
	int   expindx, hostindx, newindx;
	HMENU expmenu, hostmenu, newmenu;

	for (i = 0; i < nhost; ++i) {
		printf("adding host %s\n", host[i]);
	}

	/* Find "Experiment" pulldown menu */
	n = GetMenuItemCount(MainMenu);
	for (i = 0; i < n; ++i) {
		char mname[100];

		GetMenuString(MainMenu, i, mname, sizeof(mname), MF_BYPOSITION);
		if (strmatch("*xperiment*", mname)) break;
	}

	if (i == n) {
		fprintf(stderr, "AddHostItems: couldn't find Experiment pulldown\n");
		return;
	}

	printf("found experiment pulldown at location %d\n", i);
	/* Get a handle to this menu */
	expindx = i;
	expmenu = GetSubMenu(MainMenu, expindx);

	/* Now find the "Hosts" submenu */
	n = GetMenuItemCount(expmenu);
	for (i = 0; i < n; ++i) {
		char mname[100];

		GetMenuString(expmenu, i, mname, sizeof(mname), MF_BYPOSITION);
		if (strmatch("*ost*", mname)) break;
	}

	if (i == n) {
		fprintf(stderr, "AddHostItems: couldn't find Host pulldown\n");
		return;
	}
	hostindx = i;
	hostmenu = GetSubMenu(expmenu, hostindx);

	printf("found host submenu at location %d\n", i);

	/* Now find the dummy entry */

	n = GetMenuItemCount(hostmenu);
	for (i = 0; i < n; ++i) {
		char mname[100];

		if (GetMenuItemID(hostmenu, i) == HOST_MENU_HOST_0) break;
	}

	if (i == n) {
		fprintf(stderr, "AddHostItems: couldn't find Host_O item\n");
		return;
	}
	pos = i;
	RemoveMenu(hostmenu, pos, MF_BYPOSITION);
	if (pos == n-1) pos = -1;

	for (i = 0; i < nhost; ++i) {
 printf("AddHostItems: inserting %s before item %d\n", host[i], pos);
		InsertMenu(hostmenu, pos, MF_BYPOSITION|MF_STRING,
			   HOST_MENU_HOST_0+i, host[i]);
	}
}

INTERNAL void
AddExpHostItems(int nhost, char *host[]) {
	int   i, pos, n;
	int   expindx, hostindx, newindx;
	HMENU expmenu, hostmenu, newmenu;

	if (!nhost) return;

	for (i = 0; i < nhost; ++i) {
		printf("adding exp host %s\n", host[i]);
	}

	/* Find "Experiment" pulldown menu */
	expindx = LocateItem(MainMenu, "*xperiment*");

	if (expindx < 0) {
		fprintf(stderr, "AddExpHostItems: couldn't find Experiment pulldown\n");
		return;
	}

	/* Get a handle to this menu */
	expmenu = GetSubMenu(MainMenu, expindx);

	/* Now find the "Hosts" submenu */
	hostindx = LocateItem(expmenu, "*ost*");

	if (hostindx < 0) {
		fprintf(stderr, "AddExpHostItems: couldn't find Host pulldown\n");
		return;
	}
	hostmenu = GetSubMenu(expmenu, hostindx);

	/* Find the "new experiment" menu */
	newindx = LocateItem(hostmenu, "New*");
	if (newindx < 0) {
		fprintf(stderr, "AddExpHostItems: couldn't find New pulldown\n");
		return;
	}
	newmenu = GetSubMenu(hostmenu, newindx);

	/* Now find the dummy entry */
	n = GetMenuItemCount(newmenu);
	for (i = 0; i < n; ++i) {
		char mname[100];

		if (GetMenuItemID(newmenu, i) == HOST_MENU_NEW_0) break;
	}

	if (i == n) {
		fprintf(stderr, "AddHostExpItems: couldn't find NEW_O item\n");
		return;
	}

	pos = i;
	RemoveMenu(newmenu, pos, MF_BYPOSITION);
	if (pos == n-1) pos = -1;

	for (i = 0; i < nhost; ++i) {
 printf("AddExpHostItems: inserting %s before item %d\n", host[i], pos);
		InsertMenu(newmenu, pos, MF_BYPOSITION|MF_STRING,
			   HOST_MENU_NEW_0+i, host[i]);
	}
}

INTERNAL void
DelHostItems(void) {
	int   i, n, item;
	int   expindx, hostindx;
	HMENU expmenu, hostmenu;

	/* Find "Experiment" pulldown menu */
	expindx = LocateItem(MainMenu, "*xperiment*");

	if (expindx < 0) {
		fprintf(stderr, "DelHostItems: couldn't find Experiment pulldown\n");
		return;
	}

	/* Get a handle to this menu */
	expmenu = GetSubMenu(MainMenu, expindx);

	/* Now find the "Hosts" submenu */
	hostindx = LocateItem(expmenu, "*ost*");

	if (hostindx < 0) {
		fprintf(stderr, "DelHostItems: couldn't find Host pulldown\n");
		return;
	}
	hostmenu = GetSubMenu(expmenu, hostindx);

	/* Now find the dummy entry */
	n = GetMenuItemCount(hostmenu);
	for (i = 0; i < n; ) {
		item = GetMenuItemID(hostmenu, i);
		if (HOST_MENU_HOST_0 <= item && item <= HOST_MENU_HOST_N)

		if (GetMenuItemID(hostmenu, i) == HOST_MENU_HOST_0) {
			printf("removing host %d\n", i);
			RemoveMenu(hostmenu, i, MF_BYPOSITION);
			--n;
		}
		else ++i;
	}
}

INTERNAL void
HostMenu(int enable) {
	HMENU expmenu, hostmenu;
	int   exppos, hostpos;

	/* Find the "host/connection" submenu */
	exppos = LocateItem(MainMenu, "*xper*");
	if (exppos < 0) {
		fprintf(stderr, "HostMenu: couldn't locate experiment menu\n");
		return;
	}
	expmenu = GetSubMenu(MainMenu, exppos);

	/* Find the "host/connection" submenu */
	hostpos = LocateItem(expmenu, "*ost*");

	if (hostpos < 0) {
		fprintf(stderr, "HostMenu: couldn't locate host submenu\n");
		return;
	}
	/*
	 * Enable menu host connection function
	 */
	EnableMenuItem(expmenu, hostpos,
		       MF_BYPOSITION|(enable?MF_ENABLED:MF_GRAYED));
	EnableMenuItem(MainMenu, HOST_MENU_CONN,
		       MF_BYCOMMAND|(enable?MF_ENABLED:MF_GRAYED));

	/*
	 * Enable functions which are only allowed when connected
	 */
	EnableMenuItem(MainMenu, TOOL_MENU_CONSOLE,
		       MF_BYCOMMAND|(!enable?MF_ENABLED:MF_GRAYED));

	EnableMenuItem(MainMenu, TOOL_MENU_EXPMON,
		       MF_BYCOMMAND|(!enable?MF_ENABLED:MF_GRAYED));

	EnableMenuItem(MainMenu, TOOL_MENU_FSERV,
		       MF_BYCOMMAND|(!enable?MF_ENABLED:MF_GRAYED));

	EnableMenuItem(MainMenu, RUN_MENU_RUN,
		       MF_BYCOMMAND|(!enable?MF_ENABLED:MF_GRAYED));

	EnableMenuItem(MainMenu, HOST_MENU_DISCONN,
		       MF_BYCOMMAND|(!enable?MF_ENABLED:MF_GRAYED));

	EnableMenuItem(MainMenu, TOOL_MENU_CLEANUP,
		       MF_BYCOMMAND|(!enable?MF_ENABLED:MF_GRAYED));

	DrawMenuBar(MainWin);
}

INTERNAL void
AddProbes(int imod) {
	HMENU pmenu;
	int   pindx;
	char  mstr[200], mname[200];
	int   i, nitem, nout, mid;

	sprintf(mstr, "*%s.*", Modules[imod].instname);

	pindx = LocateItem(MainMenu, "*robe*");
	if (pindx < 0) {
		printf("Couldn't find probe menu!!\n");
		return;
	}
	pmenu = GetSubMenu(MainMenu, pindx);

	nout  = Modules[imod].noutput;
	nitem = GetMenuItemCount(pmenu);

	for (i = 0; i < nout; ++i) {
		mid = PROBE_MENU_PROBE_0 + nitem+i;
		sprintf(mname, "%s.%d", Modules[imod].instname, i+1);
		if (Debug)
		   printf("AddProbes: adding probe item %d/%d %s\n", i, mid, mname);
		AppendMenu(pmenu, MF_ENABLED|MF_STRING,
			   mid, mname);
	}
}

INTERNAL void
DelProbes(int imod) {
	HMENU pmenu;
	int   pindx;
	char  mstr[200], mname[200];
	int   mid, i, n;

	sprintf(mstr, "*%s.*", Modules[imod].instname);

	pindx = LocateItem(MainMenu, "*robe*");
	if (pindx < 0) {
		printf("Couldn't find probe menu!!\n");
		return;
	}
	pmenu = GetSubMenu(MainMenu, pindx);

	/* Look for probe items for this module */
	n = GetMenuItemCount(pmenu);
	if (Debug)
	   printf("DelProbes: %s in menu %d of %d items\n", mstr, pindx, n);
	for (i = 0; i < n; ) {
		mid = GetMenuItemID(pmenu, i);
		if (mid < PROBE_MENU_PROBE_0 || mid > PROBE_MENU_PROBE_N) {
			++i;
			continue;
		}
		GetMenuString(pmenu, i, mname, sizeof(mname), MF_BYPOSITION);
		if (strmatch(mstr, mname)) {
			if (Debug)
			    printf("DelProbes: removing probe item %d/%d %s\n", i, mid, mname);
			RemoveMenu(pmenu, mid, MF_BYCOMMAND);
			--n;
		}
		else ++i;
	}
}

void
SetDebugMenu(void) {
	int i;

	CheckMenuItem(MainMenu, DEBUG_MENU_VERBOSE,   ModVerbose?MF_CHECKED:MF_UNCHECKED);
	CheckMenuItem(MainMenu, DEBUG_MENU_TRANSPORT, ModTrans?MF_CHECKED:MF_UNCHECKED);
	CheckMenuItem(MainMenu, DEBUG_MENU_FS,        ModFS?MF_CHECKED:MF_UNCHECKED);
	for (i = 0; i <= DEBUG_MENU_DEBUG_MAX - DEBUG_MENU_DEBUG_0; ++i)
	  CheckMenuItem(MainMenu, DEBUG_MENU_DEBUG_0+i,
			(ModDebug == i)?MF_CHECKED:MF_UNCHECKED);
}
#else
INTERNAL void
AddHostItems(int n, char * names[]) { return; }

INTERNAL void
AddExpHostItems(int n, char *host[]) { return; }

INTERNAL void
DelHostItems(void) { return; }

INTERNAL void
HostMenu(int enable) { return; }

INTERNAL void
DelProbes(int imod) { return; }

INTERNAL void
AddProbes(int i) { return; }

INTERNAL void
SetDebugMenu(void) { return; }

#endif

#ifndef METASERVER
# define METASERVER "scv.bu.edu"
#endif

INTERNAL char *
SearchWorld(char *meta, char *name, char *host) {
	EVENT          event;
	EVENT_IDENTIFY eid;
	EVENT_IDENTITY *e;
	char msgbuf[200], *str;
	int  id, nc;
	int  i;
	int  response[MAX_CLIENT], nresponse = 0;
	unsigned long starttime, now, elapsed, lastreq;

	if (meta == NULL) meta = METASERVER;

	id = event_join(meta, &nc);
	if (id < 1) {
		fprintf(stderr,
			"SearchWorld: couldn't connect to meta server at %s\n",
			meta);
		return (NULL);
	}
	event_select_type(0, 0, ET_MAX);
	event_register("agentd", "worldquery", name);

	printf("SearchWorld: connected to Metaworld at %s\n", meta);

	eid.tag = 0;
	BCAST(IDENTIFY, eid);

	starttime = lastreq = event_clock();
	while (nc > nresponse) {

		/* Keep an eye on how long we have been waiting */
		now     = event_clock();
		elapsed = (now - starttime) / 1000;
		if (now - lastreq > 5000) {
			fprintf(stderr,
				"SearchWorld: requesting IDs again after %d secs\n",
				elapsed);
			BCAST(IDENTIFY, eid);
		}
		if (elapsed > 5) {
			fprintf(stderr,
				"SearchWorld: looking for %d more ID responses of %d\n",
				nc-nresponse, nc);
			if (elapsed > 120) {
				fprintf(stderr,
					"SearchWorld: Timeout after %d\n",
					elapsed);
				break;
			}
		}

		event_wait_timeout(1000);
		if (!event_receive(&event)) continue;
		switch (event.ev_head.type) {
		  case ET_IDENTITY:
		       if (elapsed > 5)
			    fprintf(stderr,
				    "SearchWorld: got Identity from %d: %s %s %s (%d of %d)\n",
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
			if ( !streq(e->class, "eventd") ||
			     !strmatch(name, e->instance) ||
			     !strmatch(host, e->host)
			    )  break;

			event_leave();

			str = (char *)malloc(strlen(e->species)+1);
			if (!str) {
				fprintf(stderr, "SearchWorld: couldn't allocate string!!\n");
				return(0);
			}
			strcpy(str, e->species);
			return(str);

		  case ET_JOIN:
			if (event.ev_join.id == id) break;
			if (elapsed > 5)
			   fprintf(stderr, "SearchWorld: client %d joined\n", event.ev_join.id);
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
			   fprintf(stderr, "SearchWorld: client %d left\n", event.ev_leave.id);
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

	fprintf(stderr,
		"SearchWorld: couldn't find world %s on host %s\n",
		name, host);

	return(NULL);
}

INTERNAL int
FindServers(char *master, char *server[], char *agent[], char *name, char *host) {
	static int       didthatbeenthere = 0;
	EVENT            event;
	EVENT_IDENTIFY   eid;
	EVENT_IDENTITY   *e;
	EVENT_AGENTD_REQ equery;
	char msgbuf[200], *str;
	int  id, nc;
	int  i, nserver = 0;
	int  response[MAX_CLIENT], nresponse = 0;
	int  nagentd = 0, nagentdreply = 0, neventd = 0;
	unsigned long starttime, now, elapsed, lastreq;

	if (didthatbeenthere) return;
	didthatbeenthere = 1;

	id = event_join(master, &nc);
	if (id < 1) {
		fprintf(stderr,
			"FindServers: couldn't connect to master server at %s\n",
			master);
		return (0);
	}

	printf("FindServers: connected to master server at %s\n", master);

	event_select_type(0, 0, ET_MAX);
	event_select_type(1, ET_AGENTD_MOD,  ET_AGENTD_MOD);
	event_select_type(1, ET_AGENTD_STAT, ET_AGENTD_STAT);
	event_register("agent", "serverquery", name);

	eid.tag = 0;
	BCAST(IDENTIFY, eid);

	starttime = lastreq = event_clock();
	while (nc > nresponse || nagentd > nagentdreply) {

		/* Keep an eye on how long we have been waiting */
		now     = event_clock();
		elapsed = (now - starttime) / 1000;
		if (now - lastreq > 5000) {
			printf("FindServers: requesting IDs again after %d secs\n",
				elapsed);
			BCAST(IDENTIFY, eid);
			lastreq = now;
		}
		if (elapsed > 5 || Debug) {
			printf("FindServers: looking for %d more ID responses of %d\n",
				nc-nresponse, nc);
			if (elapsed > 120) {
				printf("FindServers: timeout after %d seconds\n", elapsed);
				break;
			}
		}

		event_wait_timeout(1000);
		if (!event_receive(&event)) continue;
		switch (event.ev_head.type) {
		  case ET_IDENTITY:
		       if (elapsed > 5 || Debug)
			    printf("FindServers: got Identity from %d: %s %s %s (%d of %d)\n",
				    event.ev_head.from,
				    event.ev_identity.class,
				    event.ev_identity.species,
				    event.ev_identity.instance,
				    nresponse, nc);

			/* Check for duplicate responses */
			for (i = 0; i < nresponse; ++i)
			   if (event.ev_head.from == response[i]) break;
			if (i < nresponse) break; /* already heard from this client */

			response[nresponse++] = event.ev_head.from;

			e = &event.ev_identity;

			/* Check for agentd on matching hosts */
			if ( streq("daemon", e->class) &&
			     streq("agentd", e->species) &&
			     strmatch(host,  e->host)) {
				printf("FindServers: found agentd on %s\n",
					e->host);

				/* send module query for an eventd */
				equery.type     = AGENTD_CTL_QUERY;
				equery.flags    = 0;
				equery.auth     = 0;
				equery.authtype = 0;
				equery.tag      = 0;
				strcpy(equery.requester, (char *)cuserid(NULL));
				strcpy(equery.log,    "");
				strcpy(equery.server, "");
				strcpy(equery.module, "eventd");
				strcpy(equery.args,   "");

				SEND(EventFrom(event), AGENTD_REQ, equery);

				++nagentd;

				break;
			}

			/* Look for running event servers only */
			if ( !streq(e->class, "eventd") ||
			     !strmatch(name, e->instance) ||
			     !strmatch(host, e->host)
			    )  break;

			str = (char *)malloc(strlen(e->species)+1);
			if (!str) {
				fprintf(stderr, "FindServers: couldn't allocate string!!\n");
				return(0);
			}
			strcpy(str, e->species);
			server[nserver++] = str;
			break;

		  case ET_JOIN:
			if (event.ev_join.id == id) break;
			if (elapsed > 5 || Debug)
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
			if (elapsed > 5 || Debug)
			   printf("Client %d left\n", event.ev_leave.id);
			for (i = 0; i < nresponse; ++i)
			    if (event.ev_leave.id == response[i]) break;

			if (i == nresponse) {
				/* This client left before responding */
				--nc;
			}
			break;


		  case ET_AGENTD_MOD: {
			EVENT_AGENTD_MOD *emod = (EVENT_AGENTD_MOD *)&event;
			char             *avail;
			int               i, len;

			avail = &emod->modules[0];

			for (i = 0; i < emod->nmodules; ++i, avail += strlen(avail)+1)
			  if (strmatch("eventd*", avail) &&
			      strmatch(host, emod->hostname)) {
				str = (char *)malloc(strlen(emod->hostname)+1);
				if (!str) {
					fprintf(stderr, "FindServers: couldn't allocate string!!\n");
					return(0);
				}
				strcpy(str, emod->hostname);
				agent[neventd++] = str;
				printf("FindServers: eventd available on %s\n",
					emod->hostname);
				break;
			  }
			break;
		  }

		  case ET_AGENTD_STAT: {
			EVENT_AGENTD_STAT *estat = (EVENT_AGENTD_STAT *)&event;
			if (estat->reqtype != AGENTD_CTL_QUERY) break;
			nagentdreply++;
			break;
		  }

		  default:
			break;
		}
	}
	server[nserver] = NULL;
	agent[neventd]  = NULL;

	event_leave();

	printf("FindServers: found %d servers on master %s\n", nserver, master);

	return(nserver);
}

