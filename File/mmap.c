#include <func.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
    // ftruncate.c file
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");

    // 建立内存和磁盘之间的映射
    char *p = mmap(NULL, 5, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ERROR_CHECK(p, MAP_FAILED, "mmap");
    for (int i = 0; i < 5; ++i)
    {
        printf("%c", p[i]);
    }
    printf("\n");

    p[4] = 'O';
    munmap(p, 5);

    close(fd);
    return 0;
}