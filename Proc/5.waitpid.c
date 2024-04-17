#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define TASK_NUM 10

typedef void (*task_t)();
task_t tasks[TASK_NUM];

void task1()
{
    printf("这是一个执行打印日志的任务, pid: %d\n", getpid());
}

void task2()
{
    printf("这是一个执行检测网络健康状态的一个任务, pid: %d\n", getpid());
}

void task3()
{
    printf("这是一个进行绘制图形界面的任务, pid: %d\n", getpid());
}

int AddTask(task_t t);

// 任务的管理代码
void InitTask()
{
    for (int i = 0; i < TASK_NUM; i++)
        tasks[i] = NULL;
    AddTask(task1);
    AddTask(task2);
    AddTask(task3);
}

int AddTask(task_t t)
{
    int pos = 0;
    for (; pos < TASK_NUM; pos++)
    {
        if (!tasks[pos])
            break;
    }
    if (pos == TASK_NUM)
        return -1;
    tasks[pos] = t;
    return 0;
}

void DelTask()
{
}

void CheckTask()
{
}

void UpdateTask()
{
}

void ExecuteTask()
{
    for (int i = 0; i < TASK_NUM; i++)
    {
        if (!tasks[i])
            continue;
        tasks[i]();
    }
}

int main()
{
    pid_t id = fork();
    if (id < 0)
    {
        perror("fork");
        return 1;
    }
    else if (id == 0)
    {
        //  child
        int cnt = 5;
        while (cnt)
        {
            printf("I am child, pid:%d, ppid:%d, cnt: %d\n", getpid(), getppid(), cnt);
            cnt--;
            sleep(1);
        }
        exit(11);
    }
    else
    {
        // parent
        int status = 0;
        InitTask();
        while (1)
        {
            pid_t ret = waitpid(id, &status, WNOHANG); // 轮询非阻塞
            if (ret > 0)
            {
                if (WIFEXITED(status))
                {
                    printf("进程是正常跑完的, 退出码:%d\n", WEXITSTATUS(status));
                }
                else
                {
                    printf("进程出异常了\n");
                }
                break;
            }
            else if (ret < 0)
            {
                printf("wait failed!\n");
                break;
            }
            else
            {
                // ret == 0
                // printf("你好了没？子进程还没有退出，我在等等...\n");
                // sleep(1);
                ExecuteTask();
                usleep(500000);
            }
        }
    }

    return 0;
}