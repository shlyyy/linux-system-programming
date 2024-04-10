#include <func.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // ./open file
    ARGS_CHECK(argc, 2);

    // 如果flags存在O_CREAT，必须选择mode参数的open函数
    // 创建出来的文件权限会受到掩码的影响
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, 0666);
    ERROR_CHECK(fd, -1, "open");

    printf("fd = %d\n", fd);
    close(fd);
    return 0;
}
