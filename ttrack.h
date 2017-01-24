#include <ctype.h>
#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define STR_MAX 64
#define DELIM ";"
#define CURRFILE "current"
#define SAVEFILE "record"

enum Flag {AFLAG, DFLAG, NFLAG, SFLAG, TFLAG, NUMFLAG};
enum Val {AVAL, DVAL, NVAL, SVAL, TVAL, NUMVAL};
enum Sav {NAME, NOTE, TAG, NUMSAV};
const char SAV[NUMSAV][STR_MAX] = {"Name", "Note", "Tag"};

struct Timer {
	char command[STR_MAX];
	int flags[NUMFLAG];
	char vals[NUMVAL][STR_MAX];
	char sav[NUMSAV][STR_MAX];
	time_t time;
	time_t timediff;
};

const char README[] =
"ttrack COMMAND [ARGS]\n\
A simple-looking stopwatch. Starts a new timer, edits the current timer, and\n\
stops the current timer recording its label, note and run time. Tags can be\n\
used to 'group' timers, giving additional cumulative run time for tags of the\n\
same name. Additional arguments can modify the starting time and date of the\n\
new or existing timer. Arguments without a command are ignored. Given no\n\
command, it defaults to 'list'.\n\
\n\
COMMAND\n\
start	NAME	begin new timer with NAME; default Timer\n\
edit			modify currently running timers with new name and/or flag info\n\
list			print timer \"NAME: RUNTIME\" or \"No timer\" otherwise, without newline\n\
status			show details of current timer\n\
stop			stop current timer and make record\n\
\n\
report			prints full details of stopped timers in history ***\n\
delete			delete all info of timer with NAME ***\n\
help			prints this\n\
\n\
ARGS\n\
-a(go)		HH:MM:SS	set start time to current time minus this value default 00:00:00 ***\n\
-d(ate)		MM/DD/YY	set date; default Current Date ***\n\
-s(tart)	HH:MM:SS	set start time; default Current Time ***\n\
\n\
-n(ote)		STRING		set note; default blank\n\
-t(ag)		STRING		set tag; default blank\n\
\n\
*** Not implemented yet.\n";
