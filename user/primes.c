#include "kernel/types.h"
#include "user/user.h"

void pipeline(int rpd_){
	int initial, n, p[2];
	// initial number
	if (read(rpd_, &initial, 4) == 0) {
		close(rpd_); // finish(cannot) read
		exit(0);
	} else {
		printf("prime %d\n", initial);
		pipe(p);
		
		if (fork() != 0) {
			close(p[0]); // no need to read
			while (read(rpd_, &n, 4) != 0) 
				if (n % initial != 0) 
					write(p[1], &n, 4);
			close(rpd_); // finish read
			close(p[1]); // finish write
			wait(0);
		} else {
			close(p[1]); // no need to write
			pipeline(p[0]); // exit() for MAXPRIME or return from parent
		}
	}
	return; // either parent wait(0) or child pipeline() finished. For one process, we can see its function call stack: main -> pipeline -> ...(grand...grand) -> pipeline, the last exit(0) throught first if condition failed, others need return, each pipeline call, for parent, is wait(0), so for process n, main(exit(0)) -> wait(0) * n -> child exit(0),  
	// full chain:
	// plast exit(0)(in if)
	// plast - 1: wait(0) -> pipeline -> pipeline ... -> exit(0) (in main())
	// ...
	// -> p1 : wait(0) -> pipeline(in main()) -> exit(0) in main()
	// -> p0: wait(0) -> exit(0) all in main()
}

int main(int argc, char* argv[]) {
	int n = 1, p[2], MAXPRIME = 35;
	pipe(p);
	if (fork() != 0) {
		close(p[0]); // no need to read
		while (++n <= MAXPRIME) 
			write(p[1], &n, 4);
		close(p[1]);
		wait(0);
	} else {
		close(p[1]); // no need to write 
		pipeline(p[0]);
	}
	exit(0);
}
