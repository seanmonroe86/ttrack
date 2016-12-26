#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

#define STR_MAX 64
#define DELIM ";"

enum Flag {AFLAG, DFLAG, NFLAG, SFLAG, TFLAG, NUMFLAG};
enum Val {AVAL, DVAL, NVAL, SVAL, TVAL, NUMVAL};
enum Sav {NAME, TIME, NOTE, TAG, NUMSAV};

struct Timer {
	char command[STR_MAX];
	int flags[NUMFLAG];
	char vals[NUMVAL][STR_MAX];
	char name[STR_MAX];
	char sav[NUMSAV][STR_MAX];
};


void getfile(FILE **, char *, char *);
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


void getfile(FILE **fp, char *f, char *m) {
	DIR *dp;
	struct dirent *ep;
	char filename[99] = "/home/sean/.config/ttrack/";
	dp = opendir(filename);
	if (dp != NULL) {
		closedir(dp);
		strncat(filename, f, STR_MAX);
		*fp = fopen(filename, m);
	}
	else {
		mkdir(filename, 0755);
		getfile(fp, f, m);
	}
}


int enter(struct Timer *t) {
	// Find the command and call the correct action with params
	if (strncmp(t->command, "start", 5) == 0) {
		printf("start %s -a=%s -d=%s -n=%s -s=%s -t=%s\n",
				t->name, t->vals[AVAL], t->vals[DVAL],
				t->vals[NVAL], t->vals[SVAL], t->vals[TVAL]);
		start(t);
	}
	else if (strncmp(t->command, "edit", 4) == 0) {
		printf("edit -a=%s -d=%s -n=%s -s=%s -t=%s\n",
				t->vals[AVAL], t->vals[DVAL], t->vals[NVAL],
				t->vals[SVAL], t->vals[TVAL]);
		edit(t);
	}
	else if (strncmp(t->command, "list", 4) == 0) list();
	else if (strncmp(t->command, "status", 6) == 0) {
		printf("status\n");
		status();
	}
	else if (strncmp(t->command, "stop", 4) == 0) {
		printf("stop -a=%s -d=%s -n=%s -s=%s\n",
				t->vals[AVAL], t->vals[DVAL], t->vals[NVAL], t->vals[SVAL]);
		stop(t);
	}
	else if (strncmp(t->command, "report", 6) == 0) {
		printf("report\n");
		report();
	}
	else if (strncmp(t->command, "delete", 6) == 0) {
		printf("delete %s\n", t->name);
		delete(t);
	}
	else if (strncmp(t->command, "help", 4) == 0) {
		printf("help\n");
		help();
	}
	else {
		printf("Invalid command.\n");
		return 1;
	}

	return 0;
}


void getsav(char s[STR_MAX], struct Timer *t) {
	char *token;
	char empty[] = "";

	// Get current time
	time_t record_time;
	time_t current_time;
	time(&current_time);

	// Timer name
	token = strtok(s, DELIM);
	strncpy(t->sav[NAME], token, STR_MAX);

	// Timer date difference to current time
	token = strtok(NULL, DELIM);
	sscanf(token, "%d", &record_time);
	snprintf(t->sav[TIME], STR_MAX, "%d", current_time - record_time);

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

	// Get string within timer file
	getfile(&timer_file, "testwriting.txt", "r");
	fgets(timer_text, STR_MAX, timer_file);
	fclose(timer_file);

	// Parse string into name and time value
	getsav(timer_text, &timer);

	printf("%s: %ss", timer.sav[NAME], timer.sav[TIME]);

	return 0;
}


int start(struct Timer *t) {
	FILE *out;
	time_t current_time;
	time(&current_time);

	// Populate the sav array of timer struct
	strncpy(t->sav[NAME], t->name, STR_MAX);
	snprintf(t->sav[TIME], STR_MAX, "%d", current_time);
	strncpy(t->sav[NOTE], t->vals[NVAL], STR_MAX);
	strncpy(t->sav[TAG], t->vals[TVAL], STR_MAX);

	// Save data to file
	getfile(&out, "testwriting.txt", "w");
	for (int i = 0; i < NUMSAV - 1; i++) {
		fputs(t->sav[i], out);
		fputs(DELIM, out);
	}
	fputs(t->sav[NUMSAV - 1], out);
	//fputc('\0', out);
	fclose(out);

	return 0;
}


int edit(struct Timer *t) {return 0;}
int status() {return 0;}
int stop(struct Timer *t) {return 0;}
int report() {return 0;}
int delete(struct Timer *t) {return 0;}
int help() {return 0;}

