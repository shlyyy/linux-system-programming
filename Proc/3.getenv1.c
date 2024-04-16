#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    extern char **environ;
    int i = 0;
    for (; environ[i]; i++)
    {
        printf("%d: %s\n", i, environ[i]);
    }
    return 0;
}
