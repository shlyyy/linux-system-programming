#pragma once

#include <iostream>
#include <memory>
#include <sys/epoll.h>
#include "Socket.hpp"
#include "Epoller.hpp"
#include "Log.hpp"
#include "nocopy.hpp"

uint32_t EVENT_IN = (EPOLLIN);
uint32_t EVENT_OUT = (EPOLLOUT);

class EpollServer : public nocopy
{
    static const int num = 64;

public:
    EpollServer(uint16_t port)
        : _port(port),
          _listsocket_ptr(new Sock()),
          _epoller_ptr(new Epoller())
    {
    }
    void Init()
    {
        _listsocket_ptr->Socket();
        _listsocket_ptr->Bind(_port);
        _listsocket_ptr->Listen();

        lg(Info, "create listen socket success: %d\n", _listsocket_ptr->Fd());
    }
    void Accepter()
    {
        // 获取了一个新连接
        std::string clientip;
        uint16_t clientport;
        int sock = _listsocket_ptr->Accept(&clientip, &clientport);
        if (sock > 0)
        {
            // 我们能直接读取吗？不能
            _epoller_ptr->EpllerUpdate(EPOLL_CTL_ADD, sock, EVENT_IN);
            lg(Info, "get a new link, client info@ %s:%d", clientip.c_str(), clientport);
        }
    }
    // for test
    void Recver(int fd)
    {
        // demo
        char buffer[1024];
        ssize_t n = read(fd, buffer, sizeof(buffer) - 1); // bug?
        if (n > 0)
        {
            buffer[n] = 0;
            std::cout << "get a messge: " << buffer << std::endl;
            // wrirte
            std::string echo_str = "server echo $ ";
            echo_str += buffer;
            write(fd, echo_str.c_str(), echo_str.size());
        }
        else if (n == 0)
        {
            lg(Info, "client quit, me too, close fd is : %d", fd);
            // 细节3
            _epoller_ptr->EpllerUpdate(EPOLL_CTL_DEL, fd, 0);
            close(fd);
        }
        else
        {
            lg(Warning, "recv error: fd is : %d", fd);
            _epoller_ptr->EpllerUpdate(EPOLL_CTL_DEL, fd, 0);
            close(fd);
        }
    }
    void Dispatcher(struct epoll_event revs[], int num)
    {
        for (int i = 0; i < num; i++)
        {
            uint32_t events = revs[i].events;
            int fd = revs[i].data.fd;
            if (events & EVENT_IN)
            {
                if (fd == _listsocket_ptr->Fd())
                {
                    Accepter();
                }
                else
                {
                    // 其他fd上面的普通读取事件就绪
                    Recver(fd);
                }
            }
            else if (events & EVENT_OUT)
            {
            }
            else
            {
            }
        }
    }
    void Start()
    {
        // 将listensock添加到epoll中 -> listensock和他关心的事件，添加到内核epoll模型中rb_tree.
        _epoller_ptr->EpllerUpdate(EPOLL_CTL_ADD, _listsocket_ptr->Fd(), EVENT_IN);
        struct epoll_event revs[num];
        for (;;)
        {
            int n = _epoller_ptr->EpollerWait(revs, num);
            if (n > 0)
            {
                // 有事件就绪
                lg(Debug, "event happened, fd is : %d", revs[0].data.fd);
                Dispatcher(revs, n);
            }
            else if (n == 0)
            {
                lg(Info, "time out ...");
            }
            else
            {
                lg(Error, "epll wait error");
            }
        }
    }
    ~EpollServer()
    {
        _listsocket_ptr->Close();
    }

private:
    std::shared_ptr<Sock> _listsocket_ptr;
    std::shared_ptr<Epoller> _epoller_ptr;
    uint16_t _port;
};