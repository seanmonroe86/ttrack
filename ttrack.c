#include "ttrack.h"

void getfile(FILE **, char *, char *, char **);
void getsav(char [STR_MAX], struct Timer *);
int enter(struct Timer *, char *);
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
	strncpy(timer.sav[NAME], "Timer", STR_MAX);

	opterr = 0;
	for (int i = 0; i < NUMFLAG; i++) timer.flags[i] = 0;
	for (int i = 0; i < NUMVAL; i++) timer.vals[i][0] = 0;

	if (argc == 1) {
		strncpy(timer.command, "list", 5);
		optind = 1;
	}
	else if ((argc >= 3) && (argv[2][0] != '-')) {
		strncpy(timer.command, argv[1], STR_MAX);
		strncpy(timer.sav[NAME], argv[2], STR_MAX);
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
					printf("%s '-%c'.\n", "Unknown option", optopt);
				else
					printf("%s '-%x'.\n", "Unknown character", optopt);
				return 1;
			default:
				abort();
		}
	}

	// Make command lowercase, pass validated info to next step
	for (int i = 0; timer.command[i]; i++)
		timer.command[i] = tolower(timer.command[i]);
	enter(&timer, argv[0]);

	return 0;
}


int enter(struct Timer *t, char *com) {
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
		printf("%s\n", "Invalid command.");
		return 1;
	}

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


void getsav(char s[STR_MAX], struct Timer *t) {
	char *token;
	char empty[] = "";

	// Timer start time
	token = strtok(s, DELIM);
	sscanf(token, "%d", &(t->time));
	
	// Timer start difference to current time
	t->timediff = time(NULL) - t->time;

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
	FILE *f;
	int test;
	struct Timer timer;
	char timer_text[STR_MAX];
	char *filename = malloc(sizeof(char) * STR_MAX);

	// Check timer file exists
	getfile(&f, CURRFILE, "r", &filename);
	test = access(filename, F_OK);
	free(filename);
	if (test == -1) {
		printf("%s", "No timer");
		exit(0);
	}

	// Get string within timer file
	fgets(timer_text, STR_MAX, f);
	fclose(f);

	// If string is empty, there is no timer, exit
	if (timer_text[0] < '0') {
		printf("%s", "No timer");
		exit(0);
	}

	// Parse string into name and time value
	getsav(timer_text, &timer);
	printf("%s: %d%s", timer.sav[NAME], timer.timediff, "s");

	return 0;
}


int start(struct Timer *t) {
	FILE *f;
	char *filename = malloc(sizeof(char) * STR_MAX);

	// Populate the sav array of timer struct
	strncpy(t->sav[NAME], t->sav[NAME],	 STR_MAX);
	strncpy(t->sav[NOTE], t->vals[NVAL], STR_MAX);
	strncpy(t->sav[TAG],  t->vals[TVAL], STR_MAX);

	// Save data to file
	getfile(&f, CURRFILE, "w", &filename);
	fprintf(f, "%d%s", time(NULL),	  DELIM);
	fprintf(f, "%s%s", t->sav[NAME],  DELIM);
	fprintf(f, "%s%s", t->vals[NVAL], DELIM);
	fprintf(f, "%s",   t->vals[TVAL]);

	// Free pointers
	free(filename);
	fclose(f);

	return 0;
}


int stop(struct Timer *t) {
	FILE *f;
	int test;
	char timer_text[STR_MAX];
	char *filename = malloc(sizeof(char) * STR_MAX);

	// Check timer file exists
	getfile(&f, CURRFILE, "r", &filename);
	test = access(filename, F_OK);
	if (test == -1) {
		printf("%s\n", "No timer to stop.");
		exit(0);
	}

	// Get entry data
	fgets(timer_text, STR_MAX, f);
	fclose(f);

	// If string is empty, there is no timer, exit
	if (timer_text[0] < '0') {
		printf("%s\n", "No timer to stop.");
		exit(0);
	}

	// Delete start entry
	getfile(&f, CURRFILE, "w", &filename);
	fputc('\0', f);
	fclose(f);

	// Parse data
	getsav(timer_text, t);

	// Save data to file
	getfile(&f, SAVEFILE, "a", &filename);
	fprintf(f, "%d%s", t->timediff,  DELIM);
	fprintf(f, "%s%s", t->sav[NAME], DELIM);
	fprintf(f, "%s%s", t->sav[NVAL], DELIM);
	fprintf(f, "%s%c",   t->sav[TVAL], '\n');
	fclose(f);

	// Free pointers
	free(filename);

	return 0;
}


