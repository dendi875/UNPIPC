/**
 * 使用两个管道实现的客户端（父进程）－服务器（子进程）程序
 *
 * 客户端从标准输出读入一个路径名，并把它写入 IPC 通道。服务器从该 IPC 通道读出这个路径名，并尝试
 * 打开其文件来读。如果服务器能打开该文件，它就读出其中的内容，并写入另一个 IPC 通道，以作为客户
 * 的响应；否则，它就响应一个出错消息。客户端随后从该 IPC 通道读出响应，并把它写到标准输出。
 * 如果服务器无法打开文件，那么客户端读出的响应将是一个出错消息。否则，客户端读出的响应就是该文件的
 * 内容。
 */

#include "unpipc.h"

void client(int readfd, int writefd);
void server(int readfd, int writefd);

/**
 * 主程序执行以下步骤
 * 1、创建管道1（fd1[0]和fd1[1]）和管道2（fd2[0]和fd2[1]）
 * 2、fork
 * 3、父进程关闭管道1的读端
 * 4、父进程关闭管道2的写端
 * 5、子进程关闭管道1的写端
 * 6、子进程关闭管道2的读端
 */
int main(int argc, char *argv[])
{
    int     pipe1[2], pipe2[2];
    pid_t   pid;

    Pipe(pipe1);
    Pipe(pipe2);

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {  /* 子进程 */
        Close(pipe1[1]);
        Close(pipe2[0]);

        /* 子进程充当服务器角色 */
        server(pipe1[0], pipe2[1]);

        exit(0);
    }

    /* 父进程 */
    Close(pipe1[0]);
    Close(pipe2[1]);

    /* 父进程充当客户端角色 */
    client(pipe2[0], pipe1[1]);

    Waitpid(pid, NULL, 0);      /* 阻塞等待回收子进程 */

    exit(0);
}

void client(int readfd, int writefd)
{
    char    buff[MAXLINE];
    size_t  len;
    ssize_t  n;

    /* 从标准输入读 pathname */
    fgets(buff, MAXLINE, stdin);
    len = strlen(buff);
    if (buff[len-1] == '\n') {
        len--;
    }

    /* 把路径名写入第一条管道 */
    Write(writefd, buff, len);

    /* 从第二条管道中读出数据，并写到标准输出 */
    while ((n = Read(readfd, buff, MAXLINE)) > 0) {
        Write(STDOUT_FILENO, buff, n);
    }
}

void server(int readfd, int writefd)
{
    int     fd;
    ssize_t  n;
    char    buff[MAXLINE];

    /* 从管道中读出路径名 */
    if ((n = Read(readfd, buff, MAXLINE)) == 0) {
        err_quit("end-of-file while reading pathname");
    }
    buff[n] = '\0';  /* 确保是 null 字节终止路径名 */

    if ((fd = open(buff, O_RDONLY)) < 0) {
        /* 打开失败告诉客户端 */
        snprintf(buff + n, sizeof(buff) - n, "：can't open, %s\n", strerror(errno));
        n = strlen(buff);
        Write(writefd, buff, n);
    } else {
        /* 打开成功，拷贝文件内容到另一条管道 */
        while ((n = Read(fd, buff, MAXLINE)) > 0) {
            Write(writefd, buff, n);
        }
        Close(fd);
    }
}

/*
实验：
[dendi875@localhost Chapter-4-Pipes and FIFOs]$ ./mainpipe
/etc/issue
Kernel \r on an \m

读一个我们不读的文件
[dendi875@localhost Chapter-4-Pipes and FIFOs]$ ./mainpipe
/etc/shadow
/etc/shadow：can't open, Permission denied

读一个不存在的文件
[dendi875@localhost Chapter-4-Pipes and FIFOs]$ ./mainpipe
/no/such/file
/no/such/file：can't open, No such file or directory
/*