#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
    const char *fstr = "hello fwrite\n";
    const char *str = "hello write\n";

    // C
    printf("hello printf\n");              // stdout -> 1
    fprintf(stdout, "hello fprintf\n");    // stdout -> 1
    fwrite(fstr, strlen(fstr), 1, stdout); // fread, stdout->1

    // 操作提供的systemcall
    write(1, str, strlen(str)); // 1

    fork();
    return 0;
}