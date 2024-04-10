#include <func.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // ./open file
    ARGS_CHECK(argc, 2);

    int fd = open(argv[1], O_RDONLY);
    ERROR_CHECK(fd, -1, "open"); // 出错返回-1

    printf("fd = %d\n", fd);
    close(fd);
    return 0;
}
