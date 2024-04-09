#include <sys/stat.h>
#include <sys/types.h>
#include <func.h>

int main(int argc, char *argv[])
{
    // ./chmod file mode
    ARGS_CHECK(argc, 3);

    mode_t mode;
    // 将字符串转换为整数
    sscanf(argv[2], "%o", &mode);
    int ret = chmod(argv[1], mode);
    ERROR_CHECK(ret, -1, "chmod");

    return 0;
}