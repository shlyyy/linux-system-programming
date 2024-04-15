#include <func.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // ./getpid
    ARGS_CHECK(argc, 1);

    printf("pid = %d, ppid = %d, uid = %d, euid = %d\n",
           getpid(), getppid(), getuid(), geteuid());
    printf("gid = %d, egid = %d\n", getgid(), getegid());

    return 0;
}