#ifndef __COMM_HPP__
#define __COMM_HPP__

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "log.hpp"

using namespace std;

Log printlog;
// 共享内存的大小一般建议是4096的整数倍
// 4097,实际上操作系统给你的是4096*2的大小
const int size = 4096;
const string pathname = "/home/shlyyy";
const int proj_id = 0x80;

key_t GetKey()
{
    key_t k = ftok(pathname.c_str(), proj_id);
    if (k < 0)
    {
        printlog(Fatal, "ftok error: %s", strerror(errno));
        exit(1);
    }
    printlog(Info, "ftok success, key is : 0x%x", k);
    return k;
}

int GetShareMemHelper(int flag)
{
    key_t k = GetKey();
    int shmid = shmget(k, size, flag);
    if (shmid < 0)
    {
        printlog(Fatal, "create share memory error: %s", strerror(errno));
        exit(2);
    }
    printlog(Info, "create share memory success, shmid: %d", shmid);

    return shmid;
}

int CreateShm()
{
    return GetShareMemHelper(IPC_CREAT | IPC_EXCL | 0666);
}

int GetShm()
{
    return GetShareMemHelper(IPC_CREAT);
}

#define FIFO_FILE "./myfifo"
#define MODE 0664

enum
{
    FIFO_CREATE_ERR = 1,
    FIFO_DELETE_ERR,
    FIFO_OPEN_ERR
};

class Init
{
public:
    Init()
    {
        // 创建管道
        int n = mkfifo(FIFO_FILE, MODE);
        if (n == -1)
        {
            perror("mkfifo");
            exit(FIFO_CREATE_ERR);
        }
    }
    ~Init()
    {

        int m = unlink(FIFO_FILE);
        if (m == -1)
        {
            perror("unlink");
            exit(FIFO_DELETE_ERR);
        }
    }
};

#endif