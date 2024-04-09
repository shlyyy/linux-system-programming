#include <unistd.h>
#include "func.h"

int main(int argc, char *argv[])
{
    printf("hello world\n");
    // char buf[10];
    // 由系统调用malloc，动态申请内存空间
    char *cwd = getcwd(NULL, 0);
    ERROR_CHECK(cwd, NULL, "getcwd");
    
    puts(cwd);

    // 由调用者负责释放申请的内存空间
    free(cwd);
    return 0;
}
