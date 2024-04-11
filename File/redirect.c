#include <func.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    // ./redirect file
    ARGS_CHECK(argc, 2);

    // 关闭stdout之前输出到控制台
    printf("You can see me!\n");

    close(STDOUT_FILENO);

    // fopen分配一个小的文件描述符
    int fd = open(argv[1], O_WRONLY);
    ERROR_CHECK(fd, -1, "open");

    printf("fd = %d\n", fd);
    printf("You can't see me!\n");

    return 0;
}