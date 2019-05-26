/**
 * 测试系统是否支持　pipe 创建全双工管道
 */

#include "unpipc.h"

int main(void)
{
	int 	fd[2];
	pid_t	pid;
	ssize_t n;
	char 	c;

	Pipe(fd);

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {		/* 子进程 */
		sleep(3);

		if ((n = Read(fd[0], &c, 1)) != 1) {
			err_quit("child：read returned %d", n);
		}
		printf("child read %c\n", c);
		Write(fd[0], "c", 1);
		exit(0);
	}

	/* 父进程 */
	Write(fd[1], "p", 1);
	if ((n = Read(fd[1], &c, 1)) != 1) {
		err_quit("parent：returned %d", n);
	}

	printf("parent read %c\n", c);
	exit(0);
}

/*
实验：
[dendi875@192 Chapter-4-Pipes and FIFOs]$ ./fduplex
read error: Bad file descriptor
[dendi875@192 Chapter-4-Pipes and FIFOs]$ child read p
write error: Bad file descriptor
*/