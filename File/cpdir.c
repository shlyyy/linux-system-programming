#include <func.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

void copy_file(const char *src, const char *dst);
void copy_dir(const char *src, const char *dst);

int main(int argc, char *argv[])
{
    // ./cpdir src dst
    ARGS_CHECK(argc, 3);

    copy_dir(argv[1], argv[2]);

    return 0;
}

void copy_file(const char *src, const char *dst)
{
    FILE *src_fp = fopen(src, "rb");
    ERROR_CHECK(src_fp, NULL, "fopen");

    FILE *dst_fp = fopen(dst, "wb+");
    ERROR_CHECK(dst_fp, NULL, "fopen");

    char buff[1024];
    int n;
    while ((n = fread(buff, 1, SIZE(buff), src_fp)) > 0)
    {
        fwrite(buff, 1, n, dst_fp);
    }

    fclose(src_fp);
    fclose(dst_fp);
}

void copy_dir(const char *src, const char *dst)
{
    // 创建目录
    mkdir(dst, 0777);

    DIR *pdir = opendir(src);

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
        char subsrc[256];
        char subdst[256];
        sprintf(subsrc, "%s/%s", src, name);
        sprintf(subdst, "%s/%s", dst, name);

        if (pdirent->d_type == DT_DIR)
        {
            copy_dir(subsrc, subdst);
        }
        else
        {
            copy_file(subsrc, subdst);
        }
    }

    if (errno)
    {
        perror("readdir");
        exit(1);
    }

    closedir(pdir);
}