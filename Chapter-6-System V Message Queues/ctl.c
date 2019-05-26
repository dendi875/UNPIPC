/**
 * 使用 msgctl 函数的例子
 *
 * 该程序创建一个消息队列，往该队列中放置一个含有1字节数据消息，发出 msgctl的IPC_STAT命令，
 * 使用 system函数执行 ipcs 命令，最后使用 msgctl的 IPC_RMID 命令删除该队列
 */

#include "unpipc.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf {
	long mtype;		/* message type, must be > 0 */
	char mtext[1];	/* message data */
};

int main(void)
{
	int 				msgid;
	struct msgbuf		buf;
	struct msqid_ds 	info;

	msgid = Msgget(IPC_PRIVATE, SVMSG_MODE | IPC_CREAT);

	buf.mtype = 1;
	buf.mtext[0] = 1;
	Msgsnd(msgid, &buf, 1, 0);

	Msgctl(msgid, IPC_STAT, &info);
	printf("read-write：%03o, cbytes = %lu, qnum = %lu, qbytes = %lu\n",
		info.msg_perm.mode & 0777, (unsigned long) info.msg_cbytes,
		(unsigned long) info.msg_qnum, (unsigned long) info.msg_qbytes);

	system("ipcs -q");

	Msgctl(msgid, IPC_RMID, NULL);

	exit(0);
}


/*
实验：
[dendi875@192 Chapter-6-System V Message Queues]$ ./ctl
read-write：644, cbytes = 1, qnum = 1, qbytes = 65536

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages
0x00000000 0          dendi875   644        1            1
*/