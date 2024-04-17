#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    printf("pid: %d, ppid: %d\n", getpid(), getppid());

    pid_t id = fork();
    if (id == 0)
    {
        // child
        printf("befor: pid = %d, ppid = %d\n", getpid(), getppid());
        sleep(5);

        execl("/usr/bin/ls", "ls", "-a", "-l", NULL);

        printf("befor: pid = %d, ppid = %d\n", getpid(), getppid());
        exit(1); // 退出码是1说明excel执行失败
    }

    // father
    pid_t ret = waitpid(id, NULL, 0);
    if (ret > 0)
    {
        printf("wait success, father pid: %d, ppid: %d\n", getpid(), getppid());
    }
    sleep(5);

    printf("main end\n");
    return 0;
}
