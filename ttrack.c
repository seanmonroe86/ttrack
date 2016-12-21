#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>


enum Flag {AFLAG, DFLAG, NFLAG, SFLAG, TFLAG, NUMFLAG};
enum Val {AVAL, DVAL, NVAL, SVAL, TVAL, NUMVAL};


void getfile(FILE **, char *, char *);
void putfile(FILE *, char *[64]);
int enter(int *, char [NUMVAL][64], char *, char *);
int start(char *, char [NUMVAL][64]);
int edit(char *[64]);
int list();
int status();
int stop(char *[64]);
int report();
int delete(char *);
int help();


int main(int argc, char *argv[]) {
	int index, c;
	char command[64];
	int flags[NUMFLAG];
	char vals[NUMVAL][64];
	char name[64] = "Timer";

	opterr = 0;
	for (int i = 0; i < NUMFLAG; i++) flags[i] = 0;
	for (int i = 0; i < NUMVAL; i++) vals[i][0] = 0;

	if (argc == 1) {
		strncpy(command, "list", 5);
		optind = 1;
		//enter(flags, vals, "list", name);
		//return 0;
	}
	else if ((argc == 3) && (argv[2][0] != '-')) {
		strncpy(command, argv[1], 64);
		strncpy(name, argv[2], 64);
		optind = 3;
	}
	else {
		strncpy(command, argv[1], 64);
		optind = 2;
	}

	// Up to one of each option is permitted. They both require an argument.
	// Failure at any point aborts with an error message.
	while ((c = getopt(argc, argv, "a:d:n:s:t:")) != -1) {
		switch (c) {
			case 'a':
				vals[AVAL] = optarg;
				flags[AFLAG]++;
				if (flags[AFLAG] > 1) {
					printf("Only one argument -%c allowed.\n", c);
					return 1;
				}
				if (vals[AVAL][0] == '-') {
					printf("Option -%c requires an argument.\n", c);
					return 1;
				}
				break;
			case 'd':
				vals[DVAL] = optarg;
				flags[DFLAG]++;
				if (flags[DFLAG] > 1) {
					printf("Only one argument -%c allowed.\n", c);
					return 1;
				}
				if (vals[DVAL][0] == '-') {
					printf("Option -%c requires an argument.\n", c);
					return 1;
				}
				break;
			case 'n':
				vals[NVAL] = optarg;
				flags[NFLAG]++;
				if (flags[NFLAG] > 1) {
					printf("Only one argument -%c allowed.\n", c);
					return 1;
				}
				if (vals[NVAL][0] == '-') {
					printf("Option -%c requires an argument.\n", c);
					return 1;
				}
				break;
			case 's':
				vals[SVAL] = optarg;
				flags[SFLAG]++;
				if (flags[SFLAG] > 1) {
					printf("Only one argument -%c allowed.\n", c);
					return 1;
				}
				if (vals[SVAL][0] == '-') {
					printf("Option -%c requires an argument.\n", c);
					return 1;
				}
				break;
			case 't':
				vals[TVAL] = optarg;
				flags[TFLAG]++;
				if (flags[TFLAG] > 1) {
					printf("Only one argument -%c allowed.\n", c);
					return 1;
				}
				if (vals[TVAL][0] == '-') {
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
	for (int i = 0; command[i]; i++) command[i] = tolower(command[i]);
	enter(flags, vals, command, name);

	return 0;
}


void getfile(FILE **fp, char *name, char *m) {
	DIR *dp;
	struct dirent *ep;
	char filename[99] = "/home/sean/.config/ttrack/";
	dp = opendir(filename);
	if (dp != NULL) {
		closedir(dp);
		strncat(filename, name, 64);
		*fp = fopen(filename, m);
	}
	else {
		mkdir(filename, 0755);
		getfile(fp, name, m);
	}
}


void putfile(FILE *f, char *data[64]) {
	for (int i = 0; i <= NUMVAL; i++) {
		printf("fputs(data[%d], f);", i);
		fputs(data[i], f);
		printf("fputc(';', f);");
		fputc(';', f);
	}
}


int enter(int *n, char v[NUMVAL][64], char *command, char *name) {
	// Debug/verification print
	for (int i = 0; i < NUMFLAG; i++) {
		printf("arg %d = %d %s\n", i, n[i], v[i]);
	}
	printf("name = %s\n", name);
	printf("command = %s\n", command);

	// Find the command and call the correct action with params
	if (strncmp(command, "start", 5) == 0) {
		printf("Start %s -a=%s -d=%s -n=%s -s=%s -t=%s\n",
				name, v[AVAL], v[DVAL], v[NVAL], v[SVAL], v[TVAL]);
		start(name, v);
	}
	else if (strncmp(command, "edit", 4) == 0) {
		printf("Edit -a=%s -d=%s -n=%s -s=%s -t=%s\n",
				v[AVAL], v[DVAL], v[NVAL], v[SVAL], v[TVAL]);
		edit(v);
	}
	else if (strncmp(command, "list", 4) == 0) {
		printf("List\n");
		list();
	}
	else if (strncmp(command, "status", 6) == 0) {
		printf("Status\n");
		status();
	}
	else if (strncmp(command, "stop", 4) == 0) {
		printf("Stop -a=%s -d=%s -n=%s -s=%s\n",
				v[AVAL], v[DVAL], v[NVAL], v[SVAL]);
		stop(v);
	}
	else if (strncmp(command, "report", 6) == 0) {
		printf("Report\n");
		report();
	}
	else if (strncmp(command, "delete", 6) == 0) {
		printf("Delete %s\n", name);
		delete(name);
	}
	else if (strncmp(command, "help", 4) == 0) {
		printf("Help\n");
		help();
	}
	else {
		printf("Invalid command.\n");
		return 1;
	}

	return 0;
}


int list() {
	FILE *test;
	getfile(&test, "testfile.txt", "a");
	fclose(test);
	return 0;
}


int start(char *n, char v[NUMVAL][64]) {
	FILE *test;
	char d[NUMVAL + 1][64];

	strncpy(d[0], n, 64);
	for (int i = 0; i < NUMVAL; i++)
		strncpy(d[i + 1], v[i], 64);


	getfile(&test, "testwriting.txt", "a");
	//putfile(test, d);
	fclose(test);
	return 0;
}


int edit(char *v[64]) {return 0;}
int status() {return 0;}
int stop(char *v[64]) {return 0;}
int report() {return 0;}
int delete(char *n) {return 0;}
int help() {return 0;}

