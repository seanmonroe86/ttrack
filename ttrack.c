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

struct Timer {
	char command[STR_MAX];
	int flags[NUMFLAG];
	char vals[NUMVAL][STR_MAX];
	char name[STR_MAX];
	char sav[NUMSAV][STR_MAX];
	time_t time;
};


void getfile(FILE **, char *, char *, char **);
void getsav(char [STR_MAX], struct Timer *);
int enter(struct Timer *);
int start(struct Timer *);
int edit(struct Timer *);
int list();
int status();
int stop(struct Timer *);
int report();
int delete(struct Timer *);
int help();


int main(int argc, char *argv[]) {
	char c;
	struct Timer timer;
	strncpy(timer.name, "Timer", STR_MAX);

	opterr = 0;
	for (int i = 0; i < NUMFLAG; i++) timer.flags[i] = 0;
	for (int i = 0; i < NUMVAL; i++) timer.vals[i][0] = 0;

	if (argc == 1) {
		strncpy(timer.command, "list", 5);
		optind = 1;
	}
	else if ((argc == 3) && (argv[2][0] != '-')) {
		strncpy(timer.command, argv[1], STR_MAX);
		strncpy(timer.name, argv[2], STR_MAX);
		optind = 3;
	}
	else {
		strncpy(timer.command, argv[1], STR_MAX);
		optind = 2;
	}

	// Up to one of each option is permitted. They both require an argument
	// Failure at any point aborts with an error message.
	while ((c = getopt(argc, argv, "a:d:n:s:t:")) != -1) {
		switch (c) {
			case 'a':
				strncpy(timer.vals[AVAL], optarg, STR_MAX);
				timer.flags[AFLAG]++;
				if (timer.flags[AFLAG] > 1) {
					printf("Only one argument -%c allowed.\n", c);
					return 1;
				}
				if (timer.vals[AVAL][0] == '-') {
					printf("Option -%c requires an argument.\n", c);
					return 1;
				}
				break;
			case 'd':
				strncpy(timer.vals[DVAL], optarg, STR_MAX);
				timer.flags[DFLAG]++;
				if (timer.flags[DFLAG] > 1) {
					printf("Only one argument -%c allowed.\n", c);
					return 1;
				}
				if (timer.vals[DVAL][0] == '-') {
					printf("Option -%c requires an argument.\n", c);
					return 1;
				}
				break;
			case 'n':
				strncpy(timer.vals[NVAL], optarg, STR_MAX);
				timer.flags[NFLAG]++;
				if (timer.flags[NFLAG] > 1) {
					printf("Only one argument -%c allowed.\n", c);
					return 1;
				}
				if (timer.vals[NVAL][0] == '-') {
					printf("Option -%c requires an argument.\n", c);
					return 1;
				}
				break;
			case 's':
				strncpy(timer.vals[SVAL], optarg, STR_MAX);
				timer.flags[SFLAG]++;
				if (timer.flags[SFLAG] > 1) {
					printf("Only one argument -%c allowed.\n", c);
					return 1;
				}
				if (timer.vals[SVAL][0] == '-') {
					printf("Option -%c requires an argument.\n", c);
					return 1;
				}
				break;
			case 't':
				strncpy(timer.vals[TVAL], optarg, STR_MAX);
				timer.flags[TFLAG]++;
				if (timer.flags[TFLAG] > 1) {
					printf("Only one argument -%c allowed.\n", c);
					return 1;
				}
				if (timer.vals[TVAL][0] == '-') {
					printf("Option -%c requires an argument.\n", c);
					return 1;
				}
				break;
			case '?':
				if ((optopt == 'n') || (optopt == 't'))
					printf("Option -%c requires an argument.\n", optopt);
				else if (isprint(optopt))
					printf("Unknown option '-%c'.\n", optopt);
				else
					printf("Unknown character '-%x'.\n", optopt);
				return 1;
			default:
				abort();
		}
	}

	// Make command lowercase, pass validated info to next step
	for (int i = 0; timer.command[i]; i++)
		timer.command[i] = tolower(timer.command[i]);
	enter(&timer);

	return 0;
}


