/**
 * 输出由 msgget 返回的前10个标识符值。
 *
 * 每次循环由 msgget 创建一个消息队列，然后由 msgctl 删除该队列
 */

#include "unpipc.h"

int main(void)
{
    int     i, msgid;

    for (i = 0; i < 10; i++) {
        msgid = msgget(IPC_PRIVATE, SVMSG_MODE | IPC_CREAT);
		printf("msgid = %d\n", msgid);
		Msgctl(msgid, IPC_RMID, NULL);
    }
	exit(0);
}

/*
实验：
[dendi875@localhost Chapter-3-System V IPC]$ ./slot
msgid = 0
msgid = 32768
msgid = 65536
msgid = 98304
msgid = 131072
msgid = 163840
msgid = 196608
msgid = 229376
msgid = 262144
msgid = 294912

再次运行该程序，可以看出内核是跨进程维护IPC对象计数（跨进程维护槽位使用情况）
[dendi875@localhost Chapter-3-System V IPC]$ ./slot
msgid = 327680
msgid = 360448
msgid = 393216
msgid = 425984
msgid = 458752
msgid = 491520
msgid = 524288
msgid = 557056
msgid = 589824
msgid = 622592
*/