#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	int pid, p[2], CHILD = 0, READ = 0, WRITE = 1, STDOUT = 1;
	char buf[1];
	pipe(p);
	
	int fk = fork();
	pid = getpid();
	
	if (fk == CHILD) {
		read(p[READ], buf, 1);
		close(p[READ]);
		fprintf(STDOUT, "%d: received ping\n", pid);
		write(p[WRITE], buf, 1);
		close(p[WRITE]);
		exit(0);
	}
	else {
		write(p[WRITE], buf, 1);
		close(p[WRITE]);
		read(p[READ], buf, 1);
		close(p[READ]);
		fprintf(STDOUT, "%d: received pong\n", pid);
		exit(0);
	}
}
