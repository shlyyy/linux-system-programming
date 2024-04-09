#include <unistd.h>
#include "func.h"

int main(int argc, char *argv[])
{
    printf("hello world\n");

    // ./chdir dir
    ARGS_CHECK(argc, 2);

    char buf[256];
    getcwd(buf, SIZE(buf)); // 获取当前工作目录
    puts(buf);              // 打印当前工作目录

    int ret = chdir(argv[1]); // 改变当前工作目录
    ERROR_CHECK(ret, -1, "chdir");
    getcwd(buf, SIZE(buf));
    puts(buf);
    return 0;
}
