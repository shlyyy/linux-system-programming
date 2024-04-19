#include "comm.hpp"

extern Log mylog;

int main()
{
    Init init;
    int shmid = CreateShm();
    char *shmaddr = (char*)shmat(shmid, nullptr, 0);

    // ipc code 在这里！！
    // 一旦有人把数据写入到共享内存，其实我们立马能看到了！！
    // 不需要经过系统调用，直接就能看到数据了！

    int fd = open(FIFO_FILE, O_RDONLY); // 等待写入方打开之后，自己才会打开文件，向后执行， open 阻塞了！
    if (fd < 0)
    {
        printlog(Fatal, "error string: %s, error code: %d", strerror(errno), errno);
        exit(FIFO_OPEN_ERR);
    }
    struct shmid_ds shmds;
    while(true)
    {
        char c;
        ssize_t s = read(fd, &c, 1);
        if(s == 0) break;
        else if(s < 0) break;

        cout << "client say@ " << shmaddr << endl; //直接访问共享内存
        sleep(1);

        shmctl(shmid, IPC_STAT, &shmds);
        cout << "shm size: " << shmds.shm_segsz << endl;
        cout << "shm nattch: " << shmds.shm_nattch << endl;
        printf("shm key: 0x%x\n",  shmds.shm_perm.__key);
        cout << "shm mode: " << shmds.shm_perm.mode << endl;
    }

    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, nullptr);

    close(fd);
    return 0;
}