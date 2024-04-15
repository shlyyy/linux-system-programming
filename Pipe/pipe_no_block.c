#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <func.h>

int main(int argc, char *argv[])
{
    // ./pipe_no_block 1.pipe
    ARGS_CHECK(argc, 2);

    int fd1 = open(argv[1], O_RDWR); // read
    ERROR_CHECK(fd1, -1, "open fd1");
    int fd2 = open(argv[1], O_RDWR);
    ERROR_CHECK(fd2, -1, "open fd2"); // write

    printf("pipe is opend!\n");

    close(fd2);
    close(fd1);
    return 0;
}