#include <func.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // ./write file
    ARGS_CHECK(argc, 2);

    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");

    char buf[] = "hello world";
    // 写入文本数据应该使用strlen，不是sizeof
    ssize_t sret = write(fd, buf, strlen(buf));
    ERROR_CHECK(sret, -1, "write");

    printf("sret = %ld\n", sret);
    close(fd);
    return 0;
}
