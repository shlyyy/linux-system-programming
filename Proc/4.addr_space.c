#include <stdio.h>
#include <stdlib.h>

int g_val_1;
int g_val_2 = 100;

int main(int argc, char *argv[], char *env[])
{
    printf("code addr: %p\n", main);
    const char *str = "hello bit";
    printf("read only string addr: %p\n", str);
    printf("init global value addr: %p\n", &g_val_2);
    printf("uninit global value addr: %p\n", &g_val_1);
    char *mem = (char *)malloc(100);
    char *mem1 = (char *)malloc(100);
    char *mem2 = (char *)malloc(100);
    printf("heap addr: %p\n", mem);
    printf("heap addr: %p\n", mem1);
    printf("heap addr: %p\n", mem2);
    printf("stack addr: %p\n", &str);
    printf("stack addr: %p\n", &mem);
    static int a = 0;
    int b;
    int c;
    printf("a = stack addr: %p\n", &a);
    printf("stack addr: %p\n", &b);
    printf("stack addr: %p\n", &c);

    return 0;
}