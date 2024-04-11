#include <func.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // ftruncate.c file
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    
    printf("fd = %d\n", fd);
    int ret = ftruncate(fd, 3);
    ERROR_CHECK(ret, -1, "ftruncate");
    return 0;
}