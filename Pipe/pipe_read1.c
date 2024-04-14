#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <func.h>

int main(int argc, char *argv[])
{
    // ./pipe_read 1.pipe
    ARGS_CHECK(argc, 2);

    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open");
    printf("read is opend!\n");

    // 读数据阻塞 等待管道中有数据
    char buf[1024] = {0};
    int ret = read(fdr, buf, 1024);
    ERROR_CHECK(ret, -1, "read");

    printf("ret = %d, buf = %s", ret, buf);

    close(fdr);
    return 0;
}