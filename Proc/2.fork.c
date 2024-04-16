#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{

    pid_t pid = fork();

    if (pid == 0)
    {
        // child
        printf("child: pid = %d, ppid = %d\n", getpid(), getppid());
    }
    else if (pid > 0)
    {
        // father
        printf("father: pid = %d, ppid = %d\n", getpid(), getppid());
    }
    else
    {
        perror("fork!");
        return -1;
    }

    return 0;
}