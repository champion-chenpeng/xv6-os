
#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"
/*

	argc--;
	int argi = 0, isArg = 0;
	char c[MAXARG][12], *cmd = argv[1], *argv_[MAXARG];
	for (int i = 0; i < argc; i++) {
		argv_[i] = argv[i + 1];
	}
	// printf("succeed exec original cmd!");
	int i = 0;	
	while(read(0, c[argi] + i, 1) != 0) {
		// printf("c: %c\n", c[argi][i]);
		if (c[argi][i] == ' ' || c[argi][i] == '\n') {
			// printf("space...\n");
			if (isArg) {
				isArg = 0;
				i = 0;
			}
		} else {
			if (!isArg) {
				argv_[argc + argi] = c[argi];
				// printf("argv[%d]: %s\n", argi + argc, argv_[argi + argc]);
				isArg = 1;
				argi++;
			}
		}
		i++;
	}
	if (fork() == 0) {
		exec(cmd, argv_);
	} else {
		wait(0);
	}

	exit(0);
}
*/
void getArgv(char* xargs[], int argc, char * argv[]) {
	for (int i = 0; i < argc - 1; i++) {
		xargs[i] = argv[i + 1];
		// printf("xargs[%d]: %s\n", i, xargs[i]);
	}
}

void getStdin(int argc, char* xargs[]) {
	// general task: seperate stdin stream into strings
	int MAXARGLEN = 64, argl = 0, isArg = 0;
	char c, *p;
	while (read(0, &c, 1) != 0) {
		if (c == ' ' || c == '\n') { // ' ' and '\n'
			if (isArg) { // end of an arg
				xargs[argc++] = p;
			}
			isArg = 0;
		} else {
			if (!isArg) { // the first char of a arg
				p = (char*)malloc(MAXARGLEN * sizeof(char));
				argl = 0;
			}			
			p[argl++] = c;
			isArg = 1;
		}
	}
}

int main(int argc, char* argv[]) {
	// printf("MAXARG: %d\n", MAXARG);
	char* xargs[MAXARG];
	// get args in argv
	getArgv(xargs, argc, argv);
	// get args from stdin
	getStdin(argc - 1, xargs);
	
	// execute
	if (fork() == 0) {
		exec(xargs[0], xargs);
	} else {
		wait(0);
	}
	exit(0);
}
	
	
