#include "comm.hpp"
#include "log.hpp"

using namespace std;

// 管理管道文件
int main()
{
    Init init;
    Log log;
    // log.Enable(Onefile);
    log.Enable(Onefile);

    // 打开管道
    int fd = open(FIFO_FILE, O_RDONLY); // 等待写入方打开之后，自己才会打开文件，向后执行， open 阻塞了！
    if (fd < 0)
    {
        log(Fatal, "error string: %s, error code: %d", strerror(errno), errno);
        exit(FIFO_OPEN_ERR);
    }

    log(Info, "server open file done, error string: %s, error code: %d", strerror(errno), errno);
    log(Warning, "server open file done, error string: %s, error code: %d", strerror(errno), errno);
    log(Fatal, "server open file done, error string: %s, error code: %d", strerror(errno), errno);
    log(Debug, "server open file done, error string: %s, error code: %d", strerror(errno), errno);

    // 开始通信
    while (true)
    {
        char buffer[1024] = {0};
        int x = read(fd, buffer, sizeof(buffer));
        if (x > 0)
        {
            buffer[x] = 0;
            cout << "client say# " << buffer << endl;
        }
        else if (x == 0)
        {
            log(Debug, "client quit, me too!, error string: %s, error code: %d", strerror(errno), errno);
            break;
        }
        else
            break;
    }

    close(fd);
    return 0;
}