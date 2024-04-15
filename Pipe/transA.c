#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <func.h>

int main(int argc, char *argv[])
{
    // ./transA file 1.pipe
    ARGS_CHECK(argc, 3);

    int fdr = open(argv[1], O_RDONLY); // 读磁盘文件
    ERROR_CHECK(fdr, -1, "open fdr");

    int fdw = open(argv[2], O_WRONLY); // 写管道
    ERROR_CHECK(fdw, -1, "open fdw");

    printf("pipe is opend!\n");

    // 发送文件名
    int length = strlen(argv[1]);
    write(fdw, &length, sizeof(length));
    write(fdw, argv[1], length);

    // 发送文件内容
    char buf[4096] = {0};
    ssize_t sret = read(fdr, buf, sizeof(buf));
    length = sret;
    write(fdw, &length, sizeof(length));
    write(fdw, buf, length);

    close(fdw);
    close(fdr);
    return 0;
}