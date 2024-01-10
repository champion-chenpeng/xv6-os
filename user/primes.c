#include "kernel/types.h"
#include "user/user.h"

void pipeline(int rpd_){
	printf("Enter pipeline, rpd_: %d\n", rpd_);
	int initial, n, p[2];
	// initial number
	if (read(rpd_, &initial, 4) == 0) {
		close(rpd_);
		printf("Close rpd_ ******: %d\n", rpd_);
		exit(0);
		return;
	} else {

		printf("prime %d\n", initial);
		pipe(p);
		printf("open pipe %d, %d\n", p[0], p[1]);
		
		if (fork() != 0) {
			close(p[0]); // no need to read
			printf("This is parent %d\n", getpid()); // 5
			while (read(rpd_, &n, 4) != 0) {
				printf("getpid() reads %d\n", n);
				if (n % initial != 0) {
					write(p[1], &n, 4);
				}
			}
			close(rpd_);
			printf("closed read %d\n", rpd_);
			close(p[1]);
			printf("closed write %d\n", p[1]);
			wait(0);
			return;
		} else {
			sleep(1); // parent first
			printf("This is child %d\n", getpid()); // 3
			close(p[1]); // no need to write
			printf("closed write %d\n", p[1]);
			
			pipeline(p[0]);
			// close(p[0]); 
			printf("closed read %d\n", p[0]);
			exit(0);
			return;
		}
	}
	exit(0);
	return;
}

int main(int argc, char* argv[]) {
	printf("This is Main! pid:%d \n", getpid());
	int n = 2, p[2], MAXPRIME = 35;
	pipe(p);
	printf("opened read %d, write %d\n", p[0], p[1]);
	if (fork() != 0) {
		close(p[0]); // no need to read
		printf("This is parent %d!\n", getpid()); // 4
		while (n <= MAXPRIME) {
			write(p[1], &n, 4);
			n++;
		}
		close(p[1]);
		printf("closed write %d\n", p[1]);
		wait(0);
	} else {
		sleep(1); // parent first
		close(p[1]); // no need to write 
		printf("This is Child %d!\n", getpid()); // 3
		pipeline(p[0]);
		printf("main child pipeline ended!\n");
		exit(0);
	}
	exit(0);
}
