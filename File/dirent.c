#include <func.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h> // errno

int main(int argc, char *argv[])
{
    DIR *pdir = opendir(".");
    ERROR_CHECK(pdir, NULL, "opendir");

    long loc = telldir(pdir);
    struct dirent *pdirent;
    errno = 0;
    while (1)
    {
        // 事先记录目录流的位置
        long tmploc = telldir(pdir);
        pdirent = readdir(pdir);
        if (pdirent == NULL)
        {
            break;
        }
        printf("%s ", pdirent->d_name);
        if (strcmp(pdirent->d_name, "Makefile") == 0)
        {
            // 记录makefile目录位置，主要下面演示seekdir使用
            loc = tmploc;
        }
    }
    printf("\n");
    // 判断是否发生了错误
    if (errno != 0)
    {
        perror("readdir");
        exit(1);
    }

    printf("-------------------------\n");
    seekdir(pdir, loc);
    pdirent = readdir(pdir);
    puts(pdirent->d_name);
    printf("--------------------------\n");

    rewinddir(pdir);
    pdirent = readdir(pdir);
    puts(pdirent->d_name);

    // 关闭目录流
    closedir(pdir);
    return 0;
}