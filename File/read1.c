#include <func.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char buf[1024] = {0};
    ssize_t sret = read(STDIN_FILENO, buf, sizeof(buf));
    ERROR_CHECK(sret, -1, "read");
    printf("sret = %ld, buf = %s\n", sret, buf);
    return 0;
}