int edit(struct Timer *e) {
	int test;
	FILE *f;
	struct Timer timer;
	struct Timer *t = &timer;
	char timer_text[STR_MAX];
	char *filename = malloc(sizeof(char) * STR_MAX);

	// Check timer file exists
	getfile(&f, CURRFILE, "r", &filename);
	test = access(filename, F_OK);
	if (test == -1) {
		printf("%s\n", "No timer to edit.");
		exit(0);
	}

	// Get entry data
	fgets(timer_text, STR_MAX, f);
	fclose(f);

	// If string is empty, there is no timer, exit
	if (timer_text[0] < '0') {
		printf("%s\n", "No timer to edit.");
		exit(0);
	}

	// Delete start entry
	getfile(&f, CURRFILE, "w", &filename);
	fputc('\0', f);
	fclose(f);

	// Parse data
	getsav(timer_text, t);

	// Overwrite the timer struct with edit overrides, if present
	if ((strncmp(e->sav[NAME], "", STR_MAX) != 0) &&
		(strncmp(e->sav[NAME], "Timer", STR_MAX) != 0))
		strncpy(t->sav[NAME], e->sav[NAME], STR_MAX);

	if (strncmp(e->vals[NVAL], "", STR_MAX) != 0)
		strncpy(t->sav[NOTE], e->vals[NVAL], STR_MAX);

	if (strncmp(e->vals[TVAL], "", STR_MAX) != 0)
		strncpy(t->sav[TAG], e->vals[TVAL], STR_MAX);

	// Save data to file
	getfile(&f, CURRFILE, "w", &filename);
	fprintf(f, "%d%s", t->time,	     DELIM);
	fprintf(f, "%s%s", t->sav[NAME], DELIM);
	fprintf(f, "%s%s", t->sav[NOTE], DELIM);
	fprintf(f, "%s",   t->sav[TAG]);
	fclose(f);

	// Free pointers
	free(filename);

	return 0;
}


int help() {
	printf("%s\n", README);
	return 0;
}


int status() {
	int test;
	FILE *f;
	struct Timer timer;
	struct Timer *t = &timer;
	char timer_text[STR_MAX];
	char *filename = malloc(sizeof(char) * STR_MAX);

	// Check timer file exists
	getfile(&f, CURRFILE, "r", &filename);
	test = access(filename, F_OK);
	if (test == -1) {
		printf("%s\n", "No timer.");
		exit(0);
	}

	// Get entry data
	fgets(timer_text, STR_MAX, f);
	fclose(f);

	// If string is empty, there is no timer, exit
	if (timer_text[0] < '0') {
		printf("%s\n", "No timer.");
		exit(0);
	}

	// Parse data
	getsav(timer_text, t);

	// Print data
	for (int i = 0; i < NUMSAV; i++) {
		printf("%s: %s\n", SAV[i], t->sav[i]);
	}
	printf("%s: %s\n", "Started", ctime(&(t->time)));

	// Free pointers
	free(filename);
	
	return 0;
}


int report() {
	int test;
	FILE *f;
	struct Timer timer;
	struct Timer *t = &timer;
	char timer_text[STR_MAX];
	char *filename = malloc(sizeof(char) * STR_MAX);

	// Check timer file exists
	getfile(&f, SAVEFILE, "r", &filename);
	test = access(filename, F_OK);
	if (test == -1) {
		printf("%s\n", "No records.");
		exit(0);
	}

	// Read contents of record until EOF
	while ((fgets(timer_text, STR_MAX, f)) != NULL) {
		getsav(timer_text, t);

		// Print data
		for (int i = 0; i < NUMSAV; i++) {
			printf("%s: %s\n", SAV[i], t->sav[i]);
		}
		printf("%s: %s\n", "Started", ctime(&(t->time)));
	}

	// Free pointers
	free(filename);

	return 0;
}


int delete(struct Timer *t) {return 0;}