void getfile(FILE **fp, char *f, char *m, char **fn) {
	DIR *dp;
	struct dirent *ep;
	char *config;

	// Wipe filename string
	strncpy(*fn, "", STR_MAX);

	// Check for a home directory; otherwise, use script dir
	if ((config = getenv("HOME")) == NULL) {
		config = getpwuid(getuid())->pw_dir;
		strncat(*fn, config, STR_MAX);
	}
	else {
		strncat(*fn, config, STR_MAX);
		strncat(*fn, "/.config/ttrack/", STR_MAX);
	}

	// Check if .config folder exists; otherwise, make it
	dp = opendir(*fn);
	if (dp != NULL) {
		closedir(dp);
		strncat(*fn, f, STR_MAX);
		*fp = fopen(*fn, m);
	}
	else {
		mkdir(*fn, 0755);
		getfile(fp, f, m, fn);
	}
}


int enter(struct Timer *t) {
	// Find the command and call the correct action with params
	if		(strncmp(t->command, "edit", 4)   == 0) edit(t);
	else if (strncmp(t->command, "help", 4)   == 0) help();
	else if (strncmp(t->command, "list", 4)   == 0) list();
	else if (strncmp(t->command, "stop", 4)   == 0) stop(t);
	else if	(strncmp(t->command, "start", 5)  == 0) start(t);
	else if (strncmp(t->command, "delete", 6) == 0) delete(t);
	else if (strncmp(t->command, "report", 6) == 0) report();
	else if (strncmp(t->command, "status", 6) == 0) status();
	else {
		printf("Invalid command.\n");
		return 1;
	}

	return 0;
}


void getsav(char s[STR_MAX], struct Timer *t) {
	char *token;
	char empty[] = "";
	time_t record_time;

	// Timer date difference to current time
	token = strtok(s, DELIM);
	sscanf(token, "%d", &record_time);
	t->time = time(NULL) - record_time;

	// Timer name
	token = strtok(NULL, DELIM);
	strncpy(t->sav[NAME], token, STR_MAX);

	// Timer note
	token = strtok(NULL, DELIM);
	if (!token) token = (char *) empty;
	strncpy(t->sav[NOTE], token, STR_MAX);

	// Timer tag
	token = strtok(NULL, DELIM);
	if (!token) token = (char *) empty;
	strncpy(t->sav[TAG], token, STR_MAX);
}


int list() {
	FILE *timer_file;
	char timer_text[STR_MAX];
	struct Timer timer;
	char *filename = malloc(sizeof(char) * STR_MAX);
	int test;

	// Check timer file exists
	getfile(&timer_file, CURRFILE, "r", &filename);
	test = access(filename, F_OK);
	free(filename);
	if (test == -1) {
		printf("%s", "No timer");
		exit(0);
	}

	// Get string within timer file
	fgets(timer_text, STR_MAX, timer_file);
	fclose(timer_file);

	// If string is empty, there is no timer, exit
	if (timer_text[0] < '0') {
		printf("%s", "No timer");
		exit(0);
	}

	// Parse string into name and time value
	getsav(timer_text, &timer);
	printf("%s: %d%s", timer.sav[NAME], timer.time, "s");

	return 0;
}


int start(struct Timer *t) {
	FILE *out;
	char time_string[STR_MAX];
	char *filename = malloc(sizeof(char) * STR_MAX);
	int test;

	// Populate the sav array of timer struct
	strncpy(t->sav[NAME], t->name, STR_MAX);
	strncpy(t->sav[NOTE], t->vals[NVAL], STR_MAX);
	strncpy(t->sav[TAG], t->vals[TVAL], STR_MAX);
	sprintf(time_string, "%d", time(NULL));

	// Save data to file
	getfile(&out, CURRFILE, "w", &filename);
	free(filename);
	fputs(time_string, out);
	fputs(DELIM, out);
	for (int i = 0; i < NUMSAV - 1; i++) {
		fputs(t->sav[i], out);
		fputs(DELIM, out);
	}
	fputs(t->sav[NUMSAV - 1], out);
	fclose(out);

	return 0;
}


