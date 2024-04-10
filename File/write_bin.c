#include <func.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // ./write_bin file
    ARGS_CHECK(argc, 2);

    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");

    int i = 100;
    ssize_t sret = write(fd, &i, sizeof(int));
    ERROR_CHECK(sret, -1, "write");

    printf("sret = %ld\n", sret);
    close(fd);
    return 0;
}