#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{

    pid_t id = fork();
    if (id == 0)
    {
        // child
        int cnt = 5;
        while (cnt)
        {
            printf("I am child, pid:%d, ppid:%d, cnt: %d\n", getpid(), getppid(), cnt);
            cnt--;
            sleep(1);
        }
        exit(0);
    }
    else if (id > 0)
    {
        // parent
        int cnt = 10;
        while (cnt)
        {
            printf("I am father, pid:%d, ppid:%d, cnt: %d\n", getpid(), getppid(), cnt);
            cnt--;
            sleep(1);
        }
        pid_t ret = wait(NULL);
        if (ret == id)
        {
            printf("wait success, ret: %d\n", ret);
        }
        sleep(5);
    }
    else
    {
        // error
        perror("fork");
        return 1;
    }
    return 0;
}