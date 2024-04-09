#include <sys/stat.h>
#include <sys/types.h>

#include "func.h"

int main(int argc, char *argv[])
{
    printf("hello world\n");

    // ./mkdir dir mode
    ARGS_CHECK(argc, 3);

    mode_t mode;
    sscanf(argv[2], "%o", &mode);

    // 创建的目录会受文件创建掩码的影响
    int ret = mkdir(argv[1], mode);
    ERROR_CHECK(ret, -1, "mkdir");

    return 0;
}
