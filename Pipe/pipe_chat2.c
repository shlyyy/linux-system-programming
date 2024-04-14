#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <func.h>

int main(int argc, char *argv[])
{
    // ./pipe_chat2 1.pipe 2.pipe
    ARGS_CHECK(argc, 3);

    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open fdr");

    int fdw = open(argv[2], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open fdw");

    printf("chat is established!\n");

    char buf[0x1000];
    while(1)
    {
        // 先读
        memset(buf, 0, sizeof(buf));
        read(fdr, buf, sizeof(buf));
        printf("buf = %s\n", buf);

        // 后发
        memset(buf, 0, sizeof(buf));
        read(STDIN_FILENO, buf, sizeof(buf));
        write(fdw, buf, strlen(buf));
    }

    close(fdw);
    close(fdr);
    return 0;
}