#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <func.h>

int main(int argc, char *argv[])
{
    // ./transB 1.pipe
    ARGS_CHECK(argc, 2);

    int fd = open(argv[1], O_RDONLY); // read
    ERROR_CHECK(fd, -1, "open fd");

    printf("pipe is opend!\n");

    mkdir("./storage", 0777);
    char buf[4096] = {0};
    int length = 0;
    // 收文件名
    read(fd, &length, sizeof(length));
    read(fd, buf, length);

    // 文件路径
    char filepath[8192] = {0};
    sprintf(filepath, "%s/%s", "./storage", buf);

    // 新建文件
    int fdw = open(filepath, O_WRONLY | O_CREAT, 0666);
    ERROR_CHECK(fdw, -1, "open fdw");

    // 收文件内容
    memset(buf, 0, sizeof(buf));
    read(fd, &length, sizeof(length));
    ssize_t sret = read(fd, buf, length);
    printf("sret = %ld\n", sret);
    write(fdw, buf, sret);

    close(fdw);
    close(fd);
    return 0;
}