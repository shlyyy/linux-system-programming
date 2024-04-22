#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

std::string terminal = "/dev/pts/6";

int OpenTerminal()
{
    int fd = open(terminal.c_str(), O_WRONLY);
    if (fd < 0)
    {
        std::cerr << "open terminal error" << std::endl;
        return 1;
    }
    dup2(fd, 2);

    // printf("hello world\n");
    // close(fd);
    return 0;
}