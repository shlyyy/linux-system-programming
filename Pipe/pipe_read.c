#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <func.h>

int main(int argc, char *argv[])
{
    // ./pipe_read test
    ARGS_CHECK(argc, 2);

    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open");

    printf("read is opend!\n");
    close(fdr);
    return 0;
}