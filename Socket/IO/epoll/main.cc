#include <iostream>
#include <memory>
#include "EpollServer.hpp"

int main()
{
    std::unique_ptr<EpollServer> epoll_svr(new EpollServer(8888));
    epoll_svr->Init();
    epoll_svr->Start();

    // Epoller ep;
    // // Epoller ep1 = ep;

    return 0;
}