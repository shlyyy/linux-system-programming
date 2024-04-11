#include <func.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    // ./dup file
    ARGS_CHECK(argc, 2);

    // fopen分配一个小的文件描述符
    int fd = open(argv[1], O_WRONLY);
    ERROR_CHECK(fd, -1, "open");

    // 关闭fd=1指向的输出流。1未被占用
    close(STDOUT_FILENO);

    // dup选择一个最小的文件描述符1，指向打开的文件
    int newfd = dup(fd);
    ERROR_CHECK(newfd, -1, "dup");

    // fd和newfd都指向打开文件
    write(fd, "hello", 5);
    write(newfd, "world", 5);

    close(newfd);
    close(fd);
    return 0;
}