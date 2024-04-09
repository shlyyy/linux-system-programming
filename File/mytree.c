#include <func.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

int dirs, files = 0;

void dir_travel(const char *path, int space_width)
{
    DIR *pdir = opendir(path);
    ERROR_CHECK(pdir, NULL, "opendir");

    // 遍历目录流
    struct dirent *pdirent;
    errno = 0;

    while ((pdirent = readdir(pdir)) != NULL)
    {
        // 忽略 . .. 隐藏文件
        const char *name = pdirent->d_name;
        if (name[0] == '.')
        {
            continue;
        }

        // 缩进
        for (int i = 0; i < space_width; ++i)
        {
            printf(" ");
        }
        puts(name);

        // 递归目录
        if (pdirent->d_type == DT_DIR)
        {
            dirs++;
            char subpath[256];
            sprintf(subpath, "%s/%s", path, name);

            dir_travel(subpath, space_width + 4);
        }
        else
        {
            files++;
        }
    }

    if (errno)
    {
        perror("readdir");
        exit(1);
    }

    closedir(pdir);
}

int main(int argc, char *argv[])
{
    // ./mytree path
    ARGS_CHECK(argc, 2);

    puts(argv[1]);

    dir_travel(argv[1], 4);

    printf("\n%d directories, %d files\n", dirs, files);

    return 0;
}