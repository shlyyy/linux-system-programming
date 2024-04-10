#include <func.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // ./mycp src dst
    ARGS_CHECK(argc, 3);

    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open fdr");

    int fdw = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    ERROR_CHECK(fdw, -1, "open fdw");

    // buf选择char数组，不是字符串含义，作为缓冲区，char是一字节。
    char buf[0x1000] = {0};
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        ssize_t sret = read(fdr, buf, sizeof(buf));
        ERROR_CHECK(sret, -1, "read");
        if (sret == 0)
        {
            break;
        }

        write(fdw, buf, sret);
    }

    close(fdr);
    close(fdw);
    return 0;
}