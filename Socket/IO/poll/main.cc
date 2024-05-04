#include "PollServer.hpp"
#include <memory>

int main()
{
    // std::cout <<"fd_set bits num : " << sizeof(fd_set) * 8 << std::endl;

    // std::unique_ptr<SelectServer> svr(new SelectServer());
    std::unique_ptr<PollServer> svr(new PollServer());
    svr->Init();
    svr->Start();

    return 0;
}