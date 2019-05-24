/**
 * 获取并输出文件系统信息和IPC键
 */
#include "unpipc.h"

int main(int argc, char **argv)
{
    struct stat stat;

    if (argc != 2) {
        err_quit("usage：./ftok <pathname>");
    }

    Stat(argv[1], &stat);
    printf("st_dev：%lx, st_ino：%lx, key：%x\n",
            (u_long)stat.st_dev, (u_long) stat.st_ino, Ftok(argv[1], 0x57));

    exit(0);
}

/*
实验：
[dendi875@localhost Chapter-3-System V IPC]$ ./ftok /etc/services
st_dev：805, st_ino：2002a, key：5705002a
[dendi875@localhost Chapter-3-System V IPC]$ ./ftok /usr/tmp/
st_dev：805, st_ino：8001b, key：5705001b
*/