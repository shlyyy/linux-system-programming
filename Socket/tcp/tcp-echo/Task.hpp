#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include "Log.hpp"
#include "Init.hpp"

extern Log lg;
Init init;

class Task
{
public:
    Task(int sockfd, const std::string &clientip, const uint16_t &clientport)
        : sockfd_(sockfd), clientip_(clientip), clientport_(clientport)
    {
    }
    Task()
    {
    }
    void run()
    {
        // 测试代码
        char buffer[4096];
        // Tcp是面向字节流的，你怎么保证，你读取上来的数据，是"一个" "完整" 的报文呢？
        ssize_t n = read(sockfd_, buffer, sizeof(buffer)); // BUG?
        if (n > 0)
        {
            buffer[n] = 0;
            std::cout << "client key# " << buffer << std::endl;
            std::string echo_string = init.translation(buffer);

            // sleep(5);
            // // close(sockfd_);
            // lg(Warning, "close sockfd %d done", sockfd_);

            // sleep(2);
            n = write(sockfd_, echo_string.c_str(), echo_string.size()); // 100 fd 不存在
            if (n < 0)
            {
                lg(Warning, "write error, errno : %d, errstring: %s", errno, strerror(errno));
            }
        }
        else if (n == 0)
        {
            lg(Info, "%s:%d quit, server close sockfd: %d", clientip_.c_str(), clientport_, sockfd_);
        }
        else
        {
            lg(Warning, "read error, sockfd: %d, client ip: %s, client port: %d", sockfd_, clientip_.c_str(), clientport_);
        }
        close(sockfd_);
    }
    void operator()()
    {
        run();
    }
    ~Task()
    {
    }

private:
    int sockfd_;
    std::string clientip_;
    uint16_t clientport_;
};