int stop(struct Timer *t) {
	FILE *in, *out;
	char timer_text[STR_MAX];
	char time_string[STR_MAX];
	char *filename = malloc(sizeof(char) * STR_MAX);
	int test;

	// Check timer file exists
	getfile(&in, CURRFILE, "r", &filename);
	test = access(filename, F_OK);
	free(filename);
	if (test == -1) {
		printf("%s\n", "No timer to stop.");
		exit(0);
	}

	// Get entry data
	fgets(timer_text, STR_MAX, in);
	fclose(in);

	// If string is empty, there is no timer, exit
	if (timer_text[0] < '0') {
		printf("%s\n", "No timer to stop.");
		exit(0);
	}

	// Delete start entry
	getfile(&in, CURRFILE, "w", &filename);
	fputc('\0', in);
	fclose(in);

	// Parse data
	getsav(timer_text, t);
	sprintf(time_string, "%d", t->time);

	// Save data to file
	getfile(&out, SAVEFILE, "a", &filename);
	fputs(time_string, out);
	fputs(DELIM, out);
	for (int i = 0; i < NUMSAV - 1; i++) {
		fputs(t->sav[i], out);
		fputs(DELIM, out);
	}
	fputs(t->sav[NUMSAV - 1], out);
	fputs("\n", out);
	fclose(out);

	return 0;
}


int edit(struct Timer *e) {
	FILE *in, *out;
	struct Timer timer;
	struct Timer *t = &timer;
	char timer_text[STR_MAX];
	char time_string[STR_MAX];
	char *filename = malloc(sizeof(char) * STR_MAX);
	int test;

	printf("%s\n", "Beginning edit...");

	// Check timer file exists
	getfile(&in, CURRFILE, "r", &filename);
	test = access(filename, F_OK);
	if (test == -1) {
		printf("%s\n", "No timer to edit.");
		exit(0);
	}

	printf("%s\n", "File existence verified...");

	// Get entry data
	fgets(timer_text, STR_MAX, in);
	fclose(in);

	printf("%s\n", "Timer data loaded from file...");

	// If string is empty, there is no timer, exit
	if (timer_text[0] < '0') {
		printf("%s\n", "No timer to edit.");
		exit(0);
	}

	printf("%s\n", "Timer data verified...");

	// Delete start entry
	getfile(&in, CURRFILE, "w", &filename);
	fputc('\0', in);
	fclose(in);

	printf("%s\n", "Timer data reset...");

	// Parse data
	getsav(timer_text, t);
	sprintf(time_string, "%d", t->time);

	printf("%s\n", "Timer data parsed...");

	// Populate the sav array of timer struct
	strncpy(t->sav[NAME], t->name, STR_MAX);
	strncpy(t->sav[NOTE], t->vals[NVAL], STR_MAX);
	strncpy(t->sav[TAG], t->vals[TVAL], STR_MAX);
	sprintf(time_string, "%d", time(NULL));

	printf("%s\n", "Timer sav array populated...");

	// Overwrite the timer struct with edit overrides
	if (strncmp(e->name, "", STR_MAX) != 0)
		strncpy(t->sav[NAME], e->name, STR_MAX);
	if (strncmp(e->vals[NVAL], "", STR_MAX) != 0)
		strncpy(t->sav[NOTE], e->vals[NVAL], STR_MAX);
	if (strncmp(e->vals[TVAL], "", STR_MAX) != 0)
		strncpy(t->sav[TAG], e->vals[TVAL], STR_MAX);

	printf("%s\n", "Timer data sav array edited...");

	// Save data to file
	getfile(&out, CURRFILE, "w", &filename);
	fputs(time_string, out);
	fputs(DELIM, out);
	for (int i = 0; i < NUMSAV - 1; i++) {
		fputs(t->sav[i], out);
		fputs(DELIM, out);
	}
	fputs(t->sav[NUMSAV - 1], out);
	fclose(out);

	printf("%s\n", "Timer data written...");

	return 0;
}


int status() {return 0;}
int report() {return 0;}
int delete(struct Timer *t) {return 0;}
int help() {return 0;}
