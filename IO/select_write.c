#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <func.h>
#include <sys/select.h>
#include <time.h>

int main(int argc, char *argv[])
{
    // ./select_write 1.pipe
    ARGS_CHECK(argc, 2);

    int fdr = open(argv[1], O_RDWR);
    ERROR_CHECK(fdr, -1, "open fdr");

    int fdw = open(argv[1], O_RDWR);
    ERROR_CHECK(fdw, -1, "open fdw");

    printf("pipe is opened!\n");

    char buf[0x1000];
    fd_set rdset;
    fd_set wrset;
    int cnt = 0;
    while (1)
    {
        FD_ZERO(&rdset);
        FD_ZERO(&wrset);

        FD_SET(fdw, &wrset);
        FD_SET(fdr, &rdset);

        int ret = select(fdw + 1, &rdset, &wrset, NULL, NULL);
        printf("ret = %d\n", ret);
        if (FD_ISSET(fdr, &rdset))
        {
            read(fdr, buf, 2048);
            printf("read ready, cnt = %d\n", cnt++);
        }

        if (FD_ISSET(fdw, &wrset))
        {
            write(fdw, buf, 4096);
            printf("write ready, cnt = %d\n", cnt++);
        }

        sleep(1);
    }

    close(fdw);
    close(fdr);
    return 0;
}