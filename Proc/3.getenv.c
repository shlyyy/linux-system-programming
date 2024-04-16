#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    printf("PATH: %s\n", getenv("PATH"));

    char who[32];
    strcpy(who, getenv("USER"));

    if (strcmp(who, "root") == 0)
    {
        printf("让他做任何事情");
    }
    else
    {
        printf("你就是一个普通用户，受权限约束\n");
    }
    return 0;
}
