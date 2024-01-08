
#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
	argc--;
	int argi = 0, isArg = 0;
	char c[MAXARG][12], *cmd = argv[1], *argv_[MAXARG];
	for (int i = 0; i < argc; i++) {
		argv_[i] = argv[i + 1];
	}
	printf("succeed exec original cmd!");
	int i = 0;	
	while(read(0, c[argi] + i, 1) != 0) {
		printf("c: %c\n", c[argi][i]);
		if (c[argi][i] == ' ' || c[argi][i] == '\n') {
			printf("space...\n");
			if (isArg) {
				isArg = 0;
				i = 0;
			}
		} else {
			if (!isArg) {
				argv_[argc + argi] = c[argi];
				printf("argv[%d]: %s\n", argi + argc, argv_[argi + argc]);
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
