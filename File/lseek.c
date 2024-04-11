#include <func.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // ./lseek file
    ARGS_CHECK(argc, 2);

    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");

    write(fd, "hello", 5);

    // 移到当前位置的前一个字符位置
    lseek(fd, -1, SEEK_CUR);
    // lseek(fd, -1, SEEK_END);

    write(fd, "O", 1);
    close(fd);

    return 0;
}