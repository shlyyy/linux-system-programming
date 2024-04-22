#include <iostream>
#include <string>
#include <unordered_map>
#include <strings.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <functional>
#include <memory>
#include <cstdio>
#include "Log.hpp"

// using func_t = std::function<std::string(const std::string&)>;
typedef std::function<std::string(const std::string &)> func_t;

Log lg;

enum
{
    SOCKET_ERR = 1,
    BIND_ERR
};

uint16_t defaultport = 8080;
std::string defaultip = "0.0.0.0";
const int size = 1024;

class UdpServer
{
public:
    UdpServer(const uint16_t &port = defaultport,
              const std::string &ip = defaultip)
        : sockfd_(0), port_(port), ip_(ip), isrunning_(false)
    {
    }

    void Init()
    {
        // 1. 创建udp socket
        sockfd_ = socket(AF_INET, SOCK_DGRAM, 0); // PF_INET
        if (sockfd_ < 0)
        {
            lg(Fatal, "socket create error, sockfd: %d", sockfd_);
            exit(SOCKET_ERR);
        }
        lg(Info, "socket create success, sockfd: %d", sockfd_);

        // 2. bind socket
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        // 端口号和IP地址需要使用网络字节序
        local.sin_port = htons(port_);
        local.sin_addr.s_addr = inet_addr(ip_.c_str()); // char* -> uint32_t
        // local.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(sockfd_, (const struct sockaddr *)&local, sizeof(local)) < 0)
        {
            lg(Fatal, "bind error, errno: %d, err string: %s", errno, strerror(errno));
            exit(BIND_ERR);
        }

        lg(Info, "bind success, errno: %d, err string: %s", errno, strerror(errno));
    }

    void CheckUser(const struct sockaddr_in &client, const std::string clientip, uint16_t clientport)
    {
        auto iter = online_user_.find(clientip);
        if (iter == online_user_.end())
        {
            online_user_.insert({clientip, client});
            std::cout << "[" << clientip << ":" << clientport << "] add to online user." << std::endl;
        }
    }
    
    void Broadcast(const std::string &info, const std::string clientip, uint16_t clientport)
    {
        for (const auto &user : online_user_)
        {
            std::string message = "[";
            message += clientip;
            message += ":";
            message += std::to_string(clientport);
            message += "]# ";
            message += info;
            socklen_t len = sizeof(user.second);
            sendto(sockfd_, message.c_str(), message.size(), 0, (struct sockaddr *)(&user.second), len);
        }
    }

    void Run(func_t func)
    {
        isrunning_ = true;
        char inbuffer[size];
        while (isrunning_)
        {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            ssize_t n = recvfrom(sockfd_, inbuffer, sizeof(inbuffer) - 1, 0,
                                 (struct sockaddr *)&client, &len);
            if (n < 0)
            {
                lg(Warning, "recvfrom error, errno: %d, err string: %s", errno, strerror(errno));
                continue;
            }
            inbuffer[n] = 0;
            uint16_t clientport = ntohs(client.sin_port);
            std::string clientip = inet_ntoa(client.sin_addr);
            CheckUser(client, clientip, clientport);

            std::string info = inbuffer;
            Broadcast(info, clientip, clientport);
        }
    }

    ~UdpServer()
    {
        if (sockfd_ > 0)
            close(sockfd_);
    }

private:
    int sockfd_;     // 网路文件描述符
    std::string ip_; // 任意地址bind 0
    uint16_t port_;  // 表明服务器进程的端口号
    bool isrunning_;

    std::unordered_map<std::string, struct sockaddr_in> online_user_;
};

std::string Handler(const std::string &str)
{
    std::string res = "Server get a message: ";
    res += str;
    std::cout << res << std::endl;

    // pid_t id = fork();
    // if(id == 0)
    // {
    //     // ls -a -l -> "ls" "-a" "-l"
    //     // exec*();
    // }
    return res;
}

std::string ExcuteCommand(const std::string &cmd)
{
    // SafeCheck(cmd);

    FILE *fp = popen(cmd.c_str(), "r");
    if (nullptr == fp)
    {
        perror("popen");
        return "error";
    }
    std::string result;
    char buffer[4096];
    while (true)
    {
        char *ok = fgets(buffer, sizeof(buffer), fp);
        if (ok == nullptr)
            break;
        result += buffer;
    }
    pclose(fp);

    return result;
}

void Usage(std::string proc)
{
    std::cout << "\n\rUsage: " << proc << " port[1024+]\n"
              << std::endl;
}

// ./udpserver port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(0);
    }

    uint16_t port = std::stoi(argv[1]);

    std::unique_ptr<UdpServer> svr(new UdpServer(port));

    svr->Init(/**/);
    svr->Run(ExcuteCommand);

    return 0;
}