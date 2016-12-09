#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum Flag {AFLAG, DFLAG, NFLAG, SFLAG, TFLAG, NUMFLAG};
enum Val {AVAL, DVAL, NVAL, SVAL, TVAL, NUMVAL};

int enter(int *, char *[64], char *);

int main(int argc, char *argv[]) {
	int index, c;
	char command[64];
	int flags[NUMFLAG];
	char *vals[NUMVAL];

	opterr = 0;
	for (int i = 0; i < NUMFLAG; i++) flags[i] = 0;
	for (int i = 0; i < NUMVAL; i++) vals[i] = NULL;

	if (argc == 1) {
		enter(flags, vals, "list");
		return 0;
	}
	else optind = 2;

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

	// Count number of args after options, must be 0 or fail
	if (optind < argc) {
		printf("Usage: ttrack COMMAND [-OPTIONS ARG]");
		return 1;
	}

	// Pass validated info to next step
	enter(flags, vals, argv[1]);

	return 0;
}

int enter(int *n, char *v[64], char *command) {
	// Debug/verification print
	for (int i = 0; i < NUMFLAG; i++) {
		printf("arg %d = %d %s\n", i, n[i], v[i]);
	}
	printf("command = %s\n", command);

	return 0;
}
