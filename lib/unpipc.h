#ifndef __unpipc_h
#define __unpipc_h

#include "../config.h" /* configuration options for current OS */

#include <sys/types.h>  /* basic system data types */
#include <sys/time.h>   /* timeval{} for select() */
#include <time.h>       /* timespec{} for pselect() */
#include <errno.h>
#include <fcntl.h>      /* for nonblocking */
#include <limits.h>     /* PIPE_BUF */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>   /* for S_xxx file mode constants */
#include <unistd.h>
#include <sys/wait.h>

#ifdef HAVE_MQUEUE_H
#include <mqueue.h>     /* Posix message queues */
#endif

#ifdef HAVE_SEMAPHORE_H
#include <semaphore.h>  /* Posix semaphores */

#ifndef SEM_FAILED
#define SEM_FAILED  ((sem_t *)(-1))
#endif

#endif

#ifndef HAVE_SYS_MMAN_H
#include <sys/mman.h>   /* Posix shared memory */
#endif

#ifndef MAP_FAILED
#define MAP_FAILED ((void *)(-1))
#endif

#ifdef HAVE_SYS_IPC_H
#include <sys/ipc.h>    /* System V IPC */
#endif

#ifdef HAVE_SYS_MSG_H
#include <sys/msg.h>    /* System V message queues */
#endif

#ifdef HAVE_SYS_SEM_H
#ifdef __bsdi__
#undef HAVE_SYS_SEM_H   /* hack: BSDI's semctl() prototype is wrong */
#else
# include <sys/sem.h>   /* System V semaphores */
#endif

#ifndef HAVE_SEMUN_UNION
union semun {
    int                 val;
    struct semid_ds     *buf;
    unsigned short      *array;
};
#endif
#endif  /* HAVE_SYS_SEM_H */

#ifdef HAVE_SYS_SHM_H
#include <sys/shm.h>    /* System V shared memory */
#endif


/* 其它的常数 */
#define MAXLINE     4096    /* max text line length */

#define MSG_R 0400
#define MSG_W 0200
#define SVMSG_MODE  (MSG_R | MSG_W | MSG_R>>3 | MSG_R >>6)/* System V 创建新的消息队列的默认权限 */



/*********************************************************************
*   输出至stderr的出错函数原型                                       *
*********************************************************************/
void     err_ret(const char *, ...);
void     err_sys(const char *, ...);
void     err_dump(const char *, ...);
void     err_msg(const char *, ...);
void     err_quit(const char *, ...);

/*********************************************************************
*   我们自己的 unix 包裹函数原型                                     *
*********************************************************************/
key_t    Ftok(const char *, int);
void     Stat(const char *, struct stat *);


#include "error.c"
#include "wrapunix.c"

#endif  /* __unpipc_h */