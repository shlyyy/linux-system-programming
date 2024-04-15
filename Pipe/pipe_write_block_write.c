#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <func.h>

int main(int argc, char *argv[])
{
    // ./pipe_write_block_write 1.pipe
    ARGS_CHECK(argc, 2);

    int fdw = open(argv[1], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open");

    printf("write is opend!\n");

    char buf[0x1000];
    ssize_t total = 0;
    while (1)
    {
        int sret = write(fdw, buf, sizeof(buf));
        ERROR_CHECK(sret, -1, "write");
        total += sret;
        printf("sret = %d, total = %ld\n", sret, total);
    }
    close(fdw);
    return 0;
}