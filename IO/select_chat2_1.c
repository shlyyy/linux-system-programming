#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <func.h>
#include <sys/select.h>

int main(int argc, char *argv[])
{
    // ./select_chat2_1 1.pipe 2.pipe
    ARGS_CHECK(argc, 3);

    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open fdr");

    int fdw = open(argv[2], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open fdw");

    printf("chat is established!\n");

    char buf[0x1000];
    fd_set rdset;

    while (1)
    {
        FD_ZERO(&rdset);

        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(fdr, &rdset);

        select(fdr + 1, &rdset, NULL, NULL, NULL);

        if (FD_ISSET(STDIN_FILENO, &rdset))
        {
            memset(buf, 0, sizeof(buf));
            read(STDIN_FILENO, buf, sizeof(buf));
            write(fdw, buf, strlen(buf));
        }

        if (FD_ISSET(fdr, &rdset))
        {
            memset(buf, 0, sizeof(buf));
            read(fdr, buf, sizeof(buf));
            printf("buf = %s\n", buf);
        }
    }

    close(fdw);
    close(fdr);
    return 0;
}