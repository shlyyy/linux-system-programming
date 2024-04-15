#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <func.h>
#include <sys/select.h>
#include <time.h>

int main(int argc, char *argv[])
{
    // ./select_10s_1 1.pipe 2.pipe
    ARGS_CHECK(argc, 3);

    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open fdr");

    int fdw = open(argv[2], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open fdw");

    printf("chat is established!\n");

    char buf[0x1000];
    fd_set rdset;
    time_t lastActive = time(NULL);
    while (1)
    {
        FD_ZERO(&rdset);

        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(fdr, &rdset);

        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        select(fdr + 1, &rdset, NULL, NULL, &timeout);
        time_t curTime = time(NULL);
        printf("curTime = %s\n", ctime(&curTime));
        if (FD_ISSET(STDIN_FILENO, &rdset))
        {
            memset(buf, 0, sizeof(buf));
            ssize_t sret = read(STDIN_FILENO, buf, sizeof(buf));
            if (sret == 0)
            {
                // Ctrl+D退出程序
                write(fdw, "byebye!", 7);
                break;
            }
            write(fdw, buf, strlen(buf));
        }

        if (FD_ISSET(fdr, &rdset))
        {
            lastActive = time(NULL);
            memset(buf, 0, sizeof(buf));
            ssize_t sret = read(fdr, buf, sizeof(buf));
            if (sret == 0)
            {
                // 另一方写端关闭，读端返回0
                printf("Hehe!\n");
                break;
            }

            printf("buf = %s\n", buf);
        }

        if (curTime - lastActive >= 10)
        {
            write(fdw, "服务器已断开连接!", sizeof("服务器已断开连接!"));
            break;
        }
    }

    close(fdw);
    close(fdr);
    return 0;
}