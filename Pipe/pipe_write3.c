#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <func.h>

int main(int argc, char *argv[])
{
    // ./pipe_write3 1.pipe
    ARGS_CHECK(argc, 2);

    int fdw = open(argv[1], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open");

    printf("write is opend!\n");

    close(fdw);
    return 0;
}