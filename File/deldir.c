#include <func.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

void del_dir(const char *path);

int main(int argc, char *argv[])
{
    // ./deldir path
    ARGS_CHECK(argc, 2);

    del_dir(argv[1]);

    return 0;
}

void del_dir(const char *path)
{
    DIR *pdir = opendir(path);

    // 遍历目录流
    errno = 0;
    struct dirent *pdirent;
    while ((pdirent = readdir(pdir)) != NULL)
    {
        // 过滤 . ..
        const char *name = pdirent->d_name;
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
        {
            continue;
        }

        // 拷贝文件或递归目录
        char subpath[256];
        sprintf(subpath, "%s/%s", path, name);
        if (pdirent->d_type == DT_DIR)
        {
            del_dir(subpath);
        }
        else
        {
            unlink(subpath);
        }
    }

    rmdir(path);

    if (errno)
    {
        perror("readdir");
        exit(1);
    }

    closedir(pdir);
}