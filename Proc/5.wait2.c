#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

void RunChild()
{
    int cnt = 5;
    while (cnt)
    {
        printf("I am Child Process, pid: %d, ppid:%d\n", getpid(), getppid());
        sleep(1);
        cnt--;
    }
}

#define N 10
int main()
{

    for (int i = 0; i < N; i++)
    {
        pid_t id = fork();
        if (id == 0)
        {
            RunChild();
            exit(i);
        }
        printf("create child process: %d success\n", id); // 这句话只有父进程才会执行
    }

    sleep(5);

    // 等待子进程退出
    for (int i = 0; i < N; i++)
    {
        // wait当任意一个子进程退出的时候，wait回收子进程
        // 如果任意一个子进程都不退出，会一直等待
        pid_t id = wait(NULL);
        if (id > 0)
        {
            printf("wait %d success\n", id);
        }
    }

    sleep(5);

    return 0;
}