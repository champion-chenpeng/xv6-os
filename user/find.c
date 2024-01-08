#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char *path, char *name) {
	char *p, *fname;
	int fd;
	struct stat st;
	struct dirent de;
	
	if ((fd = open(path, O_RDONLY)) < 0) {
		fprintf(2, "find: cannot open %s\n", path);
		return;
	}
	
	if (fstat(fd, &st) < 0) {
		fprintf(2, "find: cannot stat %s\n", path);
		close(fd);
		return;
	}
	
	switch (st.type) {
		case T_DEVICE:
		case T_FILE:
  			// Find first character after last slash.
			for	(fname = path + strlen(path); fname >= path && *fname != '/'; fname--);
  			fname++;
			if (strcmp(fname, name) == 0) {
				printf("%s\n", path);
			}
			break;
		case T_DIR:
			if (strlen(path) + 1 + DIRSIZ + 1 > 512) {
				printf("find: path too long\n");
				break;
			}
			p = path + strlen(path);
			*p++ = '/';
			while (read(fd, &de, sizeof(de)) == sizeof(de)) {
				if (de.inum == 0 || de.name[0] == '.')
					continue;
				// append new node name to the path, here to use memmove, DIRSIZ if fixed
				memmove(p, de.name, DIRSIZ);
				p[DIRSIZ] = 0;
				if (stat(path, &st) < 0) {
					printf("find: cannot stat %s\n", path);
					continue;
				}
				find(path, name);
				// xv6 has very limited space, if create buf[512] each level, the recersive depth cannot exceed 4~5.
			}
			// remove string after '/'
			*p-- = 0;
			break;
		}
	close(fd);
}
	
int main(int argc, char *argv[]) {
	char buf[512];
	strcpy(buf, argv[1]);
	find(buf, argv[2]);
	exit(0);
}
