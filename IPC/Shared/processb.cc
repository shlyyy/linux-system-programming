#include "comm.hpp"

int main()
{
    int shmid = GetShm();
    char *shmaddr = (char *)shmat(shmid, nullptr, 0);

    int fd = open(FIFO_FILE, O_WRONLY); // 等待写入方打开之后，自己才会打开文件，向后执行， open 阻塞了！
    if (fd < 0)
    {
        printlog(Fatal, "error string: %s, error code: %d", strerror(errno), errno);
        exit(FIFO_OPEN_ERR);
    }
    // 一旦有了共享内存，挂接到自己的地址空间中，你直接把他当成你的内存空间来用即可！
    // 不需要调用系统调用
    // ipc code
    while (true)
    {
        cout << "Please Enter@ ";
        fgets(shmaddr, 4096, stdin);

        write(fd, "c", 1); // 通知对方
    }

    shmdt(shmaddr);

    close(fd);
    return 0;
}