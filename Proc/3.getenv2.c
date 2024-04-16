#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[], char *env[])
{
    int i = 0;
    for (; env[i]; i++)
    {
        printf("%d: %s\n", i, env[i]);
    }
    return 0;
}
