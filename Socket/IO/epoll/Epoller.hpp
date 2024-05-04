#pragma once

#include "nocopy.hpp"
#include "Log.hpp"
#include <cerrno>
#include <cstring>
#include <sys/epoll.h>

class Epoller : public nocopy
{
    static const int size = 128;

public:
    Epoller()
    {
        _epfd = epoll_create(size);
        if (_epfd == -1)
        {
            lg(Error, "epoll_create error: %s", strerror(errno));
        }
        else
        {
            lg(Info, "epoll_create success: %d", _epfd);
        }
    }
    int EpollerWait(struct epoll_event revents[], int num)
    {
        int n = epoll_wait(_epfd, revents, num, /*_timeout 0*/ -1);
        return n;
    }
    int EpllerUpdate(int oper, int sock, uint32_t event)
    {
        int n = 0;
        if (oper == EPOLL_CTL_DEL)
        {
            n = epoll_ctl(_epfd, oper, sock, nullptr);
            if (n != 0)
            {
                lg(Error, "epoll_ctl delete error!");
            }
        }
        else
        {
            // EPOLL_CTL_MOD || EPOLL_CTL_ADD
            struct epoll_event ev;
            ev.events = event;
            ev.data.fd = sock; // 目前，方便我们后期得知，是哪一个fd就绪了！

            n = epoll_ctl(_epfd, oper, sock, &ev);
            if (n != 0)
            {
                lg(Error, "epoll_ctl error!");
            }
        }
        return n;
    }
    ~Epoller()
    {
        if (_epfd >= 0)
            close(_epfd);
    }

private:
    int _epfd;
    int _timeout{3000};
};