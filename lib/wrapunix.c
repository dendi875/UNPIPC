#include "unpipc.h"

#include <stdarg.h>

#ifdef  HAVE_SYS_IPC_H
key_t Ftok(const char *pathname, int id)
{
    key_t   key;

    if ( (key = ftok(pathname, id)) == -1)
        err_sys("ftok error for pathname \"%s\" and id %d", pathname, id);
    return(key);
}
#endif  /* HAVE_SYS_IPC_H */

void Stat(const char *pathname, struct stat *ptr)
{
    if (stat(pathname, ptr) == -1)
        err_sys("stat error");
}

#ifdef HAVE_SYS_MSG_H
int Msgget(key_t key, int msgflg)
{
    int rc;

    if ((rc = msgget(key, msgflg)) == -1) {
        err_sys("msgget error");
    }
    return(rc);
}

void Msgctl(int msgid, int cmd, struct msqid_ds *buf)
{
    if (msgctl(msgid, cmd, buf) == -1) {
        err_sys("msgctl error");
    }
}

#endif  /* HAVE_SYS_MSG_H */

void Write(int fd, void *ptr, size_t nbytes)
{
    if (write(fd, ptr, nbytes) != nbytes) {
        err_sys("write error");
    }
}

size_t Read(int fd, void *ptr, size_t nbytes)
{
    size_t      n;

    if ((n = read(fd, ptr, nbytes)) == -1) {
        err_sys("read error");
    }

    return(n);
}

void Close(int fd)
{
    if (close(fd) == -1) {
        err_sys("close error");
    }
}

void Pipe(int *fds)
{
    if (pipe(fds) < 0) {
        err_sys("pipe error");
    }
}

pid_t Waitpid(pid_t pid, int *status, int options)
{
    pid_t    retpid;

    if ((waitpid(pid, status, options)) == -1) {
        err_sys("waitpid error");
    }
    return(retpid);
}