#include <func.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    // ./read file
    ARGS_CHECK(argc, 2);

    int fd = open(argv[1], O_RDONLY);
    ERROR_CHECK(fd, -1, "open");

    char buf[6] = {0};
    ssize_t sret = read(fd, buf, 5);
    ERROR_CHECK(sret, -1, "read");
    printf("buf = %s, sret = %ld\n", buf, sret);

    memset(buf, 0, sizeof(buf));

    sret = read(fd, buf, 5);
    ERROR_CHECK(sret, -1, "read");
    printf("buf = %s, sret = %ld\n", buf, sret);

    close(fd);

    return 0;
}