#include <func.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    // ./dup2 file
    ARGS_CHECK(argc, 2);

    // fopen分配一个小的文件描述符
    int fd = open(argv[1], O_WRONLY);
    ERROR_CHECK(fd, -1, "open");

    printf("to console\n");

    int newfd = 10;

    // 让newfd指向标注输出流
    dup2(STDOUT_FILENO, newfd);

    // 让1的指向和fd相同
    dup2(fd, STDOUT_FILENO);

    printf("to file\n");

    // 恢复，让1指向标准输出流
    dup2(newfd, STDOUT_FILENO);

    printf("to console\n");

    close(newfd);
    close(fd);
    return 0;
}