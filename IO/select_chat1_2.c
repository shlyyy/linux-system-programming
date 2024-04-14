#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <func.h>
#include <sys/select.h>

int main(int argc, char *argv[])
{
    // ./select_chat1_2 1.pipe 2.pipe
    ARGS_CHECK(argc, 3);

    int fdw = open(argv[1], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open fdw");

    int fdr = open(argv[2], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open fdr");

    printf("chat is established!\n");

    char buf[0x1000];

    // 创建监听集合
    fd_set rdset;
    FD_ZERO(&rdset);
    // 把管道读端盒stdin加入监听
    FD_SET(STDIN_FILENO, &rdset);
    FD_SET(fdr, &rdset);

    // 调用select等待监听集合中就绪
    // 第一参数是要监听描述符中最大的+1,超时设为NULL表示永久等待
    select(fdr + 1, &rdset, NULL, NULL, NULL);

    // select返回以后，监听集合变为就绪集合
    // 处理就绪集合中的IO请求
    if (FD_ISSET(STDIN_FILENO, &rdset))
    {
        // STDIN就绪
        memset(buf, 0, sizeof(buf));
        read(STDIN_FILENO, buf, sizeof(buf));
        write(fdw, buf, strlen(buf));
    }

    if (FD_ISSET(fdr, &rdset))
    {
        // fdr就绪
        memset(buf, 0, sizeof(buf));
        read(fdr, buf, sizeof(buf));
        printf("buf = %s\n", buf);
    }

    close(fdw);
    close(fdr);
    return 0;
}