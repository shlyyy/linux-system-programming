#include <func.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // ./read_bin file
    ARGS_CHECK(argc, 2);

    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");

    int data = 100;
    ssize_t sret = read(fd, &data, sizeof(int));
    ERROR_CHECK(sret, -1, "read");

    printf("sret = %ld\n", sret);
    ++data;
    printf("data = %d\n", data);
    close(fd);
    return 0;
}