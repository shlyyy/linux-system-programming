#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("pid: %d, ppid: %d\n", getpid(), getppid());

    execl("/usr/bin/ls", "-a", "-l", NULL);

    printf("main end\n");
    return 0;
}
