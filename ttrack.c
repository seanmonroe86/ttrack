#include <dirent.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void getFile(char *, FILE **);
int enter(int, char *, int, char *, char *);


int main(int argc, char *argv[]) {
	int nflag = 0;
	int tflag = 0;
	char *nval = NULL;
	char *tval = NULL;
	int index, c;
	opterr = 0;

	// Up to one of each option is permitted. They both require an argument.
	// Failure at any point aborts with an error message.
	while ((c = getopt(argc, argv, "n:t:")) != -1) {
		switch (c) {
			case 'n':
				nflag++;
				nval = optarg;
				if (nflag > 1) {
					printf("Only one argument -%c allowed.\n", c);
					return 1;
				}
				if (nval[0] == '-') {
					printf("Option -%c requires an argument.\n", c);
					return 1;
				}
				break;
			case 't':
				tflag++;
				tval = optarg;
				if (tflag > 1) {
					printf("Only one argument -%c allowed.\n", c);
					return 1;
				}
				if (tval[0] == '-') {
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

	// Count number of args after options, must be 1 or fail
	c = 0;
	for (index = optind; index < argc; index++) c++;
	if (c != 1) {
		printf("Usage: ttrack [-t TITLE] [-n NOTE] COMMAND");
		return 1;
	}

	// Pass validated info to next step
	enter(nflag, nval, tflag, tval, argv[optind]);

	return 0;
}


void getfile(char *name, FILE **fp) {
	DIR *dp;
	struct dirent *ep;
	dp = opendir("~/.config/ttrack");
	if (dp != NULL) {
		//*fp = fopen(name, "a");
		printf("Directory found.\n");
		closedir(dp);
	}
	else {
		printf("Making directory...\n");
		mkdir("~/.config/ttrack");
		printf("Directory made.\n");
		getfile(name, fp);
	}
}


int enter(int n, char *nval, int t, char *tval, char *command) {
	// Debug/verification print
	printf("n = %d\nnval = %s\nt = %d\ntval = %s\ncommand = %s\n",
			n, nval, t, tval, command);

	return 0;
}
